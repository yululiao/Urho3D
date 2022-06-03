#pragma once
#include <vector>
#include <string>
#include "Urho3D/Math/Vector2.h"
#include "Urho3D/Math/Vector3.h"
#include "Urho3D/Math/Vector4.h"
#include "Urho3D/Math/Quaternion.h"

using namespace Urho3D;

namespace Urho3DEditor
{

class EventData
{
public:
	EventData()
	{

	}
	virtual ~EventData() {};

	template <typename one_type, typename...vaclass>
	EventData(one_type type_val, vaclass... otherInput)
	{
		SetEventData(type_val, otherInput...);
	}

	template <typename one_type, typename...vaclass>
	void SetEventData(one_type type_val, vaclass... otherInput)
	{
		set_data(type_val);
		SetEventData(otherInput...);
	}

	// termination version
	/*void set_event_data()
	{

	}*/

	void PushNum(float num1)
	{
		numberList.push_back(num1);
	}

	void PushStr(std::string str1)
	{
		stringList.push_back(str1);
	}

	void PushInt(int int1)
	{
		intList.push_back(int1);
	}

	void PushBool(bool b)
	{
		boolList.push_back(b);
	}

	void PushCStr(const char* str)
	{
		stringList.push_back(str);
	}

	void PushObj(void* obj)
	{
		objList.push_back(obj);
	}

	void PushVec3(Vector3 v)
	{
		vec3List.push_back(v);
	}
	void PushVec2(Vector2 v) {
		vec2List.push_back(v);
	}
	void PushQuat(Quaternion v) {
		quatList.push_back(v);
	}

	EventData(EventData* src)
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
	std::vector<Vector3 > vec3List;
	std::vector<Vector2> vec2List;
	std::vector<Quaternion> quatList;
};
}
