#pragma once
#include "EditorWidget.h"

namespace Urho3DEditor 
{
typedef void (*ContexMenuHandle)(const String& path);
class FolderContexMenu : public Object
{
	URHO3D_OBJECT(FolderContexMenu, Object);
public:
	FolderContexMenu(const String& name);
	FolderContexMenu(const String& name, ContexMenuHandle callback);
	void SetCallBack(ContexMenuHandle callback);
	void OnClicked(const String& path);
	String& GetName();
protected:
	String _name;
	ContexMenuHandle _callBackFun;
	
};
//ECM is editor contextMenu
URHO3D_EVENT(ECM_CLICKED, Clicked) 
{
	URHO3D_PARAM(P_FolderContexMenu, FolderContexMenu);              // UIElement pointer
}
class ResTree :public EditorWidget 
{
public:
	ResTree();
	virtual ~ResTree();
	void Update() override;
	static void OnImport(const String& path);
protected:
	void OnDrag();
	void DrawNodeNoInWindows(int itemH, const String& name);
	void DrawResNode(const String& path,bool forceDraw);
	void DrawContextMenu(const String& path);
	void OnItemClick(const String& path);
	void AddContexMenu(FolderContexMenu* menu);
protected:
	int _dirIconId;
	int _nodeHeight = 0;
	Vector<FolderContexMenu*> _contexMenus;

};
}