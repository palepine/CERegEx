#include "pch.h"
#include "LuaResultWriter.h"

#define RESULT_FIELDS 2

void pushResultsTable(lua_State* luaState, const std::vector<ScanResult>& results)
{
    // outer table. narr for arrays, nrec for non-array elems (hashed values)
    lua_createtable(luaState, static_cast<int>( results.size() ), RESULT_FIELDS);

    std::size_t indx{ 1 };

    for (const auto& res : results)
    {
        // inner table for each
        lua_newtable(luaState);

        lua_pushinteger(luaState, res.address);
        lua_setfield(luaState, -2, "address"); // -1 is the top of the stack, the newlypushed int, we take the table below it

        lua_pushstring(luaState, res.text.c_str());
        lua_setfield(luaState, -2, "text");

        lua_rawseti(luaState, -2, indx++);
    }

}
