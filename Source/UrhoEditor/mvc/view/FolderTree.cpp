#include "FolderTree.h"
#include "Utils.h"
#include "ctrl/res/AssetBrowserController.h"
#include "FileContextMenus.h"

namespace Urho3DEditor 
{
FolderTree::FolderTree(AssetBrowserController& assetBrowserCtrl)
	: assetBrowserCtrl_(assetBrowserCtrl)
{
	_dirIconId = assetBrowserCtrl_.GetImguiTex("res/img/folder.png");
}
FolderTree::~FolderTree() 
{
}
void FolderTree::Update() 
{
	if(!showing)
		return;
	String& relativeRootPath = const_cast<String&>(assetBrowserCtrl_.GetRelativeAssetRoot());
	ImGui::Begin("Folders",&showing);
	ImVec2 newSize = ImGui::GetWindowSize();
	_winPos = ImGui::GetWindowPos();
	if (newSize.x != winSize.x || newSize.y != winSize.y) {
		winSize = newSize;
	}
	ImGuiIO& io = ImGui::GetIO();
	if (ImGui::IsMouseClicked(0) && IsInWindow(io.MousePos)) {
		assetBrowserCtrl_.ClearFolderSelection();
	}
	DrawResNode(relativeRootPath,true);
	ImGui::End();
}
void FolderTree::OnDrag() 
{

}

void FolderTree::DrawNodeNoInWindows(int itemH,const String& name)
{
	//Draw nothing,just jump
	ImVec2 curPos = ImGui::GetCursorScreenPos();
	curPos.y = curPos.y + itemH;
	ImGui::SetCursorScreenPos(curPos);
}

void FolderTree::DrawResNode(const String& path, bool forceDraw)
{
	if (!assetBrowserCtrl_.NodeExists(path))
		return;
	int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	//Just draw folder,files draw in FolderFiles Window.
	bool needDraw = assetBrowserCtrl_.IsFolder(path);
	const StringVector& childDirs = assetBrowserCtrl_.GetChildDirs(path);
	if(childDirs.Size() == 0)
	{
		flags |= ImGuiTreeNodeFlags_Leaf;
	}
	if(!needDraw)
		return;
	bool sellected = assetBrowserCtrl_.IsFolderSelected(path);
	if(sellected)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}
	if(forceDraw)
	{
		flags |= ImGuiTreeNodeFlags_DefaultOpen;
	}
	bool isInWindow =IsInWindow(ImGui::GetCursorScreenPos());
	//----------------------TreeNode-----------------------
	ImGui::PushID(assetBrowserCtrl_.GetNodeName(path).CString());
	bool open_node = ImGui::TreeNodeEx("", flags);
	if (_nodeHeight == 0)
		_nodeHeight = ImGui::GetItemRectSize().y;
	if (isInWindow) {
		DrawContextMenu(path);
		if (ImGui::BeginDragDropSource(0)) {
			ImGui::SetDragDropPayload("drag_folder", path.CString(), path.Length());
			OnDrag();
			ImGui::EndDragDropSource();
		}
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
			OnItemClick(path);
		}
	}
	ImGui::PopID();
	//-------------------------TreeNode End------------------------
	if (isInWindow) {
		ImGui::SameLine();
		ImGui::Image(_dirIconId, ImVec2(_nodeHeight, _nodeHeight));
		ImGui::SameLine();
		ImGui::Text(assetBrowserCtrl_.GetNodeName(path).CString());
	}
	assetBrowserCtrl_.SetNodeFolded(path, !open_node);
	if (open_node) {
		for (auto& item : childDirs) {
			DrawResNode(path + "/" + item, false);
		}
		ImGui::TreePop();

	}
}

void FolderTree::DrawContextMenu(const String& path) {
	if (ImGui::BeginPopupContextItem("FolderContext", 1)) {
		OnItemClick(path);
		FileContextMenus::DrawFolderContext(path);
		ImGui::EndPopup();
	}
}

void FolderTree::OnItemClick(const String& path) {
	assetBrowserCtrl_.SelectFolder(path);
}



}
