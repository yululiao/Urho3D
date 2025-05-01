#include "ConsoleView.h"

namespace Urho3DEditor 
{
ConsoleView::ConsoleView() 
{
}
ConsoleView::~ConsoleView()
{
}
void ConsoleView::Update() 
{
	if (!showing)
		return;
	ImGui::Begin("Cosole", &showing);
	ImGui::End();

}
}