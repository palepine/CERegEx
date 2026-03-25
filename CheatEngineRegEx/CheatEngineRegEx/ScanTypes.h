#pragma once
#include <cstdint>
#include <string>
#include <vector>

enum class StringEncoding
{
	ASCII,
	//UTF8,
	UTF16,
	//UTF32,
	//CUSTOM
	//FAKE
};

enum class RegexEngineType
{
	STD,
	RE2,
	PCRE2
};

struct RegexMatchInfo
{
	std::size_t position = 0;
	std::size_t length = 0;
	std::string matchedText;
};

struct ScanRequest
{
	std::string pattern;
	std::string protectionFlags;
	std::string encodingText = "ASCII";
	std::string regexEngineTypeText = "STD";
	std::size_t minLength = 3;
	std::size_t maxLength = 1024;
	bool isCaseSensitive = false;
	bool findOne = false;
};

struct ScanConfig
{
	std::string pattern;
	std::string protectionFlags;
	std::size_t minLength = 3;
	std::size_t maxLength = 1024;
	StringEncoding encoding = StringEncoding::ASCII;
	RegexEngineType regexType = RegexEngineType::STD;
	bool isCaseSensitive = false;
	bool findOne = false;
};

struct ScanResult
{
	std::uintptr_t address = 0;
	std::string text;
};

struct MatchResult
{
	size_t indexUnits;
	size_t lengthUnits;
	std::string textUtf8;
	std::string encoding;
};

struct CandidateString
{
	std::uintptr_t address = 0;
	std::string text;
	// helps map from a UTF8 byte index we have represented in the script back to the original mem offset in another encoding (e.g. UTF16 which is 2 bytes per char here)
	// i.e. for each utf8 character byte (index) it gets an offset to the start of the respective character in memory, it will look like this - Privet in Cyrillics {0, 0, 2, 2, 4, 4, 6, 6, 8, 8, 10, 10}
	std::vector<std::size_t> textByteToMemoryOffset;
};

struct MemoryRegion
{
	std::uintptr_t base = 0;
	std::size_t size = 0;
	DWORD protect = 0;
	DWORD state = 0;
	DWORD type = 0;
};