#include "NodeTree.h"
#include "EditorApp.h"
#include "Utils.h"
#include "Global.h"
#include "ctrl/res/AssetMgr.h"
#include "ctrl/scene/SceneCtrl.h"
#include "NodeContextMenus.h"

namespace Urho3DEditor 
{
NodeTree::NodeTree() 
{
}
NodeTree::~NodeTree() 
{
}
void NodeTree::Update() 
{
	if(!showing)
		return;
	ImGui::Begin("NodeTree",&showing);
	ImVec2 newSize = ImGui::GetWindowSize();
	_winPos = ImGui::GetWindowPos();
	if (newSize.x != winSize.x || newSize.y != winSize.y) {
		winSize = newSize;
	}
	ImVec2 oldPos = ImGui::GetCursorScreenPos();
	ImGui::PushID("NodeTreeDropRect");//创建一个不可见可接受拖拽的区域
	ImGui::Dummy(winSize);
	OnDrop();
	ImGui::PopID();
	ImGui::SetCursorScreenPos(oldPos);//跳回Dummy开始位置绘制场景节点树
	DrawNode(EditorApp::GetInstance()->GetSceneRoot(),true,0);
	/*if(ImGui::BeginPopupContextWindow("context"),1)
	{
		ImGui::MenuItem("create","",false,true);
		ImGui::EndPopup();
	}*/
	ImGui::End();
}
void NodeTree::OnClicked(Node* node)
{
	EditorApp::GetInstance()->SelectNode(node);
}
void NodeTree::OnDoubleClicked() 
{
	
}

bool NodeTree::isMouseInCurItem(int itemH)
{
	bool in = false;
	ImVec2 curPos = ImGui::GetCursorScreenPos();
	if(ImGui::GetIO().MousePos.y >= curPos.y && ImGui::GetIO().MousePos.y <= curPos.y + itemH)
	{
		in = true;
	}
	return in;
}

bool NodeTree::isMouseInCurItemTop(int itemH) {
	bool in = false;
	ImVec2 curPos = ImGui::GetCursorScreenPos();
	int gap = 2 * EditorApp::GetInstance()->GetDpiScale();
	if (ImGui::GetIO().MousePos.y >= curPos.y - gap && ImGui::GetIO().MousePos.y <= curPos.y) {
		in = true;
	}
	return in;
}

bool NodeTree::isMouseInCurItemBottom(int itemH) {
	bool in = false;
	ImVec2 curPos = ImGui::GetCursorScreenPos();
	int gap = 2 * EditorApp::GetInstance()->GetDpiScale();
	if (ImGui::GetIO().MousePos.y >= curPos.y + itemH && ImGui::GetIO().MousePos.y <= curPos.y +itemH + gap) {
		in = true;
	}
	return in;
}

void NodeTree::DrawNodeNoInWindows(int itemH)
{
	//Draw nothing
	ImVec2 curPos = ImGui::GetCursorScreenPos();
	curPos.y = curPos.y + itemH;
	ImGui::SetCursorScreenPos(curPos);
}

void NodeTree::GetDragMouseInfo(bool& isDragingInItem, bool& isDragingInItemTop, bool& isDragingInItemBottom)
{
	isDragingInItem = isMouseInCurItem(_itemH);
	isDragingInItemTop = false;
	isDragingInItemBottom = false;
	if (!isDragingInItem) {
		isDragingInItemTop =  isMouseInCurItemTop(_itemH);
	}
	if (!isDragingInItem && !isDragingInItemTop) {
		isDragingInItemBottom = isMouseInCurItemBottom(_itemH);
	}
}

void NodeTree::DrawNode(Node* node,bool isRoot,int nodeIndex)
{
	String nodeName = node->GetName();
	int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick 
		| ImGuiTreeNodeFlags_SpanAvailWidth;
	auto& children = node->GetChildren();
	auto selecednode = EditorApp::GetInstance()->GetSelectNode();
	if(isRoot)
	{
		flags |= ImGuiTreeNodeFlags_DefaultOpen;
	}
	bool isDragingInItem = false;
	bool isDragingInItemTop = false;
	bool isDragingInItemBottom = false;
	if(_isDraging)
	{
		GetDragMouseInfo(isDragingInItem, isDragingInItemTop, isDragingInItemBottom);
	}
	if((selecednode && node->GetID()== selecednode->GetID()) || isDragingInItem)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}
	else if(isDragingInItemTop)
	{
		ImGui::Separator();
	}
	if(isDragingInItem)
	{
		_dropNodeParent = node;
		_dropNodeIndex = node->GetNumChildren();
	}
	else if(isDragingInItemTop && !isRoot)
	{
		_dropNodeParent = node->GetParent();
		_dropNodeIndex = nodeIndex -1;
	}
	else if (isDragingInItemBottom && !isRoot) {
		_dropNodeParent = node->GetParent();
		_dropNodeIndex = nodeIndex;
	}
	PODVector<Node*> children_show;
	for(auto citem: children)
	{
		if(!citem->HasTag(Global::notShowTag))
		{
			children_show.Push(citem);
		}
	}
	if (children_show.Size() == 0) {
		flags |= ImGuiTreeNodeFlags_Leaf;
	}
	bool isInWindow = IsInWindow(ImGui::GetCursorScreenPos());
	ImGui::PushID(node->GetID());
	bool node_open = ImGui::TreeNodeEx(nodeName.CString(), flags);
	if (isRoot) {
		_itemH = ImGui::GetItemRectSize().y;
	}
	ImGui::PopID();
	if(isInWindow)
	{
		DrawContextMenu(node);
		if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered()) {
			OnDoubleClicked();
		}
		else if (ImGui::IsItemClicked()) {
			OnClicked(node);
		}
		if (isDragingInItemBottom) {
			ImGui::Separator();
		}
	}
	if (node_open) {
		if (children_show.Size() > 0) {
			_foldState[node->GetID()] = true;
		}
		for(int ci=0;ci<children_show.Size();++ci){
			DrawNode(children_show[ci], false,ci);
		}
		ImGui::TreePop();
	}
}

void NodeTree::OnDrop()
{
	_isDraging = false;
	if (ImGui::BeginDragDropTarget()) {
		_isDraging = true;
		if (ImGui::IsMouseReleased(0)) {
			auto data = ImGui::AcceptDragDropPayload("drag_file", ImGuiDragDropFlags_AcceptBeforeDelivery);
			if (data) {
				String path;
				path.Resize(data->DataSize);
				memcpy((void*)path.CString(), data->Data, data->DataSize);
				if (AssetMgr::getInstance()->IsModelFile(path)) {
					SceneCtrl::getInstance()->AddModel(path,_dropNodeParent,_dropNodeIndex);
				}
				std::cout << "onDrop:drag_file" << std::endl;
			}

		}
		ImGui::EndDragDropTarget();
	}
}

void NodeTree::DrawContextMenu(Node* node)
{
	if (ImGui::BeginPopupContextItem("NodeContextMenus", 1)) {
		NodeContextMenus::DrawContextMenu(node);
		ImGui::EndPopup();
	}
}

}