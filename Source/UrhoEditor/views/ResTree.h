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
	void DrawResNode(const String& path);
private:
	String _selected;
};
}