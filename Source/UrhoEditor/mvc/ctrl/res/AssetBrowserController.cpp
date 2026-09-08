#include "AssetBrowserController.h"
#include "AssetMgr.h"
#include "ctrl/scene/SelectionController.h"

namespace Urho3DEditor {

using namespace Urho3D;

AssetBrowserController::AssetBrowserController(AssetMgr& assetMgr, SelectionController& selectionCtrl)
    : assetMgr_(assetMgr)
    , selectionCtrl_(selectionCtrl)
{
}

const String& AssetBrowserController::GetRelativeAssetRoot() const
{
    return assetMgr_.GetRelativeAssetRoot();
}

void AssetBrowserController::SetWorkSpace(const String& path)
{
    assetMgr_.SetWorkSpace(path);
}

bool AssetBrowserController::IsFolder(const String& path) const
{
    return assetMgr_.nodeCache.Contains(path) && !assetMgr_.nodeCache[path].isFile;
}

bool AssetBrowserController::NodeExists(const String& path) const
{
    return assetMgr_.nodeCache.Contains(path);
}

const StringVector& AssetBrowserController::GetChildDirs(const String& path) const
{
    static const StringVector empty;
    auto it = assetMgr_.nodeCache.Find(path);
    if (it != assetMgr_.nodeCache.End())
        return it->second_.childDirs;
    return empty;
}

String AssetBrowserController::GetNodeName(const String& path) const
{
    auto it = assetMgr_.nodeCache.Find(path);
    if (it != assetMgr_.nodeCache.End())
        return it->second_.name;
    return String();
}

bool AssetBrowserController::IsFolderSelected(const String& path) const
{
    return assetMgr_.selectedFolders.Contains(path);
}

void AssetBrowserController::SelectFolder(const String& path)
{
    assetMgr_.selectedFolders.Clear();
    assetMgr_.selectedFolders.Insert(path);
    assetMgr_.lastSelectedFolder = path;
}

void AssetBrowserController::ClearFolderSelection()
{
    assetMgr_.selectedFolders.Clear();
    assetMgr_.lastSelectedFolder.Clear();
}

const String& AssetBrowserController::GetLastSelectedFolder() const
{
    return assetMgr_.lastSelectedFolder;
}

bool AssetBrowserController::IsNodeFolded(const String& path) const
{
    auto it = assetMgr_.nodeCache.Find(path);
    if (it != assetMgr_.nodeCache.End())
        return !it->second_.fold;
    return false;
}

void AssetBrowserController::SetNodeFolded(const String& path, bool folded)
{
    auto it = assetMgr_.nodeCache.Find(path);
    if (it != assetMgr_.nodeCache.End())
        it->second_.fold = !folded;
}

const StringVector& AssetBrowserController::GetChildFiles(const String& folder) const
{
    static const StringVector empty;
    auto it = assetMgr_.nodeCache.Find(folder);
    if (it != assetMgr_.nodeCache.End())
        return it->second_.childFiles;
    return empty;
}

bool AssetBrowserController::IsFileSelected(const String& path) const
{
    return assetMgr_.selectedFiles.Contains(path);
}

void AssetBrowserController::SelectFile(const String& path)
{
    assetMgr_.selectedFiles.Clear();
    assetMgr_.selectedFiles.Insert(path);
    assetMgr_.lastSlectedFile = path;
    selectionCtrl_.OnFileSelected(path);
}

void AssetBrowserController::ClearFileSelection()
{
    assetMgr_.selectedFiles.Clear();
}

const String& AssetBrowserController::GetLastSelectedFile() const
{
    return assetMgr_.lastSlectedFile;
}

bool AssetBrowserController::IsSupportedExt(const String& ext)
{
    return AssetMgr::SurportExtSet.Contains(ext);
}

bool AssetBrowserController::IsImageExt(const String& ext)
{
    return AssetMgr::ImgExtSet.Contains(ext);
}

bool AssetBrowserController::IsInspectableExt(const String& ext)
{
    return AssetMgr::CanInspectExtSet.Contains(ext);
}

String AssetBrowserController::GetNodeExt(const String& path) const
{
    auto it = assetMgr_.nodeCache.Find(path);
    if (it != assetMgr_.nodeCache.End())
        return it->second_.ext;
    return String();
}

int AssetBrowserController::GetImguiTex(const String& path)
{
    return assetMgr_.getImguiTex(path);
}

String AssetBrowserController::PathToFull(const String& path) const
{
    return assetMgr_.pathToFull(path);
}

String AssetBrowserController::PathToRelative(const String& path) const
{
    return assetMgr_.pathToRelative(path);
}

} // namespace Urho3DEditor
