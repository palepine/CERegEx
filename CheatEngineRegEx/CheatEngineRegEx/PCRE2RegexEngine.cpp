#include "pch.h"
#include "PCRE2RegexEngine.h"

bool PCRE2RegexEngine::compile(const ScanConfig& config)
{
    int errorCode = 0;
    PCRE2_SIZE errorOffset = 0;

    uint32_t options = PCRE2_UTF | PCRE2_UCP;
    if (!config.isCaseSensitive)
    {
        options |= PCRE2_CASELESS;
    }

    pcre2_code* code = pcre2_compile( reinterpret_cast<PCRE2_SPTR>( config.pattern.c_str() ), PCRE2_ZERO_TERMINATED, options, &errorCode, &errorOffset, nullptr );

    compiledRegex_.reset(code);
    return compiledRegex_ != nullptr;
}

std::optional<RegexMatchInfo> PCRE2RegexEngine::search(const std::string& text) const
{
    if (!compiledRegex_)
    {
        return std::nullopt;
    }

    pcre2_match_data* matchData = pcre2_match_data_create_from_pattern( compiledRegex_.get(), nullptr );

    if (!matchData)
    {
        return std::nullopt;
    }

    const int rc = pcre2_match( compiledRegex_.get(), reinterpret_cast<PCRE2_SPTR>(text.c_str()), text.size(), 0, 0, matchData, nullptr );

    if (rc < 0)
    {
        pcre2_match_data_free(matchData);
        return std::nullopt;
    }

    PCRE2_SIZE* ovector = pcre2_get_ovector_pointer(matchData);
    const std::size_t position = static_cast<std::size_t>( ovector[0] );
    const std::size_t end = static_cast<std::size_t>( ovector[1] );

    RegexMatchInfo info{ position, end - position, text.substr(position, end - position) };

    pcre2_match_data_free(matchData);
    return info;

}