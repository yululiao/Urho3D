#pragma once
#include <list>
#include <map>
#include "EventData.h"
#include "EventEmitter.h"

using namespace fastdelegate;

namespace Urho3DEditor 
{
struct eGlobalEventType
{
	enum
	{
		DataRefresh,
		AfterDo,
		AfterUndoRedo,
		AfterSave,
		UpdateInspector,
        SetCursor,
        CreateScene,
        AddNodeToScene,
        BeginInsertNode,
        EndInsertNode,
	};
};

class GEvent :public EventEmitter
{
public:
	~GEvent();
	static GEvent* Instance();
private:
	GEvent();
	static GEvent* _instance;


};
}


