#include "imgui.h"
#include "AssetInspector.h"
#include "VariantDrawer.h"
#include "ctrl/res/ProjectController.h"

namespace Urho3DEditor {

AssetInspector::AssetInspector(ProjectController& projectCtrl)
	: projectController_(projectCtrl)
{
}

AssetInspector::~AssetInspector()
{
}
void AssetInspector::Update()
{
	int flags = ImGuiTreeNodeFlags_DefaultOpen;
	if(projectController_.GetLastSelectedFileExt() == ".fbx")
	{
		if (ImGui::TreeNodeEx("FbxInspector", flags)) {

			if (ImGui::Button("Import")) {
				projectController_.ImportFbx(projectController_.GetLastSelectedFile());
			}
			ImGui::TreePop();
		}
	}
	
}

}