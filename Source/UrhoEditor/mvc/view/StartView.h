#pragma once
#include <functional>
#include "ctrl/res/HistoryMgr.h"
#include "Urho3D/Container/Str.h"

namespace Urho3DEditor
{

class StartView
{
public:
	using StartGameHandler = std::function<void()>;
	using SelectPathHandler = std::function<Urho3D::String()>;
	using SetWorkSpaceHandler = std::function<void(const Urho3D::String&)>;

	StartView(StartGameHandler startGame, SelectPathHandler selectPath, SetWorkSpaceHandler setWorkSpace, HistoryMgr& historyMgr);
	~StartView();
	void RenderHistoryList();
	void Update();
private:
	void OpenProject();
	void OnOpen();
	bool _isShow = true;
	HistoryMgr& historyMgr_;
	StartGameHandler startGame_;
	SelectPathHandler selectPath_;
	SetWorkSpaceHandler setWorkSpace_;
};

}