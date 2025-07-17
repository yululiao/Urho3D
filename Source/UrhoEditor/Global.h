#pragma once
#include "Urho3D/Core/Context.h"
#include "Urho3D/Container/Str.h"

using namespace Urho3D;
namespace Urho3DEditor {
struct Global
{
	static Context* context;
	static void InitGlobal();
	static String notShowTag;
	static String curSelectType;
};
}