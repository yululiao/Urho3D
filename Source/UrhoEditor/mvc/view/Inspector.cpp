#include "Inspector.h"
#include "EditorApp.h"
#include <Urho3D/Graphics/AnimatedModel.h>
#include "view/inspectors/VariantDrawer.h"
#include "Global.h"
#include "ctrl/res/AssetMgr.h"

namespace Urho3DEditor 
{
Inspector::Inspector() 
{
	_transformIns = new TransformInspector();
	_aniModelIns = new AniModelInspector();
	_assetIns = new AssetInspector();
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
	if(Global::curSelectType == "Node" && selectedNode)
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
	else if(Global::curSelectType == "File")
	{
		if(AssetMgr::getInstance()->selectedFiles.Size()>0)
		{
			_assetIns->Update();
		}
	}
	ImGui::End();

}
}