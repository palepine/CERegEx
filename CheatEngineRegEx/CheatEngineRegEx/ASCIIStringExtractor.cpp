#include "pch.h"
#include "ASCIIStringExtractor.h"

namespace
{
	constexpr std::uint8_t ASCII_START = 0x20;
	constexpr std::uint8_t ASCII_END = 0x7E;

	bool isPrintable(unsigned char c)
	{
		return c >= ASCII_START && c <= ASCII_END;
	}
}

std::vector<CandidateString> ASCIIStringExtractor::extract(const std::vector<char>& buffer, std::uintptr_t baseAddress, const ScanConfig& config) const
{
	std::vector<CandidateString> strings;
	std::size_t i = 0;

	while (i < buffer.size())
	{
		if ( !isPrintable( static_cast<unsigned char>(buffer[i]) ) )
		{
			++i;
			continue;
		}

		std::size_t start = i;
		
		while ( i < buffer.size() && isPrintable( static_cast<unsigned char>(buffer[i]) ) )
		{
			++i;
		}

		std::size_t len = i - start;
		if (len >= config.minLength)
		{
			CandidateString s;
			s.address = baseAddress + start;
			s.text.assign( buffer.data() + start, len );
			s.textByteToMemoryOffset.reserve(len);
			for (std::size_t j{ 0 }; j < len; j++)
			{
				s.textByteToMemoryOffset.push_back(j);
			}
			strings.push_back( std::move(s) );
		}
	}

	return strings;
}