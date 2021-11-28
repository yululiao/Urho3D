/*
** Lua binding: AppLuaAPI
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
TOLUA_API int tolua_AppLuaAPI_open (lua_State* tolua_S);

#include "Urho3D/Core/Context.h"
#include "Urho3D/Core/Main.h"
#include "Urho3D/Engine/Engine.h"
#include "ctrl/camera_ctrl.h"
#include "view/ui/EditorMenu.h"
#include "LuaScript/LuaFile.h"
#include "LuaScript/LuaScriptInstance.h"
#include "view/ui/EditorMenu.h"
#include "editor_app.h"
using namespace urho3d::editor;

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"editor_app");
 tolua_usertype(tolua_S,"Object");
 tolua_usertype(tolua_S,"EditorMenu");
}

/* method: get_instance of class  editor_app */
#ifndef TOLUA_DISABLE_tolua_AppLuaAPI_editor_app_get_instance00
static int tolua_AppLuaAPI_editor_app_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"editor_app",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  editor_app* tolua_ret = (editor_app*)  editor_app::get_instance();
  tolua_pushusertype(tolua_S,(void*)tolua_ret,"editor_app");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddMenu of class  editor_app */
#ifndef TOLUA_DISABLE_tolua_AppLuaAPI_editor_app_AddMenu00
static int tolua_AppLuaAPI_editor_app_AddMenu00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"editor_app",0,&tolua_err) ||
 !tolua_isurho3dstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  editor_app* self = (editor_app*)  tolua_tousertype(tolua_S,1,0);
  const String path = ((const String)  tolua_tourho3dstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddMenu'", NULL);
#endif
 {
  EditorMenu* tolua_ret = (EditorMenu*)  self->AddMenu(path);
  tolua_pushusertype(tolua_S,(void*)tolua_ret,"EditorMenu");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddMenu'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_AppLuaAPI_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 tolua_cclass(tolua_S,"editor_app","editor_app","Object",NULL);
 tolua_beginmodule(tolua_S,"editor_app");
  tolua_function(tolua_S,"get_instance",tolua_AppLuaAPI_editor_app_get_instance00);
  tolua_function(tolua_S,"AddMenu",tolua_AppLuaAPI_editor_app_AddMenu00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_AppLuaAPI (lua_State* tolua_S) {
 return tolua_AppLuaAPI_open(tolua_S);
}
#endif

#if __clang__
#pragma clang diagnostic pop
#endif