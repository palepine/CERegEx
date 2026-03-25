#pragma once
#include "pch.h"
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <algorithm>
#include <cstdint>
#include "MemoryRegionEnumerator.h"
#include "ProcessMemoryReader.h"

#include "IRegexEngine.h"
#include "IStringExtractor.h"

std::vector<ScanResult> scanRegions(const std::vector<MemoryRegion>& regions, const ScanConfig& config, const IStringExtractor& extractor, const IRegexEngine& regexEngine);

std::vector< std::vector<ScanResult> > runWorkers(const std::vector<MemoryRegion>& regions, const ScanConfig& config, const IStringExtractor& extractor, const IRegexEngine& regexEngine);

std::vector<ScanResult> mergeAndSortResults(std::vector< std::vector<ScanResult> >& threadwiseResults);