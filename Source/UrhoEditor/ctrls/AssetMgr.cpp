#include"GLEW/glew.h"
#include <fstream>
#include <sstream>
#include "AssetMgr.h"
#include "Urho3D/IO/File.h"
#include "Urho3D/Resource/JSONFile.h"
#include "EditorApp.h"
#include "AssetImporter.h"
#include "Urho3D/IO/FileSystem.h"
#include "SceneCtrl.h"

namespace Urho3DEditor {
AssetMgr* AssetMgr::_instance = nullptr;

AssetMgr::AssetMgr(Context* ctx)
    :Object(ctx)
{

}

AssetMgr::~AssetMgr()
{

}

AssetMgr* AssetMgr::getInstance() 
{ 
	if (!_instance)
    {
        _instance = new AssetMgr(EditorApp::GetInstance()->GetContext());

    }
    return _instance;
}

String AssetMgr::getTextFile(const String& path)
{
    std::ifstream ifile(path.CString());
    std::ostringstream buf;
    char ch;
    while (buf && ifile.get(ch))
        buf.put(ch);
    String str = buf.str().c_str();
    return str;
}

String AssetMgr::getBaseName(const String& path) 
{ 
    String baseName = path.Substring(path.FindLast("/")+1);
    baseName = baseName.Substring(0,baseName.FindLast("."));
    return baseName;
}

String AssetMgr::getFilePath(const String& path) 
{ 
    return path.Substring(0,path.FindLast("/")); }

String AssetMgr::pathToRelative(const String& path) 
{ 
   String workSpace = EditorApp::GetInstance()->GetWorkSpace();
    if (path.Find(workSpace) != 0)
    {
        throw("Application::pathToRelative: fullPath is not a FullPath!");
        return "";
    }
    String ralativePath = path.Substring(workSpace.Length() + 1);
    return ralativePath;
}

String AssetMgr::pathToFull(const String& path)
{
    String workSpace = EditorApp::GetInstance()->GetWorkSpace();
    return workSpace + "/" + path;
}

String AssetMgr::GetExt(const String& path) 
{ 
    //int test;
    return path.Substring(path.FindLast("."));
}

void AssetMgr::ImportFbx(const String& path)
{
    auto fileSys = GetSubsystem<FileSystem>();
    if (fileSys->FileExists(path))
    {
        String ext = GetExt(path).ToLower();
        if (ext == ".fbx")
        {
            ImportSingleFbx(path);
        }
    }
    else if (fileSys->DirExists(path))
    {
        StringVector files;
        fileSys->ScanDir(files, path, ".*", SCAN_FILES, true);
        for (int i = 0; i < files.Size();i++)
        {
            if (GetExt(files[i]).ToLower() == ".fbx")
            {
                ImportSingleFbx(path + "/" +files[i]);
            }
        }
    }
    
}

void AssetMgr::ImportSingleFbx(const String& fbxPath)
{
    String base = getBaseName(fbxPath);
    String fpath = getFilePath(fbxPath);
    // std::string cmd = "tool/AssetImporter";
    String args;
    if (base.Contains("@")) //动画
    {
        String outpath = fpath + "/" + base + ".ani";
        args = "anim " + fbxPath + " " + outpath;
    }
    else
    {
        String outpath = fpath + "/" + base + ".mdl -nm -nt";
        args = "model " + fbxPath + " " + outpath;
    }
    // char msg[128] = { 0 };
    // Utils::_system(cmd.c_str(),msg,sizeof(msg));
    // QProcess process;
    // process.startDetached(cmd.c_str(),QString(args.c_str()).split(" "));
    assimp_import_fbx(args.CString());
}

int AssetMgr::getImguiTex(const String& path)
{
    GLuint texID = 0;
    
    if (_texMap.find(path) == _texMap.end())
    {
        Image* img = new Image(EditorApp::GetInstance()->GetContext());
        File file(EditorApp::GetInstance()->GetContext(), path);
        img->BeginLoad(file);
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        GLint format = GL_RGBA;
        if (img->GetComponents() < 4)
            format = GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, img->GetWidth(), img->GetHeight(), 0, format, GL_UNSIGNED_BYTE,
                     img->GetData());
        //glGenerateMipmap(GL_TEXTURE_2D);
        ImguiTexInfo* info = new ImguiTexInfo(this->GetContext());
        info->id = texID;
        info->img = img;
        _texMap[path] = info;//缓存纹理
    }
    else
    {
        texID = _texMap[path]->id;//直接从缓存中取出
    }
    
    return texID;
}

void AssetMgr::SaveScene(const String& path) 
{ 
    auto mainScene = SceneCtrl::getInstance()->GetScene();
    File file(context_,path,FileMode::FILE_WRITE);
    mainScene->SaveJSON(file);
}

void AssetMgr::SavePrefab(Node* node,const String& path) 
{

    File file(context_, path, FileMode::FILE_WRITE);
    node->SaveJSON(file);
}

void AssetMgr::OpenScene(const String& path)
{ 
    if (GetExt(path) != ".scene")
        return;
    SceneCtrl::getInstance()->OpenScene(path);
}

void AssetMgr::OpenNewScene() 
{ 
    SceneCtrl::getInstance()->OpenNewScene();
}
}

