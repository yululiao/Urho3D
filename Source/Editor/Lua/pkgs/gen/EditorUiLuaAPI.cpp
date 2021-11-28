/*
** Lua binding: EditorUiLuaAPI
*/

//
// Copyright (c) 2008-2021 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include <toluapp/tolua++.h>
#include <Urho3D/LuaScript/ToluaUtils.h>

#if __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#endif

/* Exported function */
TOLUA_API int tolua_EditorUiLuaAPI_open (lua_State* tolua_S);

#include "Urho3D/Core/Context.h"
#include "Urho3D/Core/Main.h"
#include "LuaScript/LuaFile.h"
#include "LuaScript/LuaScriptInstance.h"
#include "view/ui/EditorMenu.h"
using namespace urho3d::editor;

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"EditorMenu");
 tolua_usertype(tolua_S,"Object");
}

/* Open function */
TOLUA_API int tolua_EditorUiLuaAPI_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 tolua_cclass(tolua_S,"EditorMenu","EditorMenu","Object",NULL);
 tolua_beginmodule(tolua_S,"EditorMenu");
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_EditorUiLuaAPI (lua_State* tolua_S) {
 return tolua_EditorUiLuaAPI_open(tolua_S);
}
#endif

#if __clang__
#pragma clang diagnostic pop
#endif