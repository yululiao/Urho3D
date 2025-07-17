#include "NodeContextMenus.h"
#include "Global.h"
#include "ctrl/res/AssetMgr.h"
#include "imgui.h"
#include "NdfMgr.h"
#include "ctrl/scene/SceneCtrl.h"

namespace Urho3DEditor
{

NodeContexMenu::NodeContexMenu(const String& name)
	:Object(Global::context),_callBackFun(nullptr)
{
	_name = name;
}

NodeContexMenu::NodeContexMenu(const String& name, NodeContexMenuHandle callback)
	:Object(Global::context)
{
	_name = name;
	_callBackFun = callback;
}

void NodeContexMenu::SetCallBack(NodeContexMenuHandle callback)
{
	_callBackFun = callback;
}

void NodeContexMenu::OnClicked(Node* node)
{
	if (_callBackFun) {
		_callBackFun(node);
	}
	//for lua script call back
	// local contexMenu = FolderContexMenu:new()
	/*
	SubscribeToEvent(contexMenu, "Clicked",
		function (eventType, eventData)
		   -- on clicked call
		end)
	*/
	using namespace Clicked;
	VariantMap& eventData = GetEventDataMap();
	eventData[P_NodeContexMenu] = this;
	SendEvent(ENCM_CLICKED, eventData);
}

String& NodeContexMenu::GetName() {
	return _name;
}

Vector<NodeContexMenu*> NodeContextMenus::contexMenus;
bool NodeContextMenus::_init = false;
void NodeContextMenus::Init()
{
	if(_init)
		return;
	//ÎÄ¼þ¼ÐÓÒ¼ü²Ëµ¥
	AddContexMenu(new NodeContexMenu("AddEmpty", &OnAddEmptyNode));
	AddContexMenu(new NodeContexMenu("Delete", &OnDeleteNode));
	_init = true;
}

void NodeContextMenus::AddContexMenu(NodeContexMenu* menu)
{
	contexMenus.Push(menu);
}

bool NodeContextMenus::DrawContextMenu(Node* node)
{
	bool clicked = false;
	for (auto item : contexMenus) {
		bool show = true;
		if(item->GetName() == "Delete"){
			if(!node)
			{
				show = false;
			}
		}
		if (show && ImGui::MenuItem(item->GetName().CString())) {
			item->OnClicked(node);
			clicked = true;
		}
		
	}
	return clicked;
}

void NodeContextMenus::OnAddEmptyNode(Node* node)
{
	SceneCtrl::getInstance()->AddEmptyNode(node);
}

void NodeContextMenus::OnDeleteNode(Node* node) 
{
	if(node)
	{
		SceneCtrl::getInstance()->DeleteNode(node);
	}
	
}


}
