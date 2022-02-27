local EditMgr = {}

function EditMgr:new()
    local obj = {}
    setmetatable(obj, self)
    self.__index = self
    obj.editNodeList = {}
    obj.curNodeId = 0
    return obj
end

function EditMgr:ToDo(cmd)
    if cmd == nil then
        return
    end
    if not cmd:reversible() then--不可恢复的操作不入操作队列
        cmd:ToDo()
        return
    end
    --每次do操作后，需要将当前游标后的节点全部删除，即不可redo
    local deleteCount = #self.editNodeList.size() - self.curNodeId;
    for i = self.curNodeId+1,deleteCount,1 do
        self.editNodeList[i] = nil
    end
    self:AddNode(cmd)
    --todo 发出事件
end
function EditMgr:UnDo()
    local nodes = self:GetUndoNodes()
    for key,value in pairs(nodes) do
        value:UnDo();
        self.curNodeId = self.curNodeId -1
    end
end

function EditMgr:ReDo()
    local nodes = self:GetRedoNodes()
    for key,value in pairs(nodes) do
        value:Redo()
        self.curNodeId = self.curNodeId + 1
    end
end

function EditMgr:CanUndo()
    local result = false
    if #self.editNodeList > 0 and self.curNodeId > 0 then
        result = true
    end
    return result
end

function EditMgr:CanRedo()
    local result = false
    if #self.editNodeList > 0 and self.curNodeId < #self.editNodeList then
        result = true
    end
    return result
end

function EditMgr:OnSave()
    self.clear()
end

function EditMgr:HasEditNode()
    local result = false
    if #self.editNodeList > 0 then
        result = true
    end
    return result
end

function EditMgr:GetUndoNodes()
    local result = {}
    local i = self.curNodeId
    result[#result+1] = self.editNodeList[i]
    if self.editNodeList[i].name ~= nil and self.editNodeList[i].name ~= "" then
        i = i-1
        while i>0 and self.editNodeList[i].name do
            result[#result+1] = self.editNodeList[i]
            i = i-1
        end
    end
    return result
end

function EditMgr:GetRedoNodes()
    local result = {}
    local i = self.curNodeId + 1
    result[#result+1] = self.editNodeList[i]
    if self.editNodeList[i].name ~= nil and self.editNodeList[i].name ~= "" then
        i = i+1
        while i <= #self.editNodeList and self.editNodeList[i].name do
            result[#result+1] = self.editNodeList[i]
            i = i+1
        end
    end
    return result
end

function EditMgr:AddNode(cmd)
    self.editNodeList[#self.editNodeList+1] = cmd
    self.curNodeId = self.curNodeId + 1
end

function EditMgr:clear()
    self.curNodeId = 0
    local nodeCount = #self.editNodeList
    for i = 1,nodeCount,1 do
        -- body
        self.editNodeList[i] = nil
    end
end


return EditMgr