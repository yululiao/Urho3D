#include "MatInspector.h"
#include "EditorApp.h"
#include "cmd/CmdDefines.h"
#include <Urho3D/Graphics/AnimatedModel.h>
#include "VariantDrawer.h"
#include "Urho3D/Graphics/Material.h"
#include "Urho3D/Graphics/Technique.h"

namespace Urho3DEditor
{

MatInspector::MatInspector() 
{
}

MatInspector::~MatInspector() 
{
}

void MatInspector::Update() 
{
	Node* selectedNode = EditorApp::getInstance()->GetSelectNode();
	if (!selectedNode || !selectedNode->HasComponent<AnimatedModel>())
		return;
	AnimatedModel* aniModel = selectedNode->GetComponent<AnimatedModel>();
	Material* mat = aniModel->GetMaterial();
	int flags = ImGuiTreeNodeFlags_DefaultOpen;
	if (mat && ImGui::TreeNodeEx("Material", flags))
	{
		auto parameters = mat->GetShaderParameters();
	}
}
}
