#pragma once
#include <map>
#include "Urho3D/Core/Context.h"
#include "Urho3D/Core/Main.h"
#include "Urho3D/Engine/Engine.h"
#include "Urho3D/Container/Str.h"
#include "Urho3D/Resource/Image.h"
#include "Urho3D/Graphics/Texture2D.h"

using namespace Urho3D;

namespace Urho3D
{
class Node;
}

namespace Urho3DEditor {
struct NodeCahce {
    StringVector childDirs;
    StringVector childFiles;
    String name;
    String ext;
    bool isFile = false;
    bool fold = true;
};
class ImguiTexInfo:public Object
{
    URHO3D_OBJECT(ImguiTexInfo, Object);

public:
    ImguiTexInfo(Context* ctx): 
        Object(ctx)
    {
        
    }
    SharedPtr<Image> img;
    //SharedPtr<Texture2D> texture;
    int id;
};

class AssetMgr : public Object
{
    URHO3D_OBJECT(AssetMgr, Object);

public:
    static AssetMgr* getInstance();
    AssetMgr(Context* ctx);
    ~AssetMgr();
    int getImguiTex(const String& path);
    //获取文本文件中的内容
    String getTextFile(const String& path);
    String getBaseName(const String& path);
    String getFilePath(const String& path);
    String pathToRelative(const String& path);
    String pathToFull(const String& path);
    String GetExt(const String& path);
    void ImportFbx(const String& path);
    void ImportSingleFbx(const String& path);
    void SaveScene(const String& path);
    void SavePrefab(Node* node,const String& path);
    void OpenScene(const String& path);
    void OpenNewScene();
    void RefreshResCache(const String& path, bool recursive);
    String& GetWorkSpace();
    String& GetAssetRoot();
    String& GetRelativeAssetRoot();
    void SetWorkSpace(const String& path);
    bool IsModelFile(const String& r_path);
public:
    HashMap<String, NodeCahce> nodeCache;
    HashSet<String> selectedFolders;
    String lastSelectedFolder;
    HashSet<String> selectedFiles;
private:
    static AssetMgr* _instance;
    //String _history_file = "res/editor_historys.json";
    std::map<String, SharedPtr<ImguiTexInfo>> _texMap;
    String _work_space;
    String _asset_root;
    String _relative_asset_root;
public:
    static Urho3D::HashSet<String> SurportExtSet;
    static Urho3D::HashSet<String> ImgExtSet;
    static Urho3D::HashSet<String> CanDragExtSet;
};

}