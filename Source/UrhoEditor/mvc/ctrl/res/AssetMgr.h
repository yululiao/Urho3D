#pragma once
#include <map>
#include "Urho3D/Core/Context.h"
#include "Urho3D/Core/Main.h"
#include "Urho3D/Engine/Engine.h"
#include "Urho3D/Container/Str.h"
#include "Urho3D/Resource/Image.h"
#include "Urho3D/Graphics/Texture2D.h"
#include "Urho3D/Graphics/Viewport.h"
#include "Urho3D/Resource/ResourceCache.h"

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

struct ShortCutRtt
{
    SharedPtr<Scene> scene;
    SharedPtr<Node> cameraNode;
    SharedPtr<Texture2D> texture;
    Vector2 viewSize;
};

class AssetMgr : public Object
{
    URHO3D_OBJECT(AssetMgr, Object);

public:
    AssetMgr(Context* ctx);
    ~AssetMgr();
    int getImguiTex(const String& path);
    //��ȡ�ı��ļ��е�����
    String getTextFile(const String& path);
    String getBaseName(const String& path);
    String getFilePath(const String& path);
    String pathToRelative(const String& path);
    String pathToFull(const String& path);
    String GetExt(const String& path);
    void ImportFbx(const String& path);
    void ImportSingleFbx(const String& path);
    void RefreshResCache(const String& path, bool recursive);
    String& GetWorkSpace();
    String& GetAssetRoot();
    String& GetRelativeAssetRoot();
    void SetWorkSpace(const String& path);
    bool IsModelFile(const String& r_path);
    void UpdateFileShortCut(const String& path);
    void InitShortCutRtt();
    ResourceCache* GetChche();

public:
    HashMap<String, NodeCahce> nodeCache;
    HashSet<String> selectedFolders;
    String lastSelectedFolder;
    HashSet<String> selectedFiles;
    String lastSlectedFile;
private:
    //String _history_file = "res/editor_historys.json";
    std::map<String, SharedPtr<ImguiTexInfo>> _texMap;
    String _work_space;
    String _asset_root;
    String _relative_asset_root;
public:
    static Urho3D::HashSet<String> SurportExtSet;
    static Urho3D::HashSet<String> ImgExtSet;
    static Urho3D::HashSet<String> CanDragExtSet;
    static Urho3D::HashSet<String> CanInspectExtSet;
    ShortCutRtt shorCutRtt;
};

}