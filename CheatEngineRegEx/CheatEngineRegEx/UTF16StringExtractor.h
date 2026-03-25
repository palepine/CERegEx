#pragma once
#include "IStringExtractor.h"
class UTF16StringExtractor : public IStringExtractor
{
public:
	std::vector<CandidateString> extract(const std::vector<char>& buffer, std::uintptr_t baseAddress, const ScanConfig& config) const override;
};

