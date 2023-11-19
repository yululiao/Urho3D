#include "ResTree.h"
#include "ctrls/AssetMgr.h"
#include "EditorApp.h"
#include "Urho3D/IO/FileSystem.h"

namespace Urho3DEditor 
{
ResTree::ResTree() 
{
}
ResTree::~ResTree() 
{
}
void ResTree::Update() 
{
	if(!showing)
		return;
	ImGui::Begin("ResTree",&showing);
	auto root = EditorApp::GetInstance()->GetAssetRoot();
	DrawResNode(root);
	ImGui::End();
}
void ResTree::OnDrag() 
{

}
void ResTree::OnImport(const String& path)
{
	AssetMgr::getInstance()->ImportFbx(path);

}
void ResTree::OnItemDoubleClicked(const String& path)
{
	if(path.Find(String(".scene"))!= String::NPOS)
	{
		AssetMgr::getInstance()->OpenScene(path);
		EditorApp::GetInstance()->SetCurTool("move");
	}
}
void ResTree::DrawResNode(const String& path)
{
	auto fileSystem = EditorApp::GetInstance()->GetSubsystem<FileSystem>();
	StringVector dirs;
	fileSystem->ScanDir(dirs,path,".*",SCAN_DIRS|SCAN_FILES,false);
	auto pathItems = path.Split('/');
	if(pathItems.Size() == 0)
		return;
	auto name = pathItems[pathItems.Size()-1];
	int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	if(dirs.Size() == 0)
	{
		flags |= ImGuiTreeNodeFlags_Leaf;
	}
	if(_selected == path)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}
	bool open_node = ImGui::TreeNodeEx(name.CString(),flags);
	if(ImGui::BeginDragDropSource(0)){
		ImGui::SetDragDropPayload("drag_res",path.CString(),path.Length());
		OnDrag();
		ImGui::EndDragDropSource();
	}
	if(ImGui::IsMouseDoubleClicked(0)&&ImGui::IsItemHovered(0))
	{
		OnItemDoubleClicked(path);
	}
	else if (ImGui::IsItemClicked()) 
	{
		_selected = path;
	}
	if(open_node)
	{
		if (ImGui::BeginPopupContextItem("ResContext", 1)) 
		{
			if (ImGui::MenuItem("Import")) 
			{
				OnImport(path);
			}
			ImGui::EndPopup();
		}
		for(auto item:dirs)
		{
			if(item != "."&&item!="..")
			{
				DrawResNode(path + "/"+item);
			}
		}
		ImGui::TreePop();
	}

}
}