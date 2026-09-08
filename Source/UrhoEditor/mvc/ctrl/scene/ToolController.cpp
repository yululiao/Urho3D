#include "ToolController.h"

namespace Urho3DEditor {

ToolController::ToolController(ToolModel& model, GizmoController& gizmoCtrl)
    : model_(model)
    , gizmoCtrl_(gizmoCtrl)
{
}

void ToolController::SetTool(const String& tool)
{
    model_.SetCurrentTool(tool);
    if (tool == "move")
        gizmoCtrl_.setMode(eTransformCtrlMode::eTranslate);
    else if (tool == "rotate")
        gizmoCtrl_.setMode(eTransformCtrlMode::eRotate);
    else if (tool == "scale")
        gizmoCtrl_.setMode(eTransformCtrlMode::eScale);
    else if (tool == "camera")
        gizmoCtrl_.detach();
}

} // namespace Urho3DEditor
