#include "NodeTree.h"
#include "EditorApp.h"
#include "Utils.h"

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
	DrawNode(EditorApp::GetInstance()->GetSceneRoot(),true);
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

void NodeTree::DrawNodeNoInWindows(int itemH)
{
	//Draw nothing
	ImVec2 curPos = ImGui::GetCursorScreenPos();
	curPos.y = curPos.y + itemH;
	ImGui::SetCursorScreenPos(curPos);
}

void NodeTree::DrawNode(Node* node,bool isRoot)
{
	String nodeName = node->GetName();
	int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	auto children = node->GetChildren(false);
	auto selecednode = EditorApp::GetInstance()->GetSelectNode();
	if(isRoot)
	{
		flags |= ImGuiTreeNodeFlags_DefaultOpen;
	}
	if(selecednode && node->GetID()== selecednode->GetID())
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}
	if(children.Size()== 0)
	{
		flags |= ImGuiTreeNodeFlags_Leaf;
	}
	if (IsInWindow(ImGui::GetCursorScreenPos()))
	{
		ImGui::PushID(node->GetID());
		bool node_open = ImGui::TreeNodeEx(nodeName.CString(), flags);
		ImGui::PopID();
		if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered()) {
			OnDoubleClicked();
		}
		else if (ImGui::IsItemClicked()) {
			OnClicked(node);
		}
		if (node_open) {
			if(children.Size() > 0)
			{
				_foldState[node->GetID()] = true;
			}
			for (auto item : children) {
				DrawNode(item,false);
			}
			ImGui::TreePop();
		}
	}
	else
	{
		DrawNodeNoInWindows(ImGui::GetItemRectSize().y);
		if (_foldState.Contains(node->GetID()) && _foldState[node->GetID()]) {
			for (auto item : children) {
				DrawNode(item,false);
			}
		}
	}
	
}
}