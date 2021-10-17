#pragma once
#include "Urho3D/Core/Variant.h"
#include "model/proj_history.h"

using namespace Urho3D;

namespace urho3d
{
namespace editor
{
class history_mgr
{
public:
    history_mgr();
    ~history_mgr();
    void add_project(const String& path);
    void delete_history(const String& path);
	void save();
	void open();
	StringVector get_history_list();

protected:
private:
    String _history_file = "res/editor_historys.json";
    proj_history* _history_data = nullptr;

};
}

}