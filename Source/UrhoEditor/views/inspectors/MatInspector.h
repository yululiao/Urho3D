#pragma once
#include "Urho3D/Container/Str.h"
#include "Urho3D/Graphics/Material.h"

namespace Urho3DEditor
{
class MatInspector
{
public:
	MatInspector();
	~MatInspector();
	int CalLongestName(Urho3D::Material* mat);
	Urho3D::String  FillItemName(const Urho3D::String& oriName,int len);
	void Update();

};
}
