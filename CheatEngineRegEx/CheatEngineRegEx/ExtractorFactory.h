#pragma once
#include "pch.h"
#include "ASCIIStringExtractor.h"
#include "UTF16StringExtractor.h"
#include "ScanOptionParsing.h"
//#include "FakeStringExtractor.h"

const IStringExtractor& getStringExtractor(StringEncoding encoding);
