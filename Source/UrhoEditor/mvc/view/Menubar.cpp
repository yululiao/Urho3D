#include "Menubar.h"
#include "imgui.h"
#include "stb/stb_image.h"

namespace Urho3DEditor 
{

Menubar::Menubar(ProjectController& projectCtrl, ToggleDemoHandler toggleDemo)
	: _menuTree(new MenuNode())
	, projectController_(projectCtrl)
	, toggleDemo_(toggleDemo)
{
	Init();
}
Menubar::~Menubar() 
{
}

void Menubar::Init() 
{
	AddMenu("File/Open", [this]() { OnOpen(); });
	AddMenu("File/Save", [this]() { OnSave(); });
	AddMenu("Edit/Undo", [this]() { OnUndo(); });
	AddMenu("Edit/Redo", [this]() { OnRedo(); });
	AddMenu("Views/ToolBar", []() { ShowToolBar(); });
	AddMenu("Views/NodeTree", []() { ShowNodeTree(); });
	AddMenu("Views/ResTree", []() { ShowResTree(); });
	AddMenu("Views/ResPreview", []() { ShowResPreview(); });
	AddMenu("Views/SceneView", []() { ShowSceneView(); });
	AddMenu("Views/Inspector", [this]() { ShowInspector(); });
	AddMenu("test/show|hide imgui demo", [this]() { ShowDemo(); });
}

void Menubar::AddMenu(const String& path, MenuHandle handle)
{
	auto pathItems = path.Split('/',true);
	if (pathItems.Size()<2)
		return;
	auto curentNode = this->_menuTree;
	for(int index=0; index <pathItems.Size() -1; ++index)
	{
		auto item = pathItems[index];
		if(curentNode->children.Contains(item))
		{
			curentNode = curentNode->children[item];
		}
		else
		{
			curentNode->children[item] = std::shared_ptr<MenuNode>(new MenuNode());
			curentNode->children[item]->name = item;
			curentNode = curentNode->children[item];
		}
	}
	// last item in pathItems is the leaf node
	auto leafName = pathItems[pathItems.Size() - 1];
	curentNode->children[leafName] = std::shared_ptr<MenuNode>(new MenuNode());
	curentNode->children[leafName]->name = leafName;
	curentNode->children[leafName]->_handle = handle;
}

void Menubar::RenderMenu(MenuNode* menuNode)
{
	if(menuNode == nullptr)
		return;
	for(auto item:menuNode->children)
	{
		String localName = item.second_->name;
		if(item.second_->_handle)
		{
			bool selected = false;
			if(ImGui::MenuItem(localName.CString(),"",&selected))
			{
				item.second_->_handle();
			}
		}
		else
		{
			if(ImGui::BeginMenu(localName.CString()))
			{
				RenderMenu(item.second_.get());
				ImGui::EndMenu();
			}
		}
	}
}

void Menubar::Update() 
{
	if (ImGui::BeginMenuBar())
	{
		RenderMenu(_menuTree.get());
		ImGui::EndMenuBar();
	}
}

void Menubar::OnOpen() 
{
	int test = 10;
}

void Menubar::OnSave() {
	String assetRoot = projectController_.GetAssetRoot();
	projectController_.SaveScene(assetRoot + "/test.uscene");
	projectController_.OnSave();
}

void Menubar::OnUndo() {
	projectController_.Undo();
}

void Menubar::OnRedo() {
	projectController_.Redo();
}

void Menubar::ShowToolBar() {
}

void Menubar::ShowNodeTree() {
}

void Menubar::ShowResTree() {
}

void Menubar::ShowResPreview() {
}

void Menubar::ShowSceneView() {
}

void Menubar::ShowInspector() {
}

void Menubar::ShowDemo() {
	if (toggleDemo_)
		toggleDemo_();
}

}