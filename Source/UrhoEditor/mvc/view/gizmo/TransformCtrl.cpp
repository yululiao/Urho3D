#include "iostream"
#include "TranslateGizmo.h"
#include "ScaleGizmo.h"
#include "RotateGizmo.h"
#include "TransformCtrl.h"
#include <Urho3D/Graphics/Camera.h>
#include "Urho3D/Graphics/Model.h"
#include "Urho3D/Graphics/Geometry.h"
#include "Urho3D//Graphics/VertexBuffer.h"
#include "Urho3D/Graphics/IndexBuffer.h"
#include "Urho3D/Graphics/GeoUtils.h"
#include "GizmoUtils.h"
#include "Utils.h"

TransformCtrl::TransformCtrl(Context* ctx,eTransformCtrlMode m,Node* gizmoRoot, IGizmoDragHandler* handler)
	:Object(ctx)
	, dragHandler_(handler)
{
	_gizmoRoot = gizmoRoot;
	mode = m;
	state = eTransformCtrlState::eRunning;
	//modelRoot = NULL;
	object = nullptr;
	gizmo = nullptr;
	oprationEnable = true;
	_dragging = false;
	createGizmo();
	//EventMgr::GetInstance()->registerEvent(eGlobalEventType::SetSelectObjectInScene, MakeDelegate(this, &TransformCtrl::onSetSelectObjInScene));

}

TransformCtrl::~TransformCtrl()
{
	//EventMgr::GetInstance()->removeEvent(eGlobalEventType::SetSelectObjectInScene, MakeDelegate(this, &TransformCtrl::onSetSelectObjInScene));

}

void TransformCtrl::createGizmo()
{
	//detach();
	if (gizmo != nullptr)
	{
		//gizmo->SetParent(NULL);
		gizmo->Remove();
		gizmo = nullptr;
	}
	switch (mode)
	{
	case eTranslate:
		gizmo = new TranslateGizmo(context_);
		break;
	case eScale:
		gizmo = new ScaleGizmo(context_);
		break;
	case eRotate:
		gizmo = new RotateGizmo(context_);
		break;
	default:
		break;
	}
}

void TransformCtrl::intersectObj(Ray& ray, Node* pnode, std::map<float,Node*>& out)
{
	StaticModel* stmodel = pnode->GetComponent<StaticModel>();
	if (stmodel)
	{
		Model* tmodel = stmodel->GetModel();
		if (tmodel)
		{
			unsigned int geoNum = tmodel->GetNumGeometries();
			float dis = M_INFINITY;
			std::string name = pnode->GetName().CString();
			for (int i = 0;i<geoNum;i++)
			{
				Geometry* geo = tmodel->GetGeometry(i, 0);
				VertexBuffer* vb = geo->GetVertexBuffer(0);
				unsigned vitemsize = vb->GetVertexSize();
				void* vbdata = vb->GetShadowData();
				void* vidata = geo->GetIndexBuffer()->GetShadowData();
				unsigned indexSize = geo->GetIndexBuffer()->GetIndexSize();
				unsigned indexCount = geo->GetIndexBuffer()->GetIndexCount();
				Matrix3x4 nodeRot = pnode->GetWorldTransform();
				Ray t_ray = ray;
				t_ray.hitTransform_ = nodeRot;
				float t_dis = t_ray.HitDistance(vbdata,vitemsize,vidata,indexSize,0,indexCount);
				if (t_dis < dis)
				{
					dis = t_dis;
				}
			}
			if (dis < M_INFINITY && out.find(dis) == out.end())
			{
				out[dis] = pnode;
			}
		}
	}
	auto& children = pnode->GetChildren();
	for (auto it = children.Begin(); it != children.End(); it++)
	{
		intersectObj(ray,*it,out);
	}

}

Node* TransformCtrl::intersectObj(Ray& ray,Node* pnode,float& dis)
{
	Node* result = nullptr;
	std::map<float,Node*> out;
	intersectObj(ray,pnode,out);
	if (out.size() == 0)
		return result;
	result = out.begin()->second;
	dis = out.begin()->first;
	return result;
}

