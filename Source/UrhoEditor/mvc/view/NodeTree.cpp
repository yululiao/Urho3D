#include "NodeTree.h"
#include "Utils.h"
#include "NodeContextMenus.h"

namespace Urho3DEditor 
{
NodeTree::NodeTree(SelectionController& selectionCtrl, SelectionModel& selectionModel, SceneCtrl& sceneCtrl, SceneManipulationController& sceneManipCtrl, float dpiScale)
	: selectionController_(selectionCtrl)
	, selectionModel_(selectionModel)
	, sceneCtrl_(sceneCtrl)
	, sceneManipController_(sceneManipCtrl)
	, dpiScale_(dpiScale)
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
	ImGui::PushID("NodeTreeDropRect");//����һ���ʹ��ڵȴ�Ĳ��ɼ��������ڽ�����ק���Ҽ��˵�
	ImGui::Dummy(winSize);
	OnDrop();
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
		_contextClickNodes.Clear();
		std::cout << "node tree Mouse_Right clicked clear _contextClickNode!!!! " << std::endl;
	}
	DrawContextMenu();
	ImGui::PopID();
	ImGui::SetCursorScreenPos(oldPos);//����Dummy��ʼλ�û��Ƴ����ڵ���
	DrawNode(sceneCtrl_.GetRoot(),true,0);
	/*if(ImGui::BeginPopupContextWindow("context"),1)
	{
		ImGui::MenuItem("create","",false,true);
		ImGui::EndPopup();
	}*/
	ImGui::End();
}
void NodeTree::OnClicked(Node* node)
{
	selectionController_.OnNodeClicked(node);
}
void NodeTree::OnDoubleClicked() 
{
	
}
//������Item����֮ǰ����
bool NodeTree::IsMouseInCurItem(int itemH)
{
	bool in = false;
	ImVec2 curPos = ImGui::GetCursorScreenPos();
	ImVec2 curMousePos = ImGui::GetIO().MousePos;
	if(curMousePos.y >= curPos.y && curMousePos.y <= curPos.y + itemH)
	{
		in = true;
	}
	return in;
}

bool NodeTree::IsMouseInCurItemTop(int itemH) {
	bool in = false;
	ImVec2 curPos = ImGui::GetCursorScreenPos();
	int gap = 2 * dpiScale_;
	if (ImGui::GetIO().MousePos.y >= curPos.y - gap && ImGui::GetIO().MousePos.y <= curPos.y) {
		in = true;
	}
	return in;
}

bool NodeTree::IsMouseInCurItemBottom(int itemH) {
	bool in = false;
	ImVec2 curPos = ImGui::GetCursorScreenPos();
	int gap = 2 * dpiScale_;
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

void NodeTree::IsDragMouseInTB( bool& isDragingInItemTop, bool& isDragingInItemBottom)
{
	isDragingInItemTop = IsMouseInCurItemTop(_itemH);
	isDragingInItemBottom = false;
	if (!isDragingInItemTop) {
		isDragingInItemBottom = IsMouseInCurItemBottom(_itemH);
	}
}

void NodeTree::DrawNode(Node* node,bool isRoot,int nodeIndex)
{
	String nodeName = node->GetName();
	int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick 
		| ImGuiTreeNodeFlags_SpanAvailWidth;
	auto& children = node->GetChildren();
	auto selecednode = selectionModel_.GetSelectedNode();
	if(isRoot)
	{
		flags |= ImGuiTreeNodeFlags_DefaultOpen;
	}
	bool isDragingInItem = false;
	bool isDragingInItemTop = false;
	bool isDragingInItemBottom = false;
	bool isMouseInCurItem = IsMouseInCurItem(_itemH);
	if(_isDraging)
	{
		if(isMouseInCurItem)
			isDragingInItem = true;
		else
			IsDragMouseInTB(isDragingInItemTop, isDragingInItemBottom);
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
		_dropNodeIndex = nodeIndex + 1;
	}
	PODVector<Node*> children_show;
	for(auto citem: children)
	{
		if(!citem->HasTag(sceneCtrl_.GetNotShowTag()))
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
		if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered()) {
			OnDoubleClicked();
		}
		else if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
			OnClicked(node);
		}
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
			if(isMouseInCurItem)
			{
				_contextClickNodes.Push(node);
				std::cout << "node tree Mouse_Right clicked,set _contextClickNode to node" << node->GetName().CString() << std::endl;
			}
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
				sceneManipController_.ImportModel(path, _dropNodeParent, _dropNodeIndex);
				std::cout << "onDrop:drag_file" << std::endl;
			}
			_isDraging = false;
			_dropNodeParent = nullptr;
		}
		ImGui::EndDragDropTarget();
	}
}

void NodeTree::DrawContextMenu()
{
	if (ImGui::BeginPopupContextItem("NodeContextMenus", 1)) {
		bool clicked = NodeContextMenus::DrawContextMenu(_contextClickNodes);
		if(clicked)
		{
			_contextClickNodes.Clear();
		}
		ImGui::EndPopup();
	}
}

}