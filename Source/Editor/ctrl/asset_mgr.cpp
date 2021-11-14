
#include "asset_mgr.h"
#include "work_space.h"
#include "utils.h"
#include <QProcess>
#include "ctrl/AssetImporter.h"

namespace urho3d
{
namespace editor
{

asset_mgr* asset_mgr::_instance = nullptr;

asset_mgr* asset_mgr::get_instance()
{
	if (_instance == nullptr)
	{
		_instance = new asset_mgr();
	}
	return _instance;
}

void asset_mgr::init()
{
    //_watcher->add_watch_path(work_space::get_instance()->get_workspace().c_str());
    //todo
    //init workspace
}

asset_mgr::asset_mgr()
{
    //_watcher = new file_system_watcher();
}

asset_mgr::~asset_mgr()
{

}

std::string asset_mgr::get_asset_path()
{
    return work_space::get_instance()->get_workspace() + "/assets";
}

std::string asset_mgr::pathToRelative(const std::string& fulPath)
{
    std::string workSpace = work_space::get_instance()->get_workspace();
    if (fulPath.find(workSpace) != 0)
    {

        throw("Application::pathToRelative: fullPath is not a FullPath!");
        return "";
    }
    std::string ralativePath = fulPath.substr(workSpace.length() + 1);
    return ralativePath;
}
std::string asset_mgr::pathToFull(const std::string& rpath)
{
     std::string workSpace = work_space::get_instance()->get_workspace();
    return workSpace + "/" + rpath;
}

void asset_mgr::import_fbx(const std::string& fbxPath)
{
    std::string base = Utils::get_base_name(fbxPath);
    std::string fpath = Utils::get_file_path(fbxPath);
    //std::string cmd = "tool/AssetImporter";
    std::string args;
    if(Utils::str_contains(base,"@"))//动画
    {
        std::string outpath = fpath + "/" + base + ".ani";
        args = "anim " + fbxPath + " " + outpath;
    }
    else
    {
        std::string outpath = fpath + "/" + base + ".mdl -nm -nt";
        args = "model " + fbxPath + " " + outpath;
    }
    //char msg[128] = { 0 };
    //Utils::_system(cmd.c_str(),msg,sizeof(msg));
    //QProcess process;
   //process.startDetached(cmd.c_str(),QString(args.c_str()).split(" "));
    assimp_import_fbx(args.c_str());
}


void asset_mgr::import_assets(const std::string &path)
{
    std::vector<std::string> files;
    Utils::get_files_in_dir(path,files);
    for(auto item:files)
    {
        std::string ext = Utils::get_file_ext(item);
        if(ext == ".fbx")
        {
            import_fbx(item);
        }
    }
}

void asset_mgr::import_all_assets()
{

}


}
}
