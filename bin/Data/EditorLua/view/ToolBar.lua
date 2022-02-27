local ToolBar = {}
local Translator = require("EditorLua/core/Translate")

ToolBar.isShow = true

function ToolBar:Update()
    if self.isShow == true then
        self.isShow = imgui.Begin(Translator:translate("ToolBar"),self.isShow)
        
        imgui.End()
    end
end

function ToolBar:Show(show)
    self.isShow = show
end

return ToolBar