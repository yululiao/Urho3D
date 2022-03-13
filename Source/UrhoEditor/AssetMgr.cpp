#include <fstream>
#include <sstream>
#include "AssetMgr.h"
#include "Urho3D/IO/File.h"
#include "Urho3D/Resource/JSONFile.h"
#include "EditorApp.h"


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
        _instance = new AssetMgr(EditorApp::getInstance()->GetContext());

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
   String workSpace = EditorApp::getInstance()->getWorkSpace();
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
    String workSpace = EditorApp::getInstance()->getWorkSpace();
    return workSpace + "/" + path;
}

int AssetMgr::getImguiTex(const String& path)
{
    GLuint texID = 0;
    
    if (_texMap.find(path) == _texMap.end())
    {
        Image* img = new Image(EditorApp::getInstance()->getContext());
        File file(EditorApp::getInstance()->getContext(), path);
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
