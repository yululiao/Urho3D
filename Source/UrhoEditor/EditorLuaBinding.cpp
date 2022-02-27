#include "EditorLuaBinding.h"

extern int luaopen_EditorLuaAPI(lua_State* tolua_S);

void EditorLuaBinding::LuaBinding(lua_State* L) 
{ 
	luaopen_EditorLuaAPI(L);
}
