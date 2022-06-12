#pragma once
//#include "Urho3D/Math/Vector2.h"
//#include "Urho3D/Math/Vector3.h"
//#include "Urho3D/Math/Vector4.h"
//#include "Urho3D/Math/Color.h"
//#include "Urho3D/Container/Str.h"
#include "Urho3D/Core/Variant.h"


namespace Urho3DEditor
{
class VariantDrawer
{
public:
	static void DrawVariant(const Urho3D::String& name,Urho3D::Variant& data);
	static void DrawBool(const Urho3D::String& name,bool& data);
	static void DrawFloat(const Urho3D::String& name, float& data,float min,float max);
	static void DrawInt(const Urho3D::String& name, int& data,int min,int max);
	static void DrawStr(const Urho3D::String& name,Urho3D::String& value);
	// filter:TEXT("Text Files(*.txt)\0*.txt\0")
	static void DrawPath(const Urho3D::String& name,Urho3D::String& path,const char* filer = nullptr,bool readOnly = false);
	static void DrawPathVec(const Urho3D::Vector<Urho3D::String>& paths, const char* filer = nullptr, bool readOnly = false);
	static void DrawVec2d(const Urho3D::String& name, Urho3D::Vector2& data);
	static void DrawVec3d(const Urho3D::String& name, Urho3D::Vector3& data);
	static void DrawVec4d(const Urho3D::String& name, Urho3D::Vector4& data);
	static void DrawColor4(const Urho3D::String& name, Urho3D::Color& data);
	static void DrawColor3(const Urho3D::String& name, Urho3D::Color& data);
};
}