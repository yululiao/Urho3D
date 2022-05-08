#pragma once

#include "EditorWidget.h"
#include "inspectors/TransformInspector.h"
#include "inspectors/AniModelInspector.h"

namespace Urho3DEditor 
{
class Inspector :public EditorWidget
{
public:
	Inspector();
	virtual ~Inspector();
	void Update() override;
private:
	TransformInspector* _transformIns = nullptr;
	AniModelInspector* _aniModelIns = nullptr;
	bool _nodeEnable = true;

};
}
