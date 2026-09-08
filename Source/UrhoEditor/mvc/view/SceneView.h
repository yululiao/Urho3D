#pragma once
#include <functional>
#include "EditorWidget.h"
#include "ctrl/scene/SelectionController.h"
#include "ctrl/scene/ToolController.h"
#include "ctrl/scene/GizmoController.h"
#include "ctrl/scene/CameraCtrl.h"
#include "ctrl/scene/SceneManipulationController.h"
#include "ctrl/scene/SceneCtrl.h"


namespace Urho3DEditor
{
class SceneView:public EditorWidget
{
public:
    using FpsProvider = std::function<int()>;

    SceneView(SelectionController& selectionCtrl, ToolController& toolCtrl, GizmoController& gizmoCtrl, CameraCtrl& cameraCtrl, SceneManipulationController& sceneManipCtrl, SceneCtrl& sceneCtrl, FpsProvider fpsProvider, const std::string& ptitle)
        : selectionController_(selectionCtrl)
        , toolController_(toolCtrl)
        , gizmoController_(gizmoCtrl)
        , cameraController_(cameraCtrl)
        , sceneManipController_(sceneManipCtrl)
        , sceneCtrl_(sceneCtrl)
        , fpsProvider_(fpsProvider)
    {
        title = ptitle;
        winSize = ImVec2(400, 300);
    }
    virtual ~SceneView();
    void Update() override;
    void GenGpuTex();
    void DelteGpuTex();
    void OnIO();
    void WheelEvent(float deta);
    void MouseHoverEvent(Vector2 pos);
    void MouseMoveEvent(Vector2 pos);
    void MouseReleaseEvent(Vector2 pos);
    void MousePressEvent(Vector2 pos);

protected:
    SelectionController& selectionController_;
    ToolController& toolController_;
    GizmoController& gizmoController_;
    CameraCtrl& cameraController_;
    SceneManipulationController& sceneManipController_;
    SceneCtrl& sceneCtrl_;
    FpsProvider fpsProvider_;
    GLuint rttTexID = 0;
    bool _is_mouse_pressed = false;
    bool _is_mouse_moved = false;
};
}