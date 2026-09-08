#pragma once

#include "Urho3D/Scene/Node.h"
#include "Urho3D/Math/Vector2.h"
#include "model/SelectionModel.h"
#include "GizmoController.h"
#include "ctrl/scene/SceneCtrl.h"

namespace Urho3DEditor {

using namespace Urho3D;

class SelectionController
{
public:
    SelectionController(SelectionModel& model, GizmoController& gizmoCtrl, SceneCtrl& sceneCtrl);

    void OnNodeClicked(Node* node);
    void OnFileSelected(const String& filePath);
    void OnSceneClicked(const Vector2& normalizedPos);
    void ClearSelection();

private:
    SelectionModel& model_;
    GizmoController& gizmoCtrl_;
    SceneCtrl& sceneCtrl_;
};

} // namespace Urho3DEditor
