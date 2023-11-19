#include "imgui.h"
#include "VariantDrawer.h"
#include "EditorApp.h"
#include "Utils.h"
#include "ctrls/AssetMgr.h"


namespace Urho3DEditor 
{
void VariantDrawer::DrawVariant(const Urho3D::String& name, Urho3D::Variant& value)
{
	ImVec2 winSize = ImGui::GetWindowSize();
	float width = winSize.x;
	VariantType vtype = value.GetType();
	switch (vtype) 
	{
	case Urho3D::VAR_INT:
	{
		int int_value = (int)value.GetInt64();
		DrawInt(name,int_value,-100,100);
		value = int_value;
		break;
	}
	case Urho3D::VAR_BOOL:
	{
		bool bool_value = value.GetBool();
		DrawBool(name, bool_value);
		value = bool_value;
		break;
	}
	case Urho3D::VAR_FLOAT:
	{
		float float_value = value.GetFloat();
		DrawFloat(name, float_value, -100, 100);
		value = float_value;
		break;
	}
	case Urho3D::VAR_VECTOR2:
		break;
	case Urho3D::VAR_VECTOR3:
	{
		if(!name.Contains("Color"))
		{
			Vector3 vec3_value = value.GetVector3();
			DrawVec3d(name, vec3_value);
			value = vec3_value;
		}
		else
		{
			Color vec_color(value.GetVector3());
			DrawColor3(name,vec_color);
			value = vec_color.ToVector3();
		}
		break;
		
	}
	case Urho3D::VAR_VECTOR4:
	{
		if (!name.Contains("Color"))
		{
			Vector4 vec4_value = value.GetVector4();
			DrawVec4d(name, vec4_value);
			value = vec4_value;
		}
		else
		{
			Color vec_color4(value.GetVector4());
			DrawColor3(name, vec_color4);
			value = vec_color4.ToVector4();
		}
		break;
	}
	case Urho3D::VAR_QUATERNION:
		break;
	case Urho3D::VAR_COLOR:
		break;
	case Urho3D::VAR_STRING:
		break;
	case Urho3D::VAR_BUFFER:
		break;
	case Urho3D::VAR_VOIDPTR:
		break;
	case Urho3D::VAR_RESOURCEREF:
		break;
	case Urho3D::VAR_RESOURCEREFLIST:
		break;
	case Urho3D::VAR_VARIANTVECTOR:
		break;
	case Urho3D::VAR_VARIANTMAP:
		break;
	case Urho3D::VAR_INTRECT:
		break;
	case Urho3D::VAR_INTVECTOR2:
		break;
	case Urho3D::VAR_PTR:
		break;
	case Urho3D::VAR_MATRIX3:
		break;
	case Urho3D::VAR_MATRIX3X4:
		break;
	case Urho3D::VAR_MATRIX4:
		break;
	case Urho3D::VAR_DOUBLE:
		break;
	case Urho3D::VAR_STRINGVECTOR:
		break;
	case Urho3D::VAR_RECT:
		break;
	case Urho3D::VAR_INTVECTOR3:
		break;
	case Urho3D::VAR_INT64:
		break;
	case Urho3D::VAR_CUSTOM_HEAP:
		break;
	case Urho3D::VAR_CUSTOM_STACK:
		break;
	case Urho3D::MAX_VAR_TYPES:
		break;
	default:
		break;
	}
}
void VariantDrawer::DrawBool(const Urho3D::String& name, bool& data)
{
	ImVec2 winSize = ImGui::GetWindowSize();
	float width = winSize.x;
	ImGui::PushItemWidth(width / 7.0f);

	ImGui::Checkbox(name.CString(),&data);

	ImGui::PopItemWidth();
}
void VariantDrawer::DrawFloat(const Urho3D::String& name, float& data, float min, float max)
{
	ImVec2 winSize = ImGui::GetWindowSize();
	float width = winSize.x;
	ImGui::PushItemWidth(width / 7.0f);
	/*ImGui::Text(name.CString());
	ImGui::SameLine();*/
	ImGui::DragFloat(name.CString(), &data, 0.1f,min, max, "%.4f");
	ImGui::PopItemWidth();
}
void VariantDrawer::DrawInt(const Urho3D::String& name, int& data, int min, int max)
{
	ImVec2 winSize = ImGui::GetWindowSize();
	float width = winSize.x;
	ImGui::PushItemWidth(width / 7.0f);

	ImGui::Text(name.CString());
	ImGui::SameLine();
	ImGui::DragInt(name.CString(), &data, 1, min,max);

	ImGui::PopItemWidth();
}
void VariantDrawer::DrawStr(const Urho3D::String& name, Urho3D::String& value) 
{
	char texBuf[99];
	ImGui::InputText(name.CString(), &texBuf[0],99);
	value = Urho3D::String(&texBuf[0]);

}
void VariantDrawer::DrawPath(const Urho3D::String& name, Urho3D::String& path, Urho3D::Vector<Urho3D::String> filter,
                             bool readOnly)
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
	std::string guid = Utils::GenGuid();
	//ImGui::PushID(guid.c_str());
	ImGui::SameLine();
	if (ImGui::Button("...",ImVec2(30,25)))
	{
        String resultPath = EditorApp::GetInstance()->DialogOpenFile(filter);
		if (resultPath != "")
        {
			resultPath = AssetMgr::getInstance()->pathToRelative(resultPath);
			path = resultPath;
		}
	}
	//ImGui::PopID();
	
}
void VariantDrawer::DrawPathVec(const Urho3D::Vector<Urho3D::String>& paths, Urho3D::Vector<Urho3D::String> filer,
                                bool readOnly)
{

}
void VariantDrawer::DrawVec2d(const Urho3D::String& name, Urho3D::Vector2& data)
{
	ImVec2 winSize = ImGui::GetWindowSize();
	float width = winSize.x;
	ImGui::PushItemWidth(width / 7.0f);

	ImGui::PushID((name + "_x").CString());
	ImGui::InputFloat("x", &data.x_, 0, 0, "%.4f");
	ImGui::PopID();

	ImGui::SameLine(); ImGui::PushID((name + "_y").CString());
	ImGui::InputFloat("y", &data.y_, 0, 0, "%.4f");
	ImGui::PopID();

	ImGui::SameLine(); 
	ImGui::Text(name.CString());

	ImGui::PopItemWidth();
}
void VariantDrawer::DrawVec3d(const Urho3D::String& name, Urho3D::Vector3& data)
{
	ImVec2 winSize = ImGui::GetWindowSize();
	float width = winSize.x;
	ImGui::PushItemWidth(width / 7.0f);

	ImGui::PushID((name + "_x").CString());
	ImGui::InputFloat("x", &data.x_, 0, 0, "%.4f");
	ImGui::PopID();

	ImGui::SameLine(); 
	ImGui::PushID((name + "_y").CString());
	ImGui::InputFloat("y", &data.y_, 0, 0, "%.4f");
	ImGui::PopID();

	ImGui::SameLine(); 
	ImGui::PushID((name + "_z").CString());
	ImGui::InputFloat("z", &data.z_, 0, 0, "%.4f");
	ImGui::PopID();

	ImGui::SameLine();
	ImGui::Text(name.CString());
	ImGui::PopItemWidth();
	
}
void VariantDrawer::DrawVec4d(const Urho3D::String& name, Urho3D::Vector4& data)
{
	ImVec2 winSize = ImGui::GetWindowSize();
	float width = winSize.x;
	ImGui::PushItemWidth(width / 7.0f);

	ImGui::PushID((name + "_x").CString());
	ImGui::InputFloat("x", &data.x_, 0, 0, "%.4f");
	ImGui::PopID();

	ImGui::SameLine(); 
	ImGui::PushID((name + "_y").CString());
	ImGui::InputFloat("y", &data.y_, 0, 0, "%.4f");
	ImGui::PopID();

	ImGui::SameLine(); 
	ImGui::PushID((name + "_z").CString());
	ImGui::InputFloat("z", &data.z_, 0, 0, "%.4f");
	ImGui::PopID();

	ImGui::SameLine(); ImGui::PushID((name + "_w").CString());
	ImGui::InputFloat("w", &data.w_, 0, 0, "%.4f");
	ImGui::PopID();

	ImGui::SameLine();
	ImGui::Text(name.CString());
	ImGui::PopItemWidth();
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