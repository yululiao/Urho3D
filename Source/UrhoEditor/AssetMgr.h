#pragma once
#include <map>
#include "Urho3D/Core/Context.h"
#include "Urho3D/Core/Main.h"
#include "Urho3D/Engine/Engine.h"
#include "Urho3D/Container/Str.h"
#include "Urho3D/Resource/Image.h"

using namespace Urho3D;

namespace Urho3D
{
class Node;
}

class ImguiTexInfo:public Object
{
    URHO3D_OBJECT(ImguiTexInfo, Object);

public:
    ImguiTexInfo(Context* ctx): 
        Object(ctx)
    {
        
    }
    SharedPtr<Image> img;
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
private:
    static AssetMgr* _instance;
    //String _history_file = "res/editor_historys.json";
    std::map<String, SharedPtr<ImguiTexInfo>> _texMap;

};