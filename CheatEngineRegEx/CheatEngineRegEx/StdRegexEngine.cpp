#include "pch.h"
#include "StdRegexEngine.h"

//std::optional<RegexMatchInfo> regexMatches(const std::string& text, const ScanConfig& config)
//{
//    try
//    {
//        std::smatch match;
//
//        if (!std::regex_search(text, match, re))
//        {
//            return std::nullopt;
//        }
//
//        
//    }
//    catch (const std::regex_error&)
//    {
//        return std::nullopt;
//    }
//}

bool StdRegexEngine::compile(const ScanConfig& config)
{
    try {
        auto flags = std::regex::ECMAScript;
        if (!config.isCaseSensitive)
        {
            flags |= std::regex::icase;
        }
        compiledRegex_ = std::regex(config.pattern, flags);
        isCompiled_ = true;
        return true;
    }
    catch (const std::regex_error&)
    {
        isCompiled_ = false;
        return false;
    }
    catch (...)
    {
        return false;
    }
}

std::optional<RegexMatchInfo> StdRegexEngine::search(const std::string& text) const
{
    if (!isCompiled_)
    {
        return std::nullopt;
    }
    
    std::smatch match;
    if ( !std::regex_search(text, match, compiledRegex_) )
    {
        return std::nullopt;
    }
    
    return RegexMatchInfo{ static_cast<std::size_t>( match.position() ), static_cast<std::size_t>( match.length() ), match.str() };
}