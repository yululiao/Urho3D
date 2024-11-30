#pragma once
#include "EditorWidget.h"
#include "Urho3D/Container/Str.h"
#include "Urho3D/Core/Context.h"
#include "Urho3D/Scene/Scene.h"

using namespace Urho3D;

namespace Urho3DEditor {
class FolderFiles :public EditorWidget {
public:
	FolderFiles();
	virtual ~FolderFiles();
	void Update() override;
private:
	void OnItemClicked(const String& path);
	void DrawNodeNoInWindows(int itemH);
	void DrawFiles();
	void OnDrag();
	void OnItemDoubleClicked(const String& path);
};
}