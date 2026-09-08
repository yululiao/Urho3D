#pragma once

#include "Urho3D/Container/Vector.h"
#include "Urho3D/Scene/Node.h"
#include "ctrl/scene/SceneCtrl.h"

namespace Urho3DEditor {

using namespace Urho3D;

class CmdMgr;
class AssetMgr;

class SceneManipulationController
{
public:
    SceneManipulationController(SceneCtrl& sceneCtrl, CmdMgr& cmdMgr, AssetMgr& assetMgr);

    void AddModel(const String& path, Node* parent, int index);
    void AddEmptyNode(Node* parent);
    void DeleteNodes(Vector<Node*> nodes);
    void ImportModel(const String& path, Node* parent, int index);

private:
    SceneCtrl& sceneCtrl_;
    CmdMgr& cmdMgr_;
    AssetMgr& assetMgr_;
};

} // namespace Urho3DEditor
