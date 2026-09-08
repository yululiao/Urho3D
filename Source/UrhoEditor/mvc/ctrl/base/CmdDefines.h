#pragma once
#include "CmdMgr.h"
#include <Urho3D/Scene/Scene.h>
#include "Urho3D/Graphics/Material.h"

using namespace Urho3D;

namespace Urho3DEditor {

//���ݵı༭�������ģʽ�е�����ģʽ�����е������޸Ķ�ͨ��cmd��ִ�У�ʵ�� undo redo
//�������ݵ�������ݵı仯���Գ����һ�¼��֣�
//�����������ݷ�Ϊlist vector map�����Ӳ�������
//ɾ�����ݿ��Է�Ϊlist vector mapɾ������
//���ڴ���Ƴ����¼�������
//CmdModify CmdModifyList CmdModifyVector CmdModifyMap
//�̳�Serializable�Ķ���Serializable�����Է��������
//Resource���̳�Serializable��Ҫ���⴦��
//����ɾ���ڵ�ҲҪ���⴦��
class CmdModify : public CmdEdit
{
public:
	CmdModify(const String& id, Serializable* obj,const String& attrName, Variant value);
	~CmdModify();
	void ToDo() override;
	void UnDo() override;
protected:
	String _attrName;
	Variant _value;
	SharedPtr<Serializable> _obj;
};

class CmdAddNode:public CmdEdit
{
public:
	CmdAddNode(const String& id,Node* addNode,Node* parent,int idx,Node* oldParent,int oldIdx);
	~CmdAddNode();
	void ToDo() override;
	void UnDo() override;
protected:
	SharedPtr<Node> _node;
	SharedPtr<Node> _parent;
	SharedPtr<Node> _oldParent;
	int _idx;
	int _oldIdx;

};


template <typename T> class CmdModifyVector:public CmdEdit
{
public:
	CmdModifyVector(const String& id, T obj,Vector<T>* vector, int idx, Vector<T>* oldVector,int oldIdx,bool isAdd)
		:CmdEdit(id)
	{
		_oldVector = oldVector;
		_idx = idx;
		_oldIdx = oldIdx;
		_isAdd = isAdd;
		_obj = obj;
	}
	~CmdModifyVector()
	{
	}
	void ToDo() override
	{
		if (_isAdd) {
			_vector->Insert(_idx, _obj);
			if (_oldVector) {
				_oldVector->Remove(_obj);
			}
		}
		else {
			_vector->Remove(_obj);
			if (_oldVector) {
				_oldVector->Insert(_oldIdx, _obj);
			}
		}
		if (_oldVector) {
			Vector<T>* tmpVec = _oldVector;
			_oldVector = _vector;
			_vector = tmpVec;
			int tmpIdx = _oldIdx;
			_oldIdx = _idx;
			_idx = tmpIdx;

		}
		_isAdd = !_isAdd;
	}
	void UnDo() override
	{
		ToDo();
	}
protected:
	bool _isAdd;
	int _idx;
	int _oldIdx;
	Vector<T>* _vector;
	Vector<T>* _oldVector;
	T _obj;
};
//���ʲ��̳���Serializable,û��attributes_�б�,û��ͨ�õ�SetAttribute�ӿ�,��Ҫ���⴦��
class CmdModifyMat:public CmdEdit
{
public:
	CmdModifyMat(const String& id, Material* mat, const String& attrName, Variant value);
	CmdModifyMat(const String& id, Material* mat, uint16_t texUnit, Variant value);
	~CmdModifyMat();
	void ToDo() override;
	void UnDo() override;
protected:
	String _attrName;
	uint16_t _texUnit;
	Variant _value;
	SharedPtr<Material> _mat;
	uint8_t _type = 0;//0 attr ,1 tex
};

}

