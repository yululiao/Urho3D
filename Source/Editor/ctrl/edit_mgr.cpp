#include "edit_mgr.h"
#include "global_event.h"

namespace urho3d
{
namespace editor
{
edit_mgr* edit_mgr::curent_mgr = nullptr;
edit_mgr::edit_mgr()
{
	currentNodeId = -1;

}

edit_mgr::~edit_mgr()
{
	clear();
}

void edit_mgr::clear()
{
	currentNodeId = -1;
	for (auto it = editNodeList.begin(); it != editNodeList.end(); it++)
	{
		delete (*it);
	}
	editNodeList.clear();
}

void edit_mgr::on_save()
{
	clear();
}

bool edit_mgr::has_edit_node()
{
	if (editNodeList.size() > 0)
		return true;
	return false;
}

void edit_mgr::todo(edit_command* node)
{
	if (node == NULL)
		return;
	if (!node->reversible())//不可恢复的操作不入操作列表
	{
		node->todo();
		return;
	}
	//每次do操作后，需要将当前游标后的节点全部删除
	size_t deleteCount = editNodeList.size() - 1 - currentNodeId;
	for (size_t i = 1; i <= deleteCount; i++)
	{
		auto node = editNodeList.back();
		delete node;
		editNodeList.pop_back();
	}
	add_node(node);
	node->todo();

	global_event::get_instance()->emit_event(eGlobalEventType::AfterDo);
}

void edit_mgr::undo()
{
	std::vector<edit_command*> nodes = get_undo_nodes(currentNodeId);
	for (auto node : nodes)
	{
		node->undo();
		currentNodeId--;
	}
	global_event::get_instance()->emit_event(eGlobalEventType::AfterUndoRedo);

}

void edit_mgr::redo()
{
	std::vector<edit_command*> nodes = get_redo_nodes(currentNodeId + 1);
	for (auto node : nodes)
	{
		node->todo();
		++currentNodeId;
	}
	global_event::get_instance()->emit_event(eGlobalEventType::AfterUndoRedo);
}

bool edit_mgr::can_undo()
{
	bool result = false;
	if (editNodeList.size() > 0 && currentNodeId >= 0)
	{
		result = true;
	}

	return result;
}

bool edit_mgr::can_redo()
{
	bool result = false;
	if (editNodeList.size() > 0 && currentNodeId < editNodeList.size() - 1)
	{
		result = true;
	}

	return result;
}

std::vector<edit_command*> edit_mgr::get_undo_nodes(int index)
{
	std::vector<edit_command*> result;
	if (index <0 || index > editNodeList.size())
		return result;
	auto it = editNodeList.begin();
	for (int i = 0; i < editNodeList.size(); i++)
	{
		if (index == i)
		{
			result.push_back(*it);
			break;
		}
		it++;
	}
	std::string cmdName = (*it)->_cmd_id;
	if (cmdName != "" && it != editNodeList.begin())
	{
		it--;
		while ((*it)->_cmd_id == cmdName)
		{
			result.push_back(*it);
			if (it == editNodeList.begin())
			{
				break;
			}
			it--;
		}
	}
	return result;
}

std::vector<edit_command*> edit_mgr::get_redo_nodes(int index)
{
	std::vector<edit_command*> result;
	if (index <0 || index > editNodeList.size())
		return result;
	auto it = editNodeList.begin();
	for (int i = 0; i < editNodeList.size(); i++)
	{
		if (index == i)
		{
			result.push_back(*it);
			break;
		}
		it++;
	}
	std::string cmdName = (*it)->_cmd_id;
	if (cmdName != "" && it != editNodeList.end())
	{
		it++;
		while (it != editNodeList.end() && (*it)->_cmd_id == cmdName)
		{
			result.push_back(*it);
			it++;
		}
	}
	return result;
}

void edit_mgr::add_node(edit_command* node)
{
	editNodeList.push_back(node);
	currentNodeId++;
}
}

}

