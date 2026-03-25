#pragma once
#include "pch.h"
#include "ScanTypes.h"
#include <vector>

class IStringExtractor
{
public:
	virtual ~IStringExtractor() = default;
	virtual std::vector<CandidateString> extract(const std::vector<char>& buffer, std::uintptr_t baseAddress, const ScanConfig& config) const = 0;
};

