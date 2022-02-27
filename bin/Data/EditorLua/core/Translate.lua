local Translate = {}
Translate.localName = "china"
Translate.english = {}

Translate.china =
{
file = "文件",
save = "保存",
open = "打开",
edit = "编辑",
undo = "撤销",
redo = "重做",
view = "视图",
ToolBar = "工具栏",
NodeTree = "节点树",
ResTree = "资源",
ResPreview = "资源预览",
Inspector = "属性"

}

function Translate:translate(str)
    return self[self.localName][str] or str
end

function Translate:setLocal(name)
    -- body
    self.localName = name
end

return Translate