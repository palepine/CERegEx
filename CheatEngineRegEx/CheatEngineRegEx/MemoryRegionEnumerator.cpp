#include "pch.h"
#include "MemoryRegionEnumerator.h"

bool isPreferredMemoryRegion(const MEMORY_BASIC_INFORMATION& mbi, const std::uint64_t protectionMask)
{
	// https://learn.microsoft.com/en-us/windows/win32/memory/memory-protection-constants

	if (mbi.State != MEM_COMMIT)
		return false;

	if (mbi.Protect & PAGE_GUARD)
		return false;

	if (mbi.Protect & PAGE_NOACCESS)
		return false;

	const DWORD baseProtect = mbi.Protect & 0xFF;
	return (protectionMask & baseProtect) != 0;
}

std::vector<MemoryRegion> getPreferredMemoryRegions(const std::uint64_t flags)
{
	SYSTEM_INFO sysInfo;
	GetNativeSystemInfo(&sysInfo);

	std::vector<MemoryRegion> regions;

	std::uintptr_t addr = reinterpret_cast<std::uintptr_t>(sysInfo.lpMinimumApplicationAddress);
	const std::uintptr_t maxAddr = reinterpret_cast<std::uintptr_t>(sysInfo.lpMaximumApplicationAddress);

	while (addr < maxAddr)
	{
		MEMORY_BASIC_INFORMATION mbi{};

		if (!VirtualQueryEx(getProcessHandle(), reinterpret_cast<LPVOID>(addr), &mbi, sizeof(mbi)))
			break;

		if (isPreferredMemoryRegion(mbi, flags))
		{
			regions.push_back({ reinterpret_cast<std::uintptr_t>(mbi.BaseAddress), mbi.RegionSize, mbi.Protect, mbi.State, mbi.Type });
		}

		addr += mbi.RegionSize;
	}

	return regions;
}
