local Inspector = {}
Inspector.isShow = true
local Translator = require("EditorLua/core/Translate")

function Inspector:Update()
    if self.isShow == true then
        self.isShow = imgui.Begin(Translator:translate("Inspector"),self.isShow)
        imgui.End()
    end
end

function Inspector:Show(show)
    self.isShow = show
end

return Inspector