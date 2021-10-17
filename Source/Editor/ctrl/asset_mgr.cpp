
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

asset_mgr::asset_mgr()
{

}

asset_mgr::~asset_mgr()
{

}

void asset_mgr::import_all_assets()
{

}



}
}