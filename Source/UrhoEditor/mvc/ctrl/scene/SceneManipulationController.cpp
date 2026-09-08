#include "SceneManipulationController.h"
#include "ctrl/res/AssetMgr.h"
#include "ctrl/base/CmdDefines.h"
#include "ctrl/base/CmdMgr.h"
#include "Utils.h"
#include "Urho3D/Graphics/Graphics.h"
#include "Urho3D/Graphics/AnimatedModel.h"
#include "Urho3D/Graphics/Material.h"
#include "Urho3D/Graphics/Model.h"
#include "Urho3D/Resource/ResourceCache.h"

namespace Urho3DEditor {

SceneManipulationController::SceneManipulationController(SceneCtrl& sceneCtrl, CmdMgr& cmdMgr, AssetMgr& assetMgr)
    : sceneCtrl_(sceneCtrl)
    , cmdMgr_(cmdMgr)
    , assetMgr_(assetMgr)
{
}

void SceneManipulationController::AddModel(const String& path, Node* parent, int index)
{
    sceneCtrl_.GetSubsystem<Graphics>()->MakeCurrent();
    auto* cache = sceneCtrl_.GetSubsystem<ResourceCache>();
    String name = assetMgr_.getBaseName(path);
    String mdl_path = assetMgr_.getFilePath(path) + "/" + name + ".mdl";

    Node* modelNode = new Node(sceneCtrl_.GetContext());
    modelNode->SetName(name);
    modelNode->SetScale(Vector3(0.01f, 0.01f, 0.01f));
    auto* modelObject = modelNode->CreateComponent<AnimatedModel>();
    Model* model = cache->GetResource<Model>(mdl_path);
    modelObject->SetModel(model);
    auto matRes = cache->GetResource<Material>("Materials/Default.xml");
    SharedPtr<Material> defMat(matRes->Clone());
    modelObject->SetMaterial(defMat);

    Node* targetParent = parent ? parent : sceneCtrl_.GetRoot();
    int targetIndex = parent ? index : targetParent->GetNumChildren();
    cmdMgr_.ToDo(new CmdAddNode(Utils::GenGuid().c_str(), modelNode, targetParent, targetIndex, nullptr, 0));
}

void SceneManipulationController::AddEmptyNode(Node* parent)
{
    sceneCtrl_.GetSubsystem<Graphics>()->MakeCurrent();
    Node* emptyNode = new Node(sceneCtrl_.GetContext());
    emptyNode->SetName("EmptyNode");
    Node* targetParent = parent ? parent : sceneCtrl_.GetRoot();
    int targetIndex = parent ? parent->GetNumChildren() : targetParent->GetNumChildren();
    cmdMgr_.ToDo(new CmdAddNode(Utils::GenGuid().c_str(), emptyNode, targetParent, targetIndex, nullptr, 0));
}

void SceneManipulationController::DeleteNodes(Vector<Node*> nodes)
{
    String id(Utils::GenGuid().c_str());
    for (auto* nodeItem : nodes)
    {
        Node* parent = nodeItem->GetParent();
        cmdMgr_.ToDo(new CmdAddNode(id, nodeItem, nullptr, 0, parent, parent->GetNumChildren()));
    }
}

void SceneManipulationController::ImportModel(const String& path, Node* parent, int index)
{
    if (assetMgr_.IsModelFile(path))
        AddModel(path, parent, index);
}

} // namespace Urho3DEditor
