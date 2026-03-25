#pragma once
#include "IRegexEngine.h"
#include <string>
#include <regex>
#include <optional>
#include <cstddef>
#include "ScanTypes.h"

class StdRegexEngine : public IRegexEngine
{
private:
    std::regex compiledRegex_;
    bool isCompiled_ = false;

public:
    bool compile(const ScanConfig& config) override;
    std::optional<RegexMatchInfo> search(const std::string& text) const override;
};

