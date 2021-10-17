#pragma once
#include <vector>
#include <string>

namespace urho3d
{
namespace editor
{

class event_data
{
public:
	event_data()
	{

	}
	virtual ~event_data() {};

	template <typename one_type, typename...vaclass>
	event_data(one_type type_val, vaclass... otherInput)
	{
		set_event_data(type_val, otherInput...);
	}

	template <typename one_type, typename...vaclass>
	void set_event_data(one_type type_val, vaclass... otherInput)
	{
		set_data(type_val);
		set_event_data(otherInput...);
	}

	// termination version
	void set_event_data()
	{

	}

	void set_data(float num1)
	{
		numberList.push_back(num1);
	}

	void set_data(std::string str1)
	{
		stringList.push_back(str1);
	}

	void set_data(int int1)
	{
		intList.push_back(int1);
	}

	void set_data(bool b)
	{
		boolList.push_back(b);
	}

	void set_data(const char* str)
	{
		stringList.push_back(str);
	}

	void set_data(void* obj)
	{
		objList.push_back(obj);
	}

	event_data(event_data* src)
	{
		if (src == nullptr)
			return;
		stringList.assign(src->stringList.begin(), src->stringList.end());
		numberList.assign(src->numberList.begin(), src->numberList.end());
		boolList.assign(src->boolList.begin(), src->boolList.end());
		intList.assign(src->intList.begin(), src->intList.end());
		objList.assign(src->objList.begin(), src->objList.end());
	}
	std::vector<std::string>  stringList;
	std::vector<float> numberList;
	std::vector<bool> boolList;
	std::vector<int> intList;
	std::vector<void*> objList;
};
}
}
