#include "AniModelInspector.h"
#include "EditorApp.h"
#include "cmd/CmdDefines.h"
#include <Urho3D/Graphics/AnimatedModel.h>
#include "VariantDrawer.h"

class AniModelInspector 
{

};

Urho3DEditor::AniModelInspector::AniModelInspector()
{
	_matInspector = new MatInspector();
}

Urho3DEditor::AniModelInspector::~AniModelInspector() 
{
	if(_matInspector)
	{
		delete _matInspector;
		_matInspector = nullptr;
	}
}

void Urho3DEditor::AniModelInspector::Update() 
{
	ImVec2 winSize = ImGui::GetWindowSize();
	int flags = ImGuiTreeNodeFlags_DefaultOpen;
	Node* selectedNode = EditorApp::GetInstance()->GetSelectNode();
	if (!selectedNode || !selectedNode->HasComponent<AnimatedModel>())
		return;
	AnimatedModel* aniModel = selectedNode->GetComponent<AnimatedModel>();
	//const Vector<AttributeInfo>* attributes = aniModel->GetAttributes();
	if(ImGui::TreeNodeEx("AnimatedModel",flags))
	{
		//filter: TEXT("Text Files(*.txt)\0*.txt\0")
		_modelPath = aniModel->GetModelAttr().name_;
        VariantDrawer::DrawPath("model", _modelPath, {"Model Files", "mdl"}, true);
		_matInspector->Update();
		ImGui::TreePop();

	}
}
