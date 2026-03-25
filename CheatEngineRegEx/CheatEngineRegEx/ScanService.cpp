#include "pch.h"
#include "ScanService.h"

ScanServiceResult performScan(const ScanRequest& request)
{
	// get out if we aren't attaached
	if (getProcessHandle() == NULL)
	{
		return { ScanStatus::NO_PROCESS, {} };
	}

	// resolving encoding from a text arg
	StringEncoding encoding;
	if (parseEncoding(request.encodingText, encoding) != EncodingParseStatus::SUCCESS)
	{
		return { ScanStatus::INVALID_ENCODING , {} };
	}
	
	// resolving engine type from a text arg
	RegexEngineType regexType;
	if (parseRegexEngineType(request.regexEngineTypeText, regexType) != RegexEngineTypeParseStatus::SUCCESS)
	{
		return { ScanStatus::INVALID_REGEX_TYPE , {} }; // maybe change the enum to be more regex specific
	}

	// creating & validating config
	ScanConfig config{ request.pattern, request.protectionFlags, request.minLength, request.maxLength, encoding, regexType, request.isCaseSensitive, request.findOne };
	if (validateConfig(config) == ConfigStatus::INVALID)
	{
		return { ScanStatus::INVALID_CONFIG, {} };
	}

	// getting the right regex engine
	std::unique_ptr<IRegexEngine> regexEngine = createRegexEngine(regexType);
	if ( !regexEngine->compile(config) )
	{
		return { ScanStatus::REGEX_COMPILATION_ERROR, {} };
	}

	// resolving protection flag from text and getting the memregions accodingly
	std::uint64_t flags = parseProtectionFlags(config.protectionFlags.c_str());
	std::vector<MemoryRegion> regions = getPreferredMemoryRegions(flags);
	if (regions.empty())
	{
		return { ScanStatus::NO_REGIONS, {} };
	}
	
	// getting the right str extractor (they're stateless for now)
	const IStringExtractor& extractor = getStringExtractor(config.encoding);

	std::vector<ScanResult> results = scanRegions(regions, config, extractor, *regexEngine);

	if (results.empty())
	{
		return { ScanStatus::NO_RESULTS, {} };
	}

	return { ScanStatus::SUCCESS, std::move(results) };
}