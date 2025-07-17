#pragma once
#include "Urho3D/Core/Context.h"
#include "Urho3D/Core/Main.h"
#include "Urho3D/Engine/Engine.h"
#include "Urho3D/Container/Str.h"
#include "Urho3D/Container/Vector.h"
#include "Urho3D/Scene/Node.h"

using namespace Urho3D;
namespace Urho3DEditor
{
typedef void (*NodeContexMenuHandle)(Node* node);
class NodeContexMenu : public Object {
	URHO3D_OBJECT(NodeContexMenu, Object);
public:
	NodeContexMenu(const String& name);
	NodeContexMenu(const String& name, NodeContexMenuHandle callback);
	void SetCallBack(NodeContexMenuHandle callback);
	void OnClicked(Node* node);
	String& GetName();
protected:
	String _name;
	NodeContexMenuHandle _callBackFun;

};
class NodeContextMenus
{
public:
	static void Init();
	static void AddContexMenu(NodeContexMenu* menu);
	static void DrawContextMenu(Node* node);
	static void OnAddEmptyNode(Node* node);
protected:
	static bool _init;
	static Vector<NodeContexMenu*> contexMenus;
};

//ECM is editor node contextMenu
URHO3D_EVENT(ENCM_CLICKED, Clicked) {
	URHO3D_PARAM(P_NodeContexMenu, NodeContexMenu);              // UIElement pointer
};

}