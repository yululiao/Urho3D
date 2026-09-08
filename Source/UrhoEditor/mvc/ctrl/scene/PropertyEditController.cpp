#include "PropertyEditController.h"
#include "ctrl/base/CmdDefines.h"
#include "ctrl/base/CmdMgr.h"
#include "ctrl/res/AssetMgr.h"
#include "Utils.h"
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Graphics.h>

namespace Urho3DEditor {

PropertyEditController::PropertyEditController(CmdMgr& cmdMgr, AssetMgr& assetMgr)
    : cmdMgr_(cmdMgr)
    , assetMgr_(assetMgr)
{
}

void PropertyEditController::SetNodeProperty(Node* node, const String& attrName, const Variant& value)
{
    if (!node)
        return;
    String cmdGuid = String(Utils::GenGuid().c_str());
    cmdMgr_.ToDo(new CmdModify(cmdGuid, node, attrName, value));
}

void PropertyEditController::SetNodeProperty(Node* node, const String& attrName, const Variant& value, const String& cmdGuid)
{
    if (!node)
        return;
    cmdMgr_.ToDo(new CmdModify(cmdGuid, node, attrName, value));
}

void PropertyEditController::SetNodeEnabled(Node* node, bool enabled)
{
    if (!node)
        return;
    String cmdGuid = String(Utils::GenGuid().c_str());
    cmdMgr_.ToDo(new CmdModify(cmdGuid, node, "Is Enabled", enabled));
}

void PropertyEditController::SetNodeMaterial(Node* node, const String& matPath)
{
    if (!node)
        return;
    Context* ctx = node->GetContext();
    auto* graphics = ctx->GetSubsystem<Graphics>();
    if (graphics)
        graphics->MakeCurrent();
    auto* cache = ctx->GetSubsystem<ResourceCache>();
    if (!cache)
        return;
    SharedPtr<Material> mat(cache->GetResource<Material>(matPath)->Clone());
    String cmdGuid = String(Utils::GenGuid().c_str());
    cmdMgr_.ToDo(new CmdModify(cmdGuid, node, "Material", ResourceRefList(Material::GetTypeStatic(), { matPath })));
}

void PropertyEditController::SetMaterialParam(Material* mat, const String& paramName, const Variant& value)
{
    if (!mat)
        return;
    String cmdGuid = String(Utils::GenGuid().c_str());
    cmdMgr_.ToDo(new CmdModifyMat(cmdGuid, mat, paramName, value));
}

void PropertyEditController::SetMaterialParam(Material* mat, const String& paramName, const Variant& value, const String& cmdGuid)
{
    if (!mat)
        return;
    cmdMgr_.ToDo(new CmdModifyMat(cmdGuid, mat, paramName, value));
}

void PropertyEditController::SetMaterialTexture(Material* mat, uint16_t texUnit, const String& path)
{
    if (!mat)
        return;
    String cmdGuid = String(Utils::GenGuid().c_str());
    cmdMgr_.ToDo(new CmdModifyMat(cmdGuid, mat, texUnit, path));
}

void PropertyEditController::SaveMaterial(Material* mat, const String& path)
{
    if (!mat)
        return;
    mat->SaveFile(assetMgr_.pathToFull(path));
}

} // namespace Urho3DEditor
