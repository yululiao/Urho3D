#pragma once
#include "Urho3D/Core/Context.h"

using namespace Urho3D;
namespace Urho3DEditor {
struct Global
{
	static Context* context;
	static void InitGlobal();

};
}