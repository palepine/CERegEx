#pragma once
#include "pch.h"
#include <string>
#include <algorithm>
#include <cctype>
#include "ScanTypes.h"

enum class EncodingParseStatus
{
	SUCCESS,
	INVALID_VALUE
};

enum class RegexEngineTypeParseStatus
{
	SUCCESS,
	INVALID_VALUE
};

EncodingParseStatus parseEncoding(const std::string& encodingStr, StringEncoding& encoding);

RegexEngineTypeParseStatus parseRegexEngineType(const std::string& engineTypeStr, RegexEngineType& engineType);