#pragma once
#include <memory>
#include "Urho3D/Core/Context.h"
#include "Urho3D/Core/Main.h"
#include "Urho3D/Engine/Engine.h"

using namespace Urho3D;
namespace Urho3DEditor
{

typedef void (*ToolbarHandle)();

class Toolbar
{
public:
	Toolbar();
	~Toolbar();
	void Update();
private:
	void DrawTransformTool();
	void DrawTrigerTool();
	void Init();

	static void OnSave();
	static void OnRedo();
	static void OnUndo();
	static void OnNewScene();
	void OnTransformTool(const String& name);
private:
	int itemSize = 20;
	bool isShow = true;
	//String curToolName;
	unsigned selectedColor = 0x77999999;
};
}