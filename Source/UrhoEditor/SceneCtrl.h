#pragma once
#include <vector>
#include <map>
#include "Urho3D/Container/Str.h"
#include "Urho3D/Core/Context.h"

namespace Urho3D
{
class Node;
class Scene;
class Sprite;
class Texture2D;
class Viewport;
class Material;
class Camera;
class Ray;

class SceneCtrl :public Object
{
	URHO3D_OBJECT(SceneCtrl, Object);
public:
	~SceneCtrl();
	static SceneCtrl* getInstance();
	static Context* _ctx;
	void create_scene();
    void create_rttScene();
    void create_models();
    void deleteNode(Urho3D::Node* node);
    void update_grids();
	void update();
	Node* select(float x, float y);
    void addModel(const String& path);
    Node* getRoot(){return _scene_root;}
    void genRttTex();
    char* getRttData() { return rttData; }
    void OnResizeView(int w, int h);
    Node* intersectObj(Ray& ray, Node* pnode, float& dis);
    void intersectObj(Ray& ray, Node* pnode, std::map<float, Node*>& out);

protected:
    char* rttData = nullptr;

private:
	SceneCtrl(Context* ctx);
	static SceneCtrl* _instance;
    SharedPtr<Node> _modelNode = nullptr;
    SharedPtr<Scene> scene_;
    SharedPtr<Node> _scene_root = nullptr;
    /// Camera scene node.
    SharedPtr<Node> cameraNode_;
    SharedPtr<Viewport> rttViewport_ = nullptr;

public:
	
    SharedPtr<Node> _grid_root = nullptr;
    SharedPtr<Texture2D> renderTexture = nullptr;
    std::vector<SharedPtr<Node>> _grid_lines;

     SharedPtr<Scene> rttScene_;
    /// Camera scene node in the render-to-texture scene.
     SharedPtr<Node> rttCameraNode_;
    Node* rttSceneRoot_ = nullptr;
    Vector2 viewSize_;
    SharedPtr<Material> renderMaterial_ = nullptr;
    Camera* rttCam_ = nullptr;
    Vector2 constRttSize;
};

}
