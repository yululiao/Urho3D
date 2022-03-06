/*
** Lua binding: EditorLuaAPI
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
TOLUA_API int tolua_EditorLuaAPI_open (lua_State* tolua_S);

#include "LuaScript/LuaFile.h"
#include "LuaScript/LuaScriptInstance.h"
#include "ImguiUpdater.h"
#include "EditorApp.h"
#include "AssetMgr.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"EditorApp");
 tolua_usertype(tolua_S,"Object");
 tolua_usertype(tolua_S,"AssetMgr");
 tolua_usertype(tolua_S,"Scene");
 tolua_usertype(tolua_S,"MenuBarUpdater");
 tolua_usertype(tolua_S,"ImguiUpdater");
 tolua_usertype(tolua_S,"Node");
}

/* method: getInstance of class  EditorApp */
#ifndef TOLUA_DISABLE_tolua_EditorLuaAPI_EditorApp_getInstance00
static int tolua_EditorLuaAPI_EditorApp_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"EditorApp",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  EditorApp* tolua_ret = (EditorApp*)  EditorApp::getInstance();
  tolua_pushusertype(tolua_S,(void*)tolua_ret,"EditorApp");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getUiUpdater of class  EditorApp */
#ifndef TOLUA_DISABLE_tolua_EditorLuaAPI_EditorApp_getUiUpdater00
static int tolua_EditorLuaAPI_EditorApp_getUiUpdater00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EditorApp",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EditorApp* self = (EditorApp*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getUiUpdater'", NULL);
#endif
 {
  ImguiUpdater* tolua_ret = (ImguiUpdater*)  self->getUiUpdater();
  tolua_pushusertype(tolua_S,(void*)tolua_ret,"ImguiUpdater");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getUiUpdater'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getManuBarUpdater of class  EditorApp */
#ifndef TOLUA_DISABLE_tolua_EditorLuaAPI_EditorApp_getManuBarUpdater00
static int tolua_EditorLuaAPI_EditorApp_getManuBarUpdater00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EditorApp",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EditorApp* self = (EditorApp*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getManuBarUpdater'", NULL);
#endif
 {
  MenuBarUpdater* tolua_ret = (MenuBarUpdater*)  self->getManuBarUpdater();
  tolua_pushusertype(tolua_S,(void*)tolua_ret,"MenuBarUpdater");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getManuBarUpdater'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setCurTool of class  EditorApp */
#ifndef TOLUA_DISABLE_tolua_EditorLuaAPI_EditorApp_setCurTool00
static int tolua_EditorLuaAPI_EditorApp_setCurTool00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EditorApp",0,&tolua_err) ||
 !tolua_isurho3dstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EditorApp* self = (EditorApp*)  tolua_tousertype(tolua_S,1,0);
  const String name = ((const String)  tolua_tourho3dstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCurTool'", NULL);
#endif
 {
  self->setCurTool(name);
 tolua_pushurho3dstring(tolua_S,(const char*)name);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setCurTool'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRootNode of class  EditorApp */
#ifndef TOLUA_DISABLE_tolua_EditorLuaAPI_EditorApp_getRootNode00
static int tolua_EditorLuaAPI_EditorApp_getRootNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EditorApp",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EditorApp* self = (EditorApp*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRootNode'", NULL);
#endif
 {
  Node* tolua_ret = (Node*)  self->getRootNode();
  tolua_pushusertype(tolua_S,(void*)tolua_ret,"Node");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRootNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getScene of class  EditorApp */
#ifndef TOLUA_DISABLE_tolua_EditorLuaAPI_EditorApp_getScene00
static int tolua_EditorLuaAPI_EditorApp_getScene00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EditorApp",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EditorApp* self = (EditorApp*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getScene'", NULL);
#endif
 {
  Scene* tolua_ret = (Scene*)  self->getScene();
  tolua_pushusertype(tolua_S,(void*)tolua_ret,"Scene");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getScene'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: openWorkSpace of class  EditorApp */
#ifndef TOLUA_DISABLE_tolua_EditorLuaAPI_EditorApp_openWorkSpace00
static int tolua_EditorLuaAPI_EditorApp_openWorkSpace00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EditorApp",0,&tolua_err) ||
 !tolua_isurho3dstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EditorApp* self = (EditorApp*)  tolua_tousertype(tolua_S,1,0);
  const String path = ((const String)  tolua_tourho3dstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'openWorkSpace'", NULL);
#endif
 {
  self->openWorkSpace(path);
 tolua_pushurho3dstring(tolua_S,(const char*)path);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'openWorkSpace'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWorkSpace of class  EditorApp */
#ifndef TOLUA_DISABLE_tolua_EditorLuaAPI_EditorApp_getWorkSpace00
static int tolua_EditorLuaAPI_EditorApp_getWorkSpace00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EditorApp",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EditorApp* self = (EditorApp*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWorkSpace'", NULL);
#endif
 {
  String tolua_ret = (String)  self->getWorkSpace();
 tolua_pushurho3dstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWorkSpace'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAssetRoot of class  EditorApp */
#ifndef TOLUA_DISABLE_tolua_EditorLuaAPI_EditorApp_getAssetRoot00
static int tolua_EditorLuaAPI_EditorApp_getAssetRoot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EditorApp",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EditorApp* self = (EditorApp*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAssetRoot'", NULL);
#endif
 {
  String tolua_ret = (String)  self->getAssetRoot();
 tolua_pushurho3dstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAssetRoot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showSceneView of class  EditorApp */
#ifndef TOLUA_DISABLE_tolua_EditorLuaAPI_EditorApp_showSceneView00
static int tolua_EditorLuaAPI_EditorApp_showSceneView00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EditorApp",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EditorApp* self = (EditorApp*)  tolua_tousertype(tolua_S,1,0);
  bool show = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showSceneView'", NULL);
#endif
 {
  self->showSceneView(show);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showSceneView'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: startGame of class  EditorApp */
#ifndef TOLUA_DISABLE_tolua_EditorLuaAPI_EditorApp_startGame00
static int tolua_EditorLuaAPI_EditorApp_startGame00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EditorApp",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EditorApp* self = (EditorApp*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'startGame'", NULL);
#endif
 {
  self->startGame();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'startGame'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: dialogSelectPath of class  EditorApp */
#ifndef TOLUA_DISABLE_tolua_EditorLuaAPI_EditorApp_dialogSelectPath00
static int tolua_EditorLuaAPI_EditorApp_dialogSelectPath00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EditorApp",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EditorApp* self = (EditorApp*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'dialogSelectPath'", NULL);
#endif
 {
  String tolua_ret = (String)  self->dialogSelectPath();
 tolua_pushurho3dstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dialogSelectPath'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: dialogOpenFile of class  EditorApp */
#ifndef TOLUA_DISABLE_tolua_EditorLuaAPI_EditorApp_dialogOpenFile00
static int tolua_EditorLuaAPI_EditorApp_dialogOpenFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EditorApp",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EditorApp* self = (EditorApp*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'dialogOpenFile'", NULL);
#endif
 {
  String tolua_ret = (String)  self->dialogOpenFile();
 tolua_pushurho3dstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dialogOpenFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: dialogSaveFile of class  EditorApp */
#ifndef TOLUA_DISABLE_tolua_EditorLuaAPI_EditorApp_dialogSaveFile00
static int tolua_EditorLuaAPI_EditorApp_dialogSaveFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EditorApp",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EditorApp* self = (EditorApp*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'dialogSaveFile'", NULL);
#endif
 {
  self->dialogSaveFile();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dialogSaveFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInstance of class  AssetMgr */
#ifndef TOLUA_DISABLE_tolua_EditorLuaAPI_AssetMgr_getInstance00
static int tolua_EditorLuaAPI_AssetMgr_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"AssetMgr",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  AssetMgr* tolua_ret = (AssetMgr*)  AssetMgr::getInstance();
  tolua_pushusertype(tolua_S,(void*)tolua_ret,"AssetMgr");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTextFile of class  AssetMgr */
#ifndef TOLUA_DISABLE_tolua_EditorLuaAPI_AssetMgr_getTextFile00
static int tolua_EditorLuaAPI_AssetMgr_getTextFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"AssetMgr",0,&tolua_err) ||
 !tolua_isurho3dstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  AssetMgr* self = (AssetMgr*)  tolua_tousertype(tolua_S,1,0);
  const String path = ((const String)  tolua_tourho3dstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTextFile'", NULL);
#endif
 {
  String tolua_ret = (String)  self->getTextFile(path);
 tolua_pushurho3dstring(tolua_S,(const char*)tolua_ret);
 tolua_pushurho3dstring(tolua_S,(const char*)path);
 }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTextFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getImguiTex of class  AssetMgr */
#ifndef TOLUA_DISABLE_tolua_EditorLuaAPI_AssetMgr_getImguiTex00
static int tolua_EditorLuaAPI_AssetMgr_getImguiTex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"AssetMgr",0,&tolua_err) ||
 !tolua_isurho3dstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  AssetMgr* self = (AssetMgr*)  tolua_tousertype(tolua_S,1,0);
  const String path = ((const String)  tolua_tourho3dstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getImguiTex'", NULL);
#endif
 {
  int tolua_ret = (int)  self->getImguiTex(path);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 tolua_pushurho3dstring(tolua_S,(const char*)path);
 }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getImguiTex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_EditorLuaAPI_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 tolua_cclass(tolua_S,"ImguiUpdater","ImguiUpdater","Object",NULL);
 tolua_beginmodule(tolua_S,"ImguiUpdater");
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"MenuBarUpdater","MenuBarUpdater","Object",NULL);
 tolua_beginmodule(tolua_S,"MenuBarUpdater");
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"EditorApp","EditorApp","Object",NULL);
 tolua_beginmodule(tolua_S,"EditorApp");
  tolua_function(tolua_S,"getInstance",tolua_EditorLuaAPI_EditorApp_getInstance00);
  tolua_function(tolua_S,"getUiUpdater",tolua_EditorLuaAPI_EditorApp_getUiUpdater00);
  tolua_function(tolua_S,"getManuBarUpdater",tolua_EditorLuaAPI_EditorApp_getManuBarUpdater00);
  tolua_function(tolua_S,"setCurTool",tolua_EditorLuaAPI_EditorApp_setCurTool00);
  tolua_function(tolua_S,"getRootNode",tolua_EditorLuaAPI_EditorApp_getRootNode00);
  tolua_function(tolua_S,"getScene",tolua_EditorLuaAPI_EditorApp_getScene00);
  tolua_function(tolua_S,"openWorkSpace",tolua_EditorLuaAPI_EditorApp_openWorkSpace00);
  tolua_function(tolua_S,"getWorkSpace",tolua_EditorLuaAPI_EditorApp_getWorkSpace00);
  tolua_function(tolua_S,"getAssetRoot",tolua_EditorLuaAPI_EditorApp_getAssetRoot00);
  tolua_function(tolua_S,"showSceneView",tolua_EditorLuaAPI_EditorApp_showSceneView00);
  tolua_function(tolua_S,"startGame",tolua_EditorLuaAPI_EditorApp_startGame00);
  tolua_function(tolua_S,"dialogSelectPath",tolua_EditorLuaAPI_EditorApp_dialogSelectPath00);
  tolua_function(tolua_S,"dialogOpenFile",tolua_EditorLuaAPI_EditorApp_dialogOpenFile00);
  tolua_function(tolua_S,"dialogSaveFile",tolua_EditorLuaAPI_EditorApp_dialogSaveFile00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"AssetMgr","AssetMgr","Object",NULL);
 tolua_beginmodule(tolua_S,"AssetMgr");
  tolua_function(tolua_S,"getInstance",tolua_EditorLuaAPI_AssetMgr_getInstance00);
  tolua_function(tolua_S,"getTextFile",tolua_EditorLuaAPI_AssetMgr_getTextFile00);
  tolua_function(tolua_S,"getImguiTex",tolua_EditorLuaAPI_AssetMgr_getImguiTex00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_EditorLuaAPI (lua_State* tolua_S) {
 return tolua_EditorLuaAPI_open(tolua_S);
}
#endif

#if __clang__
#pragma clang diagnostic pop
#endif