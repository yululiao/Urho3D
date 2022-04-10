local NodeTree = {}
NodeTree.isShow = true
local Translator = require("EditorLua/core/Translate")

local OnClicked = function(id)
    NodeTree.app.cppApp:SelectNode(id)
end

local OnDoubleClicked = function()
end

local DrawNode
DrawNode = function(node)
    local nodeName = node:GetName()
    if imgui.TreeNode(nodeName) then
        --events
        if imgui.IsItemClicked(0) then
            OnClicked(node:GetID())
        end
        if imgui.IsMouseDoubleClicked(0) and imgui.IsItemHovered(0) then
            OnDoubleClicked(path)
        end
        --events end
        --children
        local children = node:GetChildren(false)
        for k,v in ipairs(children) do
            DrawNode(v)
        end
        imgui.TreePop()
    end
end

function NodeTree:Update()
    if self.isShow == true then
        self.isShow = imgui.Begin(Translator:translate("NodeTree"),self.isShow)
        DrawNode(self.app.cppApp:GetSceneRoot())
        if imgui.BeginPopupContextWindow("context",1) then
            imgui.MenuItem("create","",false,true)
            --imgui.BeginMenu sub menu
            imgui.EndPopup()
        end
        imgui.End()
    end
end

function NodeTree:Show(show)
    self.isShow = show
end

return NodeTree