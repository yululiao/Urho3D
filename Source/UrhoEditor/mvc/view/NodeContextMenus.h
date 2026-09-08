#pragma once
#include "Urho3D/Core/Context.h"
#include "Urho3D/Core/Main.h"
#include "Urho3D/Engine/Engine.h"
#include "Urho3D/Container/Str.h"
#include "Urho3D/Container/Vector.h"
#include "Urho3D/Scene/Node.h"
#include "ctrl/scene/SceneManipulationController.h"

using namespace Urho3D;
namespace Urho3DEditor
{
typedef void (*NodeContexMenuHandle)(Vector<Node*> selectNodes);
class NodeContexMenu : public Object {
	URHO3D_OBJECT(NodeContexMenu, Object);
public:
	NodeContexMenu(Context* context, const String& name);
	NodeContexMenu(Context* context, const String& name, NodeContexMenuHandle callback);
	void SetCallBack(NodeContexMenuHandle callback);
	void OnClicked(Vector<Node*> selectNodes);
	String& GetName();
protected:
	String _name;
	NodeContexMenuHandle _callBackFun;

};
class NodeContextMenus
{
public:
	static void Init(Context* context, SceneManipulationController* sceneManipCtrl);
	static void AddContexMenu(NodeContexMenu* menu);
	static bool DrawContextMenu(Vector<Node*> selectNodes);
	static void OnAddEmptyNode(Vector<Node*> selectNodes);
	static void OnDeleteNode(Vector<Node*> selectNodes);
protected:
	static bool _init;
	static Context* context_;
	static Vector<NodeContexMenu*> contexMenus;
	static SceneManipulationController* sceneManipController_;
};

//ECM is editor node contextMenu
URHO3D_EVENT(ENCM_CLICKED, Clicked) {
	URHO3D_PARAM(P_NodeContexMenu, NodeContexMenu);              // UIElement pointer
};

}