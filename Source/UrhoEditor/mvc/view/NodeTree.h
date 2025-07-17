#pragma once
#include "EditorWidget.h"
#include "Urho3D/Container/Str.h"
#include "Urho3D/Core/Context.h"
#include "Urho3D/Scene/Scene.h"

using namespace Urho3D;

namespace Urho3DEditor
{
class NodeTree:public EditorWidget
{
public:
	NodeTree();
	virtual ~NodeTree();
	void Update() override;
private:
	void OnClicked(Node* node);
	void OnDoubleClicked();
	bool isMouseInCurItem(int itemH);
	bool isMouseInCurItemTop(int itemH);
	bool isMouseInCurItemBottom(int itemH);
	void DrawNodeNoInWindows(int itemH);
	void GetDragMouseInfo(bool& isInItem, bool& isInItemTop, bool& isInItemBottom);
	void DrawNode(Node* node,bool isRoot,int nodeIdex);
	void OnDrop();
	void DrawContextMenu(Node* node);
	HashMap<int, bool> _foldState;
	float _itemH = 12;
	bool _isDraging = false;
	Node* _dropNodeParent = nullptr;
	int _dropNodeIndex = 0;

};
}