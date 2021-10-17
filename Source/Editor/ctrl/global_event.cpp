#include <iostream>
#include "global_event.h"

namespace urho3d
{
namespace editor
{
global_event* global_event::_instance = nullptr;

global_event::~global_event()
{
}

global_event::global_event()
{

}

global_event* global_event::get_instance()
{
	if (_instance == nullptr)
	{
		_instance = new global_event();
	}

	return _instance;
}
}
}

