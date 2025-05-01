#pragma once
#include "Urho3D/Container/Str.h"


//监听文件的改变
namespace Urho3DEditor {

class EditorFileWatch {
public:
	static void WatchAssetPath(const Urho3D::String& assetRoot);
	static void OnAddFile(const Urho3D::String& path);
	static void OnRemoveFile(const Urho3D::String& path);
	static void OnModifiedFile(const Urho3D::String& path);
	static void OnRenameFileOld(const Urho3D::String& path);
	static void OnRenameFileNew(const Urho3D::String& path);
	static void StopWatch();
};
}