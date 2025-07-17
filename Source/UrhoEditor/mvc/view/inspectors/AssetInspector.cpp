#include "AssetInspector.h"
#include "VariantDrawer.h"
#include "EditorApp.h"
#include "ctrl/res/AssetMgr.h"

namespace Urho3DEditor {

AssetInspector::AssetInspector()
{
}

AssetInspector::~AssetInspector()
{
}
void AssetInspector::Update()
{
	int flags = ImGuiTreeNodeFlags_DefaultOpen;
	auto assetMgr = AssetMgr::getInstance();
	auto& nodeCache = assetMgr->nodeCache;
	if(nodeCache[assetMgr->lastSlectedFile].ext == ".fbx")
	{
		if (ImGui::TreeNodeEx("FbxInspector", flags)) {

			if (ImGui::Button("Import")) {
				AssetMgr::getInstance()->ImportFbx(assetMgr->lastSlectedFile);
			}
			ImGui::TreePop();
		}
	}
	
}

}