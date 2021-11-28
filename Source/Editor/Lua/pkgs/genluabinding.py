import os

cmd_str = "tolua++ -L ToCppHook.lua -o  gen/EditorViewLuaAPI.cpp EditorViewLuaAPI.pkg"

os.system(cmd_str)

cmd_str = "tolua++ -L ToCppHook.lua -o gen/EditorCtrlLuaAPI.cpp EditorCtrlLuaAPI.pkg"

os.system(cmd_str)