#pragma once

#include "Urho3D/Scene/Node.h"
#include "Urho3D/Scene/Scene.h"
#include "view/gizmo/TransformCtrl.h"

namespace Urho3DEditor {

using namespace Urho3D;

class CmdMgr;

class GizmoController : public IGizmoDragHandler
{
public:
    GizmoController(Context* context, Node* gizmoRoot, Scene* scene, Node* cameraNode, CmdMgr& cmdMgr);
    ~GizmoController();

    void attach(Node* obj);
    void detach();
    void setMode(eTransformCtrlMode mode);
    void pause(bool paused);
    void update();

    void onPointerDown(float x, float y);
    void onPointerHover(float x, float y);
    void onPointerMove(float x, float y);
    void onPointerUp(float x, float y, bool isTempCamera = false);

    bool isDraging() const;
    Node* getAttachedNode() const;

    // IGizmoDragHandler
    void OnGizmoDragStart(const std::string& cmdName, const std::string& axis, const Vector3& offset,
                          const Vector3& startPos, const Vector3& startScale,
                          const Matrix4& oriRotationMatrix, const Matrix4& oriworldRotationMatrix,
                          const Matrix4& oriworldRotationMatrixInverse, const Vector3& oriworldPos,
                          const Vector3& oriworldScale, const Matrix4& oriparentRotationMatrix,
                          const Vector3& oriparentScale) override;
    void OnGizmoDragMove(float x, float y, eTransformCtrlMode mode, const std::string& axis) override;
    void OnGizmoDragEnd() override;

private:
    TransformCtrl* transformCtrl_ = nullptr;

    std::string cmdName_;
    std::string axis_;
    Vector3 offset_;
    Vector3 startPos_;
    Vector3 startScale_;
    Matrix4 oriRotationMatrix_;
    Matrix4 oriworldRotationMatrix_;
    Matrix4 oriworldRotationMatrixInverse_;
    Vector3 oriworldPos_;
    Vector3 oriworldScale_;
    Matrix4 oriparentRotationMatrix_;
    Vector3 oriparentScale_;

    CmdMgr& cmdMgr_;

    void translate(float x, float y);
    void scale(float x, float y);
    void rotate(float x, float y);
};

} // namespace Urho3DEditor
