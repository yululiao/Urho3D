#pragma once

#include "EditorWidget.h"
#include "inspectors/TransformInspector.h"
#include "inspectors/AniModelInspector.h"
#include "inspectors/AssetInspector.h"
#include "model/SelectionModel.h"
#include "ctrl/scene/PropertyEditController.h"
#include "ctrl/res/ProjectController.h"

namespace Urho3DEditor 
{
class Inspector :public EditorWidget
{
public:
	Inspector(SelectionModel& selectionModel, PropertyEditController& propEditCtrl, ProjectController& projectCtrl);
	virtual ~Inspector();
	void Update() override;
private:
	SelectionModel& selectionModel_;
	PropertyEditController& propertyEditController_;
	ProjectController& projectController_;
	TransformInspector* _transformIns = nullptr;
	AniModelInspector* _aniModelIns = nullptr;
	AssetInspector* _assetIns = nullptr;
	bool _nodeEnable = true;

};
}
