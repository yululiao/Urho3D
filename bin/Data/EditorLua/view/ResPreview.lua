local ResPreview = {}
ResPreview.isShow = true
local Translator = require("EditorLua/core/Translate")

function ResPreview:Update()
    if self.isShow == true then
        self.isShow = imgui.Begin(Translator:translate("ResPreview"),self.isShow)
        imgui.End()
    end
    
end

function ResPreview:Show(show)
    self.isShow = show
end

return ResPreview