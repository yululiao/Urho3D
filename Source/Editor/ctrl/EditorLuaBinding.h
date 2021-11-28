#pragma once
#include "LuaJIT/lua.h"

namespace urho3d
{
namespace editor
{

class EditorLuaBinding
{
public:
    static void LuaBinding(lua_State* L);
};

}
}