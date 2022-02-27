#include "TransformGizmo.h"
#include "Urho3D/Graphics/GeoUtils.h"
#include "EditorApp.h"
#include "Urho3D/Graphics/Technique.h"
#include "Urho3D/Math/Matrix3x4.h"
#include "Urho3D/Math/Quaternion.h"
#include "GizmoUtils.h"


TransformGizmo::TransformGizmo(Context* ctx)
	:Node(ctx)
{
	//this->set_scale(60);
	activePlane = NULL;
}

TransformGizmo::~TransformGizmo()
{

}

void TransformGizmo::init()
{
	Context* ctx = EditorApp::getInstance()->getContext();
	this->handles = new Node(ctx);
	this->pickers = new Node(ctx);
	this->planes = new Node(ctx);

	this->AddChild(handles);
	this->AddChild(pickers);
	this->AddChild(planes);

	planesMap["XY"] = GeoUtils::create_plane(ctx,5, 5);
	planesMap["YZ"] = GeoUtils::create_plane(ctx,5, 5);
	planesMap["XZ"] = GeoUtils::create_plane(ctx,5, 5);
	planesMap["XYZE"] = GeoUtils::create_plane(ctx,5, 5);
	this->activePlane = planesMap["XYZE"];
	planesMap["YZ"]->Yaw(90);
	planesMap["XZ"]->Pitch(-90);
	for (auto it = planesMap.begin(); it != planesMap.end(); it++)
	{
		it->second->SetName(it->first.c_str());
		planes->AddChild(it->second);
		GizmoUtils::set_default_mat(it->second);
		auto* mat = it->second->GetComponent<StaticModel>()->GetMaterial();
		mat->SetShaderParameter("MatDiffColor",Vector4(1.0,1.0,0.0,0.0));
	}
			
	this->setupGizmos(handleGizmos,this->handles);
	this->setupGizmos(pickerGizmos, this->pickers);
}

void TransformGizmo::setupGizmos(std::map<std::string, std::vector<GizmogeoInfo*>> gizmoMap, Node * parent)
{
	Context* ctx = EditorApp::getInstance()->getContext();
	for (auto it = gizmoMap.begin(); it != gizmoMap.end(); it++)
	{
		for (int i = 0; i < it->second.size(); ++i)
		{
			auto obj = it->second[i]->geo;
			auto position = it->second[i]->pos;
			auto rotation = it->second[i]->rot;
			obj->SetPosition(position);
			//obj->set_angles(rotation);
			obj->Pitch(rotation.x_);
			obj->Yaw(rotation.y_);
			obj->Roll(rotation.z_);
			obj->SetName(it->first.c_str());
			Node* objParent = new Node(ctx);//用一个节点管理，方便保留局部坐标，便于旋转
			objParent->AddChild(obj);
			parent->AddChild(objParent);
		}
	}
}

void TransformGizmo::highlight(std::string axis)
{
	for (auto it = handleGizmos.begin(); it != handleGizmos.end(); it++)
	{
		auto handleList = it->second;//handleGizmos[axis];
		for (auto listit = handleList.begin(); listit != handleList.end(); listit++)
		{
			if (it->first == axis)
			{
				(*listit)->hilight(true);
			}
			else 
			{
				(*listit)->hilight(false);
			}
		}
	}
			
}

void TransformGizmo::deHilightAll()
{
	for (auto it = handleGizmos.begin(); it != handleGizmos.end(); it++)
	{
		auto handleList = it->second;//handleGizmos[axis];
		for (auto listit = handleList.begin(); listit != handleList.end(); listit++)
		{
			(*listit)->hilight(false);

		}
	}

}

void TransformGizmo::update(Matrix4 objMatrix, Vector3 eye)
{
	Matrix3x4 t_mat34(objMatrix.ToMatrix3());
	this->SetRotation(objMatrix.Rotation());
	if (mAllChild.size() <= 0)
	{
		this->getAllChildren(this, mAllChild);
	}
	for (auto it = mAllChild.begin(); it != mAllChild.end(); it++)
	{
		String name = (*it)->GetName();
        if (name.Contains("E"))
		{
			(*it)->SetWorldDirection(eye);
		}
	}
}

void TransformGizmo::getAllChildren(Node* node,std::vector<Node*>& list)
{
	auto childList = node->GetChildren();
	for (auto it = childList.Begin(); it != childList.End(); it++)
	{
		list.push_back(*it);
		getAllChildren((*it), list);
	}
}