void TransformCtrl::onPointerDown(float x, float y)
{
	if (state == eTransformCtrlState::ePause || !object || _dragging || !oprationEnable)
		return;
	//line3d world_ray;
	Ray world_ray = _camNode->GetComponent<Camera>()->GetScreenRay(x,y);
	float dis = 100000;
	Node* pickHit = intersectObj(world_ray,gizmo->pickers,dis);
	if (pickHit)
	{
		axis = pickHit->GetName().CString();
		eye = _camNode->GetWorldPosition() - object->GetWorldPosition();
		gizmo->setActivePlane(axis, eye);
		//std::string activePlaneName = gizmo->activePlane->GetName().CString();
		Node* planeHit = intersectObj(world_ray,gizmo->activePlane,dis);
		if (planeHit)
		{
			std::string hitName = planeHit->GetName().CString();
			Vector3 hitpoint = world_ray.origin_ + world_ray.direction_*dis;//world_ray.evaluate(dis);
			/*SharedPtr<Node> debugNode = geometry_util::create_cube(context_,0.1);
			GizmoUtils::set_default_mat(debugNode);
			GizmoUtils::set_geo_color(debugNode, 0xffff0000);
			debugNode->SetPosition(hitpoint);
			_scene->AddChild(debugNode);*/
			offset = hitpoint;
			startScale = object->GetScale();
			startPos = object->GetWorldPosition();

			oriRotationMatrix = object->GetRotation().RotationMatrix();
			//object->get_world_mat().get_prs(oriworldPos, oriworldRotationMatrix, oriworldScale);
			oriworldPos = object->GetWorldPosition();
			oriworldRotationMatrix = object->GetWorldRotation().RotationMatrix();
			oriworldScale = object->GetWorldScale();
			//oriworldRotationMatrix.inverse_to(oriworldRotationMatrixInverse);
			oriworldRotationMatrixInverse = oriworldRotationMatrix.Inverse();
			//Vector3 parentWordPos;
			oriparentRotationMatrix = object->GetParent()->GetWorldRotation().RotationMatrix();
			//mat44 parentWordMatrixInverse;
			//oriparentRotationMatrix.inverse_to(parentWordMatrixInverse);
			//parentWordMatrixInverse.get_scale(oriparentScale);
			oriparentScale = object->GetParent()->GetWorldScale();
			_dragging = true;
			std::string cmdName = Urho3DEditor::Utils::GenGuid();
			if (dragHandler_)
			{
				dragHandler_->OnGizmoDragStart(cmdName, axis, offset, startPos, startScale,
					oriRotationMatrix, oriworldRotationMatrix, oriworldRotationMatrixInverse,
					oriworldPos, oriworldScale, oriparentRotationMatrix, oriparentScale);
			}
		}

	}

}
void TransformCtrl::onPointerHover(float x, float y)
{
	if (state == eTransformCtrlState::ePause || !object || _dragging)
	{
		return;
	}
	Ray world_ray = _camNode->GetComponent<Camera>()->GetScreenRay(x, y);
	//camera->screen_to_world(vec2d(x, y), world_ray);
	float dis = 100000;
	//auto hitresult = gizmo->pickers->hit_test_all(world_ray, dis, true);
	Node* hitresult = intersectObj(world_ray, gizmo->pickers,dis);
	if (hitresult)
	{
		std::string hitName = hitresult->GetName().CString();
		gizmo->highlight(hitName);
	}
	else
	{
		gizmo->deHilightAll();
	}
}

void TransformCtrl::onPointerMove(float x, float y)
{
	if (!object || axis == "" || !_dragging || state == eTransformCtrlState::ePause)
		return;
	if (!oprationEnable)
		return;
	if (dragHandler_)
		dragHandler_->OnGizmoDragMove(x, y, mode, axis);
}

void TransformCtrl::onPointerUp(float x, float y, bool isTempCamera)
{
	if (state == eTransformCtrlState::ePause)
		return;
	if (_dragging)
	{
		_dragging = false;
		if (dragHandler_)
			dragHandler_->OnGizmoDragEnd();
		return;
	}
}

void TransformCtrl::update()
{
	if (!object || object->GetParent() == nullptr)
	{
		object = nullptr;
		return;
	}
	Vector3 camPos = _camNode->GetWorldPosition();
	float distanse = (camPos - object->GetWorldPosition()).Length();
	//float magic = 0.15f;
	//this->gizmo->set_scale(std::log2(distanse*distanse*distanse) * magic);
	this->gizmo->SetScale(distanse * 0.1);
	this->gizmo->SetWorldPosition(object->GetWorldPosition());//位置更新
	//eye.normalize();
	Vector3 worldPos;
	Matrix4 worldRotation;
	worldPos = object->GetWorldPosition();
	worldRotation = object->GetWorldRotation().RotationMatrix();
	//object->get_world_mat().get_prs(worldPos,worldRotation,worldScale);
	Vector3 eye1 = camPos -worldPos;
	gizmo->update(worldRotation, eye1);

}

void TransformCtrl::attach(Node* obj)
{
	if (obj != object)
	{
		object = obj;
		gizmo->Remove();
	}
	if (state == eTransformCtrlState::ePause)
		return;
	_gizmoRoot->AddChild(gizmo);
	this->state = eTransformCtrlState::eRunning;
}

void TransformCtrl::detach()
{
	object = nullptr;
	if (gizmo)
		gizmo->Remove();
}

void TransformCtrl::setMode(eTransformCtrlMode m)
{
	if (mode == m)
		return;
	mode = m;
	createGizmo();
	if (object)
	{
		attach(object);
	}
}

void TransformCtrl::pause(bool isp)
{
	auto targetState = isp == true ? eTransformCtrlState::ePause : eTransformCtrlState::eRunning;
	if (state == targetState)
		return;
	if (isp)
	{
		state = eTransformCtrlState::ePause;
	}
	else
	{
		if (object)
		{
			//gizmo->set_visible(true);
			_gizmoRoot->AddChild(gizmo);
		}
		state = eTransformCtrlState::eRunning;
	}
			
}

void TransformCtrl::setOperationEnable(bool enable)
{
	this->oprationEnable = enable;
}

void TransformCtrl::stop()
{
	this->detach();
	state = eTransformCtrlState::ePause;
}


