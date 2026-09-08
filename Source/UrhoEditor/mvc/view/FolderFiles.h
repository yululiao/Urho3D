#pragma once
#include "EditorWidget.h"
#include "Urho3D/Container/Str.h"
#include "Urho3D/Core/Context.h"
#include "Urho3D/Scene/Scene.h"
#include "ctrl/res/ProjectController.h"
#include "ctrl/scene/ToolController.h"

using namespace Urho3D;

namespace Urho3DEditor {
class AssetBrowserController;

class FolderFiles :public EditorWidget {
public:
	FolderFiles(ProjectController& projectCtrl, ToolController& toolCtrl, AssetBrowserController& assetBrowserCtrl, float fontSize);
	virtual ~FolderFiles();
	void Update() override;
private:
	void OnItemClicked(const String& path);
	void DrawNodeNoInWindows(int itemH);
	void DrawFiles();
	void OnDrag(const String& path);
	void OnItemDoubleClicked(const String& path);
	void DrawContextMenu(const String& path);

	ProjectController& projectController_;
	ToolController& toolController_;
	AssetBrowserController& assetBrowserCtrl_;
	float fontSize_;
};
}
