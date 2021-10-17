#pragma once
#include "Urho3D/Scene/Serializable.h"
#include "Urho3D/Core/Variant.h"

using namespace Urho3D;

namespace urho3d
{
namespace editor
{

class proj_history : public Serializable
{
	URHO3D_OBJECT(proj_history, Serializable);
public:
	explicit proj_history(Context* context);
	virtual ~proj_history();
	static void RegisterObject(Context* context);
	StringVector _history;
protected:
	
private:
};

}
}