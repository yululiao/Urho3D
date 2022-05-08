#include "TransformInspector.h"
#include "EditorApp.h"


namespace Urho3DEditor 
{
TransformInspector::TransformInspector() 
{
}
TransformInspector::~TransformInspector()
{
}
void TransformInspector::Update() 
{
	ImVec2 winSize = ImGui::GetWindowSize();
	float width = winSize.x;
	int flags = ImGuiTreeNodeFlags_DefaultOpen;
	Node* selectedNode = EditorApp::getInstance()->GetSelectNode();
	if(!selectedNode)
		return;
	_pos = selectedNode->GetPosition();
	_rot = selectedNode->GetRotation().EulerAngles();
	_scale = selectedNode->GetScale();
	if(ImGui::TreeNodeEx("Transform",flags))
	{
		//pos
		ImGui::Text("Position");
		ImGui::PushItemWidth(width / 5.0);
		ImGui::SameLine();
		ImGui::InputFloat("x",&_pos.x_,0,0,"%.4f");

		ImGui::SameLine();
		ImGui::InputFloat("y", &_pos.y_, 0, 0, "%.4f");

		ImGui::SameLine();
		ImGui::InputFloat("z", &_pos.z_, 0, 0, "%.4f");
		//rotation
		ImGui::Text("Rotation");
		ImGui::SameLine();ImGui::PushID("rx");
		ImGui::InputFloat("x", &_rot.x_, 0, 0, "%.4f");
		ImGui::PopID();

		ImGui::SameLine();ImGui::PushID("ry");
		ImGui::InputFloat("y", &_rot.y_, 0, 0, "%.4f");
		ImGui::PopID();

		ImGui::SameLine();ImGui::PushID("rz");
		ImGui::InputFloat("z", &_rot.z_, 0, 0, "%.4f");
		ImGui::PopID();
		//Scale
		ImGui::Text("Scale   ");
		ImGui::SameLine();ImGui::PushID("sx");
		ImGui::InputFloat("x", &_scale.x_, 0, 0, "%.4f");
		ImGui::PopID();

		ImGui::SameLine();ImGui::PushID("sy");
		ImGui::InputFloat("y", &_scale.y_, 0, 0, "%.4f");
		ImGui::PopID();

		ImGui::SameLine();ImGui::PushID("sz");
		ImGui::InputFloat("z", &_scale.z_, 0, 0, "%.4f");
		ImGui::PopID();

		ImGui::PopItemWidth();
		ImGui::TreePop();
	}

	if(_pos != selectedNode->GetPosition())
	{
		selectedNode->SetPosition(_pos);
	}
	if (_rot != selectedNode->GetRotation().EulerAngles()) {
		selectedNode->SetRotation(Quaternion(_rot));
	}
	if (_scale != selectedNode->GetScale()) {
		selectedNode->SetScale(_scale);
	}
}
}