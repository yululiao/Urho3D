#pragma once

#include "Urho3D/Container/Str.h"
#include "Urho3D/Container/Vector.h"
#include "Urho3D/Container/HashSet.h"
#include "Urho3D/Container/HashMap.h"
#include "Urho3D/Core/Variant.h"

namespace Urho3DEditor {

class AssetMgr;
class SelectionController;
struct NodeCahce;

class AssetBrowserController
{
public:
    AssetBrowserController(AssetMgr& assetMgr, SelectionController& selectionCtrl);

    // Project/workspace
    const Urho3D::String& GetRelativeAssetRoot() const;
    void SetWorkSpace(const Urho3D::String& path);

    // Folder tree state
    bool IsFolder(const Urho3D::String& path) const;
    bool NodeExists(const Urho3D::String& path) const;
    const Urho3D::StringVector& GetChildDirs(const Urho3D::String& path) const;
    Urho3D::String GetNodeName(const Urho3D::String& path) const;
    bool IsFolderSelected(const Urho3D::String& path) const;
    void SelectFolder(const Urho3D::String& path);
    void ClearFolderSelection();
    const Urho3D::String& GetLastSelectedFolder() const;
    bool IsNodeFolded(const Urho3D::String& path) const;
    void SetNodeFolded(const Urho3D::String& path, bool folded);

    // File list state
    const Urho3D::StringVector& GetChildFiles(const Urho3D::String& folder) const;
    bool IsFileSelected(const Urho3D::String& path) const;
    void SelectFile(const Urho3D::String& path);
    void ClearFileSelection();
    const Urho3D::String& GetLastSelectedFile() const;

    // File type checks
    static bool IsSupportedExt(const Urho3D::String& ext);
    static bool IsImageExt(const Urho3D::String& ext);
    static bool IsInspectableExt(const Urho3D::String& ext);

    // Utility
    Urho3D::String GetNodeExt(const Urho3D::String& path) const;
    int GetImguiTex(const Urho3D::String& path);
    Urho3D::String PathToFull(const Urho3D::String& path) const;
    Urho3D::String PathToRelative(const Urho3D::String& path) const;

private:
    AssetMgr& assetMgr_;
    SelectionController& selectionCtrl_;
};

} // namespace Urho3DEditor
