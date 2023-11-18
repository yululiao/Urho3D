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
#include "cmd/CmdDefines.h"
#include "Utils.h"

TransformCtrl::TransformCtrl(Context* ctx,eTransformCtrlMode m,Node* gizmoRoot)
	:Object(ctx)
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
			_cmdName = Urho3DEditor::Utils::GenGuid();
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
	switch (mode)
	{
	case eTranslate:
		translate(x, y);
		break;
	case eScale:
		scale(x,y);
		break;
	case eRotate:
		rotate(x, y);
		break;
	default:
		break;
	}
}

void TransformCtrl::onPointerUp(float x, float y, bool isTempCamera)
{
	if (state == eTransformCtrlState::ePause)
		return;
	if (_dragging)
	{
		_dragging = false;
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

void TransformCtrl::translate(float x, float y)
{
	Ray world_ray = _camNode->GetComponent<Camera>()->GetScreenRay(x, y);
	//camera->screen_to_world(vec2d(x, y), world_ray);
	float dis = 100000;
	std::string name = gizmo->activePlane->GetName().CString();
	//auto hitActPlane = gizmo->activePlane->hit_test_all(world_ray, dis, true);//要保证一定击中
	Node* hitActPlane = intersectObj(world_ray, gizmo->activePlane, dis);
	if (hitActPlane && hitActPlane == gizmo->activePlane.Get())
	{
		//vec3d hitPoint = world_ray.evaluate(dis);
		Vector3 hitPoint = world_ray.origin_ + world_ray.direction_ * dis;
		point = hitPoint;
		point = point - offset;
		point = point * oriparentScale;
		Vector3 localPoint;
		//oriworldRotationMatrixInverse.vec_mul(point, localPoint);
		localPoint = oriworldRotationMatrixInverse * point;
				
		//point.copy_from(localPoint);
		//QString qstr_axis = Utils::str2qstr(axis);
        Urho3D::String qstr_axis(axis.c_str());
		if (!qstr_axis.Contains("X")) localPoint.x_ = 0;
		if (!qstr_axis.Contains("Y")) localPoint.y_ = 0;
        if (!qstr_axis.Contains("Z")) localPoint.z_ = 0;
		//oriworldRotationMatrix.vec_mul(localPoint, point);
		point = oriworldRotationMatrix * localPoint;
				
		//object->set_world_pos(startPos + point);
		//object->SetWorldPosition(startPos + point);
		Vector3 pos =(startPos + point)- object->GetParent()->GetWorldPosition();
		Urho3DEditor::DoModify(String(_cmdName.c_str()),object,"Position",pos);
	}
}
void TransformCtrl::scale(float x, float y)
{
	Ray world_ray = _camNode->GetComponent<Camera>()->GetScreenRay(x, y);
	float dis = 100000;
	std::string name = gizmo->activePlane->GetName().CString();
	//auto hitActPlane = gizmo->activePlane->hit_test_all(world_ray, dis, true);//要保证一定击中
	Node* hitActPlane = intersectObj(world_ray,gizmo->activePlane,dis);
	if (hitActPlane && hitActPlane == gizmo->activePlane.Get())
	{
		Vector3 hitPoint = world_ray.origin_ + world_ray.direction_ * dis;
		point = hitPoint;
		point = point - offset;
		point = point * oriparentScale;
		//std::cout << "point:" << point.x << ", " << point.y << ", " << point.z << std::endl;
		//std::cout << "offset:" << offset.x << ", " << offset.y << ", " << offset.z << std::endl;
		Vector3 newScale = startScale;
		if (axis == "XYZ")
		{
			newScale = startScale * (1 + point.y_);
		}
		else
		{
			if (axis == "X")
				newScale.x_ = startScale.x_ * (1 + point.x_);
			if (axis == "Y")
				newScale.y_ = startScale.y_ * (1 + point.y_);
			if (axis == "Z")
				newScale.z_ = startScale.z_ * (1 + point.z_);

		}
		//object->SetScale(newScale);
		Urho3DEditor::DoModify(String(_cmdName.c_str()), object, "Scale", newScale);
	}

}

void TransformCtrl::rotate(float x, float y)
{
	Ray world_ray = _camNode->GetComponent<Camera>()->GetScreenRay(x, y);
	float dis = 100000;
	std::string name = gizmo->activePlane->GetName().CString();
	//auto hitActPlane = gizmo->activePlane->hit_test_all(world_ray, dis, true);//要保证一定击中
	Node* hitActPlane = intersectObj(world_ray, gizmo->activePlane,dis);
	if (hitActPlane && hitActPlane == gizmo->activePlane.Get())
	{
		Vector3 hitPoint = world_ray.origin_ + world_ray.direction_ * dis;
		point = hitPoint;
		Vector3 worldPosition = object->GetWorldPosition();

		point = point - worldPosition;
		point = point * oriparentScale;
		Vector3 tempVector = offset - worldPosition;
		tempVector = tempVector * oriparentScale;
		
		Matrix4 matrixInverse;
		//object->get_rot().inverse_to(matrixInverse);
		matrixInverse = object->GetRotation().RotationMatrix().Inverse();
		Vector3 localPoint;
		Vector3 localOffet;
		//matrixInverse.vec_mul(point, localPoint);
		//matrixInverse.vec_mul(tempVector, localOffet);
		localPoint = matrixInverse * point;
		localOffet = matrixInverse * tempVector;
		Vector3 unitX(1, 0, 0);
		Vector3 unitY(0, 1, 0);
		Vector3 unitZ(0, 0, 1);

		Vector3 rotation(atan2(localPoint.z_, localPoint.y_), atan2(localPoint.x_, localPoint.z_), atan2(localPoint.y_, localPoint.x_));
		Vector3 offsetRotation(atan2(localOffet.z_, localOffet.y_), atan2(localOffet.x_, localOffet.z_), atan2(localOffet.y_, localOffet.x_));

		Quaternion quaternionXYZ;
		//mat44_to_quat(oriRotationMatrix, quaternionXYZ);
		quaternionXYZ = oriRotationMatrix.Rotation();
		Quaternion quaternionX;
		Quaternion quaternionY;
		Quaternion quaternionZ;

		float pi = 3.1415926;
		quaternionX.FromAngleAxis( (rotation.x_-offsetRotation.x_)*180/pi ,unitX);
		quaternionY.FromAngleAxis( (rotation.y_-offsetRotation.y_)*180/pi, unitY);
		quaternionZ.FromAngleAxis( (rotation.z_-offsetRotation.z_)*180/pi, unitZ);

		if(axis == "X")
			quaternionXYZ = quaternionXYZ * quaternionX;
		if (axis == "Y")
			quaternionXYZ = quaternionXYZ * quaternionY;
		if (axis == "Z")
			quaternionXYZ = quaternionXYZ * quaternionZ;

				
		//std::cout << "mouseXY:" << "x" << x << ",y" << y;
		//std::cout << "quaternionY:" << "x" << quaternionY.x << ",y" << quaternionY.y << ",z" << quaternionY.z << ",w" << quaternionY.w << std::endl;
		//object->SetRotation(quaternionXYZ);
		//Urho3DEditor::TransformCmd::Rot(_cmdName,object, quaternionXYZ.EulerAngles());
		Urho3DEditor::DoModify(String(_cmdName.c_str()), object, "Rotation", quaternionXYZ);


	}
}
