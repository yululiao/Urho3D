#include "ResPreview.h"

namespace Urho3DEditor 
{
ResPreview::ResPreview()
{
}
ResPreview::~ResPreview()
{
}
void ResPreview::Update() 
{
	if (!showing)
		return;
	ImGui::Begin("ResPreview", &showing);
	ImGui::End();

}
}