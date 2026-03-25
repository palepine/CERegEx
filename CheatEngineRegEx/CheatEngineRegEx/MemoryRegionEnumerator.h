#pragma once
#include "pch.h"
#include "PluginHelper.h"
#include "ScanTypes.h"
#include <vector>

enum class PreferredRegionFlag
{
	INCLUDE = 1,
	EXCLUDE = 0,
	UNSPECIFIED = 2
};

bool isPreferredMemoryRegion(const MEMORY_BASIC_INFORMATION& mbi, const std::uint64_t protectionMask);

std::vector<MemoryRegion> getPreferredMemoryRegions(const std::uint64_t flags);