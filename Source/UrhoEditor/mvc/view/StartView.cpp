#include "StartView.h"
#include "imgui.h"
#include "Urho3D/Core/Context.h"
#include "Urho3D/Core/Main.h"
#include "Urho3D/Engine/Engine.h"
#include "imgui_impl_glfw.h"
#include "GLFW/glfw3.h"
#include "imgui_impl_opengl3.h"

namespace Urho3DEditor 
{

StartView::StartView(StartGameHandler startGame, SelectPathHandler selectPath, SetWorkSpaceHandler setWorkSpace, HistoryMgr& historyMgr)
	: startGame_(startGame)
	, selectPath_(selectPath)
	, setWorkSpace_(setWorkSpace)
	, historyMgr_(historyMgr)
{
}

StartView::~StartView() 
{
}

void StartView::RenderHistoryList() 
{
	auto historys = historyMgr_.get_history_list();
	for(auto item: historys)
	{
		ImGui::Text("");
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button,0x00ffffff);
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign,ImVec2(0,0));
		if(ImGui::Button(item.CString()))
		{
			if (setWorkSpace_)
				setWorkSpace_(item);
			if (startGame_)
				startGame_();
			_isShow = false;
		}
		ImGui::PopStyleVar(1);
		ImGui::SameLine();
		if( ImGui::Button("x"))
		{
			historyMgr_.delete_history(item);
			historyMgr_.save();
		}
		ImGui::PopStyleColor(1);
	}
}

void StartView::Update() 
{
	if(!_isShow)
		return;
	ImGui::BeginChild("StartView");
	ImGui::Spacing();
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign,ImVec2(0,0));
	ImGui::Text("");
	ImGui::SameLine();
	if (ImGui::Button("Open..."))
	{
		String path = selectPath_ ? selectPath_() : String();
		if(path != "")
		{
			historyMgr_.add_project(path);
			historyMgr_.save();
			if (setWorkSpace_)
				setWorkSpace_(path);
			if (startGame_)
				startGame_();
			_isShow = false;
		}
		
	}
	ImGui::PopStyleVar(1);
	ImGui::Spacing(), ImGui::Spacing(), ImGui::Spacing();
	ImGui::Separator();
	this->RenderHistoryList();
	
	ImGui::EndChild();
}
void StartView::OpenProject() 
{
}
void StartView::OnOpen() 
{
}
}