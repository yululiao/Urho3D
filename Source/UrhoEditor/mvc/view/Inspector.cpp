#include "Inspector.h"
#include <Urho3D/Graphics/AnimatedModel.h>
#include "view/inspectors/VariantDrawer.h"

namespace Urho3DEditor 
{
Inspector::Inspector(SelectionModel& selectionModel, PropertyEditController& propEditCtrl, ProjectController& projectCtrl)
	: selectionModel_(selectionModel)
	, propertyEditController_(propEditCtrl)
	, projectController_(projectCtrl)
{
	_transformIns = new TransformInspector(selectionModel, propEditCtrl);
	_aniModelIns = new AniModelInspector(selectionModel, propEditCtrl, projectCtrl);
	_assetIns = new AssetInspector(projectCtrl);
	VariantDrawer::SetPathToRelativeHandler([&projectCtrl](const String& path) { return projectCtrl.PathToRelative(path); });
}
Inspector::~Inspector() 
{
}
void Inspector::Update() 
{
	if(!showing)
		return;
	Node* selectedNode = selectionModel_.GetSelectedNode();
	ImGui::Begin("Inspector",&showing);
	if(selectionModel_.GetSelectType() == "Node" && selectedNode)
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
			propertyEditController_.SetNodeEnabled(selectedNode, _nodeEnable);
		}
	}
	else if(selectionModel_.GetSelectType() == "File")
	{
		if(projectController_.HasSelectedFiles())
		{
			_assetIns->Update();
		}
	}
	ImGui::End();

}
}