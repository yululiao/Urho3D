#include "Toolbar.h"
#include "ctrls/AssetMgr.h"
#include "EditorApp.h"

namespace Urho3DEditor
{

Toolbar::Toolbar() 
{
	Init();
}
Toolbar::~Toolbar() 
{
}
void Toolbar::Update() 
{
	auto winSize = ImGui::GetWindowSize();
	ImGui::BeginChild("ToolBar",ImVec2(winSize.x,itemSize+6));
	this->DrawTrigerTool();
	this->DrawTransformTool();
	ImGui::EndChild();
}
void Toolbar::DrawTransformTool() 
{
	Vector<String> toolList = { "camera","move","rotate","scale" };
	Vector<int> imgList;
	imgList.Push(AssetMgr::getInstance()->getImguiTex("res/img/hand.png"));
	imgList.Push(AssetMgr::getInstance()->getImguiTex("res/img/move.png"));
	imgList.Push(AssetMgr::getInstance()->getImguiTex("res/img/rotate.png"));
	imgList.Push(AssetMgr::getInstance()->getImguiTex("res/img/scale.png"));
	
	unsigned curColor = 0x00ffffff;
	for (int i= 0;i< toolList.Size();++i)
	{
		auto item = toolList[i];
		if (item == curToolName) 
		{
			curColor = selectedColor;
		}
		else
		{
			curColor = 0x00ffffff;
		}
		ImGui::PushStyleColor(ImGuiCol_Button, curColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0x77999999);
		ImGui::SameLine();
		if(ImGui::ImageButton(imgList[i],ImVec2(itemSize,itemSize)))
		{
			this->OnTransformTool(item);
		}
		if(ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::SetTooltip(item.CString());
			ImGui::EndTooltip();
		}
		ImGui::PopStyleColor(2);
	}
	
}
void Toolbar::DrawTrigerTool() 
{
	Vector<ToolbarHandle> handleList ={&OnNewScene,&OnSave,&OnUndo,&OnRedo};
	Vector<String> toolList = { "newScene","save","undo","redo" };
	Vector<int> imgList;
	imgList.Push(AssetMgr::getInstance()->getImguiTex("res/img/grid.png"));
	imgList.Push(AssetMgr::getInstance()->getImguiTex("res/img/save.png"));
	imgList.Push(AssetMgr::getInstance()->getImguiTex("res/img/undo.png"));
	imgList.Push(AssetMgr::getInstance()->getImguiTex("res/img/redo.png"));
	ImGui::PushStyleColor(ImGuiCol_Button, 0x00ffffff);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0x77999999);
	for (int i = 0; i < toolList.Size(); ++i) 
	{
		auto item = toolList[i];
		ImGui::SameLine();
		if(ImGui::ImageButton(imgList[i], ImVec2(itemSize, itemSize)))
		{
			handleList[i]();
		}
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::SetTooltip(item.CString());
			ImGui::EndTooltip();
		}
	}
	ImGui::PopStyleColor(2);
}
void Toolbar::Init() 
{
	curToolName = "camera";

}
void Toolbar::OnSave() 
{
	String assetRoot = EditorApp::getInstance()->getAssetRoot();
	AssetMgr::getInstance()->SaveScene(assetRoot + "test.scene");
}
void Toolbar::OnRedo() 
{
}
void Toolbar::OnUndo() 
{
}
void Toolbar::OnNewScene() 
{
	AssetMgr::getInstance()->OpenNewScene();
}
void Toolbar::OnTransformTool(const String& name)
{
	curToolName = name;
	EditorApp::getInstance()->setCurTool(name);
}
}