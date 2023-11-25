#pragma once
#include <list>
#include <vector>
#include "CmdMgr.h"
#include "EditCmd.h"

using namespace std;

namespace Urho3DEditor 
{
class CmdMgr
{
public:
	CmdMgr();
	~CmdMgr();
	void ToDo(EditCmd* command);
	void UnDo();
	void ReDo();
	bool CanUnDo();
	bool CanReDo();

	void OnSave();
	bool HasNode();

protected:
	std::vector<EditCmd*> GetUnDoNodes(int index);
	std::vector<EditCmd*> GetRedoNodes(int index);
	void AddNode(EditCmd* node);
	void Clear();

private:
	list<EditCmd*> editNodeList;
	int currentNodeId = -1;
	static CmdMgr* _instance;
public:
	static CmdMgr* Instance();
};
}