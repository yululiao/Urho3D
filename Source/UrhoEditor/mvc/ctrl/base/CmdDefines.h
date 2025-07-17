#pragma once
#include "CmdMgr.h"
#include <Urho3D/Scene/Scene.h>
#include "Urho3D/Graphics/Material.h"

using namespace Urho3D;

namespace Urho3DEditor {

//数据的编辑采用设计模式中的命令模式，所有的属性修改都通过cmd来执行，实现 undo redo
//基于数据的命令，数据的变化可以抽象成一下几种：
//其中增加数据分为list vector map中增加插入数据
//删除数据可以分为list vector map删除数据
//基于此设计出以下几种命令
//CmdModify CmdModifyList CmdModifyVector CmdModifyMap
//继承Serializable的对象，Serializable有属性反射的能力
//Resource不继承Serializable需要特殊处理
//添加删除节点也要特殊处理
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
//材质不继承自Serializable,没有attributes_列表,没有通用的SetAttribute接口,需要特殊处理
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
void DoModify(const String& id, Serializable* obj, const String& attrName, Variant value);
void DoAddNode(const String& id, Node* addNode, Node* parent, int idx, Node* oldParent, int oldIdx);
void DoMatModify(const String& id, Material* mat, const String& attrName, Variant value);
void DoMatTexModify(const String& id, Material* mat, uint16_t texUnit, Variant value);

template <typename T>
void DoModifyVector(const String& id, T obj, Vector<T>* vector, int idx, Vector<T>* oldVector, int oldIdx, bool isAdd)
{
	CmdModifyVector<T>* cmd = new CmdModifyVector(id, obj, vector, idx, oldVector, oldIdx, isAdd);
	CmdMgr::Instance()->ToDo(cmd);
}

}

