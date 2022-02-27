#pragma once
#include "vector"
#include <string>
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
class SceneCtrl :public Object
{
	URHO3D_OBJECT(SceneCtrl, Object);
public:
	~SceneCtrl();
	static SceneCtrl* get_inatance();
	static Context* _ctx;
	void create_scene();
    void create_models();
    void deleteNode(Urho3D::Node* node);
    void update_grids();
	void update();
	Node* select(float x, float y);
    void addModel(const std::string& path);
    Node* getRoot(){return _scene_root;}
protected:
	
private:
	SceneCtrl(Context* ctx);
	static SceneCtrl* _instance;
    SharedPtr<Node> _modelNode = nullptr;

public:
	SharedPtr<Scene> _scene;
	SharedPtr<Node> _scene_root = nullptr;
	/// Camera scene node.
    SharedPtr<Node> _cameraNode = nullptr;
    SharedPtr<Node> _grid_root = nullptr;
    std::vector<SharedPtr<Node>> _grid_lines;


};

}
}
