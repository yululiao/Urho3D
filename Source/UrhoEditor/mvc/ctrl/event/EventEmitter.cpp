#include <iostream>
#include "EventEmitter.h"

namespace Urho3DEditor {

EventEmitter::~EventEmitter()
{
}

EventEmitter::EventEmitter()
{

}

void EventEmitter::RegEvent(int eventId, editor_delegate1 listener)
{
	if (HasEvent(eventId))
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

void EventEmitter::RemoveEvent(int eventId, editor_delegate1 listener)
{
	if (HasEvent(eventId))
	{
		EventHandlerList& list = m_handlers.at(eventId);
		list.remove(listener);
	}
}

void EventEmitter::EmitEvent(int eventId, EventData* data)
{
	if (HasEvent(eventId))
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

bool EventEmitter::HasEvent(int eventId)
{
	return m_handlers.count(eventId);//m_handlers.find(eventId) != m_handlers.end();
}
}

