#pragma once
#include <map>
#include <vector>
#include <string>

#include <Urho3D/Scene/Scene.h>
#include "Urho3d/Graphics/StaticModel.h"
#include "Urho3d/Graphics/Material.h"
#include "Urho3D/Math/Color.h"
#include "Urho3D/Core/Variant.h"
#include "Urho3D/Math/Vector4.h"

using namespace Urho3D;

class GizmogeoInfo
{
public:
    GizmogeoInfo()
    {
        geo = NULL;
        isHilight = false;
    }
    SharedPtr<Node> geo;
    Vector3 pos;
    Vector3 rot;
    void hilight(bool hi)
    {
        if (hi)
        {
            if (isHilight)
                return;
            Material* mat = geo->GetComponent<StaticModel>()->GetMaterial();
            oriColor = mat->GetShaderParameter("MatDiffColor").GetVector4();
            // geo->set_color(0xffff00);
            mat->SetShaderParameter("MatDiffColor", Vector4(1.0, 1.0, 0.0, 1.0));
            isHilight = true;
        }
        else
        {
            if (isHilight)
            {
                isHilight = false;
                Material* mat = geo->GetComponent<StaticModel>()->GetMaterial();
                mat->SetShaderParameter("MatDiffColor", oriColor);
            }
        }
    }

protected:
    Vector4 oriColor;
    bool isHilight;
};
class TransformGizmo : public Node
{
    URHO3D_OBJECT(TransformGizmo, Node);

public:
    TransformGizmo(Context* ctx);
    ~TransformGizmo();
    void init();
    void setupGizmos(std::map<std::string, std::vector<GizmogeoInfo*>> gizmoMap, Node* parent);
    void highlight(std::string axis);
    void deHilightAll();
    virtual void setActivePlane(std::string axis, Vector3 eye) = 0;
    virtual void update(Matrix4 objMatrix, Vector3 eye);
    SharedPtr<Node> pickers;
    SharedPtr<Node> planes;
    SharedPtr<Node> activePlane;
    std::map<std::string, std::vector<GizmogeoInfo*>> handleGizmos;
    void getAllChildren(Node* node, std::vector<Node*>& list);

protected:
    bool showPickers{false};
    bool showActivePlane{false};
    Node* handles;
    std::map<std::string, SharedPtr<Node>> planesMap;
    std::map<std::string, std::vector<GizmogeoInfo*>> pickerGizmos;
    std::vector<Node*> mAllChild;

private:
};