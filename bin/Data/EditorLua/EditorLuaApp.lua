local MenuBar = require "EditorLua/view/MenuBar"
local ToolBar = require "EditorLua/view/ToolBar"
local NodeTree = require "EditorLua/view/NodeTree"
local ResTree = require "EditorLua/view/ResTree"
local ResPreview = require "EditorLua/view/ResPreview"
local Inspector = require "EditorLua/view/Inspector"

local EditorLuaApp = {}
EditorLuaApp.workSpace = "D:/Urho3D/platform/projects/test"
EditorLuaApp.assetRoot = EditorLuaApp.workSpace.."/assets"
local app = EditorApp.getInstance(EditorApp)
local uiUpdater = app:getUiUpdater()
local menuUpdater = app:getManuBarUpdater()
MenuBar.app = EditorLuaApp
ResTree.app = EditorLuaApp

function onUiUpdate()
    ToolBar:Update()
    NodeTree:Update()
    ResTree:Update()
    ResPreview:Update()
    Inspector:Update()
    -- body
end

function onUpdateMenuBar()
    MenuBar:update()
    -- body
end

function EditorLuaApp.ShowToolBar()
    ToolBar:Show(true)
end

function EditorLuaApp.ShowNodeTree()
    NodeTree:Show(true)
end

function EditorLuaApp.ShowResTree()
    ResTree:Show(true)
end
function EditorLuaApp.ShowResPreview()
    ResPreview:Show(true)
end
function EditorLuaApp.ShowSceneView()
   print("showSceneView")
end
function EditorLuaApp.ShowInspector()
    Inspector:Show(true)
 end


function EditorLuaApp.Run()
    MenuBar:init()
    SubscribeToEvent(uiUpdater, "imguiUpdate", "onUiUpdate")
    SubscribeToEvent(menuUpdater, "menuBarUpdate", "onUpdateMenuBar")
end

return EditorLuaApp