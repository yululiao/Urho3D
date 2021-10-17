#pragma once
#include <list>
#include <vector>
#include "edit_command.h"

using namespace std;

namespace urho3d
{
namespace editor
{
class edit_mgr
{
public:
	edit_mgr();
	~edit_mgr();
	void todo(edit_command* command);
	void undo();
	void redo();
	bool can_undo();
	bool can_redo();

	void on_save();
	bool has_edit_node();

protected:
	std::vector<edit_command*> get_undo_nodes(int index);
	std::vector<edit_command*> get_redo_nodes(int index);
	void add_node(edit_command* node);
	void clear();

private:
	list<edit_command*> editNodeList;
	int currentNodeId;
	static edit_mgr* curent_mgr;
public:
	static edit_mgr* get_current_mgr() { return curent_mgr; }
	static void set_current_mgr(edit_mgr* mgr) { curent_mgr = mgr; }
};
}
}