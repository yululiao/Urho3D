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

using namespace Urho3D;

namespace Urho3DEditor
{

MatInspector::MatInspector() 
{
}

MatInspector::~MatInspector() 
{
}

int MatInspector::CalLongestName(Urho3D::Material* mat)
{
	int len = 0;
	auto textures = mat->GetTextures();
	for (auto item : textures) {
		Urho3D::String name = mat->GetTextureUnitName(item.first_);
		if(name.Length() > len)
			len = name.Length();
	}
	auto parameters = mat->GetShaderParameters();
	for (auto item : parameters) {
		Urho3D::String name = item.second_.name_;
		if (name.Length() > len)
			len = name.Length();
	}
	return len;
}

Urho3D::String MatInspector::FillItemName(const Urho3D::String& oriName, int len) 
{
	String result = oriName;
	if(oriName.Length() < len)
	{
		for(int i=0;i<len - oriName.Length();i++)
		{
			result = result + " ";
		}
	}
	return result;
}

void MatInspector::Update() 
{
	Node* selectedNode = EditorApp::GetInstance()->GetSelectNode();
	if (!selectedNode || !selectedNode->HasComponent<AnimatedModel>())
		return;
	auto* cache = SceneCtrl::getInstance()->GetSubsystem<ResourceCache>();
	AnimatedModel* aniModel = selectedNode->GetComponent<AnimatedModel>();
	int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
	SharedPtr<Material> mat(aniModel->GetMaterial());
	Urho3D::String matPath = "";
	if(mat)
	{
		matPath = mat->GetName();
		
	}
	bool isDefaultMat = matPath == "Materials/Default.xml" || matPath == "";
	if (mat && ImGui::TreeNodeEx("Material", flags))
	{
		if (!isDefaultMat)
		{
            ImGui::SameLine();
            if (ImGui::Button("Save"))
            {
                mat->SaveFile(AssetMgr::getInstance()->pathToFull(matPath));
            }
		}
		ImGui::SameLine();
		ImGui::PushID("MatPath");
		VariantDrawer::DrawPath("", matPath, { "Mat Files", "xml" }, false);
		ImGui::PopID();
		if (matPath != mat->GetName()) {
			SceneCtrl::getInstance()->GetSubsystem<Graphics>()->MakeCurrent();
			auto cache = SceneCtrl::getInstance()->GetSubsystem<ResourceCache>();
			SharedPtr<Material> mat(cache->GetResource<Material>(matPath));
			DoObjModifyPropPtr(Utils::GenGuid().c_str(), aniModel, mat.Get());
			ImGui::TreePop();
			return;
		}
        if (isDefaultMat)
        {
            ImGui::TreePop();
            return;
        }
		int longLen = CalLongestName(mat);
		auto textures = mat->GetTextures();
		for (auto item : textures) {
			Urho3D::String name = mat->GetTextureUnitName(item.first_);
			String displaName = FillItemName(name,longLen);
			Urho3D::String path = item.second_->GetName();
            VariantDrawer::DrawPath(displaName, path, {"Texture Files", "png,tga,jpg,dds"}, false);
			if(path != item.second_->GetName())
			{
				DoMatTexModify(Utils::GenGuid().c_str(),mat, (uint16_t)item.first_, path);
			}
			
		}
		auto parameters = mat->GetShaderParameters();
		for(auto item:parameters)
		{
			Urho3D::String name = item.second_.name_;
			String displaName = FillItemName(name, longLen);
			Variant value = item.second_.value_;
			VariantDrawer::DrawVariant(displaName,value);
			if(value != item.second_.value_)
			{
				String cmdGuid = EditorApp::GetInstance()->GetLastCmdGuid();
				if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
					cmdGuid = String(Utils::GenGuid().c_str());
				}
				DoMatModify(cmdGuid, mat, name, value);//鼠标拖拽弹起做为一次操作
			}
			
		}
		ImGui::TreePop();
	}
}

}
