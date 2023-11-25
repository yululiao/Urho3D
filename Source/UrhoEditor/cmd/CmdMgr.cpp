#include "CmdMgr.h"
#include "event/GEvent.h"
#include "Urho3D/Container/Str.h"

using namespace Urho3D;
namespace Urho3DEditor 
{
CmdMgr* CmdMgr::_instance = nullptr;
CmdMgr::CmdMgr()
{
	
}

CmdMgr::~CmdMgr()
{
	Clear();
}

void CmdMgr::Clear()
{
	currentNodeId = -1;
	for (auto it = editNodeList.begin(); it != editNodeList.end(); it++)
	{
		delete (*it);
	}
	editNodeList.clear();
}

CmdMgr* CmdMgr::Instance()
{
	if(!_instance)
	{
		_instance = new CmdMgr();
	}
	return _instance;
}

void CmdMgr::OnSave()
{
	Clear();
}

bool CmdMgr::HasNode()
{
	if (editNodeList.size() > 0)
		return true;
	return false;
}

void CmdMgr::ToDo(EditCmd* node)
{
	if (node == NULL)
		return;
	if (!node->Reversible())//不可恢复的操作不入操作列表
	{
		node->ToDo();
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
	AddNode(node);
	node->ToDo();

	GEvent::Instance()->EmitEvent(eGlobalEventType::AfterDo);
}

void CmdMgr::UnDo()
{
	std::vector<EditCmd*> nodes = GetUnDoNodes(currentNodeId);
	for (auto node : nodes)
	{
		node->UnDo();
		currentNodeId--;
	}
	GEvent::Instance()->EmitEvent(eGlobalEventType::AfterUndoRedo);

}

void CmdMgr::ReDo()
{
	std::vector<EditCmd*> nodes = GetRedoNodes(currentNodeId + 1);
	for (auto node : nodes)
	{
		node->ToDo();
		++currentNodeId;
	}
	GEvent::Instance()->EmitEvent(eGlobalEventType::AfterUndoRedo);
}

bool CmdMgr::CanUnDo()
{
	bool result = false;
	if (editNodeList.size() > 0 && currentNodeId >= 0)
	{
		result = true;
	}

	return result;
}

bool CmdMgr::CanReDo()
{
	bool result = false;
	int lastIndex = (int)editNodeList.size() - 1;
	if (editNodeList.size() > 0 && currentNodeId < lastIndex)
	{
		result = true;
	}

	return result;
}

std::vector<EditCmd*> CmdMgr::GetUnDoNodes(int index)
{
	std::vector<EditCmd*> result;
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
	String cmdName = (*it)->_id;
	if (cmdName != "" && it != editNodeList.begin())
	{
		it--;
		while ((*it)->_id == cmdName)
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

std::vector<EditCmd*> CmdMgr::GetRedoNodes(int index)
{
	std::vector<EditCmd*> result;
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
	String cmdName = (*it)->_id;
	if (cmdName != "" && it != editNodeList.end())
	{
		it++;
		while (it != editNodeList.end() && (*it)->_id == cmdName)
		{
			result.push_back(*it);
			it++;
		}
	}
	return result;
}

void CmdMgr::AddNode(EditCmd* node)
{
	editNodeList.push_back(node);
	currentNodeId++;
}
}


