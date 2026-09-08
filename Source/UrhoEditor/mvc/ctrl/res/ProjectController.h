#pragma once

#include "Urho3D/Container/Str.h"
#include "Urho3D/Scene/Node.h"
#include "ctrl/scene/SceneCtrl.h"
#include "ctrl/scene/SceneManipulationController.h"
#include "ctrl/scene/SelectionController.h"
#include "ctrl/scene/ToolController.h"
#include "AssetMgr.h"

namespace Urho3D
{
class ResourceCache;
}

namespace Urho3DEditor {

using namespace Urho3D;

class CmdMgr;

class ProjectController
{
public:
    ProjectController(SceneCtrl& sceneCtrl, AssetMgr& assetMgr, CmdMgr& cmdMgr,
                      SceneManipulationController& sceneManipCtrl,
                      SelectionController& selectionCtrl, ToolController& toolCtrl);

    void OpenScene(const String& path);
    void SaveScene(const String& path);
    void SavePrefab(Node* node, const String& path);
    void OpenNewScene();
    void ImportFbx(const String& path);
    void ExplorePath(const String& path) const;
    void SetWorkSpace(const String& path);
    String GetLastSelectedFile() const;
    String GetLastSelectedFileExt() const;
    String GetAssetRoot() const;
    void Undo();
    void Redo();
    void OnSave();
    bool HasNode() const;
    bool CanUndo() const;
    bool CanRedo() const;
    bool HasSelectedFiles() const;
    Urho3D::ResourceCache* GetResourceCache() const;
    Urho3D::String PathToRelative(const Urho3D::String& path) const;

private:
    SceneCtrl& sceneCtrl_;
    AssetMgr& assetMgr_;
    CmdMgr& cmdMgr_;
    SceneManipulationController& sceneManipCtrl_;
    SelectionController& selectionCtrl_;
    ToolController& toolCtrl_;
};

} // namespace Urho3DEditor
