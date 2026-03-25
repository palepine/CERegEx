#pragma once
#include "pch.h"
#include "IRegexEngine.h"
#include "StdRegexEngine.h"
#include "RE2RegexEngine.h"
#include "PCRE2RegexEngine.h"

std::unique_ptr<IRegexEngine> createRegexEngine(RegexEngineType engineType);