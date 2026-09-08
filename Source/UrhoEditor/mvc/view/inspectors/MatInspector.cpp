#include "MatInspector.h"
#include "imgui.h"
#include "ctrl/scene/PropertyEditController.h"
#include <Urho3D/Graphics/AnimatedModel.h>
#include "VariantDrawer.h"
#include "Urho3D/Graphics/Material.h"
#include "Urho3D/Graphics/Technique.h"
#include "Urho3D/Graphics/Texture2D.h"
#include <Urho3D/Resource/ResourceCache.h>
#include "Utils.h"
#include "Urho3D/Container/Vector.h"

using namespace Urho3D;

namespace Urho3DEditor
{

MatInspector::MatInspector(SelectionModel& selectionModel, PropertyEditController& propEditCtrl)
	: selectionModel_(selectionModel)
	, propertyEditController_(propEditCtrl)
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
	Node* selectedNode = selectionModel_.GetSelectedNode();
	if (!selectedNode || !selectedNode->HasComponent<AnimatedModel>())
		return;
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
                propertyEditController_.SaveMaterial(mat, matPath);
            }
		}
		ImGui::SameLine();
		ImGui::PushID("MatPath");
		VariantDrawer::DrawPath("", matPath, { "Mat Files", "xml" }, false);
		ImGui::PopID();
		if (matPath != mat->GetName()) {
			propertyEditController_.SetNodeMaterial(selectedNode, matPath);
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
				propertyEditController_.SetMaterialTexture(mat, (uint16_t)item.first_, path);
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
				String cmdGuid = _lastCmdGuid;
				if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
					cmdGuid = String(Utils::GenGuid().c_str());
					_lastCmdGuid = cmdGuid;
				}
				propertyEditController_.SetMaterialParam(mat, name, value, cmdGuid);
			}
			
		}
		ImGui::TreePop();
	}
}

}
