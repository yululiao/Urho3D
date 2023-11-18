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
	Node* selectedNode = EditorApp::getInstance()->GetSelectNode();
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
	

	if(_pos != selectedNode->GetPosition())
	{
		DoModify(Utils::GenGuid().c_str(), selectedNode, "Position", _pos);
	}
	if (_rot != selectedNode->GetRotation().EulerAngles()) 
	{
		DoModify(Utils::GenGuid().c_str(), selectedNode, "Rotation",Quaternion(_rot));
	}
	if (_scale != selectedNode->GetScale()) 
	{
		DoModify(Utils::GenGuid().c_str(), selectedNode, "Scale", _scale);
	}
}
}