#pragma once
#include "CmdMgr.h"
#include <Urho3D/Scene/Scene.h>
#include "Urho3D/Graphics/Material.h"

using namespace Urho3D;

namespace Urho3DEditor {

//�������ݵ�������ݵı仯���Գ����һ�¼��֣�
//1.�����޸� 2.�������� 3.ɾ������
//�����������ݷ�Ϊlist vector map�����Ӳ�������
//ɾ�����ݿ��Է�Ϊlist vector mapɾ������
//���ڴ���Ƴ����¼�������
//CmdModify CmdInsertList CmdInsertVector CmdInsertMap CmdDeleteList CmdDeleteVector CmdDeleteMap
//���ݵ�Ԫ�Ǽ̳�Serializable�Ķ���Serializable�����Է��������
//Resource���̳�Serializable��Ҫ���⴦��
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
//���ʲ��̳���Serializable,��Ҫ���⴦��
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

