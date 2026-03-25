#include "pch.h"
#include "RegexFactory.h"

std::unique_ptr<IRegexEngine> createRegexEngine(RegexEngineType engineType)
{
	switch (engineType)
	{
	case RegexEngineType::STD:
		return std::make_unique<StdRegexEngine>();
	case RegexEngineType::RE2:
		return std::make_unique<RE2RegexEngine>();
	case RegexEngineType::PCRE2:
		return std::make_unique<PCRE2RegexEngine>();
	default:
		return std::make_unique<StdRegexEngine>();
		//throw std::runtime_error("Unsupported regex engine type!");
	}
}