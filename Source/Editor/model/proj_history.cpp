#include "proj_history.h"

#include "Urho3D/Core/Context.h"

namespace urho3d
{
namespace editor
{
proj_history::proj_history(Context* context)
	:Serializable(context)
{
	RegisterObject(context);
}
proj_history::~proj_history()
{
}

void proj_history::RegisterObject(Context* context)
{
	context->RegisterFactory<proj_history>("editors");
	URHO3D_ATTRIBUTE("_history", StringVector, _history, StringVector(), AM_DEFAULT);
}

}
}