local ToolBar = {}
local Translator = require("EditorLua/core/Translate")

ToolBar.isShow = true

function ToolBar:Update()
    if self.isShow == true then
        self.isShow = imgui.Begin(Translator:translate("ToolBar"),self.isShow)
        local undotex = self.assetMgr:getImguiTex("res/img/undo.png")
        imgui.PushStyleColor(imgui.constant.Col.Button,0x00ffffff)
        imgui.PushStyleColor(imgui.constant.Col.ButtonHovered,0x55555500)
        imgui.ImageButton(undotex,28,28)
        imgui.PopStyleColor(2)
        if imgui.IsItemHovered(0) then
            imgui.BeginTooltip()
            --imgui.TextUnformatted("undo","")
            imgui.SetTooltipText("undo")
            imgui.EndTooltip()
        end
        imgui.End()
    end
end

function ToolBar:Show(show)
    self.isShow = show
end

return ToolBar