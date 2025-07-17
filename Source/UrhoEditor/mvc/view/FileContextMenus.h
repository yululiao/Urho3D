#pragma once
#include "Urho3D/Core/Context.h"
#include "Urho3D/Core/Main.h"
#include "Urho3D/Engine/Engine.h"
#include "Urho3D/Container/Str.h"
#include "Urho3D/Container/Vector.h"

using namespace Urho3D;
namespace Urho3DEditor
{
typedef void (*ContexMenuHandle)(const String& path);
class FileContexMenu : public Object {
	URHO3D_OBJECT(FileContexMenu, Object);
public:
	FileContexMenu(const String& name);
	FileContexMenu(const String& name, ContexMenuHandle callback);
	void SetCallBack(ContexMenuHandle callback);
	void OnClicked(const String& path);
	String& GetName();
protected:
	String _name;
	ContexMenuHandle _callBackFun;

};
class FileContextMenus
{
public:
	static void Init();
	static void RegistFolderContextMenu();
	static void RegistFileContextMenu();
	static void AddFolderContexMenu(FileContexMenu* menu);
	static void AddFileContexMenu(FileContexMenu* menu);
	static void DrawFolderContext(const String& path);
	static void DrawFileContext(const String& path);
	static void OnFolderImport(const String& path);
	static void OnDeleteFile(const String& path);
	static void OnGoToFile(const String& path);
	static void OnGoToFolder(const String& path);
protected:
	static bool _init;
	static Vector<FileContexMenu*> folderContexMenus;
	static Vector<FileContexMenu*> fileContexMenus;
};

//ECM is editor file contextMenu
URHO3D_EVENT(EFCM_CLICKED, Clicked) {
	URHO3D_PARAM(P_FileContexMenu, FileContexMenu);              // UIElement pointer
};

}