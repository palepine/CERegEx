#include "pch.h"
#include "ExtractorFactory.h"

const IStringExtractor& getStringExtractor(StringEncoding encoding)
{
	static const ASCIIStringExtractor asciiExtractor;
	static const UTF16StringExtractor utf16Extractor;
	//static const FakeStringExtractor fakeExtractor;

	switch (encoding)
	{
	case StringEncoding::ASCII:
		return asciiExtractor;
	case StringEncoding::UTF16:
		return utf16Extractor;
	//case StringEncoding::FAKE:
	//	return fakeExtractor;
	default:
		return asciiExtractor;
		//throw std::runtime_error("Unsupported string encoding!");
	}
}