#ifndef Swing_StaticStackArray_H
#define Swing_StaticStackArray_H

#include "SESystem.h"
#include "SEStaticArray.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:��̬��ջ������
// ˵��:
//----------------------------------------------------------------------------
template<class Type>
class StaticStackArray : public StaticArray<Type> 
{
public:
	INDEX sa_UsedCount;         // ��ǰʹ������
	INDEX sa_ctAllocationStep;  // ��ջ���ʱ�·�����ڴ��С

public:
	StaticStackArray(void);
	virtual ~StaticStackArray(void);

public:

	inline void SetAllocationStep(INDEX ctStep);
	inline void Append(INDEX iCount);
	// �ͷ��ڴ�,�ָ���ʼ��״̬
	inline void Delete(void);
	// ����Delete�ͷ��ڴ�,�ָ���ʼ��״̬
	inline void Clear(void);

	// �ڶ�ջ����ѹ���¶���
	// Pushֻ���Ӷ�ջʹ�ü���,��û������ѹ���¶���,��Ҫ�����Ժ��ֶ�ѹ���¶���,
	// example:Push() = Type,û�в�������,ֱ�ӷ������鸳ֵ,Ч�ʽϸ�
	inline Type &Push(void);
	inline Type *Push(INDEX ct);
	// �Ӷ�ջ��������һ������,������ջ�ڴ�
	inline Type &Pop(void);
	// ������iNewTop����ֵ�ߵ����ж���,������ջ�ڴ�
	inline void PopUntil(INDEX iNewTop);
	// �������ж���,��������ջ�ڴ�
	inline void PopAll(void);

	inline Type &operator[](INDEX iObject);
	inline const Type &operator[](INDEX iObject) const;
	INDEX Count(void) const;
	INDEX Index(Type *ptObject);
	void MoveArray(StaticStackArray<Type> &arOther);

	StaticStackArray<Type> &operator=(const StaticStackArray<Type> &arOriginal);
};

}

#endif

