#include <iostream>
#include "event_emitter.h"

namespace urho3d
{
namespace editor
{

event_emitter::~event_emitter()
{
}

event_emitter::event_emitter()
{

}

void event_emitter::register_event(int eventId, editor_delegate1 listener)
{
	if (has_event(eventId))
	{
		EventHandlerList& list = m_handlers.at(eventId);
		list.push_back(listener);
	}
	else
	{
		std::list<editor_delegate1> list;
		list.push_back(listener);
		m_handlers.insert(std::make_pair(eventId, list));
	}
}

void event_emitter::remove_event(int eventId, editor_delegate1 listener)
{
	if (has_event(eventId))
	{
		EventHandlerList& list = m_handlers.at(eventId);
		list.remove(listener);
	}
}

void event_emitter::emit_event(int eventId, event_data* data)
{
	if (has_event(eventId))
	{
		EventHandlerList& list = m_handlers.at(eventId);
		for (auto& func : list)
		{
			func(data);
		}
	}
	else
	{
		std::cout << "cannot find eventId" << eventId << std::endl;
	}

	delete data;
}

bool event_emitter::has_event(int eventId)
{
	return m_handlers.find(eventId) != m_handlers.end();
}
}
}

