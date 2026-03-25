#pragma once
#include "pch.h"
#include "MemoryScan.h"
#include <vector>
#include "ScanTypes.h"
#include "ProtectionFlags.h"
#include "ConfigValidator.h"
#include "ExtractorFactory.h"
#include "RegexFactory.h"
#include "IStringExtractor.h"
#include "IRegexEngine.h"

enum class ScanStatus
{
	SUCCESS,
	NO_PROCESS,
	INVALID_ENCODING,
	INVALID_REGEX_TYPE,
	NO_REGIONS,
	INVALID_CONFIG,
	REGEX_COMPILATION_ERROR,
	NO_RESULTS,
	SCAN_ERROR
};

struct ScanServiceResult
{
	ScanStatus status = ScanStatus::SCAN_ERROR;
	std::vector<ScanResult> results;
};

ScanServiceResult performScan(const ScanRequest& request);