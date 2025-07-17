#pragma once
#include "EditorWidget.h"


namespace Urho3DEditor{
class ResTree :public EditorWidget 
{
public:
	ResTree();
	virtual ~ResTree();
	void Update() override;
protected:
	void OnDrag();
	void DrawNodeNoInWindows(int itemH, const String& name);
	void DrawResNode(const String& path,bool forceDraw);
	void DrawContextMenu(const String& path);
	void OnItemClick(const String& path);
	
protected:
	int _dirIconId;
	int _nodeHeight = 0;

};
}