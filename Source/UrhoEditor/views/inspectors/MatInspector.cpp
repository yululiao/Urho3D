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
	auto* cache = SceneCtrl::getInstance()->GetSubsystem<ResourceCache>();
	AnimatedModel* aniModel = selectedNode->GetComponent<AnimatedModel>();
	SharedPtr<Material> mat(aniModel->GetMaterial());
	int flags = ImGuiTreeNodeFlags_DefaultOpen;
	Urho3D::String matPath = "";
	if(mat)
	{
		matPath = mat->GetName();
		VariantDrawer::DrawPath("MatPath", matPath, "Mat Files(*.xml)\0*.xml\0\0", false);
		if (matPath != mat->GetName()) 
		{
			mat = cache->GetResource<Urho3D::Material>(matPath);
			SceneCtrl::getInstance()->GetSubsystem<Graphics>()->MakeCurrent();
			aniModel->SetMaterial(mat);
			return;
		}
	}
	if(matPath == "Materials/Default.xml" || matPath == "")//默认材质不允许编辑
		return;
	if (mat && ImGui::TreeNodeEx("Material", flags))
	{
		auto textures = mat->GetTextures();
		for (auto item : textures) {
			Urho3D::String name = mat->GetTextureUnitName(item.first_);
			Urho3D::String path = item.second_->GetName();
			VariantDrawer::DrawPath(name,path, "Texture Files(*.dds)\0*.dds\0\0", false);
			if(path != item.second_->GetName())
			{
				Urho3D::Texture2D* newTex = cache->GetResource<Urho3D::Texture2D>(path);
				//SceneCtrl::getInstance()->GetSubsystem<Graphics>()->MakeCurrent();
				mat->SetTexture(item.first_, newTex);
			}
			
		}
		auto parameters = mat->GetShaderParameters();
		for(auto item:parameters)
		{
			Urho3D::String name = item.second_.name_;
			Variant value = item.second_.value_;
			VariantDrawer::DrawVariant(name,value);
			mat->SetShaderParameter(name,value);
		}
		ImGui::TreePop();
	}
}
}
