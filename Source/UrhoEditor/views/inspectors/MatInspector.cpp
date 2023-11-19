#include "MatInspector.h"
#include "EditorApp.h"
#include "cmd/CmdDefines.h"
#include <Urho3D/Graphics/AnimatedModel.h>
#include "VariantDrawer.h"
#include "Urho3D/Graphics/Material.h"
#include "Urho3D/Graphics/Technique.h"
#include "ctrls/SceneCtrl.h"
#include "Urho3D/Graphics/Texture2D.h"
#include <Urho3D/Resource/ResourceCache.h>
#include "Urho3D/Graphics/Graphics.h"
#include "cmd/CmdDefines.h"
#include "Utils.h"
#include "ctrls/AssetMgr.h"

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
	Node* selectedNode = EditorApp::GetInstance()->GetSelectNode();
	if (!selectedNode || !selectedNode->HasComponent<AnimatedModel>())
		return;
	auto* cache = SceneCtrl::getInstance()->GetSubsystem<ResourceCache>();
	AnimatedModel* aniModel = selectedNode->GetComponent<AnimatedModel>();
	SharedPtr<Material> mat(aniModel->GetMaterial());
	int flags = ImGuiTreeNodeFlags_DefaultOpen;
	Urho3D::String matPath = "";
	if(mat)
	{
		matPath = mat->GetName();
        VariantDrawer::DrawPath("MatPath", matPath, {"Mat Files", "xml"}, false);
		if (matPath != mat->GetName()) 
		{
			DoResPathModify(Utils::GenGuid().c_str(), aniModel, matPath,CmdModifyResPath::ResType::MAT);
			return;
		}
	}
	if(matPath == "Materials/Default.xml" || matPath == "")//默认材质不允许编辑
		return;
	if (mat && ImGui::TreeNodeEx("Material", flags))
	{
		ImGui::SameLine();
		if (ImGui::Button("Save")) {
			mat->SaveFile(AssetMgr::getInstance()->pathToFull(matPath));
		}
		auto textures = mat->GetTextures();
		for (auto item : textures) {
			Urho3D::String name = mat->GetTextureUnitName(item.first_);
			Urho3D::String path = item.second_->GetName();
            VariantDrawer::DrawPath(name, path, {"Texture Files", "png,tga,jpg,dds"}, false);
			if(path != item.second_->GetName())
			{
				DoMatTexModify(Utils::GenGuid().c_str(),mat, (uint16_t)item.first_, path);
			}
			
		}
		auto parameters = mat->GetShaderParameters();
		for(auto item:parameters)
		{
			Urho3D::String name = item.second_.name_;
			Variant value = item.second_.value_;
			VariantDrawer::DrawVariant(name,value);
			if(value != item.second_.value_)
			{
				DoMatModify(Utils::GenGuid().c_str(), mat,name,value);
			}
			
		}
		ImGui::TreePop();
	}
}
}
