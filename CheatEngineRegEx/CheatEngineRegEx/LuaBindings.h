#pragma once
#include "pch.h"
#include "ScanService.h"
#include "LuaResultWriter.h"
#include "LuaHelpers.h"
#include "ScanOptionParsing.h"
#include <string>

int __cdecl l_regexScanStrings(lua_State* luaState);

int __cdecl l_pingPlugin(lua_State* luaState);