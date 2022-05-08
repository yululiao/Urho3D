#pragma once
#include "EditorWidget.h"
#include "Urho3D/Container/Str.h"
#include "Urho3D/Core/Context.h"
#include "Urho3D/Scene/Scene.h"

using namespace Urho3D;

namespace Urho3DEditor
{
class ConsoleView :public EditorWidget
{
public:
	ConsoleView();
	virtual ~ConsoleView();
	void Update() override;
private:

};
}