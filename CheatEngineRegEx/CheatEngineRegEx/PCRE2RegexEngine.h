#pragma once
#include "IRegexEngine.h"
#define PCRE2_CODE_UNIT_WIDTH 8
#include "pcre2.h"
#include <memory>
#include <optional>
#include <string>

struct PCRE2CodeDeleter
{
    void operator()(pcre2_code* code) const
    {
        if (code)
        {
            pcre2_code_free(code);
        }
    }
};

class PCRE2RegexEngine : public IRegexEngine
{
private:
    std::unique_ptr<pcre2_code, PCRE2CodeDeleter> compiledRegex_;

public:
    bool compile(const ScanConfig& config) override;
    std::optional<RegexMatchInfo> search(const std::string& text) const override;
};

