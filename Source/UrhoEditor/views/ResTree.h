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
	void OnItemDoubleClicked(const String& path);
	void DrawNodeNoInWindows(int itemH, const String& name);
	void DrawResNode(const String& path,bool forceDraw);
	void RefreshNodeCahce(const String& path);
	struct NodeCahce
	{
		StringVector dirs;
		String name;
		String ext;
		bool fold = true;
	};
private:
	String _selected;
	HashMap <String,NodeCahce> _nodeCache;
	bool _cacheDirty = true;
	int _nodeHeight = 0;
};
}