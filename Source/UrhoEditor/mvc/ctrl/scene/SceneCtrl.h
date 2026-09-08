#pragma once
#include <vector>
#include <map>
#include "Urho3D/Container/Str.h"
#include "Urho3D/Core/Context.h"
#include "Urho3D/Graphics/Texture2D.h"
#include "Urho3D/Graphics/Viewport.h"


using namespace Urho3D;
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
}

namespace Urho3DEditor
{
class SceneCtrl :public Object
{
	URHO3D_OBJECT(SceneCtrl, Object);
public:
	SceneCtrl(Context* ctx);
	~SceneCtrl();
	//void create_scene();
    void CreateScene();
    void UpdateGrids();
	void Update();
	Node* Select(float x, float y);
    Node* GetRoot() { return rttSceneRoot_; }
    Node* GetEditorRoot() { return editorRoot_; }
    void GenRttTex();
    char* GetRttData() { return rttData; }
    void OnResizeView(int w, int h);
    Node* IntersectObj(Ray& ray, Node* pnode, float& dis);
    void IntersectObj(Ray& ray, Node* pnode, std::map<float, Node*>& out);
    Scene* GetScene() { return rttScene_; }
    const String& GetNotShowTag() const { return notShowTag_; }
    void Clear();
    void InitScene(bool hasRoot);
    void OpenScene(const String& path);
    void OpenNewScene();
protected:
    char* rttData = nullptr;

private:
    //SharedPtr<Node> _modelNode = nullptr;
    //SharedPtr<Scene> scene_;
    //SharedPtr<Node> _scene_root = nullptr;
    /// Camera scene node.
    //SharedPtr<Node> cameraNode_;
    SharedPtr<Viewport> rttViewport_ = nullptr;

public:

    SharedPtr<Node> _grid_root = nullptr;
    SharedPtr<Texture2D> renderTexture = nullptr;
    std::vector<SharedPtr<Node>> _grid_lines;

    SharedPtr<Scene> rttScene_;
    /// Camera scene node in the render-to-texture scene.
    SharedPtr<Node> rttCameraNode_;
    //SharedPtr<Node> lightNode;
    SharedPtr<Node> rttSceneRoot_ = nullptr;
    Vector2 viewSize_;
    SharedPtr<Material> renderMaterial_ = nullptr;
    Camera* rttCam_ = nullptr;
    Vector2 constRttSize;
    SharedPtr<Node> editorRoot_ = nullptr;
    String notShowTag_ = "NotShowInNodeTree";
};

}
