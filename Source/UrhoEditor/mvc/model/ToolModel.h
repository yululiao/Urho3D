#pragma once

#include "Urho3D/Container/Str.h"

namespace Urho3DEditor {

using namespace Urho3D;

class ToolModel
{
public:
    void SetCurrentTool(const String& tool) { currentTool_ = tool; }
    const String& GetCurrentTool() const { return currentTool_; }

private:
    String currentTool_ = "camera";
};

} // namespace Urho3DEditor
