local MenuBar = require "EditorLua/view/MenuBar"
local ToolBar = require "EditorLua/view/ToolBar"
local NodeTree = require "EditorLua/view/NodeTree"
local ResTree = require "EditorLua/view/ResTree"
local ResPreview = require "EditorLua/view/ResPreview"
local Inspector = require "EditorLua/view/Inspector"
local StartView = require"EditorLua/view/StartView"
local Json = require"json"

local EditorLuaApp = {}

local app = EditorApp.getInstance(EditorApp)
local assetMgr = AssetMgr.getInstance(AssetMgr)

EditorLuaApp.cppApp = app;

local historyFile = "res/editor_historys.json"
local json_str = assetMgr:getTextFile(historyFile)
StartView.history = Json.decode(json_str)

local uiUpdater = app:getUiUpdater()
local menuUpdater = app:getManuBarUpdater()
MenuBar.app = EditorLuaApp
ResTree.app = EditorLuaApp
NodeTree.app = EditorLuaApp
ResTree.assetMgr = assetMgr
StartView.app = EditorLuaApp
ToolBar.app = EditorLuaApp
ToolBar.assetMgr = assetMgr
StartView.assetMgr = assetMgr
EditorLuaApp.toolBar = ToolBar

local isStartView = true;

function onUiUpdate()
    if isStartView then
        StartView:Update()
    else
        NodeTree:Update()
        ResTree:Update()
        ResPreview:Update()
        Inspector:Update()
        --ToolBar:Update()
    end
    -- body
end

function onUpdateMenuBar()
    if isStartView == false then
        ToolBar:Update()
        MenuBar:update()
    end
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
    app:showSceneView(true)
   print("showSceneView")
end
function EditorLuaApp.ShowInspector()
    Inspector:Show(true)
 end

 function EditorLuaApp.DialogGetPath()
    local path = app:dialogSelectPath()
    return path
end

function EditorLuaApp.StartGame(path)
    app:openWorkSpace(path)
    EditorLuaApp.workSpace = app:getWorkSpace()
    EditorLuaApp.assetRoot = EditorLuaApp.workSpace.."/assets"
    isStartView = false
    app:startGame()
end

function EditorLuaApp.Run()
    MenuBar:init()
    SubscribeToEvent(uiUpdater, "imguiUpdate", "onUiUpdate")
    SubscribeToEvent(menuUpdater, "menuBarUpdate", "onUpdateMenuBar")
end


return EditorLuaApp