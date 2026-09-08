#pragma once
#include "TransformGizmo.h"
#include "Urho3D/Math/Ray.h"

enum eTransformCtrlState
{
eRunning,
ePause,
eStop
};

enum eTransformCtrlMode
{
eTranslate,
eScale,
eRotate
};

// Implemented by GizmoController
class IGizmoDragHandler
{
public:
    virtual ~IGizmoDragHandler() = default;
    virtual void OnGizmoDragStart(const std::string& cmdName, const std::string& axis, const Vector3& offset,
                                  const Vector3& startPos, const Vector3& startScale,
                                  const Matrix4& oriRotationMatrix, const Matrix4& oriworldRotationMatrix,
                                  const Matrix4& oriworldRotationMatrixInverse, const Vector3& oriworldPos,
                                  const Vector3& oriworldScale, const Matrix4& oriparentRotationMatrix,
                                  const Vector3& oriparentScale) = 0;
    virtual void OnGizmoDragMove(float x, float y, eTransformCtrlMode mode, const std::string& axis) = 0;
    virtual void OnGizmoDragEnd() = 0;
};

class TransformCtrl:public Object
{
URHO3D_OBJECT(TransformCtrl, Object);
public:
TransformCtrl(Context* ctx,eTransformCtrlMode m,Node* gizmoRoot, IGizmoDragHandler* handler);
virtual ~TransformCtrl();
void onPointerDown(float x, float y);
void onPointerHover(float x, float y);
void onPointerMove(float x, float y);
void onPointerUp(float x, float y, bool isTempCamera = false);
virtual void update();
void attach(Node* obj);
void detach();
Node* getAttachedNode() const { return object; }
void setMode(eTransformCtrlMode mode);
void pause(bool isp);
void setOperationEnable(bool enable);
void stop();
//void setGizmosVisible(bool visibility) {/* gizmo->(visibility);*/ }
void setCameraNode(Node* camNode) { _camNode = camNode; }
void setScene(Scene* sc) { _scene = sc; }
Node* intersectObj(Ray& ray, Node* pnode,float& dis);
void intersectObj(Ray& ray, Node* pnode, std::map<float,Node*>& out);
bool isDraging() { return _dragging; }

// Expose captured gizmo state for GizmoController
Node* getActivePlane() const { return gizmo ? gizmo->activePlane.Get() : nullptr; }
Node* getCameraNode() const { return _camNode; }
Scene* getScene() const { return _scene; }
const std::string& getAxis() const { return axis; }
const Vector3& getOffset() const { return offset; }
const Vector3& getStartPos() const { return startPos; }
const Vector3& getStartScale() const { return startScale; }
const Matrix4& getOriRotationMatrix() const { return oriRotationMatrix; }
const Matrix4& getOriWorldRotationMatrix() const { return oriworldRotationMatrix; }
const Matrix4& getOriWorldRotationMatrixInverse() const { return oriworldRotationMatrixInverse; }
const Vector3& getOriWorldPos() const { return oriworldPos; }
const Vector3& getOriWorldScale() const { return oriworldScale; }
const Matrix4& getOriParentRotationMatrix() const { return oriparentRotationMatrix; }
const Vector3& getOriParentScale() const { return oriparentScale; }

protected:
void createGizmo();
//void onSetSelectObjInScene(EventData* data);
eTransformCtrlMode mode;
SharedPtr<TransformGizmo> gizmo = nullptr;
Node* object;
Node* sceneRoot;
//scene_3d* scene3d;
//Node* modelRoot;
std::string axis;
bool _dragging;
eTransformCtrlState state;
Vector3 eye;
Vector3 offset;
Vector3 point;
Vector3 startScale;
Vector3 startPos;
bool oprationEnable;
/*oldRotationMatrix*/
Matrix4 oriRotationMatrix;
/*worldRotationMatrix*/
Matrix4 oriworldRotationMatrix;
Matrix4 oriworldRotationMatrixInverse;
Vector3 oriworldPos;
Vector3 oriworldScale;
/*parentRotationMatrix*/
Matrix4 oriparentRotationMatrix;
/*parentScale*/
Vector3 oriparentScale;
Node* _gizmoRoot = nullptr;
Node* _camNode = nullptr;
Scene* _scene = nullptr;
IGizmoDragHandler* dragHandler_ = nullptr;

};
