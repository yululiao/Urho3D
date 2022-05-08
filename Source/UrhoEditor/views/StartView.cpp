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
		ImGui::PushStyleColor(ImGuiCol_Button,0x00ffffff);
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign,ImVec2(0,0));
		if(ImGui::Button(item.CString(),ImVec2(300,25)))
		{
			EditorApp::getInstance()->openWorkSpace(item);
			EditorApp::getInstance()->startGame();
			_isShow = false;
		}
		ImGui::PopStyleVar(1);
		ImGui::SameLine();
		if( ImGui::Button("x",ImVec2(25,25)))
		{
			_historyMgr->delete_history(item);
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
	if (ImGui::Button("Open...",ImVec2(80,25)))
	{
		String path = EditorApp::getInstance()->dialogSelectPath();
		if(path != "")
		{
			EditorApp::getInstance()->openWorkSpace(path);
			EditorApp::getInstance()->startGame();
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