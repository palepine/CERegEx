#pragma once
#include "ScanTypes.h"

enum class ConfigStatus
{
	VALID,
	INVALID
};

ConfigStatus validateConfig(const ScanConfig& config);