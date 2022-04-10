local EditorUtils = require("EditorLua/core/EditorUtils")
local Translator = require("EditorLua/core/Translate")
local MenuBar = {}
MenuBar.menuTree = {children = {}}
MenuBar.app = nil

function MenuBar.onOpen()
    print("onOpenFile")
end
function MenuBar.onSave()
    --print("onOpen")
end

function MenuBar.onUndo()
    print("onUndo")
end
function MenuBar.onRedo()
    print("onRedo")
end

function MenuBar:init()
    self:addMenu("file/open", self.onOpen)
    self:addMenu("file/save", self.onSave)
    self:addMenu("edit/undo", self.onUndo)
    self:addMenu("edit/redo", self.onRedo)
    self:addMenu("view/ToolBar", self.app.ShowToolBar)
    self:addMenu("view/NodeTree", self.app.ShowNodeTree)
    self:addMenu("view/ResTree", self.app.ShowResTree)
    self:addMenu("view/ResPreview", self.app.ShowResPreview)
    self:addMenu("view/SceneView", self.app.ShowSceneView)
    self:addMenu("view/Inspector", self.app.ShowInspector)
    print("init MenuBar")
end

function MenuBar:addMenu(path,func)
    local pathItems = EditorUtils.StrSplit(path, "/")
    if #pathItems < 2 then
        return
    end
    local curTreeNode = self.menuTree
    for index = 1,#pathItems -1,1 do
        local item = pathItems[index]
        if curTreeNode.children[item] then
            curTreeNode = curTreeNode.children[item]
        else
            curTreeNode.children[item] = {}
            curTreeNode.children[item].children = {}
            curTreeNode.children[item].name = item
            curTreeNode = curTreeNode.children[item]
        end
    end
    --leaf node
    local leafName = pathItems[#pathItems]
    curTreeNode.children[leafName] = {}
    curTreeNode.children[leafName].children = {}
    curTreeNode.children[leafName].name = leafName
    curTreeNode.children[leafName].func = func
end

local renderMenu
renderMenu = function(menuNode)
    for key,value in pairs(menuNode.children) do
        local localName = Translator:translate(value.name)
        if value.func then
            local selected = false
            if imgui.MenuItem(localName,nil,selected) then
                value.func()
            end
        else
            if imgui.BeginMenu(localName) then
                renderMenu(value)
                imgui.EndMenu()
            end
        end
    end
end

function MenuBar:update()
    if imgui.BeginMenuBar() then
        renderMenu(self.menuTree)
        imgui.EndMenuBar()
    end
end

return MenuBar