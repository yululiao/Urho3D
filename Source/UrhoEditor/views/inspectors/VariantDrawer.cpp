#include "imgui.h"
#include "VariantDrawer.h"
#include "EditorApp.h"
#include "Utils.h"


namespace Urho3DEditor 
{
void VariantDrawer::DrawVariant(const Urho3D::String& name, Urho3D::Variant& data) 
{
}
void VariantDrawer::DrawBool(const Urho3D::String& name, bool& data)
{
	ImGui::Checkbox(name.CString(),&data);
}
void VariantDrawer::DrawFloat(const Urho3D::String& name, float& data, float min, float max)
{
	ImGui::Text(name.CString());
	ImGui::SameLine();
	ImGui::InputFloat(name.CString(), &data, 0, 0, "%.4f");
}
void VariantDrawer::DrawInt(const Urho3D::String& name, int& data, int min, int max)
{
	ImGui::Text(name.CString());
	ImGui::SameLine();
	ImGui::InputInt(name.CString(), &data, 0, 0);
}
void VariantDrawer::DrawStr(const Urho3D::String& name, Urho3D::String& value) 
{
	char texBuf[99];
	ImGui::InputText(name.CString(), &texBuf[0],99);
	value = Urho3D::String(&texBuf[0]);

}
void VariantDrawer::DrawPath(const Urho3D::String& name, Urho3D::String& path, const char* filter, bool readOnly)
{
	char texBuf[256];
	strcpy(texBuf,path.CString());
	
	if(readOnly)
	{
		ImGui::InputText(name.CString(), &texBuf[0], 256, ImGuiInputTextFlags_ReadOnly);
		return;
	}
	if (ImGui::InputText(name.CString(), &texBuf[0], 256, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		path = Urho3D::String(texBuf);
	}
	auto openFun = [](Urho3D::String& path1, const char* filter1)
	{
		Urho3D::String openFile = EditorApp::getInstance()->dialogOpenFile(filter1);
		if (openFile != "")
			path1 = openFile;
	};
	std::string guid = Utils::GenGuid();
	//ImGui::PushID(guid.c_str());
	ImGui::SameLine();
	if (ImGui::Button("...",ImVec2(30,25)))
	{
		openFun(path,filter);
	}
	//ImGui::PopID();
	
}
void VariantDrawer::DrawPathVec(const Urho3D::Vector<Urho3D::String>& paths, const char* filer, bool readOnly)
{
}
void VariantDrawer::DrawVec2d(const Urho3D::String& name, Urho3D::Vector2& data)
{
	ImGui::Text(name.CString());
	ImGui::SameLine(); ImGui::PushID((name + "_x").CString());
	ImGui::InputFloat("x", &data.x_, 0, 0, "%.4f");
	ImGui::PopID();

	ImGui::SameLine(); ImGui::PushID((name + "_y").CString());
	ImGui::InputFloat("y", &data.y_, 0, 0, "%.4f");
	ImGui::PopID();
}
void VariantDrawer::DrawVec3d(const Urho3D::String& name, Urho3D::Vector3& data)
{
	ImGui::Text(name.CString());
	ImGui::SameLine(); ImGui::PushID((name + "_x").CString());
	ImGui::InputFloat("x", &data.x_, 0, 0, "%.4f");
	ImGui::PopID();

	ImGui::SameLine(); ImGui::PushID((name + "_y").CString());
	ImGui::InputFloat("y", &data.y_, 0, 0, "%.4f");
	ImGui::PopID();

	ImGui::SameLine(); ImGui::PushID((name + "_z").CString());
	ImGui::InputFloat("z", &data.z_, 0, 0, "%.4f");
	ImGui::PopID();
}
void VariantDrawer::DrawVec4d(const Urho3D::String& name, Urho3D::Vector4& data)
{
	ImGui::Text(name.CString());
	ImGui::SameLine(); ImGui::PushID((name + "_x").CString());
	ImGui::InputFloat("x", &data.x_, 0, 0, "%.4f");
	ImGui::PopID();

	ImGui::SameLine(); ImGui::PushID((name + "_y").CString());
	ImGui::InputFloat("y", &data.y_, 0, 0, "%.4f");
	ImGui::PopID();

	ImGui::SameLine(); ImGui::PushID((name + "_z").CString());
	ImGui::InputFloat("z", &data.z_, 0, 0, "%.4f");
	ImGui::PopID();

	ImGui::SameLine(); ImGui::PushID((name + "_w").CString());
	ImGui::InputFloat("w", &data.w_, 0, 0, "%.4f");
	ImGui::PopID();
}
void VariantDrawer::DrawColor4(const Urho3D::String& name, Urho3D::Color& data)
{
	float color_vec[4] = {data.r_,data.g_,data.b_,data.a_};
	ImGui::ColorEdit4(name.CString(),color_vec);
	data = Urho3D::Color(color_vec);

}
void VariantDrawer::DrawColor3(const Urho3D::String& name, Urho3D::Color& data) {
	float color_vec[3] = { data.r_,data.g_,data.b_};
	ImGui::ColorEdit3(name.CString(), color_vec);
	data = Urho3D::Color(color_vec[0], color_vec[1], color_vec[2]);
}
}