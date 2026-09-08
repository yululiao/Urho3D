#pragma once
#include "EditorWidget.h"
#include "Urho3D/Container/Str.h"
#include "Urho3D/Core/Context.h"
#include "Urho3D/Scene/Scene.h"
#include "ctrl/scene/SelectionController.h"
#include "model/SelectionModel.h"
#include "ctrl/scene/SceneCtrl.h"
#include "ctrl/scene/SceneManipulationController.h"

using namespace Urho3D;

namespace Urho3DEditor
{
class NodeTree:public EditorWidget
{
public:
	NodeTree(SelectionController& selectionCtrl, SelectionModel& selectionModel, SceneCtrl& sceneCtrl, SceneManipulationController& sceneManipCtrl, float dpiScale);
	virtual ~NodeTree();
	void Update() override;
private:
	void OnClicked(Node* node);
	void OnDoubleClicked();
	bool IsMouseInCurItem(int itemH);
	bool IsMouseInCurItemTop(int itemH);
	bool IsMouseInCurItemBottom(int itemH);
	void DrawNodeNoInWindows(int itemH);
	void IsDragMouseInTB(bool& isInItemTop, bool& isInItemBottom);
	void DrawNode(Node* node,bool isRoot,int nodeIdex);
	void OnDrop();
	void DrawContextMenu();
	HashMap<int, bool> _foldState;
	float _itemH = 12;
	bool _isDraging = false;
	Node* _dropNodeParent = nullptr;
	int _dropNodeIndex = 0;
	Vector<Node*> _contextClickNodes;;

	SelectionController& selectionController_;
	SelectionModel& selectionModel_;
	SceneCtrl& sceneCtrl_;
	SceneManipulationController& sceneManipController_;
	float dpiScale_;
};
}