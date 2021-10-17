#pragma once
#include <string>
#include <vector>

namespace urho3d
{
namespace editor
{

class edit_command
{
public:
	edit_command() {};
	virtual ~edit_command() {
		_default_cmd_name = "DefaultCommand";
	}
	virtual void todo() = 0;
	virtual void undo() = 0;
	virtual bool reversible() { return can_reverse; };
	std::string _cmd_id;
	std::string _cmd_name;
	std::string _default_cmd_name;
	bool can_reverse = true;
	virtual std::string get_name()
	{
		if (_cmd_name.empty())
			return _default_cmd_name;
		else
			return _cmd_name;
	}
};

}
}