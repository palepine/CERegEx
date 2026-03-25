#pragma once
#include "pch.h"
#include <string>
#include "ScanTypes.h"

ScanRequest parseScanRequest(lua_State* luaState, int tableIndex);