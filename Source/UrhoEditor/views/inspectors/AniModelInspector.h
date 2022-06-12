#pragma once
#include "Urho3D/Container/Str.h"
#include "imgui.h"

namespace Urho3DEditor
{
class AniModelInspector
{
public:
	AniModelInspector();
	~AniModelInspector();
	void Update();
private:
	Urho3D::String _modelPath;
};
}