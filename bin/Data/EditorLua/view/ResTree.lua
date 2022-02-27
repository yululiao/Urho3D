local ResTree = {}
ResTree.isShow = true
local Translator = require("EditorLua/core/Translate")
local EditorUtils = require("EditorLua/core/EditorUtils")

local drawResTree
drawResTree = function(path)
    local dirs = fileSystem:ScanDir(path,"",2,false)
    local pathItems = EditorUtils.StrSplit(path, "/")
    if #pathItems == 0 then
         return nil
    end
    local name = pathItems[#pathItems]
    if imgui.TreeNode(name) then
        for i=1,#dirs,1 do
            if dirs[i] ~= "." and dirs[i] ~= ".." then
                drawResTree(path.."/"..dirs[i])
            end
        end
        imgui.TreePop()
    end
 end

function ResTree:Update()
    if self.isShow == true then
        self.isShow = imgui.Begin(Translator:translate("ResTree"),self.isShow)
        drawResTree(self.app.assetRoot)
        imgui.End()
    end
end



function ResTree:Show(show)
    self.isShow = show
end

return ResTree