#pragma once


namespace urho3d
{
namespace editor
{

class asset_mgr
{
public:
	static asset_mgr* get_instance();
	~asset_mgr();
	void import_all_assets();
protected:

private:
	asset_mgr();
	static asset_mgr* _instance;
};

}
}
