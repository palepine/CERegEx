#pragma once
#include "ScanTypes.h"
#include <string>
#include <optional>

class IRegexEngine
{
public:
    virtual ~IRegexEngine() = default;

    virtual bool compile(const ScanConfig& config) = 0;
    virtual std::optional<RegexMatchInfo> search(const std::string& text) const = 0;
};
