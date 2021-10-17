#pragma once
#include <list>
#include <map>
#include "FastDelegate.h"
#include "editor_delegate.h"

namespace urho3d
{
namespace editor
{
class event_emitter
{
private:
	
	typedef std::list<editor_delegate1> EventHandlerList;
	std::map<int, EventHandlerList> m_handlers;

public:
	event_emitter();
	~event_emitter();
	void register_event(int eventId, editor_delegate1 listener);
	void remove_event(int eventId, editor_delegate1 listener);
	void emit_event(int eventId, event_data* data = NULL);
protected:
	bool has_event(int eventId);
};
}
}


