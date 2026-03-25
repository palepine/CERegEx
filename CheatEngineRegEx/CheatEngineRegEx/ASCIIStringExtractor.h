#pragma once
#include "ScanTypes.h"
#include <vector>
#include <IStringExtractor.h>

class ASCIIStringExtractor : public IStringExtractor
{
public:

	std::vector<CandidateString> extract(const std::vector<char>& buffer, std::uintptr_t baseAddress, const ScanConfig& config) const override;
};