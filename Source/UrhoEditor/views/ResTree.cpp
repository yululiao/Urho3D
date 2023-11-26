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
	ImGui::Begin("ResTree",&showing);
	ImVec2 newSize = ImGui::GetWindowSize();
	_winPos = ImGui::GetWindowPos();
	if (newSize.x != winSize.x || newSize.y != winSize.y) {
		winSize = newSize;
	}
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
	AssetMgr::getInstance()->OpenScene(path);
	EditorApp::GetInstance()->SetCurTool("move");
}

void ResTree::DrawNodeNoInWindows(int itemH)
{
	//Draw nothing
	ImVec2 curPos = ImGui::GetCursorScreenPos();
	curPos.y = curPos.y + itemH;
	ImGui::SetCursorScreenPos(curPos);
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
	bool needDraw = true;
	if(dirs.Size() == 0)
	{
		String lowerExt = GetExtension(name, true);
		if(!SurportExtSet.Contains(lowerExt))
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
	if(IsInWindow(ImGui::GetCursorScreenPos()))
	{
		bool open_node = ImGui::TreeNodeEx(name.CString(), flags);
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
		if (open_node) {
			if (dirs.Size() > 0) {
				_foldState[path] = true;
			}
			if (ImGui::BeginPopupContextItem("ResContext", 1)) {
				if (ImGui::MenuItem("Import")) {
					OnImport(path);
				}
				ImGui::EndPopup();
			}
			for (auto item : dirs) {
				if (item != "." && item != "..") {
					DrawResNode(path + "/" + item);
				}
			}
			ImGui::TreePop();
		}
	}
	else
	{
		DrawNodeNoInWindows(ImGui::GetItemRectSize().y);
		if(_foldState.Contains(path)&&_foldState[path])
		{
			for (auto item : dirs) {
				if (item != "." && item != "..") {
					DrawResNode(path + "/" + item);
				}
			}
		}
	}
	

}
}