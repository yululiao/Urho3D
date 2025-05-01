#include <iostream>
#include "GEvent.h"

namespace Urho3DEditor 
{
GEvent* GEvent::_instance = nullptr;

GEvent::~GEvent()
{
}

GEvent::GEvent()
{

}

GEvent* GEvent::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new GEvent();
	}

	return _instance;
}
}

