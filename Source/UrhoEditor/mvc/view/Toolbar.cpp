#include "imgui.h"
#include "Toolbar.h"

namespace Urho3DEditor
{

Toolbar::Toolbar(ToolController& toolCtrl, ProjectController& projectCtrl, ImguiTexProvider texProvider)
	: toolController_(toolCtrl)
	, projectController_(projectCtrl)
	, texProvider_(texProvider)
{
	Init();
}
Toolbar::~Toolbar() 
{
}
void Toolbar::Update() 
{
	auto winSize = ImGui::GetWindowSize();
	ImGui::BeginChild("ToolBar",ImVec2(winSize.x,itemSize+ itemSize*0.15));
	this->DrawTrigerTool();
	this->DrawTransformTool();
	ImGui::EndChild();
}
void Toolbar::DrawTransformTool() 
{
	Vector<String> toolList = { "camera","move","rotate","scale" };
	Vector<int> imgList;
	imgList.Push(texProvider_("res/img/hand.png"));
	imgList.Push(texProvider_("res/img/move.png"));
	imgList.Push(texProvider_("res/img/rotate.png"));
	imgList.Push(texProvider_("res/img/scale.png"));
	
	unsigned curColor = 0x00ffffff;
	for (int i= 0;i< toolList.Size();++i)
	{
		auto item = toolList[i];
		if (item == toolController_.GetCurrentTool()) 
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
	Vector<ToolbarHandle> handleList ={
		[this]() { OnNewScene(); },
		[this]() { OnSave(); },
		[this]() { OnUndo(); },
		[this]() { OnRedo(); }
	};
	Vector<String> toolList = { "newScene","save","undo","redo" };
	Vector<int> imgList;
	imgList.Push(texProvider_("res/img/grid.png"));
	imgList.Push(texProvider_("res/img/save.png"));
	imgList.Push(texProvider_("res/img/undo.png"));
	imgList.Push(texProvider_("res/img/redo.png"));
	Vector<int> grayImgList;
	grayImgList.Push(0);
	grayImgList.Push(texProvider_("res/img/save_gray.png"));
	grayImgList.Push(texProvider_("res/img/undo_gray.png"));
	grayImgList.Push(texProvider_("res/img/redo_gray.png"));
	Vector<bool> activeList;
	activeList.Push(true);
	activeList.Push(projectController_.HasNode());
	activeList.Push(projectController_.CanUndo());
	activeList.Push(projectController_.CanRedo());

	ImGui::PushStyleColor(ImGuiCol_Button, 0x00ff0000);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0x77999999);
	for (int i = 0; i < toolList.Size(); ++i) 
	{
		auto item = toolList[i];
		ImGui::SameLine();
		int tmpImg = imgList[i];
		if(!activeList[i])
		{
			tmpImg = grayImgList[i];
		}
		if(ImGui::ImageButton(tmpImg, ImVec2(itemSize, itemSize)))
		{
			if(activeList[i])
			{
				handleList[i]();
			}
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
	//curToolName = "camera";
}
void Toolbar::IntItemSize(float dpiScale) 
{ 
	this->itemSize = (int) (this->itemSize * dpiScale);
}
void Toolbar::OnSave() 
{
	String assetRoot = projectController_.GetAssetRoot();
	projectController_.SaveScene(assetRoot + "/test.uscene");
	projectController_.OnSave();
}
void Toolbar::OnRedo() 
{
	projectController_.Redo();

}
void Toolbar::OnUndo() 
{
	projectController_.Undo();
}
void Toolbar::OnNewScene() 
{
	projectController_.OpenNewScene();
}
void Toolbar::OnTransformTool(const String& name)
{
	toolController_.SetTool(name);
}
}