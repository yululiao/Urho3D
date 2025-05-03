#pragma once
#include "DelegateFast.h"
#include "ctrl/base/EventData.h"

namespace Urho3DEditor
{
typedef fastdelegate::FastDelegate0<> editor_delegate0;
typedef fastdelegate::FastDelegate1<EventData*> editor_delegate1;
}


