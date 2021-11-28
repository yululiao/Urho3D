import os

cmd_str = "tolua++ -L ToCppHook.lua -o  gen/AppLuaAPI.cpp AppLuaAPI.pkg"

os.system(cmd_str)

cmd_str = "tolua++ -L ToCppHook.lua -o gen/EditorUiLuaAPI.cpp EditorUiLuaAPI.pkg"

os.system(cmd_str)