#include "pch.h"
#include "LuaBindings.h"

//regexScanStrings(pattern, protectionFlags, encodingStr, regexEngineStr, bfindOne, bCaseSensitive, minLength, maxLength)
int __cdecl l_regexScanStrings(lua_State* luaState)
{
    try
    {
        ScanRequest request = parseScanRequest(luaState, 1);

        ScanServiceResult scanRresult = performScan(request);

        switch (scanRresult.status)
        {
		case ScanStatus::SUCCESS:
            pushResultsTable(luaState, scanRresult.results);
            return 1;
        case ScanStatus::NO_PROCESS:
            #ifdef _DEBUG
                Exported.ShowMessage(const_cast<char*>("No process attached."));
            #endif
            return luaL_error(luaState, "No process attached.");;
        case ScanStatus::INVALID_ENCODING:
            #ifdef _DEBUG
                Exported.ShowMessage(const_cast<char*>("Unsupported encoding passed."));
            #endif
            return luaL_error(luaState, "Invalid or unsupported encoding.");
        case ScanStatus::INVALID_REGEX_TYPE:
            #ifdef _DEBUG
                Exported.ShowMessage(const_cast<char*>("Wrong regex engine type passed."));
            #endif
            return luaL_error(luaState, "Invalid regex engine type.");
        case ScanStatus::NO_REGIONS:
            #ifdef _DEBUG
                Exported.ShowMessage(const_cast<char*>("No memory regions found with the specified flags."));
            #endif
            lua_pushnil(luaState);
            return 1;
        case ScanStatus::INVALID_CONFIG:
            #ifdef _DEBUG
                Exported.ShowMessage(const_cast<char*>("Invalid config passed."));
            #endif
            return luaL_error(luaState, "Invalid config received.");
        case ScanStatus::NO_RESULTS:
            #ifdef _DEBUG
                Exported.ShowMessage(const_cast<char*>("No results found."));
            #endif
            lua_pushnil(luaState);
            return 1;
        case ScanStatus::REGEX_COMPILATION_ERROR:
            #ifdef _DEBUG
                Exported.ShowMessage(const_cast<char*>("RegEx compilation failed."));
            #endif
            return luaL_error(luaState, "RegEx compilation failed.");
        case ScanStatus::SCAN_ERROR:
            #ifdef _DEBUG
                Exported.ShowMessage(const_cast<char*>("Unexpected scan error."));
            #endif
            return luaL_error(luaState, "Unexpected scan error.");
        default:
            #ifdef _DEBUG
                Exported.ShowMessage(const_cast<char*>("Unhandled scan status in bound function."));
            #endif
            return luaL_error(luaState, "Unexpected scan status.");
        }

    }
    catch (...)
    {
#ifdef _DEBUG
        Exported.ShowMessage(const_cast<char*>("Error occured in main lregex function."));
#endif
        lua_pushnil(luaState);
        return 1;
    }
}


int __cdecl l_pingPlugin(lua_State* luaState)
{
    lua_pushstring(luaState, "Ping from regex plugin");
    return 1;
}