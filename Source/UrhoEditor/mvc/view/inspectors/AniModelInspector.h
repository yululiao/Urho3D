#pragma once
#include "Urho3D/Container/Str.h"
#include "imgui.h"
#include "MatInspector.h"
#include "model/SelectionModel.h"

namespace Urho3DEditor
{
class PropertyEditController;
class ProjectController;

class AniModelInspector
{
public:
	AniModelInspector(SelectionModel& selectionModel, PropertyEditController& propEditCtrl, ProjectController& projectCtrl);
	~AniModelInspector();
	void Update();
private:
	SelectionModel& selectionModel_;
	PropertyEditController& propertyEditController_;
	ProjectController& projectController_;
	Urho3D::String _modelPath;
    Urho3D::String _animationPath;
    Urho3D::String _aniName;
	MatInspector* _matInspector = nullptr;
};
}