local StartView = {}
StartView.isShow = true
local Translator = require("EditorLua/core/Translate")
StartView.app = nil
StartView.trigged = false


function StartView:Update()
    if self.isShow == true then
        self.isShow = imgui.Begin(Translator:translate("StartView"),self.isShow)
        --local undotex = self.assetMgr:getImguiTex("res/img/undo.png")
        --imgui.Image(undotex,32,32)
        if imgui.Button("Open...",80,25) then
            self.trigged = true;
        end
        if self.trigged then
            local path = self.app.DialogGetPath()
            if path ~= "" then
                self.app.StartGame(path)
            end
        end
        imgui.End()
    end
    
end

function StartView:Show(show)
    self.isShow = show
end

return StartView