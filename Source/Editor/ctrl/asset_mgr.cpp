
#include "asset_mgr.h"
#include "work_space.h"

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

void asset_mgr::import_fbx(const std::string& fbxPath)
{

}


void asset_mgr::import_assets(const std::string &path)
{

}

void asset_mgr::import_all_assets()
{

}


}
}
