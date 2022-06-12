#include "VariantDrawer.h"
#include "TransformInspector.h"
#include "EditorApp.h"
#include "cmd/CmdDefines.h"


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
		ImGui::PushItemWidth(width / 5.0);
		//pos
		VariantDrawer::DrawVec3d("Position",_pos);
		//rotation
		VariantDrawer::DrawVec3d("Rotation", _rot);
		//Scale
		VariantDrawer::DrawVec3d("Scale   ", _scale);

		ImGui::PopItemWidth();
		ImGui::TreePop();
	}
	

	if(_pos != selectedNode->GetPosition())
	{
		//selectedNode->SetPosition(_pos);
		Urho3DEditor::TransformCmd::Translate("Tranform",selectedNode,_pos);
	}
	if (_rot != selectedNode->GetRotation().EulerAngles()) 
	{
		//selectedNode->SetRotation(Quaternion(_rot));
		Urho3DEditor::TransformCmd::Rot("Tranform",selectedNode, _rot);
	}
	if (_scale != selectedNode->GetScale()) 
	{
		//selectedNode->SetScale(_scale);
		Urho3DEditor::TransformCmd::Scale("Tranform",selectedNode, _scale);
	}
}
}