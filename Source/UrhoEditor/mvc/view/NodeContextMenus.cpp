#include "NodeContextMenus.h"
#include "imgui.h"

namespace Urho3DEditor
{

NodeContexMenu::NodeContexMenu(Context* context, const String& name)
	:Object(context),_callBackFun(nullptr)
{
	_name = name;
}

NodeContexMenu::NodeContexMenu(Context* context, const String& name, NodeContexMenuHandle callback)
	:Object(context)
{
	_name = name;
	_callBackFun = callback;
}

void NodeContexMenu::SetCallBack(NodeContexMenuHandle callback)
{
	_callBackFun = callback;
}

void NodeContexMenu::OnClicked(Vector<Node*> selectNodes)
{
	if (_callBackFun) {
		_callBackFun(selectNodes);
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
Context* NodeContextMenus::context_ = nullptr;
SceneManipulationController* NodeContextMenus::sceneManipController_ = nullptr;
void NodeContextMenus::Init(Context* context, SceneManipulationController* sceneManipCtrl)
{
	if(_init)
		return;
	context_ = context;
	sceneManipController_ = sceneManipCtrl;
	//�ļ����Ҽ��˵�
	AddContexMenu(new NodeContexMenu(context_, "AddEmpty", &OnAddEmptyNode));
	AddContexMenu(new NodeContexMenu(context_, "Delete", &OnDeleteNode));
	_init = true;
}

void NodeContextMenus::AddContexMenu(NodeContexMenu* menu)
{
	contexMenus.Push(menu);
}

bool NodeContextMenus::DrawContextMenu(Vector<Node*> selectNodes)
{
	bool clicked = false;
	for (auto item : contexMenus) {
		bool show = true;
		if(item->GetName() == "Delete"){
			if(selectNodes.Empty())
			{
				show = false;
			}
		}
		if (show && ImGui::MenuItem(item->GetName().CString())) {
			item->OnClicked(selectNodes);
			clicked = true;
		}
		
	}
	return clicked;
}

void NodeContextMenus::OnAddEmptyNode(Vector<Node*> selectNodes)
{
	Node* node = nullptr;
	if(selectNodes.Size()>0)
		node = selectNodes[selectNodes.Size()-1];
	if (sceneManipController_)
		sceneManipController_->AddEmptyNode(node);
}

void NodeContextMenus::OnDeleteNode(Vector<Node*> selectNodes)
{
	if (sceneManipController_)
		sceneManipController_->DeleteNodes(selectNodes);
}


}
