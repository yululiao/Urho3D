#pragma once
#include <string>

namespace urho3d
{
namespace editor
{
class work_space
{
public:
	static work_space* get_instance();
	work_space();
	~work_space();
	void set_workspace(const std::string& root);
	std::string get_workspace();
	bool check_in_work_space(const std::string& fullPath);
	std::string path_to_full(const std::string& relativePath);
	std::string path_to_relative(const std::string& fullPath);
protected:
private:
	std::string _work_space;
	static work_space* _instance;
};
}
}