#include "ProjectController.h"
#include "Urho3D/IO/File.h"
#include "Urho3D/Graphics/Graphics.h"
#include "ctrl/base/CmdMgr.h"
#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#endif

namespace Urho3DEditor {

ProjectController::ProjectController(SceneCtrl& sceneCtrl, AssetMgr& assetMgr, CmdMgr& cmdMgr,
                                     SceneManipulationController& sceneManipCtrl,
                                     SelectionController& selectionCtrl, ToolController& toolCtrl)
    : sceneCtrl_(sceneCtrl)
    , assetMgr_(assetMgr)
    , cmdMgr_(cmdMgr)
    , sceneManipCtrl_(sceneManipCtrl)
    , selectionCtrl_(selectionCtrl)
    , toolCtrl_(toolCtrl)
{
}

void ProjectController::OpenScene(const String& path)
{
    if (assetMgr_.GetExt(path) != ".uscene")
        return;
    sceneCtrl_.OpenScene(path);
    selectionCtrl_.ClearSelection();
    toolCtrl_.SetTool("move");
}

void ProjectController::SaveScene(const String& path)
{
    sceneCtrl_.GetSubsystem<Graphics>()->MakeCurrent();
    Scene* mainScene = sceneCtrl_.GetScene();
    File file(sceneCtrl_.GetContext(), path, FileMode::FILE_WRITE);
    mainScene->SaveJSON(file);
}

void ProjectController::SavePrefab(Node* node, const String& path)
{
    if (!node)
        return;
    File file(sceneCtrl_.GetContext(), path, FileMode::FILE_WRITE);
    node->SaveJSON(file);
}

void ProjectController::OpenNewScene()
{
    sceneCtrl_.OpenNewScene();
    selectionCtrl_.ClearSelection();
    toolCtrl_.SetTool("move");
}

void ProjectController::ImportFbx(const String& path)
{
    assetMgr_.ImportFbx(path);
}

void ProjectController::ExplorePath(const String& path) const
{
    String fullPath = assetMgr_.pathToFull(path);
#ifdef _WIN32
    fullPath.Replace("/", "\\", false);
    String pathCmd = "/select, \"" + fullPath + "\"";
    ShellExecute(NULL, "open", "explorer.exe", pathCmd.CString(), NULL, SW_SHOWNORMAL);
#endif
}

void ProjectController::SetWorkSpace(const String& path)
{
    assetMgr_.SetWorkSpace(path);
}

String ProjectController::GetLastSelectedFile() const
{
    return assetMgr_.lastSlectedFile;
}

String ProjectController::GetLastSelectedFileExt() const
{
    return assetMgr_.nodeCache[assetMgr_.lastSlectedFile].ext;
}

String ProjectController::GetAssetRoot() const
{
    return assetMgr_.GetAssetRoot();
}

void ProjectController::Undo()
{
    cmdMgr_.UnDo();
}

void ProjectController::Redo()
{
    cmdMgr_.ReDo();
}

void ProjectController::OnSave()
{
    cmdMgr_.OnSave();
}

bool ProjectController::HasNode() const
{
    return cmdMgr_.HasNode();
}

bool ProjectController::CanUndo() const
{
    return cmdMgr_.CanUnDo();
}

bool ProjectController::CanRedo() const
{
    return cmdMgr_.CanReDo();
}

bool ProjectController::HasSelectedFiles() const
{
    return assetMgr_.selectedFiles.Size() > 0;
}

ResourceCache* ProjectController::GetResourceCache() const
{
    return assetMgr_.GetChche();
}

String ProjectController::PathToRelative(const String& path) const
{
    return assetMgr_.pathToRelative(path);
}

} // namespace Urho3DEditor
