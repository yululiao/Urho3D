#pragma once
#include "Urho3D/Core/Context.h"
#include "Urho3D/Core/Main.h"
#include "Urho3D/Container/Str.h"

using namespace Urho3D;
namespace Urho3DEditor
{

class NdfMgr: public Object
{
	URHO3D_OBJECT(NdfMgr, Object);
public:
	static NdfMgr* GetInstance();
	NdfMgr(Context* context);
	~NdfMgr();
	void GoToPath(const String& path);
protected:
	static NdfMgr* _instance;
};
}