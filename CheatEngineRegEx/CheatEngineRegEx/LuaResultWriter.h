#pragma once
#include "pch.h"
#include <vector>
#include "ScanTypes.h"

void pushResultsTable(lua_State* luaState, const std::vector<ScanResult>& results);