#pragma once
#include "CmdMgr.h"
#include <Urho3D/Scene/Scene.h>
#include "Urho3D/Graphics/Material.h"

using namespace Urho3D;

namespace Urho3DEditor {

//基于数据的命令，数据的变化可以抽象成一下几种：
//1.数据修改 2.增加数据 3.删除数据
//其中增加数据分为list vector map中增加插入数据
//删除数据可以分为list vector map删除数据
//基于此设计出以下几种命令
//CmdModify CmdInsertList CmdInsertVector CmdInsertMap CmdDeleteList CmdDeleteVector CmdDeleteMap
//数据单元是继承Serializable的对象，Serializable有属性反射的能力
//Resource不继承Serializable需要特殊处理
class CmdModify : public EditCmd
{
public:
	CmdModify(const String& id, Serializable* obj,const String& attrName, Variant value);
	~CmdModify();
	void ToDo() override;
	void UnDo() override;
private:
	String _attrName;
	Variant _value;
	SharedPtr<Serializable> _obj;
};
//材质不继承自Serializable,需要特殊处理
class CmdModifyMat:public EditCmd
{
public:
	CmdModifyMat(const String& id, Material* mat, const String& attrName, Variant value);
	CmdModifyMat(const String& id, Material* mat, uint16_t texUnit, Variant value);
	~CmdModifyMat();
	void ToDo() override;
	void UnDo() override;
private:
	String _attrName;
	uint16_t _texUnit;
	Variant _value;
	SharedPtr<Material> _mat;
	uint8_t _type = 0;//0 attr ,1 tex ,2 reset path
};


class CmdModifyResPath:public EditCmd
{
public:
	enum ResType
	{
		FBX,
		MAT
	};
	CmdModifyResPath(const String& id,Serializable* obj,const String& path,ResType type);
	~CmdModifyResPath();
	void ToDo() override;
	void UnDo() override;
private:
	SharedPtr<Serializable> _obj;
	String _path;
	ResType _resType;
};


void DoModify(const String& id, Serializable* obj, const String& attrName, Variant value);
void DoMatModify(const String& id, Material* mat, const String& attrName, Variant value);
void DoMatTexModify(const String& id, Material* mat, uint16_t texUnit, Variant value);
void DoResPathModify(const String& id, Serializable* obj, const String& path, CmdModifyResPath::ResType type);

}

