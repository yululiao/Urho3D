#pragma once
#include "ctrl/file_system_watcher.h"

namespace urho3d
{
namespace editor
{

class asset_mgr
{
public:
	static asset_mgr* get_instance();
	~asset_mgr();
    void init();
	void import_all_assets();
    void import_assets(const std::string& path);
    void import_fbx(const std::string& fbxPath);
    std::string get_asset_path();

    std::string pathToRelative(const std::string& fulPath);
    std::string pathToFull(const std::string& rpath);
protected:

private:
	asset_mgr();
	static asset_mgr* _instance;
    //file_system_watcher* _watcher = nullptr;
};

}
}
