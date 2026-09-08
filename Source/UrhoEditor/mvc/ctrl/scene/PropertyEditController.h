#pragma once

#include "Urho3D/Scene/Node.h"
#include "Urho3D/Graphics/Material.h"

namespace Urho3DEditor {

using namespace Urho3D;

class CmdMgr;
class AssetMgr;

class PropertyEditController
{
public:
    PropertyEditController(CmdMgr& cmdMgr, AssetMgr& assetMgr);

    void SetNodeProperty(Node* node, const String& attrName, const Variant& value);
    void SetNodeProperty(Node* node, const String& attrName, const Variant& value, const String& cmdGuid);
    void SetNodeEnabled(Node* node, bool enabled);
    void SetNodeMaterial(Node* node, const String& matPath);
    void SetMaterialParam(Material* mat, const String& paramName, const Variant& value);
    void SetMaterialParam(Material* mat, const String& paramName, const Variant& value, const String& cmdGuid);
    void SetMaterialTexture(Material* mat, uint16_t texUnit, const String& path);
    void SaveMaterial(Material* mat, const String& path);

private:
    CmdMgr& cmdMgr_;
    AssetMgr& assetMgr_;
};

} // namespace Urho3DEditor
