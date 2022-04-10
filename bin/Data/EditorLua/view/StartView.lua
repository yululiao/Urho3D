local StartView = {}
StartView.isShow = true
local Translator = require("EditorLua/core/Translate")
StartView.app = nil
StartView.trigged = false

function StartView:RemoveHistoryItem(path)
    --self.app.RemoveHistoryItem();
end

function StartView:RenderHistoryList()
    imgui.TextUnformatted("Open Recent:")
    for k,v in ipairs(self.history.attributes._history) do
        -- body
        --ImGuiCol_Button
        imgui.PushStyleColor(imgui.constant.Col.Button,0x00ffffff)
        --ImGuiStyleVar_ButtonTextAlign
        --PushStyleVar
        --button text align left
        imgui.PushStyleVar_2(imgui.constant.StyleVar.ButtonTextAlign,0,0)
        if imgui.Button(v,300,25) then
            self.app.StartGame(v)
        end
        imgui.PopStyleVar(1)
        --imgui.SetNextWindowPos(320,height,0,0,0)
        imgui.SameLine();
        if imgui.Button("x",25,25) then
            self:RemoveHistoryItem(v)
        end
        
        imgui.PopStyleColor(1)
        
    end
end


function StartView:Update()
    if self.isShow == true then
        self.isShow = imgui.Begin(Translator:translate("StartView"),self.isShow)
        --local undotex = self.assetMgr:getImguiTex("res/img/undo.png")
        --imgui.Image(undotex,32,32)
        imgui.PushStyleVar_2(imgui.constant.StyleVar.ButtonTextAlign,0,0)
        if imgui.Button("Open...",80,25) then
            self.trigged = true;
        end
        imgui.PopStyleVar(1)
        imgui.Spacing()
        imgui.Spacing()
        imgui.Spacing()
        imgui.Separator()
        self:RenderHistoryList()
        if self.trigged then
            local path = self.app.DialogGetPath()
            if path ~= "" then
                self.app.StartGame(path)
            end
        end
        self.trigged = false
        imgui.End()
    end
    
end

function StartView:Show(show)
    self.isShow = show
end

return StartView