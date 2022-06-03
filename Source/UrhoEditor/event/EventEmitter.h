#pragma once
#include <list>
#include <map>
#include "delegate/FastDelegate.h"
#include "delegate/Delegate.h"

namespace Urho3DEditor
{

class EventEmitter
{
private:
	
	typedef std::list<editor_delegate1> EventHandlerList;
	std::map<int, EventHandlerList> m_handlers;

public:
	EventEmitter();
	~EventEmitter();
	void RegEvent(int eventId, editor_delegate1 listener);
	void RemoveEvent(int eventId, editor_delegate1 listener);
	void EmitEvent(int eventId, EventData* data = NULL);
protected:
	bool HasEvent(int eventId);
};
}


