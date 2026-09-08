#include "FolderFiles.h"
#include "ctrl/res/AssetBrowserController.h"
#include "FileContextMenus.h"

namespace Urho3DEditor{

FolderFiles::FolderFiles(ProjectController& projectCtrl, ToolController& toolCtrl, AssetBrowserController& assetBrowserCtrl, float fontSize)
	: projectController_(projectCtrl)
	, toolController_(toolCtrl)
	, assetBrowserCtrl_(assetBrowserCtrl)
	, fontSize_(fontSize)
{
}
FolderFiles::~FolderFiles() {
}
void FolderFiles::Update() {
	ImGui::Begin("Files", &showing);
	ImVec2 newSize = ImGui::GetWindowSize();
	_winPos = ImGui::GetWindowPos();
	if (newSize.x != winSize.x || newSize.y != winSize.y) {
		winSize = newSize;
	}
	DrawFiles();
	ImGui::End();

}

void FolderFiles::OnItemClicked(const String& path)
{
	assetBrowserCtrl_.SelectFile(path);
}
void FolderFiles::DrawNodeNoInWindows(int itemH) {

}
void FolderFiles::DrawFiles() {
	ImGuiIO& io = ImGui::GetIO();
	const String& lastSelectedFolder = assetBrowserCtrl_.GetLastSelectedFolder();
	if(lastSelectedFolder.Empty())
		return;
	const StringVector& files = assetBrowserCtrl_.GetChildFiles(lastSelectedFolder);
	int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick 
		| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;
	//flags |= ImGuiTreeNodeFlags_Selected;
	if(ImGui::IsMouseClicked(0) &&IsInWindow(io.MousePos))
	{
		assetBrowserCtrl_.ClearFileSelection();
	}
	for(auto& item:files)
	{
		//ImGui::Text(item.CString());
		String path = lastSelectedFolder + "/" + item;
		if(!AssetBrowserController::IsSupportedExt(assetBrowserCtrl_.GetNodeExt(path)))
		{
			continue;
		}
		int flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth;
		bool selected = assetBrowserCtrl_.IsFileSelected(path);
		if(selected)
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}
		ImGui::PushID(item.CString());
		bool open_node = ImGui::TreeNodeEx("", flags);
		DrawContextMenu(path);
		OnDrag(path);
		if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered(0)) {
			OnItemDoubleClicked(assetBrowserCtrl_.PathToFull(path));
		}
		else if (ImGui::IsItemClicked(ImGuiMouseButton_Left) ) {
			OnItemClicked(path);
			if(AssetBrowserController::IsInspectableExt(assetBrowserCtrl_.GetNodeExt(path)))
			{
				// selection type is updated through SelectionController::OnFileSelected
			}
			
		}
		ImGui::PopID();
		ImGui::SameLine();
		int fileIcon = 0;
		if(AssetBrowserController::IsImageExt(assetBrowserCtrl_.GetNodeExt(path)))
		{
			fileIcon = assetBrowserCtrl_.GetImguiTex(assetBrowserCtrl_.PathToFull(path));
			if(fileIcon == 0)
			{
				fileIcon = assetBrowserCtrl_.GetImguiTex("res/img/file.png");
			}
		}
		else
		{
			fileIcon = assetBrowserCtrl_.GetImguiTex("res/img/file.png");
		}
		ImGui::Image(fileIcon, ImVec2(fontSize_, fontSize_));
		ImGui::SameLine();
		ImGui::Text(item.CString());
		if (open_node) {
			ImGui::TreePop();
		}
	}

}

void FolderFiles::OnDrag(const String& path) {
	if (ImGui::BeginDragDropSource(0)) {
		ImGui::SetDragDropPayload("drag_file", path.CString(), path.Length());
		ImGui::EndDragDropSource();
	}
}

void FolderFiles::OnItemDoubleClicked(const String& path) {
	projectController_.OpenScene(path);
	toolController_.SetTool("move");
}

void FolderFiles::DrawContextMenu(const String& path)
{
	if (ImGui::BeginPopupContextItem("FileContext", 1)) {
		OnItemClicked(path);
		FileContextMenus::DrawFileContext(path);
		ImGui::EndPopup();
	}
}

}
