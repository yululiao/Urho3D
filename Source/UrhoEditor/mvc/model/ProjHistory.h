#pragma once
#include "Urho3D/Scene/Serializable.h"
#include "Urho3D/Core/Variant.h"

using namespace Urho3D;

namespace Urho3DEditor {

class ProjHistory : public Serializable
{
	URHO3D_OBJECT(ProjHistory, Serializable);
public:
	explicit ProjHistory(Context* context);
	virtual ~ProjHistory();
	static void RegisterObject(Context* context);
	StringVector _history;
protected:
	
private:
};

}