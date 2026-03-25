#include "pch.h"
#include "ProtectionFlags.h"

namespace
{
	std::uint64_t buildMaskProtectionFlags(const ProtectionFlags& flags)
	{

		constexpr std::uint64_t categoryAll =
			PAGE_EXECUTE |
			PAGE_READONLY |
			PAGE_EXECUTE_READ |
			PAGE_EXECUTE_READWRITE |
			PAGE_EXECUTE_WRITECOPY |
			PAGE_READWRITE |
			PAGE_WRITECOPY;

		constexpr std::uint64_t categoryWritable =
			PAGE_READWRITE |
			PAGE_WRITECOPY |
			PAGE_EXECUTE_READWRITE |
			PAGE_EXECUTE_WRITECOPY;

		constexpr std::uint64_t categoryReadable =
			PAGE_READONLY |
			PAGE_EXECUTE_READ |
			PAGE_READWRITE |
			PAGE_EXECUTE_READWRITE;

		constexpr std::uint64_t categoryExecutable =
			PAGE_EXECUTE |
			PAGE_EXECUTE_READ |
			PAGE_EXECUTE_READWRITE |
			PAGE_EXECUTE_WRITECOPY;

		constexpr std::uint64_t categoryCopyOnWrite =
			PAGE_WRITECOPY |
			PAGE_EXECUTE_WRITECOPY;

		std::uint64_t mask = categoryAll;

		if (flags.scanWritable == PreferredRegionFlag::INCLUDE)
			mask &= categoryWritable;
		else if (flags.scanWritable == PreferredRegionFlag::EXCLUDE)
			mask &= ~categoryWritable;

		if (flags.scanReadable == PreferredRegionFlag::INCLUDE)
			mask &= categoryReadable;
		else if (flags.scanReadable == PreferredRegionFlag::EXCLUDE)
			mask &= ~categoryReadable;

		if (flags.scanExecutable == PreferredRegionFlag::INCLUDE)
			mask &= categoryExecutable;
		else if (flags.scanExecutable == PreferredRegionFlag::EXCLUDE)
			mask &= ~categoryExecutable;

		if (flags.scanCopyOnWrite == PreferredRegionFlag::INCLUDE)
			mask &= categoryCopyOnWrite;
		else if (flags.scanCopyOnWrite == PreferredRegionFlag::EXCLUDE)
			mask &= ~categoryCopyOnWrite;

		return mask;

	}
}

std::uint64_t parseProtectionFlags(const char* flags)
{
	std::size_t len = strlen(flags);
	ProtectionFlags protectionFlags{};

	PreferredRegionFlag currentState = PreferredRegionFlag::UNSPECIFIED;

	for (int i{ 0 }; i < len; i++)
	{
		switch (toupper(flags[i]))
		{
		case '-':
			currentState = PreferredRegionFlag::EXCLUDE;
			break;
		case '+':
			currentState = PreferredRegionFlag::INCLUDE;
			break;
		case 'R':
			protectionFlags.scanReadable = currentState;
			currentState = PreferredRegionFlag::UNSPECIFIED;
			break;
		case 'W':
			protectionFlags.scanWritable = currentState;
			currentState = PreferredRegionFlag::UNSPECIFIED;
			break;
		case 'X':
			protectionFlags.scanExecutable = currentState;
			currentState = PreferredRegionFlag::UNSPECIFIED;
			break;
		case 'C':
			protectionFlags.scanCopyOnWrite = currentState;
			currentState = PreferredRegionFlag::UNSPECIFIED;
			break;
		}
	}

	return buildMaskProtectionFlags(protectionFlags);
}