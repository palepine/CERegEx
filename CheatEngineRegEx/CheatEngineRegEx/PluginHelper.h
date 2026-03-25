#pragma once
#include "pch.h"
#include "PluginContext.h"

inline HANDLE getProcessHandle()
{
	return *(Exported.OpenedProcessHandle);
}