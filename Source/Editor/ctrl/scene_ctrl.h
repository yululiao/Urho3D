#pragma once

#include "Urho3D/Core/Context.h"


using namespace Urho3D;

namespace Urho3D
{

class Node;
class Scene;
class Sprite;

}
namespace urho3d
{
namespace editor
{
class scene_ctrl :public Object
{
	URHO3D_OBJECT(scene_ctrl, Object);
public:
	~scene_ctrl();
	static scene_ctrl* get_inatance();
	static Context* _ctx;
	void create_scene();
	void update();
	Node* select(float x, float y);
protected:
	
private:
	scene_ctrl(Context* ctx);
	static scene_ctrl* _instance;
	SharedPtr<Node> _modelNode = nullptr;

public:
	SharedPtr<Scene> _scene;
	SharedPtr<Node> _scene_root = nullptr;
	/// Camera scene node.
	SharedPtr<Node> _cameraNode;


};

}
}