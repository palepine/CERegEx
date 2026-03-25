#include "pch.h"
#include "ScanOptionParsing.h"

namespace
{
	std::string toUpper(std::string str)
	{
		std::transform(str.begin(), str.end(), str.begin(),  [](unsigned char c) { return std::toupper(c); }  );

		return str;
	}
}

EncodingParseStatus parseEncoding(const std::string& encodingStr, StringEncoding& encoding)
{
	std::string normalizedStr = toUpper(encodingStr);

	if (normalizedStr.empty() || normalizedStr == "ASCII")
	{
		encoding = StringEncoding::ASCII;
		return EncodingParseStatus::SUCCESS;
	}
	else if (normalizedStr == "UTF16" || normalizedStr == "UTF-16" || normalizedStr == "UTF-16LE" || normalizedStr == "UTF16LE")
	{
		encoding = StringEncoding::UTF16;
		return EncodingParseStatus::SUCCESS;
	}
	else
	{
		return EncodingParseStatus::INVALID_VALUE;
	}
}

RegexEngineTypeParseStatus parseRegexEngineType(const std::string& engineTypeStr, RegexEngineType& engineType)
{
	std::string normalizedStr = toUpper(engineTypeStr);

	if (normalizedStr.empty() || normalizedStr == "STD")
	{
		engineType = RegexEngineType::STD;
		return RegexEngineTypeParseStatus::SUCCESS;
	}
	else if (normalizedStr == "RE2")
	{
		engineType = RegexEngineType::RE2;
		return RegexEngineTypeParseStatus::SUCCESS;
	}
	else if (normalizedStr == "PCRE2")
	{
		engineType = RegexEngineType::PCRE2;
		return RegexEngineTypeParseStatus::SUCCESS;
	}
	else
	{
		return RegexEngineTypeParseStatus::INVALID_VALUE;
	}
}