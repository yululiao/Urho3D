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
	Material* mat = aniModel->GetMaterial();
	int flags = ImGuiTreeNodeFlags_DefaultOpen;
	if (mat && ImGui::TreeNodeEx("Material", flags))
	{
		auto textures = mat->GetTextures();
		for (auto item : textures) {
			Urho3D::String name = mat->GetTextureUnitName(item.first_);
			Urho3D::String path = item.second_->GetName();
			VariantDrawer::DrawPath(name,path, "Model Files(*.dds)\0*.dds\0\0", false);
			if(path != item.second_->GetName())
			{
				Urho3D::Texture2D* newTex = cache->GetResource<Urho3D::Texture2D>(path);
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
		
		/*unsigned color = 0xffff0000;
		Color t_color(color, Color::ARGB);
		mat->SetShaderParameter("MatDiffColor", t_color.ToVector4());*/
		ImGui::TreePop();
	}
}
}
