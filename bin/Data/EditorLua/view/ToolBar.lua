local ToolBar = {}
local Translator = require("EditorLua/core/Translate")
ToolBar.itemSize = 20

function ToolBar:init()
    self.isShow = true
    self.curToolName = "camera"--"move" "rotate" "scale"
    self.selectedColor = 0x77999999
end

function ToolBar:OnSave()
    self.assetMgr:SaveScene(self.app.assetRoot .."/test.scene")
end

function ToolBar:OnUndo()
end

function ToolBar:OnRedo()
end

function ToolBar:OnNewScene()
    self.assetMgr:OpenNewScene()
end

function ToolBar:OnTransformTool(name)
    self.curToolName = name
    self.app.cppApp:setCurTool(name)
end

function ToolBar:DrawTransformTool()
    local toolList = {"camera","move","rotate","scale"}
    local imgList = {}
    imgList[1] = self.assetMgr:getImguiTex("res/img/hand.png")
    imgList[2] = self.assetMgr:getImguiTex("res/img/move.png")
    imgList[3] = self.assetMgr:getImguiTex("res/img/rotate.png")
    imgList[4] = self.assetMgr:getImguiTex("res/img/scale.png")
    local curColor = 0x00ffffff;
    for k,v in ipairs(toolList) do
        if v == self.curToolName then
            curColor = self.selectedColor
        else
            curColor = 0x00ffffff;
        end
        imgui.PushStyleColor(imgui.constant.Col.Button,curColor)
        imgui.PushStyleColor(imgui.constant.Col.ButtonHovered,0x77999999)
        imgui.SameLine()
        if imgui.ImageButton(imgList[k],self.itemSize,self.itemSize) then
           self:OnTransformTool(v)
        end
        if imgui.IsItemHovered(0) then
            imgui.BeginTooltip()
            imgui.SetTooltipText(v)
            imgui.EndTooltip()
        end

        imgui.PopStyleColor(2)
    end
end

function ToolBar:DrawTrigerTool()
    local toolList = {"newScene","save","undo","redo"}
    local handleList = {"OnNewScene","OnSave","OnUndo","OnRedo"}
    local imgList = {}
    imgList[1] = self.assetMgr:getImguiTex("res/img/grid.png")
    imgList[2] = self.assetMgr:getImguiTex("res/img/save.png")
    imgList[3] = self.assetMgr:getImguiTex("res/img/undo.png")
    imgList[4] = self.assetMgr:getImguiTex("res/img/redo.png")
    imgui.PushStyleColor(imgui.constant.Col.Button,0x00ffffff)
    imgui.PushStyleColor(imgui.constant.Col.ButtonHovered,0x77999999)
    for k,v in ipairs(toolList) do
        imgui.SameLine()
        if imgui.ImageButton(imgList[k],self.itemSize,self.itemSize) then
            self[handleList[k]](self)
        end
        if imgui.IsItemHovered(0) then
            imgui.BeginTooltip()
            imgui.SetTooltipText(v)
            imgui.EndTooltip()
        end
    end
    imgui.PopStyleColor(2)
end

function ToolBar:Update()
    if self.isShow == true then
        local winsizeX = imgui.GetWindowSize()
        self.isShow = imgui.BeginChild("ToolBar",winsizeX,self.itemSize+6)
        --self.isShow = imgui.Begin(Translator:translate("ToolBar"),self.isShow)
        self:DrawTrigerTool()
        self:DrawTransformTool()
        imgui.EndChild()
        --imgui.End()
    end
end

function ToolBar:Show(show)
    self.isShow = show
end

ToolBar:init()
return ToolBar