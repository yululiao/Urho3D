#pragma once
#include "Urho3D/Container/Str.h"
#include "Urho3D/Graphics/Material.h"
#include "model/SelectionModel.h"

namespace Urho3DEditor
{
class PropertyEditController;

class MatInspector
{
public:
	MatInspector(SelectionModel& selectionModel, PropertyEditController& propEditCtrl);
	~MatInspector();
	int CalLongestName(Urho3D::Material* mat);
	Urho3D::String  FillItemName(const Urho3D::String& oriName,int len);
	void Update();

private:
	SelectionModel& selectionModel_;
	PropertyEditController& propertyEditController_;
	String _lastCmdGuid;
};
}
