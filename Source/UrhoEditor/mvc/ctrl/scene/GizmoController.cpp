#include "GizmoController.h"
#include "Urho3D/Graphics/Camera.h"
#include "ctrl/base/CmdDefines.h"
#include "ctrl/base/CmdMgr.h"
#include "Utils.h"

namespace Urho3DEditor {

GizmoController::GizmoController(Context* context, Node* gizmoRoot, Scene* scene, Node* cameraNode, CmdMgr& cmdMgr)
    : cmdMgr_(cmdMgr)
{
    transformCtrl_ = new TransformCtrl(context, eTransformCtrlMode::eTranslate, gizmoRoot, this);
    transformCtrl_->setScene(scene);
    transformCtrl_->setCameraNode(cameraNode);
}

GizmoController::~GizmoController()
{
    if (transformCtrl_)
    {
        delete transformCtrl_;
        transformCtrl_ = nullptr;
    }
}

void GizmoController::attach(Node* obj)
{
    if (transformCtrl_)
        transformCtrl_->attach(obj);
}

void GizmoController::detach()
{
    if (transformCtrl_)
        transformCtrl_->detach();
}

void GizmoController::setMode(eTransformCtrlMode mode)
{
    if (transformCtrl_)
        transformCtrl_->setMode(mode);
}

void GizmoController::pause(bool paused)
{
    if (transformCtrl_)
        transformCtrl_->pause(paused);
}

void GizmoController::update()
{
    if (transformCtrl_)
        transformCtrl_->update();
}

void GizmoController::onPointerDown(float x, float y)
{
    if (transformCtrl_)
        transformCtrl_->onPointerDown(x, y);
}

void GizmoController::onPointerHover(float x, float y)
{
    if (transformCtrl_)
        transformCtrl_->onPointerHover(x, y);
}

void GizmoController::onPointerMove(float x, float y)
{
    if (transformCtrl_)
        transformCtrl_->onPointerMove(x, y);
}

void GizmoController::onPointerUp(float x, float y, bool isTempCamera)
{
    if (transformCtrl_)
        transformCtrl_->onPointerUp(x, y, isTempCamera);
}

bool GizmoController::isDraging() const
{
    return transformCtrl_ ? transformCtrl_->isDraging() : false;
}

Node* GizmoController::getAttachedNode() const
{
    return transformCtrl_ ? transformCtrl_->getAttachedNode() : nullptr;
}

void GizmoController::OnGizmoDragStart(const std::string& cmdName, const std::string& axis, const Vector3& offset,
                                       const Vector3& startPos, const Vector3& startScale,
                                       const Matrix4& oriRotationMatrix, const Matrix4& oriworldRotationMatrix,
                                       const Matrix4& oriworldRotationMatrixInverse, const Vector3& oriworldPos,
                                       const Vector3& oriworldScale, const Matrix4& oriparentRotationMatrix,
                                       const Vector3& oriparentScale)
{
    cmdName_ = cmdName;
    axis_ = axis;
    offset_ = offset;
    startPos_ = startPos;
    startScale_ = startScale;
    oriRotationMatrix_ = oriRotationMatrix;
    oriworldRotationMatrix_ = oriworldRotationMatrix;
    oriworldRotationMatrixInverse_ = oriworldRotationMatrixInverse;
    oriworldPos_ = oriworldPos;
    oriworldScale_ = oriworldScale;
    oriparentRotationMatrix_ = oriparentRotationMatrix;
    oriparentScale_ = oriparentScale;
}

void GizmoController::OnGizmoDragMove(float x, float y, eTransformCtrlMode mode, const std::string& axis)
{
    switch (mode)
    {
    case eTranslate:
        translate(x, y);
        break;
    case eScale:
        scale(x, y);
        break;
    case eRotate:
        rotate(x, y);
        break;
    default:
        break;
    }
}

void GizmoController::OnGizmoDragEnd()
{
}

void GizmoController::translate(float x, float y)
{
    if (!transformCtrl_)
        return;
    Node* object = transformCtrl_->getAttachedNode();
    Node* camNode = transformCtrl_->getCameraNode();
    Node* activePlane = transformCtrl_->getActivePlane();
    if (!object || !camNode || !activePlane)
        return;

    Ray world_ray = camNode->GetComponent<Camera>()->GetScreenRay(x, y);
    float dis = 100000;
    Node* hitActPlane = transformCtrl_->intersectObj(world_ray, activePlane, dis);
    if (hitActPlane && hitActPlane == activePlane)
    {
        Vector3 hitPoint = world_ray.origin_ + world_ray.direction_ * dis;
        Vector3 point = hitPoint;
        point = point - offset_;
        point = point * oriparentScale_;
        Vector3 localPoint = oriworldRotationMatrixInverse_ * point;

        Urho3D::String qstr_axis(axis_.c_str());
        if (!qstr_axis.Contains("X")) localPoint.x_ = 0;
        if (!qstr_axis.Contains("Y")) localPoint.y_ = 0;
        if (!qstr_axis.Contains("Z")) localPoint.z_ = 0;

        point = oriworldRotationMatrix_ * localPoint;
        Vector3 pos = (startPos_ + point) - object->GetParent()->GetWorldPosition();
        cmdMgr_.ToDo(new CmdModify(String(cmdName_.c_str()), object, "Position", pos));
    }
}

void GizmoController::scale(float x, float y)
{
    if (!transformCtrl_)
        return;
    Node* object = transformCtrl_->getAttachedNode();
    Node* camNode = transformCtrl_->getCameraNode();
    Node* activePlane = transformCtrl_->getActivePlane();
    if (!object || !camNode || !activePlane)
        return;

    Ray world_ray = camNode->GetComponent<Camera>()->GetScreenRay(x, y);
    float dis = 100000;
    Node* hitActPlane = transformCtrl_->intersectObj(world_ray, activePlane, dis);
    if (hitActPlane && hitActPlane == activePlane)
    {
        Vector3 hitPoint = world_ray.origin_ + world_ray.direction_ * dis;
        Vector3 point = hitPoint;
        point = point - offset_;
        point = point * oriparentScale_;

        Vector3 newScale = startScale_;
        if (axis_ == "XYZ")
        {
            newScale = startScale_ * (1 + point.y_);
        }
        else
        {
            if (axis_ == "X")
                newScale.x_ = startScale_.x_ * (1 + point.x_);
            if (axis_ == "Y")
                newScale.y_ = startScale_.y_ * (1 + point.y_);
            if (axis_ == "Z")
                newScale.z_ = startScale_.z_ * (1 + point.z_);
        }
        cmdMgr_.ToDo(new CmdModify(String(cmdName_.c_str()), object, "Scale", newScale));
    }
}

void GizmoController::rotate(float x, float y)
{
    if (!transformCtrl_)
        return;
    Node* object = transformCtrl_->getAttachedNode();
    Node* camNode = transformCtrl_->getCameraNode();
    Node* activePlane = transformCtrl_->getActivePlane();
    if (!object || !camNode || !activePlane)
        return;

    Ray world_ray = camNode->GetComponent<Camera>()->GetScreenRay(x, y);
    float dis = 100000;
    Node* hitActPlane = transformCtrl_->intersectObj(world_ray, activePlane, dis);
    if (hitActPlane && hitActPlane == activePlane)
    {
        Vector3 hitPoint = world_ray.origin_ + world_ray.direction_ * dis;
        Vector3 point = hitPoint;
        Vector3 worldPosition = object->GetWorldPosition();

        point = point - worldPosition;
        point = point * oriparentScale_;
        Vector3 tempVector = offset_ - worldPosition;
        tempVector = tempVector * oriparentScale_;

        Matrix3 matrixInverse = object->GetRotation().RotationMatrix().Inverse();
        Vector3 localPoint = matrixInverse * point;
        Vector3 localOffet = matrixInverse * tempVector;
        Vector3 unitX(1, 0, 0);
        Vector3 unitY(0, 1, 0);
        Vector3 unitZ(0, 0, 1);

        Vector3 rotation(atan2(localPoint.z_, localPoint.y_), atan2(localPoint.x_, localPoint.z_), atan2(localPoint.y_, localPoint.x_));
        Vector3 offsetRotation(atan2(localOffet.z_, localOffet.y_), atan2(localOffet.x_, localOffet.z_), atan2(localOffet.y_, localOffet.x_));

        Quaternion quaternionXYZ = oriRotationMatrix_.Rotation();
        Quaternion quaternionX;
        Quaternion quaternionY;
        Quaternion quaternionZ;

        float pi = 3.1415926f;
        quaternionX.FromAngleAxis((rotation.x_ - offsetRotation.x_) * 180 / pi, unitX);
        quaternionY.FromAngleAxis((rotation.y_ - offsetRotation.y_) * 180 / pi, unitY);
        quaternionZ.FromAngleAxis((rotation.z_ - offsetRotation.z_) * 180 / pi, unitZ);

        if (axis_ == "X")
            quaternionXYZ = quaternionXYZ * quaternionX;
        if (axis_ == "Y")
            quaternionXYZ = quaternionXYZ * quaternionY;
        if (axis_ == "Z")
            quaternionXYZ = quaternionXYZ * quaternionZ;

        cmdMgr_.ToDo(new CmdModify(String(cmdName_.c_str()), object, "Rotation", quaternionXYZ));
    }
}

} // namespace Urho3DEditor
