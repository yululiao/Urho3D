#include "EditorLuaBinding.h"

extern int tolua_EditorCtrlLuaAPI_open(lua_State* tolua_S);
extern int tolua_EditorViewLuaAPI_open(lua_State* tolua_S);

namespace urho3d
{
namespace editor
{

void EditorLuaBinding::LuaBinding(lua_State* L) 
{ 
	tolua_EditorCtrlLuaAPI_open(L);
    tolua_EditorViewLuaAPI_open(L);
}

}
} // namespace urho3d