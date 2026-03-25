#include "pch.h"
#include "MemoryScan.h"

std::vector<ScanResult> scanRegions( const std::vector<MemoryRegion>& regions, const ScanConfig& config, const IStringExtractor& extractor, const IRegexEngine& regexEngine )
{
	std::vector< std::vector<ScanResult> > threadwiseResults = runWorkers(regions, config, extractor, regexEngine);
	if (config.findOne)
		return threadwiseResults.empty() ? std::vector<ScanResult>{} : std::move(threadwiseResults[0]);
	return mergeAndSortResults(threadwiseResults);
}

std::vector< std::vector<ScanResult> > runWorkers(const std::vector<MemoryRegion>& regions, const ScanConfig& config, const IStringExtractor& extractor, const IRegexEngine& regexEngine)
{
	std::uint16_t threadCount = std::thread::hardware_concurrency();
	if (threadCount == 0)
		threadCount = 4;

	std::atomic<std::size_t> nextIndex{ 0 };
	std::atomic<bool> stopRequested{ false };
	std::mutex firstResultMutex;
	std::optional<ScanResult> firstResult;
	std::vector< std::vector<ScanResult> > threadwiseResults(threadCount); // each thread will use its own vector
	std::vector<std::thread> workers;
	workers.reserve(threadCount);

	for (std::uint16_t threadId{ 0 }; threadId < threadCount; threadId++)
	{
		workers.emplace_back([&, threadId]()
			{
				std::vector<ScanResult>& localResults = threadwiseResults[threadId];

				while (true)
				{
					std::size_t index = nextIndex.fetch_add(1, std::memory_order_relaxed);
					if (index >= regions.size())
						break;

					// checking if any tread found 1 result
					if (config.findOne && stopRequested.load(std::memory_order_relaxed))
						break;

					const MemoryRegion& region = regions[index];

					// filling the buffer with raw memory via winAPI
					std::vector<char> buffer = readRegionMemory(region);

					// retrieving only readable strings from the buffer
					std::vector<CandidateString> candidates = extractor.extract(buffer, region.base, config);

					for (const CandidateString& candidate : candidates)
					{
						// checking if any tread found 1 result again
						if (config.findOne && stopRequested.load(std::memory_order_relaxed))
						{
							break;
						}

						if (auto result = regexEngine.search(candidate.text))
						{
							const RegexMatchInfo& matchInfo = *result;

							if (matchInfo.length > config.maxLength)
								continue;

							// in utf16-to-utf8 1 code unit can be multiple UTF8 bytes, the textByteToMemoryOffset lets us map to the orignial memory
							if (matchInfo.position >= candidate.textByteToMemoryOffset.size())
								continue;

							ScanResult res;
							res.address = candidate.address + candidate.textByteToMemoryOffset[matchInfo.position];
							res.text = matchInfo.matchedText;

							if (config.findOne)
							{
								// locking to save the result and let other threads to stop
								std::lock_guard<std::mutex> lock(firstResultMutex);

								if (!firstResult.has_value())
								{
									firstResult = res;
									stopRequested.store(true, std::memory_order_relaxed);
								}

								break;
							}
							else
							{
								localResults.push_back(std::move(res));
							}

						}
					}
				}
			}
		);
	}

	for (std::thread& worker : workers)
	{
		worker.join();
	}

	if (config.findOne)
	{
		if (firstResult.has_value())
			return std::vector< std::vector<ScanResult> > { { *firstResult } };
		return std::vector< std::vector<ScanResult> >{ { } };
	}

	return threadwiseResults;
}

std::vector<ScanResult> mergeAndSortResults(std::vector< std::vector<ScanResult> >& threadwiseResults)
{
	std::vector<ScanResult> finalResults;
	for (const std::vector<ScanResult>& local : threadwiseResults)
	{
		finalResults.insert(finalResults.end(), std::make_move_iterator(local.begin()), std::make_move_iterator(local.end()));
	}

	std::sort(
		finalResults.begin(),
		finalResults.end(),
		[](const ScanResult& a, const ScanResult& b) { return a.address < b.address; }
	);
	
	return finalResults;
}