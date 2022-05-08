#include "ProjHistory.h"

#include "Urho3D/Core/Context.h"

namespace Urho3DEditor
{
ProjHistory::ProjHistory(Context* context)
	:Serializable(context)
{
	RegisterObject(context);
}
ProjHistory::~ProjHistory()
{
}

void ProjHistory::RegisterObject(Context* context)
{
	context->RegisterFactory<ProjHistory>("editors");
	URHO3D_ATTRIBUTE("_history", StringVector, _history, StringVector(), AM_DEFAULT);
}

}