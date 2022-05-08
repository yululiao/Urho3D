#include "NodeTree.h"
#include "EditorApp.h"

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
	DrawNode(EditorApp::getInstance()->GetSceneRoot());
	/*if(ImGui::BeginPopupContextWindow("context"),1)
	{
		ImGui::MenuItem("create","",false,true);
		ImGui::EndPopup();
	}*/
	ImGui::End();
}
void NodeTree::OnClicked(Node* node)
{
	EditorApp::getInstance()->SelectNode(node);
}
void NodeTree::OnDoubleClicked() 
{
	
}
void NodeTree::DrawNode(Node* node)
{
	String nodeName = node->GetName();
	int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	auto children = node->GetChildren(false);
	auto selecednode = EditorApp::getInstance()->GetSelectNode();
	if(selecednode && node->GetID()== selecednode->GetID())
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}
	if(children.Size()== 0)
	{
		flags |= ImGuiTreeNodeFlags_Leaf;
	}
	bool node_open = ImGui::TreeNodeEx(nodeName.CString(),flags);
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