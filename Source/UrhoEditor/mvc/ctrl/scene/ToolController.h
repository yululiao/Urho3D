#pragma once

#include "Urho3D/Container/Str.h"
#include "model/ToolModel.h"
#include "GizmoController.h"

namespace Urho3DEditor {

using namespace Urho3D;

class ToolController
{
public:
    ToolController(ToolModel& model, GizmoController& gizmoCtrl);

    void SetTool(const String& tool);
    const String& GetCurrentTool() const { return model_.GetCurrentTool(); }

private:
    ToolModel& model_;
    GizmoController& gizmoCtrl_;
};

} // namespace Urho3DEditor
