#include "work_space.h"

namespace urho3d
{
namespace editor
{

work_space* work_space::_instance = NULL;

work_space* work_space::get_instance()
{
	if (_instance == NULL)
	{
		_instance = new work_space();
	}
	return _instance;
}
work_space::work_space()
{
}
work_space::~work_space()
{
}

void work_space::set_workspace(const std::string& root)
{
	_work_space = root;
}

std::string work_space::get_workspace()
{
	return _work_space;
}

std::string work_space::path_to_full(const std::string& relativePath)
{
	return get_workspace() + "/" + relativePath;
}

bool work_space::check_in_work_space(const std::string& fullPath)
{
	if (fullPath.find(_work_space) != 0)
	{
		return false;
	}
	return true;
}

std::string work_space::path_to_relative(const std::string& fullPath)
{
	if (fullPath.find(_work_space) != 0)
	{

		throw("Application::pathToRelative: fullPath is not a FullPath!");
		return "";
	}
	std::string ralativePath = fullPath.substr(_work_space.length() + 1);
	return ralativePath;
}


}
}