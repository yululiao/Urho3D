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
	void DrawNode(Node* node);

};
}