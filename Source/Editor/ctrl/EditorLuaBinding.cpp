#include "EditorLuaBinding.h"

extern int tolua_AppLuaAPI_open(lua_State* tolua_S);
extern int tolua_EditorUiLuaAPI_open(lua_State* tolua_S);

namespace urho3d
{
namespace editor
{

void EditorLuaBinding::LuaBinding(lua_State* L) 
{ 
	tolua_AppLuaAPI_open(L);
    tolua_EditorUiLuaAPI_open(L);
}

}
} // namespace urho3d