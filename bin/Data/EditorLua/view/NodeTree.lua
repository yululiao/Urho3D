local NodeTree = {}
NodeTree.isShow = true
local Translator = require("EditorLua/core/Translate")

function NodeTree:Update()
    if self.isShow == true then
        self.isShow = imgui.Begin(Translator:translate("NodeTree"),self.isShow)
        if imgui.BeginPopupContextWindow("context",1) then
            imgui.MenuItem("delete","",false,true)
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