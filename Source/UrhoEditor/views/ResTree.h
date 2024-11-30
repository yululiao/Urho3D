#pragma once
#include "EditorWidget.h"

namespace Urho3DEditor 
{
class ResTree :public EditorWidget 
{
public:
	ResTree();
	virtual ~ResTree();
	void Update() override;
private:
	void OnDrag();
	void OnImport(const String& path);
	void DrawNodeNoInWindows(int itemH, const String& name);
	void DrawResNode(const String& path,bool forceDraw);
private:
	int _dirIconId;
	int _nodeHeight = 0;

};
}