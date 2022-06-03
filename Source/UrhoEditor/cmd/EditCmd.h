#pragma once
#include <string>
#include <vector>

namespace Urho3DEditor 
{

class EditCmd
{
public:
	EditCmd(const std::string& id) 
	{
		_id = id;
	};
	virtual ~EditCmd() 
	{
	
	}
	virtual void ToDo() = 0;
	virtual void UnDo() = 0;
	virtual bool Reversible() { return can_reverse; };
	bool can_reverse = true;
public:
	std::string _id;
};



}