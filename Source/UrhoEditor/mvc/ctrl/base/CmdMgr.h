#pragma once
#include <list>
#include <vector>
#include "CmdMgr.h"
#include "CmdEdit.h"

using namespace std;

namespace Urho3DEditor 
{
class CmdMgr
{
public:
	CmdMgr();
	~CmdMgr();
	void ToDo(CmdEdit* command);
	void UnDo();
	void ReDo();
	bool CanUnDo();
	bool CanReDo();

	void OnSave();
	bool HasNode();

protected:
	std::vector<CmdEdit*> GetUnDoNodes(int index);
	std::vector<CmdEdit*> GetRedoNodes(int index);
	void AddNode(CmdEdit* node);
	void Clear();

private:
	list<CmdEdit*> editNodeList;
	int currentNodeId = -1;
	static CmdMgr* _instance;
public:
	static CmdMgr* Instance();
};
}