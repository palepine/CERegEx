#include "pch.h"
#include "PluginContext.h"
#include "LuaBindings.h"

static char pluginName[] = "CERegEx";
int selfID;
ExportedFunctions Exported;
PExportedFunctions PExported;

BOOL __stdcall CEPlugin_GetVersion(PPluginVersion pv, int sizeofpluginversion)
{
    if (!pv) return FALSE;
    pv->version = CESDK_VERSION;
    pv->pluginname = (char*)pluginName;
    return TRUE;
}

BOOL __stdcall CEPlugin_InitializePlugin(PExportedFunctions ef, int pluginID)
{
    if (!ef)
        return FALSE;
    selfID = pluginID;
    Exported = *ef;
    PExported = ef;

    if (Exported.sizeofExportedFunctions != sizeof(Exported))
        return FALSE;

    lua_State* luaState = PExported->GetLuaState();
    if (!luaState)
        return FALSE;

    lua_register(luaState, "lregexScan", l_regexScanStrings);
    lua_register(luaState, "lregexPing", l_pingPlugin);
    return TRUE;
}

BOOL __stdcall CEPlugin_DisablePlugin(void)
{
    return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
