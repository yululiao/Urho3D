import os

cmd_str = "tolua++ -L ToCppHook.lua -o  gen/EditorLuaAPI.cpp EditorLuaAPI.pkg"

os.system(cmd_str)