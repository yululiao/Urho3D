#include "EditorFileWatch.h"
#include "FileWatchUtil.hpp"
#include "Urho3D/Core/ProcessUtils.h"

using namespace Urho3D;

namespace Urho3DEditor {
filewatch::FileWatch<std::string>* _watch = nullptr;
void EditorFileWatch::WatchAssetPath(const Urho3D::String& assetRoot) {

	_watch = new filewatch::FileWatch<std::string>(
		assetRoot.CString(),
		[](const std::string& path, const filewatch::Event change_type){
			switch (change_type) {
			case filewatch::Event::added:
				OnAddFile(String(path.c_str()));
				break;
			case filewatch::Event::removed:
				OnRemoveFile(String(path.c_str()));
				break;
			case filewatch::Event::modified:
				OnModifiedFile(String(path.c_str()));
				break;
			case filewatch::Event::renamed_old:
				OnRenameFileOld(String(path.c_str()));
				break;
			case filewatch::Event::renamed_new:
				OnRenameFileNew(String(path.c_str()));
				break;
			};
		}
	);
}
void EditorFileWatch::OnAddFile(const Urho3D::String& path) {
	PrintLine("AddFile:"+path);
}
void EditorFileWatch::OnRemoveFile(const Urho3D::String& path) {
	PrintLine("RemoveFile:" + path);

}
void EditorFileWatch::OnModifiedFile(const Urho3D::String& path) {
	PrintLine("ModifyFile:" + path);
}
void EditorFileWatch::OnRenameFileOld(const Urho3D::String& path) {
	PrintLine("RenameFile old name:" + path);
}
void EditorFileWatch::OnRenameFileNew(const Urho3D::String& path) {
	PrintLine("RenameFile new name:" + path);

}

void EditorFileWatch::StopWatch() {
	delete _watch;
	_watch = nullptr;
}

}