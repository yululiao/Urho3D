local NodeTree = {}
NodeTree.isShow = true
local Translator = require("EditorLua/core/Translate")
require("EditorLua/core/BitUtils")

local OnClicked = function(node)
    NodeTree.app.cppApp:SelectNode(node)
end

local OnDoubleClicked = function()
end

local DrawNode
DrawNode = function(node)
    local nodeName = node:GetName()
    --ImGuiTreeNodeFlags_Selected
    --ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    local flags = BitOr(imgui.constant.TreeNodeFlags.OpenOnArrow,imgui.constant.TreeNodeFlags.OpenOnDoubleClick)
    flags =  BitOr(flags,imgui.constant.TreeNodeFlags.SpanAvailWidth)
    local children = node:GetChildren(false)
    if NodeTree.app.cppApp:GetSelectNode() and node:GetID() == NodeTree.app.cppApp:GetSelectNode():GetID() then
        flags =  BitOr(flags,imgui.constant.TreeNodeFlags.Selected)
    end
    if(#children == 0) then
        --ImGuiTreeNodeFlags_Leaf |ImGuiTreeNodeFlags_NoTreePushOnOpen
        flags =  BitOr(flags,imgui.constant.TreeNodeFlags.Leaf)
    end
    local node_open = imgui.TreeNodeEx(nodeName,flags)
    --events
    if imgui.IsItemClicked(0) then
        OnClicked(node)
    end
    if imgui.IsMouseDoubleClicked(0) and imgui.IsItemHovered(0) then
        OnDoubleClicked(path)
    end
    --events end
    if node_open then
        --children
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