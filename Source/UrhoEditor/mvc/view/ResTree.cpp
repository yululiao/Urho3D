#include "ResTree.h"
#include "ctrl/res/AssetMgr.h"
#include "EditorApp.h"
#include "Urho3D/IO/FileSystem.h"
#include "Urho3D/Container/HashSet.h"
#include "Global.h"

namespace Urho3DEditor 
{
ResTree::ResTree() 
{
	_dirIconId = AssetMgr::getInstance()->getImguiTex("res/img/folder.png");
	AddContexMenu(new FolderContexMenu("Import", &OnImport));
	
}
ResTree::~ResTree() 
{
}
void ResTree::Update() 
{
	if(!showing)
		return;
	String& relativeRootPath = AssetMgr::getInstance()->GetRelativeAssetRoot();
	ImGui::Begin("Folders",&showing);
	ImVec2 newSize = ImGui::GetWindowSize();
	_winPos = ImGui::GetWindowPos();
	if (newSize.x != winSize.x || newSize.y != winSize.y) {
		winSize = newSize;
	}
	ImGuiIO& io = ImGui::GetIO();
	if (ImGui::IsMouseClicked(0) && IsInWindow(io.MousePos)) {
		AssetMgr::getInstance()->selectedFolders.Clear();
		AssetMgr::getInstance()->lastSelectedFolder = "";
	}
	DrawResNode(relativeRootPath,true);
	ImGui::End();
}
void ResTree::OnDrag() 
{

}
void ResTree::OnImport(const String& path)
{
	AssetMgr::getInstance()->ImportFbx(path);

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
	auto& nodeCache = AssetMgr::getInstance()->nodeCache;
	int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	//int flags = ImGuiTreeNodeFlags_OpenOnArrow;
	//Just draw folder,files draw in FolderFiles Window.
	bool needDraw = !nodeCache[path].isFile;
	/*if(!SurportExtSet.Contains(nodeCache[path].ext))
	{
		needDraw = false;
	}
	*/
	if(nodeCache[path].childDirs.Size() == 0)
	{
		flags |= ImGuiTreeNodeFlags_Leaf;
	}
	if(!needDraw)
		return;
	bool sellected = AssetMgr::getInstance()->selectedFolders.Contains(path);
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
	ImGui::PushID(nodeCache[path].name.CString());
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
		ImGui::Text(nodeCache[path].name.CString());
	}
	nodeCache[path].fold = !open_node;
	if (open_node) {
		for (auto& item : nodeCache[path].childDirs) {
			DrawResNode(path + "/" + item, false);
		}
		ImGui::TreePop();

	}
}

void ResTree::DrawContextMenu(const String& path) {
	if (ImGui::BeginPopupContextItem("ResContext", 1)) {
		OnItemClick(path);
		for(auto item:_contexMenus){
			if (ImGui::MenuItem(item->GetName().CString())) {
				item->OnClicked(path);
			}
		}
		
		ImGui::EndPopup();
	}
}

void ResTree::OnItemClick(const String& path) {
	AssetMgr::getInstance()->selectedFolders.Clear();
	AssetMgr::getInstance()->selectedFolders.Insert(path);
	AssetMgr::getInstance()->lastSelectedFolder = path;
}

void ResTree::AddContexMenu(FolderContexMenu* menu)
{
	_contexMenus.Push(menu);
}

FolderContexMenu::FolderContexMenu(const String& name)
	:Object(Global::context)
{
	_name = name;
}

FolderContexMenu::FolderContexMenu(const String& name, ContexMenuHandle callback)
   :Object(Global::context)
{
	_name = name;
	_callBackFun = callback;
}

void FolderContexMenu::SetCallBack(ContexMenuHandle callback) {
	_callBackFun = callback;
}

void FolderContexMenu::OnClicked(const String& path) {
	if(_callBackFun)
	{
		_callBackFun(path);
	}
	//for lua script call back
	// local contexMenu = FolderContexMenu:new()
	/*
	SubscribeToEvent(contexMenu, "Clicked",
        function (eventType, eventData)
           -- on clicked call
        end)
	*/
	using namespace Clicked;
	VariantMap& eventData = GetEventDataMap();
	eventData[P_FolderContexMenu] = this;
	SendEvent(ECM_CLICKED, eventData);

}

String& FolderContexMenu::GetName() {
	return _name;
}


}