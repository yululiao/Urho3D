#pragma once
#include "CmdMgr.h"
#include <Urho3D/Scene/Scene.h>

using namespace Urho3D;

namespace Urho3DEditor {

class TransformCmd : public EditCmd
{
public:
	TransformCmd(const std::string& id, const std::string& tag = "")
		:EditCmd(id),_tag(tag)
	{

	}
	virtual ~TransformCmd() {}
	void ToDo() {
		Vector3 oldData;
		if (_tag == "pos") {
			oldData = _node->GetPosition();
			_node->SetPosition(_data);
		}
		else if (_tag == "rot") {
			oldData = _node->GetRotation().EulerAngles();
			_node->SetRotation(Quaternion(_data));
		}
		else {
			oldData = _node->GetScale();
			_node->SetScale(_data);
		}
		_data = oldData;
	}
	void UnDo() {
		ToDo();
	}

	static void Translate(const std::string&name,Node* node, Vector3 data) 
	{
		SetTransform(name,"pos",node,data);
	}
	static void Scale(const std::string& name,Node* node, Vector3 data)
	{
		SetTransform(name,"scale", node, data);
	}
	static void Rot(const std::string& name,Node* node, Vector3 data)
	{
		SetTransform(name,"rot", node, data);
	}
private:
	static void SetTransform(const std::string& name,const std::string& tag, Node* node, Vector3 data)
	{
		Urho3DEditor::TransformCmd* cmd = new Urho3DEditor::TransformCmd(name, tag);
		cmd->_data = data;
		cmd->_node = node;
		Urho3DEditor::CmdMgr::Instance()->ToDo(cmd);
	}
public:
	Vector3 _data;
	Node* _node = nullptr;
	std::string _tag;
};

}