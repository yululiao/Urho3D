#pragma once
#include "EditorWidget.h"


namespace Urho3DEditor{
class AssetBrowserController;

class FolderTree :public EditorWidget 
{
public:
	FolderTree(AssetBrowserController& assetBrowserCtrl);
	virtual ~FolderTree();
	void Update() override;
protected:
	void OnDrag();
	void DrawNodeNoInWindows(int itemH, const String& name);
	void DrawResNode(const String& path,bool forceDraw);
	void DrawContextMenu(const String& path);
	void OnItemClick(const String& path);
	
protected:
	AssetBrowserController& assetBrowserCtrl_;
	int _dirIconId;
	int _nodeHeight = 0;

};
}
