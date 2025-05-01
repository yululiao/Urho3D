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
class TransformCtrl:public Object
{
URHO3D_OBJECT(TransformCtrl, Object);
public:
TransformCtrl(Context* ctx,eTransformCtrlMode m,Node* gizmoRoot);
virtual ~TransformCtrl();
void onPointerDown(float x, float y);
void onPointerHover(float x, float y);
void onPointerMove(float x, float y);
void onPointerUp(float x, float y, bool isTempCamera = false);
virtual void update();
void attach(Node* obj);
void detach();
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
protected:
void createGizmo();
void translate(float x, float y);
void scale(float x, float y);
void rotate(float x, float y);
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
std::string _cmdName;

};