#include "CmdDefines.h"
#include "Urho3D/Scene/Node.h"
#include "ctrls/SceneCtrl.h"
#include "Urho3D/Graphics/Texture2D.h"
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Graphics/Graphics.h>

namespace Urho3DEditor {



CmdModify::CmdModify(const String& id, Serializable* obj,const String& attrName, Variant value)
	:EditCmd(id){
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

CmdModifyMat::CmdModifyMat(const String& id, Material* mat, const String& attrName, Variant value)
	:EditCmd(id)
{
	_mat = mat;
	_attrName = attrName;
	_value = value;
}

CmdModifyMat::CmdModifyMat(const String& id, Material* mat, uint16_t texUnit, Variant value)
	:EditCmd(id)
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
		String oldPath = _mat->GetTexture((TextureUnit)_texUnit)->GetName();
		auto* cache = SceneCtrl::getInstance()->GetSubsystem<ResourceCache>();
		Urho3D::Texture2D* newTex = cache->GetResource<Urho3D::Texture2D>(_value.GetString());
		_mat->SetTexture((TextureUnit)_texUnit, newTex);
		_value = oldPath;
	}
}

void CmdModifyMat::UnDo() {
	ToDo();
}

CmdModifyResPath::CmdModifyResPath(const String& id, Serializable* obj, const String& path, ResType type)
	:EditCmd(id)
{
	_obj = obj;
	_path = path;
	_resType = type;
}

CmdModifyResPath::~CmdModifyResPath() {

}

void CmdModifyResPath::ToDo() {
	if(_resType == ResType::MAT)
	{
		auto cache = SceneCtrl::getInstance()->GetSubsystem<ResourceCache>();
		auto aniModel = dynamic_cast<StaticModel*>(_obj.Get());
		if(aniModel)
		{
			SharedPtr<Material> mat(aniModel->GetMaterial());
			String oldPath = mat->GetName();
			mat = cache->GetResource<Urho3D::Material>(_path);
			//SceneCtrl::getInstance()->GetSubsystem<Graphics>()->MakeCurrent();
			aniModel->SetMaterial(mat);
			_path = oldPath;
		}
	}
}

void CmdModifyResPath::UnDo() {
	ToDo();
}

void DoModify(const String& id, Serializable* obj, const String& attrName, Variant value) {
	CmdModify* cmd = new CmdModify(id, obj, attrName, value);
	CmdMgr::Instance()->ToDo(cmd);
}

void DoMatModify(const String& id, Material* mat, const String& attrName, Variant value) {
	CmdModifyMat* cmd = new CmdModifyMat(id,mat,attrName,value);
	CmdMgr::Instance()->ToDo(cmd);
}

void DoMatTexModify(const String& id, Material* mat, uint16_t texUnit, Variant value) {
	CmdModifyMat* cmd = new CmdModifyMat(id, mat, texUnit, value);
	CmdMgr::Instance()->ToDo(cmd);
}

void DoResPathModify(const String& id, Serializable* obj, const String& path, CmdModifyResPath::ResType type) {
	CmdModifyResPath* cmd = new CmdModifyResPath(id, obj, path,type);
	CmdMgr::Instance()->ToDo(cmd);
}


}