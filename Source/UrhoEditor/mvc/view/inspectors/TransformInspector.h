#pragma once

#include "Urho3D/Scene/Scene.h"
#include "model/SelectionModel.h"
#include "ctrl/scene/PropertyEditController.h"
using namespace Urho3D;

namespace Urho3DEditor 
{
class TransformInspector
{
public:
	TransformInspector(SelectionModel& selectionModel, PropertyEditController& propEditCtrl);
	~TransformInspector();
	void Update();
private:
	SelectionModel& selectionModel_;
	PropertyEditController& propertyEditController_;
	Vector3 _pos;
	Vector3 _rot;
	Vector3 _scale;
	String _lastCmdGuid;
};
}
