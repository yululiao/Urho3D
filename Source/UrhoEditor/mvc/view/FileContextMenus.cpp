#include "FileContextMenus.h"
#include "Global.h"
#include "ctrl/res/AssetMgr.h"
#include "imgui.h"
#include "NdfMgr.h"

namespace Urho3DEditor
{

FileContexMenu::FileContexMenu(const String& name)
	:Object(Global::context),_callBackFun(nullptr)
{
	_name = name;
}

FileContexMenu::FileContexMenu(const String& name, ContexMenuHandle callback)
	:Object(Global::context)
{
	_name = name;
	_callBackFun = callback;
}

void FileContexMenu::SetCallBack(ContexMenuHandle callback) 
{
	_callBackFun = callback;
}

void FileContexMenu::OnClicked(const String& path)
{
	if (_callBackFun) {
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
	eventData[P_FileContexMenu] = this;
	SendEvent(ECM_CLICKED, eventData);
}

String& FileContexMenu::GetName() {
	return _name;
}

Vector<FileContexMenu*> FileContextMenus::folderContexMenus;
Vector<FileContexMenu*> FileContextMenus::fileContexMenus;
bool FileContextMenus::_init = false;
void FileContextMenus::Init()
{
	if(_init)
		return;
	//文件夹右键菜单
	AddFolderContexMenu(new FileContexMenu("GoTo", &OnGoToFolder));
	AddFolderContexMenu(new FileContexMenu("Import", &OnFolderImport));
	//文件右键菜单
	AddFileContexMenu(new FileContexMenu("GoTo", &OnGoToFile));
	AddFileContexMenu(new FileContexMenu("Delete", &OnDeleteFile));
	_init = true;
}
void FileContextMenus::RegistFolderContextMenu() {
}
void FileContextMenus::RegistFileContextMenu() {
}
void FileContextMenus::AddFolderContexMenu(FileContexMenu* menu)
{
	folderContexMenus.Push(menu);
}
void FileContextMenus::AddFileContexMenu(FileContexMenu* menu)
{
	fileContexMenus.Push(menu);
}

void FileContextMenus::DrawFolderContext(const String& path)
{
	for (auto item : folderContexMenus) {
		if (ImGui::MenuItem(item->GetName().CString())) {
			item->OnClicked(path);
		}
	}

}

void FileContextMenus::OnFolderImport(const String& path) {
	AssetMgr::getInstance()->ImportFbx(path);
}

void FileContextMenus::DrawFileContext(const String& path)
{
	for (auto item : fileContexMenus) {
		if (ImGui::MenuItem(item->GetName().CString())) {
			item->OnClicked(path);
		}
	}
}

void FileContextMenus::OnDeleteFile(const String& path) {

}

void FileContextMenus::OnGoToFile(const String& path) {
	NdfMgr::GetInstance()->GoToPath(path);
}

void FileContextMenus::OnGoToFolder(const String& path) {
	NdfMgr::GetInstance()->GoToPath(path);
}

}
