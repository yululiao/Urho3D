#pragma once
#include "Urho3D/Container/Str.h"

namespace Urho3DEditor 
{

class CmdEdit
{
public:
	CmdEdit(const Urho3D::String& id)
	{
		_id = id;
	};
	virtual ~CmdEdit()
	{
	
	}
	virtual void ToDo() = 0;
	virtual void UnDo() = 0;
	virtual bool Reversible() { return can_reverse; };
	bool can_reverse = true;
public:
	Urho3D::String _id;
};



}