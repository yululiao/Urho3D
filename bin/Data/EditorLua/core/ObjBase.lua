local ObjBase = {}

function ObjBase.new(base)
    local obj = {}
    setmetatable(obj, base)
    base.__index = base
    return obj
end

return ObjBase