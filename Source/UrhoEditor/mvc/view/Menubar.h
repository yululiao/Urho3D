#pragma once
#include <memory>
#include <functional>
#include "Urho3D/Core/Context.h"
#include "Urho3D/Core/Main.h"
#include "Urho3D/Engine/Engine.h"
#include "ctrl/res/ProjectController.h"

using namespace Urho3D;

namespace Urho3DEditor 
{

typedef std::function<void()> MenuHandle;

struct MenuNode
{
	String name;
	HashMap<String,std::shared_ptr<MenuNode>> children;
	MenuHandle _handle = nullptr;
};

class Menubar 
{
public:
	using ToggleDemoHandler = std::function<void()>;

	Menubar(ProjectController& projectCtrl, ToggleDemoHandler toggleDemo);
	~Menubar();
	void Update();
	void AddMenu(const String& path, MenuHandle handle);
private:
	void Init();
	void RenderMenu(MenuNode* menuNode);
	void OnOpen();
	void OnSave();
	void OnUndo();
	void OnRedo();

	static void ShowToolBar();
	static void ShowNodeTree();
	static void ShowResTree();
	static void ShowResPreview();
	static void ShowSceneView();
	static void ShowInspector();
	void ShowDemo();
private:
	std::shared_ptr<MenuNode> _menuTree;
	ProjectController& projectController_;
	ToggleDemoHandler toggleDemo_;
};

}
