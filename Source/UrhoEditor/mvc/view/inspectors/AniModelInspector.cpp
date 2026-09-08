#include "AniModelInspector.h"
#include "ctrl/scene/PropertyEditController.h"
#include "ctrl/res/ProjectController.h"
#include <Urho3D/Graphics/AnimatedModel.h>
#include "VariantDrawer.h"
#include <Urho3D/Graphics/Animation.h>
#include <Urho3D/Graphics/AnimationState.h>
#include <Urho3D/Resource/ResourceCache.h>

class AniModelInspector 
{

};

Urho3DEditor::AniModelInspector::AniModelInspector(SelectionModel& selectionModel, PropertyEditController& propEditCtrl, ProjectController& projectCtrl)
	: selectionModel_(selectionModel)
	, propertyEditController_(propEditCtrl)
	, projectController_(projectCtrl)
{
	_matInspector = new MatInspector(selectionModel, propEditCtrl);
}

Urho3DEditor::AniModelInspector::~AniModelInspector() 
{
	if(_matInspector)
	{
		delete _matInspector;
		_matInspector = nullptr;
	}
}


void Urho3DEditor::AniModelInspector::Update() 
{
	ImVec2 winSize = ImGui::GetWindowSize();
	int flags = ImGuiTreeNodeFlags_DefaultOpen;
	Node* selectedNode = selectionModel_.GetSelectedNode();
	if (!selectedNode || !selectedNode->HasComponent<AnimatedModel>())
		return;
	AnimatedModel* aniModel = selectedNode->GetComponent<AnimatedModel>();
	//const Vector<AttributeInfo>* attributes = aniModel->GetAttributes();
	if(ImGui::TreeNodeEx("AnimatedModel",flags))
	{
		//filter: TEXT("Text Files(*.txt)\0*.txt\0")
		_modelPath = aniModel->GetModelAttr().name_;
        VariantDrawer::DrawPath("Model  ", _modelPath, {"Model Files", "mdl"}, true);
        String oldPath = _animationPath;
        VariantDrawer::DrawPath("Animation  ", _animationPath, {"Animation Files", "ani"}, false);
        if (oldPath.Empty() && !_animationPath.Empty())
        {
            auto* cache = projectController_.GetResourceCache();
            auto* animation = cache->GetResource<Animation>(_animationPath);
            _aniName = animation->GetName();
            AnimationState * state = aniModel->AddAnimationState(animation);
            state->SetWeight(1.0f);
            state->SetLooped(true);
            state->SetTime(Random(animation->GetLength()));
		}
        if (!_aniName.Empty())
        {
            AnimationState* state = aniModel->GetAnimationStates()[0];
            state->AddTime(0.03);
        }
		ImGui::TreePop();
	}
	_matInspector->Update();
}
