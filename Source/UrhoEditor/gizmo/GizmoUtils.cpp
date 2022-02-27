#include "GizmoUtils.h"
#include <Urho3D/Scene/Scene.h>
#include "Urho3D/Math/Vector2.h"
#include "Urho3D/Math/Vector3.h"
#include "Urho3D/Math/Vector4.h"
#include "Urho3D/Math/Color.h"
#include "Urho3D/Graphics/StaticModel.h"

#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Material.h>
#include<Urho3D/Graphics/Technique.h>

using namespace Urho3D;


void GizmoUtils::set_default_mat(Urho3D::Node* geoNode)
{
	auto* cache = geoNode->GetSubsystem<ResourceCache>();
	SharedPtr<Material> t_mat = cache->GetResource<Material>("Materials/DefaultGreyAlpha.xml")->Clone();
	t_mat->SetCullMode(CullMode::CULL_NONE);
	t_mat->GetPass(0, "alpha")->SetDepthTestMode(CompareMode::CMP_ALWAYS);
	geoNode->GetComponent<StaticModel>()->SetMaterial(t_mat);
}

void GizmoUtils::set_geo_color(Urho3D::Node* geoNode, unsigned value)
{
	Color t_color(value, Color::ARGB);
	geoNode->GetComponent<StaticModel>()->GetMaterial()->SetShaderParameter("MatDiffColor", t_color.ToVector4());

}
