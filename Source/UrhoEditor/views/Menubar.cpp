#include "Menubar.h"
#include "imgui.h"

namespace Urho3DEditor 
{

Menubar::Menubar():_menuTree(new MenuNode())
{
	Init();
}
Menubar::~Menubar() 
{
}

void Menubar::Init() 
{
	AddMenu("file/open",&OnOpen);
	AddMenu("file/save",&OnSave);
	AddMenu("edit/undo",&OnUndo);
	AddMenu("edit/redo",&OnRedo);
	AddMenu("view/ToolBar",&ShowToolBar);
	AddMenu("view/NodeTree",&ShowNodeTree);
	AddMenu("view/ResTree",&ShowResTree);
	AddMenu("view/ResPreview",&ShowResPreview);
	AddMenu("view/SceneView",&ShowSceneView);
	AddMenu("view/Inspector",ShowInspector);
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
	//pathItems中的最后一项是叶子节点
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
}

void Menubar::OnUndo() {
}

void Menubar::OnRedo() {
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

}