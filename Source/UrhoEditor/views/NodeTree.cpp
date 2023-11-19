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
	DrawNode(EditorApp::GetInstance()->GetSceneRoot());
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
void NodeTree::DrawNode(Node* node)
{
	String nodeName = node->GetName();
	int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	auto children = node->GetChildren(false);
	auto selecednode = EditorApp::GetInstance()->GetSelectNode();
	if(selecednode && node->GetID()== selecednode->GetID())
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}
	if(children.Size()== 0)
	{
		flags |= ImGuiTreeNodeFlags_Leaf;
	}
	//std::string guid = Utils::GenGuid();
	ImGui::PushID(node->GetID());
	//ImGui::PushID((nodeName+ String(guid.c_str())).CString());
	bool node_open = ImGui::TreeNodeEx(nodeName.CString(),flags);
	ImGui::PopID();
	if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered())
	{
		OnDoubleClicked();
	}
	else if(ImGui::IsItemClicked())
	{
		OnClicked(node);
	}
	if(node_open)
	{
		for(auto item:children)
		{
			DrawNode(item);
		}
		ImGui::TreePop();
	}
}
}