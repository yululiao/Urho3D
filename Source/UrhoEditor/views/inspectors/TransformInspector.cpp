#include "VariantDrawer.h"
#include "TransformInspector.h"
#include "EditorApp.h"
#include "cmd/CmdDefines.h"
#include "Utils.h"


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
	/*ImVec2 winSize = ImGui::GetWindowSize();
	float width = winSize.x;*/
	int flags = ImGuiTreeNodeFlags_DefaultOpen;
	Node* selectedNode = EditorApp::GetInstance()->GetSelectNode();
	if(!selectedNode)
		return;
	_pos = selectedNode->GetPosition();
	_rot = selectedNode->GetRotation().EulerAngles();
	_scale = selectedNode->GetScale();
	if(ImGui::TreeNodeEx("Transform",flags))
	{
		//pos
		VariantDrawer::DrawVec3d("Position",_pos);
		//rotation
		VariantDrawer::DrawVec3d("Rotation", _rot);
		//Scale
		VariantDrawer::DrawVec3d("Scale   ", _scale);
		ImGui::TreePop();
	}
	
	String cmdGuid = EditorApp::GetInstance()->GetLastCmdGuid();
	if(_pos != selectedNode->GetPosition())
	{
		if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
			cmdGuid = String(Utils::GenGuid().c_str());
		}
		DoModify(cmdGuid, selectedNode, "Position", _pos);
	}
	if (_rot != selectedNode->GetRotation().EulerAngles()) 
	{
		if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
			cmdGuid = String(Utils::GenGuid().c_str());
		}
		DoModify(cmdGuid, selectedNode, "Rotation",Quaternion(_rot));
	}
	if (_scale != selectedNode->GetScale()) 
	{
		if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
			cmdGuid = String(Utils::GenGuid().c_str());
		}
		DoModify(cmdGuid, selectedNode, "Scale", _scale);
	}
}
}