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
    return String();
    std::ifstream ifile(path.CString());
    std::ostringstream buf;
    char ch;
    while (buf && ifile.get(ch))
        buf.put(ch);
    String str = buf.str().c_str();
    return str;
}

int AssetMgr::getImguiTex(const String& path)
{
    GLuint texID = 0;
    Image* img = nullptr;
    if (_texMap.find(path) == _texMap.end())
    {
        img = new Image(EditorApp::getInstance()->getContext());
        File texFile(EditorApp::getInstance()->getContext(), path);
        img->BeginLoad(texFile);
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->GetWidth(), img->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                     img->GetData());
        ImguiTexInfo* info = new ImguiTexInfo(this->GetContext());
        _texMap[path] = info;//»º´æÎÆÀí
    }
    else
    {
        texID = _texMap[path]->id;
    }
    
    return texID;
}
