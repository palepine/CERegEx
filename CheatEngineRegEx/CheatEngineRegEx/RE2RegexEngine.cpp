#include "pch.h"
#include "RE2RegexEngine.h"

bool RE2RegexEngine::compile(const ScanConfig& config)
{
    RE2::Options options;
    options.set_case_sensitive(config.isCaseSensitive);
    options.set_log_errors(false);

    compiledRegex_ = std::make_unique<RE2>(config.pattern, options);

    return compiledRegex_ && compiledRegex_->ok();
}

std::optional<RegexMatchInfo> RE2RegexEngine::search(const std::string& text) const
{
    if (!compiledRegex_ || !compiledRegex_->ok())
    {
        return std::nullopt;
    }

    re2::StringPiece input(text);
    re2::StringPiece groups[1];
    if ( !compiledRegex_->Match( input, 0, input.size(), RE2::UNANCHORED, groups, 1) )
    {
        return std::nullopt;
    }

    const std::size_t position = static_cast<std::size_t>(groups[0].data() - text.data());

    return RegexMatchInfo{ position, static_cast<std::size_t>( groups[0].size() ), std::string( groups[0].data(), groups[0].size() ) };

}