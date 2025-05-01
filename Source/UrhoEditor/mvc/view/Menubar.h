#pragma once
#include <memory>
#include "Urho3D/Core/Context.h"
#include "Urho3D/Core/Main.h"
#include "Urho3D/Engine/Engine.h"

using namespace Urho3D;

namespace Urho3DEditor 
{

typedef void (*MenuHandle)();

struct MenuNode
{
	String name;
	HashMap<String,std::shared_ptr<MenuNode>> children;
	MenuHandle _handle = nullptr;
};

class Menubar 
{
public:
	Menubar();
	~Menubar();
	void Update();
	void AddMenu(const String& path, MenuHandle handle);
private:
	void Init();
	void RenderMenu(MenuNode* menuNode);
	static void OnOpen();
	static void OnSave();
	static void OnUndo();
	static void OnRedo();

	static void ShowToolBar();
	static void ShowNodeTree();
	static void ShowResTree();
	static void ShowResPreview();
	static void ShowSceneView();
	static void ShowInspector();
	static void ShowDemo();
private:
	std::shared_ptr<MenuNode> _menuTree;

};

}
