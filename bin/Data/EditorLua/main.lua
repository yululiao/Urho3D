--require("socket/core").tcp()
require("LuaPanda").start("127.0.0.1",8818);
local luaApp = require "EditorLua/EditorLuaApp"

luaApp.Run()
