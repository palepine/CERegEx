#include "pch.h"
#include "UTF16StringExtractor.h"

namespace
{
    namespace UTF16
    {
        constexpr std::uint16_t SURROGATE_MIN = 0xD800;
        constexpr std::uint16_t SURROGATE_MAX = 0xDFFF;
        //constexpr std::uint16_t HIGH_SURROGATE_MIN = 0xD800;
        //constexpr std::uint16_t HIGH_SURROGATE_MAX = 0xDBFF;
        //constexpr std::uint16_t LOW_SURROGATE_MIN = 0xDC00;
        //constexpr std::uint16_t LOW_SURROGATE_MAX = 0xDFFF;

        constexpr std::uint16_t NULL_CODEPOINT = 0x0000;
        constexpr std::uint16_t CONTROL_MAX = 0x001F; // first printable char is 0x20 (space)

        constexpr std::uint32_t ONE_BYTE_MAX = 0x7F; // 0000 to 007F - 1 byte code point's max value
        constexpr std::uint32_t TWO_BYTE_MAX = 0x7FF; // 0080 to 07FF - 2 byte code point's max value

        constexpr std::uint8_t two_byte_lead = 0xC0;
        constexpr std::uint8_t three_byte_lead = 0xE0;
        constexpr std::uint8_t continuation_lead = 0x80;

        constexpr std::uint32_t continuation_mask = 0x3F;
    }

    std::uint16_t readUTF16LECodeUnit(const std::vector<char>& buffer, std::size_t offset)
    {
        const std::uint8_t byte1 = static_cast<std::uint8_t>(buffer[offset]);
        const std::uint8_t byte2 = static_cast<std::uint8_t>(buffer[offset + 1]);
        return static_cast<std::uint16_t>(byte1 | (byte2 << 8));
    }

    bool isUTF16Surrogate(std::uint16_t codeUnit)
    {
        return codeUnit >= UTF16::SURROGATE_MIN && codeUnit <= UTF16::SURROGATE_MAX;
    }

    bool isAllowedUTF16CodeUnit(std::uint16_t codeUnit)
    {
        if (codeUnit == UTF16::NULL_CODEPOINT)
            return false;

        if (isUTF16Surrogate(codeUnit))
            return false;

        if (codeUnit <= UTF16::CONTROL_MAX)
            return false;

        return true;
    }

    void appendUTF8FromBMPCodeUnit(std::uint16_t codeUnit, std::size_t memoryOffset, std::string& outText, std::vector<std::size_t>& outOffsets)
    {
        std::uint32_t codePoint = codeUnit;

        if (codePoint <= UTF16::ONE_BYTE_MAX)
        {
            outText.push_back( static_cast<char>(codePoint) );
            outOffsets.push_back(memoryOffset);
        }
        else if (codePoint <= UTF16::TWO_BYTE_MAX)
        {
            outText.push_back( static_cast<char>( UTF16::two_byte_lead | (codePoint >> 6) ) );
            outText.push_back( static_cast<char>( UTF16::continuation_lead | (codePoint & UTF16::continuation_mask) ) );

            outOffsets.push_back(memoryOffset);
            outOffsets.push_back(memoryOffset);
        }
        else
        {
            outText.push_back( static_cast<char>( UTF16::three_byte_lead | (codePoint >> 12) ) );
            outText.push_back( static_cast<char>( UTF16::continuation_lead | ((codePoint >> 6) & UTF16::continuation_mask) ) );
            outText.push_back( static_cast<char>( UTF16::continuation_lead | (codePoint & UTF16::continuation_mask) ) );

            outOffsets.push_back(memoryOffset);
            outOffsets.push_back(memoryOffset);
            outOffsets.push_back(memoryOffset);
        }

    };

}
// UTF-16LE that handles __only BMP code points__ (no surrogate pairs, which might be an overkill).
// It reads 2 bytes at a time, checks if it's a valid code unit & not surrogate, then collects until it finds an invalid one.
// Then it converts the collected UTF-16 string to UTF-8 and adds it to the results if it meets the length criteria.
// The address of the candidate string is calculated based on the base address and the offset in the buffer where the string starts.
std::vector<CandidateString> UTF16StringExtractor::extract(const std::vector<char>& buffer, std::uintptr_t baseAddress, const ScanConfig& config) const
{
	std::vector<CandidateString> strings;
	std::size_t i = 0;
	while (i + 1 < buffer.size())
	{
        std::uint16_t codeUnit = readUTF16LECodeUnit(buffer, i);
        if ( !isAllowedUTF16CodeUnit(codeUnit) )
		{
			i += 2;
			continue;
		}
		std::size_t start = i;
        std::u16string collected;

		while (i + 1 < buffer.size())
		{
            codeUnit = readUTF16LECodeUnit(buffer, i);
            if (!isAllowedUTF16CodeUnit(codeUnit))
            {
                break;
            }

            collected.push_back( static_cast<char16_t>(codeUnit) );
			i += 2;
		}

        const std::size_t lengthUnits = collected.size();

        if (lengthUnits >= config.minLength)
		{
            CandidateString s;
            s.address = baseAddress + start;

            for (std::size_t j = start; j < i; j += 2)
            {
                std::uint16_t codeUnit = readUTF16LECodeUnit(buffer, j);
                appendUTF8FromBMPCodeUnit( codeUnit, j - start, s.text, s.textByteToMemoryOffset );
            }
            
            strings.push_back(std::move(s));
		}

        if (i == start)
        {
            i += 2;
        }
	}

	return strings;
}