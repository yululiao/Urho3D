#pragma once
#include "Urho3D/Core/Variant.h"
#include "model/ProjHistory.h"

using namespace Urho3D;
namespace Urho3DEditor
{
class HistoryMgr
{
public:
    HistoryMgr();
    ~HistoryMgr();
    void add_project(const String& path);
    void delete_history(const String& path);
	void save();
	void open();
	StringVector get_history_list();

protected:
private:
    String _history_file = "res/editor_historys.json";
    ProjHistory* _history_data = nullptr;

};
}