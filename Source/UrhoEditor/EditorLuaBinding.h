#pragma once
#include "LuaJIT/lua.h"

class EditorLuaBinding
{
public:
    static void LuaBinding(lua_State* L);
};