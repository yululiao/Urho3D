#include "FolderFiles.h"
#include "ctrls/AssetMgr.h"
#include "EditorApp.h"

namespace Urho3DEditor{

FolderFiles::FolderFiles() {

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

void FolderFiles::OnItemClicked(const String& path) {
}
void FolderFiles::DrawNodeNoInWindows(int itemH) {

}
void FolderFiles::DrawFiles() {
	ImGuiIO& io = ImGui::GetIO();
	auto assetMgr = AssetMgr::getInstance();
	auto& nodeCache = assetMgr->nodeCache;
	if(assetMgr->lastSelectedFolder.Empty())
		return;
	auto& files = nodeCache[assetMgr->lastSelectedFolder].childFiles;
	int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick 
		| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;
	//flags |= ImGuiTreeNodeFlags_Selected;
	if(ImGui::IsMouseClicked(0) &&IsInWindow(io.MousePos))
	{
		assetMgr->selectedFiles.Clear();
	}
	for(auto& item:files)
	{
		//ImGui::Text(item.CString());
		String path = assetMgr->lastSelectedFolder + "/" + item;
		if(!AssetMgr::SurportExtSet.Contains(nodeCache[path].ext))
		{
			continue;
		}
		int flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth;
		if(assetMgr->selectedFiles.Contains(path))
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}
		ImGui::PushID(item.CString());
		bool open_node = ImGui::TreeNodeEx("", flags);
		if (ImGui::BeginDragDropSource(0)) {

			ImGui::SetDragDropPayload("drag_res", path.CString(), path.Length());
			OnDrag();
			ImGui::EndDragDropSource();
		}
		if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered(0)) {
			OnItemDoubleClicked(assetMgr->pathToFull(path));
		}
		else if (ImGui::IsItemClicked()) {
			assetMgr->selectedFiles.Clear();
			assetMgr->selectedFiles.Insert(path);
		}
		ImGui::PopID();
		ImGui::SameLine();
		int fileIcon = 0;
		if(AssetMgr::ImgExtSet.Contains(assetMgr->nodeCache[path].ext))
		{
			fileIcon = assetMgr->getImguiTex(assetMgr->pathToFull(path));
			if(fileIcon == 0)
			{
				fileIcon = assetMgr->getImguiTex("res/img/file.png");
			}
		}
		else
		{
			fileIcon = assetMgr->getImguiTex("res/img/file.png");
		}
		ImGui::Image(fileIcon, ImVec2(30, 30));
		ImGui::SameLine();
		ImGui::Text(item.CString());
		if (open_node) {
			ImGui::TreePop();
		}
	}

}

void FolderFiles::OnDrag() {

}

void FolderFiles::OnItemDoubleClicked(const String& path) {
	AssetMgr::getInstance()->OpenScene(path);
	EditorApp::GetInstance()->SetCurTool("move");
}

}
