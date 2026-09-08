#include "imgui.h"
#include "VariantDrawer.h"
#include "TransformInspector.h"
#include "Utils.h"


namespace Urho3DEditor 
{

TransformInspector::TransformInspector(SelectionModel& selectionModel, PropertyEditController& propEditCtrl)
	: selectionModel_(selectionModel)
	, propertyEditController_(propEditCtrl)
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
	Node* selectedNode = selectionModel_.GetSelectedNode();
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
	
	String cmdGuid = _lastCmdGuid;
	if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
		cmdGuid = String(Utils::GenGuid().c_str());
		_lastCmdGuid = cmdGuid;
	}
	if(_pos != selectedNode->GetPosition())
	{
		propertyEditController_.SetNodeProperty(selectedNode, "Position", _pos, cmdGuid);
	}
	if (_rot != selectedNode->GetRotation().EulerAngles()) 
	{
		propertyEditController_.SetNodeProperty(selectedNode, "Rotation", Quaternion(_rot), cmdGuid);
	}
	if (_scale != selectedNode->GetScale()) 
	{
		propertyEditController_.SetNodeProperty(selectedNode, "Scale", _scale, cmdGuid);
	}
}
}