#pragma once
#include "Urho3D/Container/Str.h"
#include "Urho3D/Graphics/Material.h"

namespace Urho3DEditor {
class ProjectController;

class AssetInspector {
public:
	AssetInspector(ProjectController& projectCtrl);
	~AssetInspector();
	void Update();

private:
	ProjectController& projectController_;
};
}
