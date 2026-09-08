#include "SelectionController.h"

namespace Urho3DEditor {

SelectionController::SelectionController(SelectionModel& model, GizmoController& gizmoCtrl, SceneCtrl& sceneCtrl)
    : model_(model)
    , gizmoCtrl_(gizmoCtrl)
    , sceneCtrl_(sceneCtrl)
{
}

void SelectionController::OnNodeClicked(Node* node)
{
    model_.SetSelectedNode(node);
    if (node)
        gizmoCtrl_.attach(node);
    else
        gizmoCtrl_.detach();
}

void SelectionController::OnFileSelected(const String& filePath)
{
    model_.SetSelectedFile(filePath);
}

void SelectionController::OnSceneClicked(const Vector2& normalizedPos)
{
    Node* hit = sceneCtrl_.Select(normalizedPos.x_, normalizedPos.y_);
    model_.SetSelectedNode(hit);
    if (hit)
        gizmoCtrl_.attach(hit);
    else
        gizmoCtrl_.detach();
}

void SelectionController::ClearSelection()
{
    model_.ClearSelection();
    gizmoCtrl_.detach();
}

} // namespace Urho3DEditor
