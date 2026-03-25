#include "pch.h"
#include "ConfigValidator.h"

ConfigStatus validateConfig(const ScanConfig& config)
{
	//if (config.pattern.empty())
	//	return ConfigStatus::INVALID;
	if (config.minLength <= 0 || config.maxLength <= 0 || config.minLength > config.maxLength)
		return ConfigStatus::INVALID;

	return ConfigStatus::VALID;
}