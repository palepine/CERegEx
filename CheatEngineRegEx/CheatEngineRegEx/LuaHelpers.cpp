#include "pch.h"
#include "LuaHelpers.h"

#define MIN_STRING_LENGTH 3
#define MAX_STRING_LENGTH 1024
namespace
{
    std::string getRequiredLuaStringField(lua_State* luaState, int tableIndex, const char* key)
    {
        lua_getfield(luaState, tableIndex, key);

        if ( lua_isnil(luaState, -1) )
        {
            lua_pop(luaState, 1);
            luaL_error(luaState, "Missing required field '%s'", key);
            return {};
        }

        if ( !lua_isstring(luaState, -1) )
        {
            lua_pop(luaState, 1);
            luaL_error(luaState, "Field '%s' must be a string", key);
            return {};
        }

        std::string result = lua_tostring(luaState, -1);
        lua_pop(luaState, 1);
        return result;
    }

    std::string getOptionalLuaStringField(lua_State* luaState, int tableIndex, const char* key, const std::string& defaultValue)
    {
        lua_getfield(luaState, tableIndex, key);

        std::string result = defaultValue;

        if ( !lua_isnil(luaState, -1) )
        {
            if ( lua_isstring(luaState, -1) )
            {
                result = lua_tostring(luaState, -1);
            }
        }

        lua_pop(luaState, 1);
        return result;
    }

    bool getOptionalLuaBoolField(lua_State* luaState, int tableIndex, const char* key, bool defaultValue)
    {
        lua_getfield(luaState, tableIndex, key);

        bool result = defaultValue;

        if ( !lua_isnil(luaState, -1) )
        {
            if ( lua_isboolean(luaState, -1) )
            {
                result = lua_toboolean(luaState, -1) != 0;
            }
            else if (lua_isnumber(luaState, -1))
            {
                result = lua_toboolean(luaState, -1) != 0;
            }
        }

        lua_pop(luaState, 1);
        return result;
    }

    int getOptionalLuaIntField(lua_State* luaState, int tableIndex, const char* key, int defaultValue)
    {
        lua_getfield(luaState, tableIndex, key);

        int result = defaultValue;

        if ( !lua_isnil(luaState, -1) )
        {
            if ( lua_isinteger(luaState, -1) )
            {
                result = static_cast<int>( lua_tointeger(luaState, -1) );
            }
            else if ( lua_isnumber(luaState, -1) )
            {
                result = static_cast<int>( lua_tonumber(luaState, -1) );
            }
        }

        lua_pop(luaState, 1);
        return result;
    }
}

ScanRequest parseScanRequest(lua_State* luaState, int tableIndex)
{
    ScanRequest request;
    luaL_checktype(luaState, tableIndex, LUA_TTABLE);
    tableIndex = lua_absindex(luaState, tableIndex);

    request.pattern = getRequiredLuaStringField(luaState, tableIndex, "pattern");
    request.protectionFlags = getOptionalLuaStringField(luaState, tableIndex, "protection", "");
    request.encodingText = getOptionalLuaStringField(luaState, tableIndex, "encoding", "ASCII");
    request.regexEngineTypeText = getOptionalLuaStringField(luaState, tableIndex, "engine", "RE2");
    request.findOne = getOptionalLuaBoolField(luaState, tableIndex, "findOne", false);
    request.isCaseSensitive = getOptionalLuaBoolField(luaState, tableIndex, "caseSensitive", false);
    request.minLength = getOptionalLuaIntField(luaState, tableIndex, "minLength", MIN_STRING_LENGTH);
    request.maxLength = getOptionalLuaIntField(luaState, tableIndex, "maxLength", MAX_STRING_LENGTH);

    return request;
}
