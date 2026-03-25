#pragma once
#include "pch.h"
#include <memory>
#include <optional>
#include <string>
#include <re2/re2.h>
#include "IRegexEngine.h"

class RE2RegexEngine : public IRegexEngine
{
private:
	std::unique_ptr<RE2> compiledRegex_;

public:
	bool compile(const ScanConfig& config) override;
	std::optional<RegexMatchInfo> search(const std::string& text) const override;
};