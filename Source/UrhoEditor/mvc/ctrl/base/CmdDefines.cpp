#include "CmdDefines.h"
#include "Urho3D/Core/Context.h"
#include "Urho3D/Scene/Node.h"
#include "Urho3D/Graphics/Texture2D.h"
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Graphics/Graphics.h>

namespace Urho3DEditor {



CmdModify::CmdModify(const String& id, Serializable* obj,const String& attrName, Variant value)
	:CmdEdit(id){
	_obj = obj;
	_attrName = attrName;
	_value = value;
}

CmdModify::~CmdModify() {


}

void CmdModify::ToDo() {
	Variant oldValue = _obj->GetAttribute(_attrName);
	_obj->SetAttribute(_attrName,_value);
	_value = oldValue;
}

void CmdModify::UnDo() {
	ToDo();
}

CmdAddNode::CmdAddNode(const String& id, Node* addNode, Node* parent, int idx, Node* oldParent, int oldIdx)
	:CmdEdit(id)
{
	_node = addNode;
	_parent = parent;
	_idx = idx;
	_oldParent = oldParent;
	_oldIdx = oldIdx;
}
CmdAddNode::~CmdAddNode()
{
}

void CmdAddNode::ToDo()
{
	if(_oldParent)
	{
		_oldParent->RemoveChild(_node);
	}
	if(_parent)
	{
		_parent->AddChild(_node, _idx);
	}
	int tmpIdx = _idx;
	_idx = _oldIdx;
	_oldIdx = tmpIdx;
	SharedPtr<Node> tmpNode = _parent;
	_parent = _oldParent;
	_oldParent = tmpNode;
}

void CmdAddNode::UnDo()
{
	ToDo();
}

CmdModifyMat::CmdModifyMat(const String& id, Material* mat, const String& attrName, Variant value)
	:CmdEdit(id)
{
	_mat = mat;
	_attrName = attrName;
	_value = value;
}

CmdModifyMat::CmdModifyMat(const String& id, Material* mat, uint16_t texUnit, Variant value)
	:CmdEdit(id)
{
	_type = 1;
	_mat = mat;
	_texUnit = texUnit;
	_value = value;
}

CmdModifyMat::~CmdModifyMat() {

}

void CmdModifyMat::ToDo() {
	if(_type == 0)
	{
		Variant oldValue = _mat->GetShaderParameter(_attrName);
		_mat->SetShaderParameter(_attrName, _value);
		_value = oldValue;
	}
	else if(_type == 1)
	{
		Context* ctx = _mat->GetContext();
		auto* graphics = ctx->GetSubsystem<Graphics>();
		if (graphics)
			graphics->MakeCurrent();
		String oldPath = _mat->GetTexture((TextureUnit)_texUnit)->GetName();
		auto* cache = ctx->GetSubsystem<ResourceCache>();
		Urho3D::Texture2D* newTex = cache->GetResource<Urho3D::Texture2D>(_value.GetString());
		_mat->SetTexture((TextureUnit)_texUnit, newTex);
		_value = oldPath;
	}
}

void CmdModifyMat::UnDo() {
	ToDo();
}

}