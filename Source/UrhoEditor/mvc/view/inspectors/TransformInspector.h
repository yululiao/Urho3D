#pragma once

#include "Urho3D/Scene/Scene.h"
using namespace Urho3D;

namespace Urho3DEditor 
{
class TransformInspector
{
public:
	TransformInspector();
	~TransformInspector();
	void Update();
private:
	Vector3 _pos;
	Vector3 _rot;
	Vector3 _scale;
};
}
