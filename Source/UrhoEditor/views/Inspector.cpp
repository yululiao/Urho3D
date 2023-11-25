#include "Inspector.h"
#include "EditorApp.h"
#include <Urho3D/Graphics/AnimatedModel.h>
#include "views/inspectors/VariantDrawer.h"

namespace Urho3DEditor 
{
Inspector::Inspector() 
{
	_transformIns = new TransformInspector();
	_aniModelIns = new AniModelInspector();
}
Inspector::~Inspector() 
{
}
void Inspector::Update() 
{
	if(!showing)
		return;
	Node* selectedNode = EditorApp::GetInstance()->GetSelectNode();
	ImGui::Begin("Inspector",&showing);
	if(selectedNode)
	{
		_nodeEnable = selectedNode->IsEnabled();
		VariantDrawer::DrawBool("  Enable", _nodeEnable);
		_transformIns->Update();
		if(selectedNode->HasComponent<AnimatedModel>())
		{
			_aniModelIns->Update();
		}
		if(_nodeEnable != selectedNode->IsEnabled())
		{
			selectedNode->SetEnabled(_nodeEnable);
		}
	}
	ImGui::End();

}
}