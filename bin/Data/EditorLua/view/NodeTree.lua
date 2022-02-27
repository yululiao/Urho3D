local NodeTree = {}
NodeTree.isShow = true
local Translator = require("EditorLua/core/Translate")

function NodeTree:Update()
    if self.isShow == true then
        self.isShow = imgui.Begin(Translator:translate("NodeTree"),self.isShow)
        imgui.End()
    end
end

function NodeTree:Show(show)
    self.isShow = show
end

return NodeTree