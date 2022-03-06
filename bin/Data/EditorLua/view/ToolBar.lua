local ToolBar = {}
local Translator = require("EditorLua/core/Translate")

ToolBar.isShow = true

function ToolBar:Update()
    if self.isShow == true then
        self.isShow = imgui.Begin(Translator:translate("ToolBar"),self.isShow)
        local undotex = self.assetMgr:getImguiTex("res/img/undo.png")
        imgui.ImageButton(undotex,28,28)
        imgui.End()
    end
end

function ToolBar:Show(show)
    self.isShow = show
end

return ToolBar