#pragma once
#include <cstdint>
#include "MemoryRegionEnumerator.h"

struct ProtectionFlags
{
	PreferredRegionFlag scanWritable = PreferredRegionFlag::UNSPECIFIED;
	PreferredRegionFlag scanReadable = PreferredRegionFlag::UNSPECIFIED;
	PreferredRegionFlag scanExecutable = PreferredRegionFlag::UNSPECIFIED;
	PreferredRegionFlag scanCopyOnWrite = PreferredRegionFlag::UNSPECIFIED;
};

std::uint64_t parseProtectionFlags(const char* flags);