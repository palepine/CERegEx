#include "pch.h"
#include "ProcessMemoryReader.h"

std::vector<char> readRegionMemory(const MemoryRegion& region)
{
	std::vector<char> buffer(region.size);
	SIZE_T bytesRead = 0;

	if (ReadProcessMemory(getProcessHandle(), reinterpret_cast<LPCVOID>(region.base), reinterpret_cast<LPVOID>(buffer.data()), region.size, &bytesRead))
	{
		buffer.resize(bytesRead);
	}

	return buffer;
}