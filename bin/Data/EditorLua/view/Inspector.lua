local Inspector = {}
Inspector.isShow = true
local Translator = require("EditorLua/core/Translate")
local TransformIns = require("EditorLua/view/inspectors/TransformInspector")
local AniModelIns = require("EditorLua/view/inspectors/AniModelInspector")
Inspector.nodeEnable = true

function Inspector:Update()
    TransformIns.app = self.app
    if self.isShow == true then
        self.isShow = imgui.Begin(Translator:translate("Inspector"),self.isShow)
        if self.app.cppApp:GetSelectNode() ~= nil then
            self.nodeEnable = self.app.cppApp:GetSelectNode():IsEnabled()
            local tmpBool
            tmpBool,self.nodeEnable = imgui.Checkbox("Enable",self.nodeEnable)
            TransformIns:Update()
            local aniModel = self.app.cppApp:GetSelectNode():GetComponent("AnimatedModel",false)
            if aniModel ~= nil then
                AniModelIns:Update()
            end
            if self.nodeEnable ~= self.app.cppApp:GetSelectNode():IsEnabled() then
                --self.app.cppApp:MakeCurent()
                self.app.cppApp:GetSelectNode():SetEnabled(self.nodeEnable)
            end
        end
        imgui.End()
    end
end

function Inspector:Show(show)
    self.isShow = show
end

return Inspector