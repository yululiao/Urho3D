#pragma once
#include "ctrl/res/HistoryMgr.h"

namespace Urho3DEditor
{

class StartView
{
public:
	StartView();
	~StartView();
	void RenderHistoryList();
	void Update();
private:
	void OpenProject();
	void OnOpen();
	bool _isShow = true;
	HistoryMgr* _historyMgr = nullptr;
};

}