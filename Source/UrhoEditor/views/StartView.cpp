#include "StartView.h"
#include "imgui.h"
#include "Urho3D/Core/Context.h"
#include "Urho3D/Core/Main.h"
#include "Urho3D/Engine/Engine.h"
#include "imgui_impl_glfw.h"
#include "GLFW/glfw3.h"
#include "imgui_impl_opengl3.h"
#include "EditorApp.h"

namespace Urho3DEditor 
{

StartView::StartView() 
{
	_historyMgr = new HistoryMgr();
}

StartView::~StartView() 
{
	delete _historyMgr;
	_historyMgr = nullptr;
}

void StartView::RenderHistoryList() 
{
	auto historys = _historyMgr->get_history_list();
	for(auto item: historys)
	{
		ImGui::Text("");
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button,0x00ffffff);
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign,ImVec2(0,0));
		if(ImGui::Button(item.CString()))
		{
			EditorApp::GetInstance()->OpenWorkSpace(item);
			EditorApp::GetInstance()->StartGame();
			_isShow = false;
		}
		ImGui::PopStyleVar(1);
		ImGui::SameLine();
		if( ImGui::Button("x"))
		{
			_historyMgr->delete_history(item);
			_historyMgr->save();
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
		String path = EditorApp::GetInstance()->DialogSelectPath();
		if(path != "")
		{
			_historyMgr->add_project(path);
			_historyMgr->save();
			EditorApp::GetInstance()->OpenWorkSpace(path);
			EditorApp::GetInstance()->StartGame();
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