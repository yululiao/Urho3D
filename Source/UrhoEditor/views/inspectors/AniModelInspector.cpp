#include "AniModelInspector.h"

class AniModelInspector 
{

};

Urho3DEditor::AniModelInspector::AniModelInspector()
{
}

Urho3DEditor::AniModelInspector::~AniModelInspector() 
{
}

void Urho3DEditor::AniModelInspector::Update() 
{
	ImVec2 winSize = ImGui::GetWindowSize();
	int flags = ImGuiTreeNodeFlags_DefaultOpen;
	if(ImGui::TreeNodeEx("AnimatedModel",flags))
	{
		ImGui::TreePop();
	}
}
