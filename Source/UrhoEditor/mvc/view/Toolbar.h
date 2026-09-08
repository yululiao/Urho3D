#pragma once
#include <memory>
#include <functional>
#include "Urho3D/Core/Context.h"
#include "Urho3D/Core/Main.h"
#include "Urho3D/Engine/Engine.h"
#include "ctrl/scene/ToolController.h"
#include "ctrl/res/ProjectController.h"

using namespace Urho3D;
namespace Urho3DEditor
{

typedef std::function<void()> ToolbarHandle;

class Toolbar
{
public:
	using ImguiTexProvider = std::function<int(const String&)>;

	Toolbar(ToolController& toolCtrl, ProjectController& projectCtrl, ImguiTexProvider texProvider);
	~Toolbar();
	void Update();
    void IntItemSize(float dpiScale);
private:
	void DrawTransformTool();
	void DrawTrigerTool();
	void Init();
    
	void OnSave();
	void OnRedo();
	void OnUndo();
	void OnNewScene();
	void OnTransformTool(const String& name);
private:
	int itemSize = 20;
	bool isShow = true;
	//String curToolName;
	unsigned selectedColor = 0x77999999;
	ToolController& toolController_;
	ProjectController& projectController_;
	ImguiTexProvider texProvider_;
};
}