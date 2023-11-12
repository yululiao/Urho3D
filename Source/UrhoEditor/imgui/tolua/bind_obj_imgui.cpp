#include "bind_obj_imgui.hpp"


void LoadImguiBindings(lua_State* gLuaState)
{
    lua_bind::bind_imgui(gLuaState);
}
