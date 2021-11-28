#pragma once
#include <list>
#include <map>
#include "event_data.h"
#include "event_emitter.h"

using namespace fastdelegate;

namespace urho3d
{
namespace editor
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
        EndInsertNode

	};
};

class global_event :public event_emitter
{
public:
	~global_event();
	static global_event* get_instance();
private:
	global_event();
	static global_event* _instance;


};
}
}


