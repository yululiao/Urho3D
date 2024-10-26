#include "ResTree.h"
#include "ctrls/AssetMgr.h"
#include "EditorApp.h"
#include "Urho3D/IO/FileSystem.h"
#include "Urho3D/Container/HashSet.h"

namespace Urho3DEditor 
{
static Urho3D::HashSet<String> SurportExtSet={
	".fbx",".xml",".json",".umaterial",
	".png",".tga",".jpg",".dds",".uprefab",".uscene"
};
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
	auto rootPath = EditorApp::GetInstance()->GetAssetRoot();
	if(_cacheDirty){
		RefreshNodeCahce(rootPath);
		_cacheDirty = false;
	}
	ImGui::Begin("ResTree",&showing);
	ImVec2 newSize = ImGui::GetWindowSize();
	_winPos = ImGui::GetWindowPos();
	if (newSize.x != winSize.x || newSize.y != winSize.y) {
		winSize = newSize;
	}
	DrawResNode(rootPath,true);
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
	AssetMgr::getInstance()->OpenScene(path);
	EditorApp::GetInstance()->SetCurTool("move");
}

void ResTree::DrawNodeNoInWindows(int itemH,const String& name)
{
	//Draw nothing,just jump
	ImVec2 curPos = ImGui::GetCursorScreenPos();
	curPos.y = curPos.y + itemH;
	ImGui::SetCursorScreenPos(curPos);
}

void ResTree::DrawResNode(const String& path, bool forceDraw)
{
	int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	bool needDraw = true;
	if(_nodeCache[path].dirs.Size() == 0)
	{
		if(!SurportExtSet.Contains(_nodeCache[path].ext))
		{
			needDraw = false;
		}
		flags |= ImGuiTreeNodeFlags_Leaf;
	}
	if(!needDraw)
		return;
	if(_selected == path)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}
	bool isInWindow = IsInWindow(ImGui::GetCursorScreenPos());
	if(isInWindow ||forceDraw)
	{
		bool open_node = ImGui::TreeNodeEx(_nodeCache[path].name.CString(), flags);
		if(_nodeHeight == 0)
			_nodeHeight = ImGui::GetItemRectSize().y;
		if (ImGui::BeginDragDropSource(0)) {
			ImGui::SetDragDropPayload("drag_res", path.CString(), path.Length());
			OnDrag();
			ImGui::EndDragDropSource();
		}
		if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered(0)) {
			OnItemDoubleClicked(path);
		}
		else if (ImGui::IsItemClicked()) {
			_selected = path;
		}
		_nodeCache[path].fold = !open_node;
		if (open_node) {
			if (ImGui::BeginPopupContextItem("ResContext", 1)) {
				if (ImGui::MenuItem("Import")) {
					OnImport(path);
				}
				ImGui::EndPopup();
			}
			for (auto &item : _nodeCache[path].dirs) {
				DrawResNode(path + "/" + item,false);
			}
			ImGui::TreePop();
		}
	}
	else
	{
		DrawNodeNoInWindows(_nodeHeight, _nodeCache[path].name);
		if (!_nodeCache[path].fold) {
			for (auto& item : _nodeCache[path].dirs) {
				DrawResNode(path + "/" + item,false);
			}
		}
	}
}
void ResTree::RefreshNodeCahce(const String& path) {
	auto fileSystem = EditorApp::GetInstance()->GetSubsystem<FileSystem>();
	StringVector dirs;
	fileSystem->ScanDir(dirs, path, ".*", SCAN_DIRS | SCAN_FILES, false);
	for (auto &item: dirs){
		if (item != "." && item != "..") {
			_nodeCache[path].dirs.Push(item);
			RefreshNodeCahce(path + "/" + item);
		}
	}
	auto pathItems = path.Split('/');
	if (pathItems.Size() == 0)
		return;
	_nodeCache[path].name = pathItems[pathItems.Size() - 1];
	_nodeCache[path].ext = GetExtension(_nodeCache[path].name, true);

}
}