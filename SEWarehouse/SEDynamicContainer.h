#ifndef Swing_DynamicContainer_H
#define Swing_DynamicContainer_H

#include "SESystem.h"
#include "SEStaticStackArray.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:��̬������
// ˵��:��������ౣ��������������ָ��,ע��������StaticStackArray<Type *>
//----------------------------------------------------------------------------
template<class Type>
class DynamicContainer : public StaticStackArray<Type *> 
{
public:
#if CHECKARRAYLOCKING
	INDEX m_iLockCt;          // lock counter for getting indices
#endif

	/* Get index of an object from it's pointer without locking. */
	INDEX GetIndex(Type *ptMember);

public:
	DynamicContainer(void);
	DynamicContainer(DynamicContainer<Type> &coOriginal);
	virtual ~DynamicContainer(void);

public:
	// ���һ����������
	void Add(Type *ptNewObject);
	// ����������һ������
	void Remove(Type *ptOldObject);
	// �������
	void Clear(void);
	// ָ�������Ƿ���������
	BOOL IsMember(Type *ptOldObject);

	// �����������ض���ָ��
	Type *Pointer(INDEX iObject);
	const Type *Pointer(INDEX iObject) const;
	inline Type &operator[](INDEX iObject) { return *Pointer(iObject); };
	inline const Type &operator[](INDEX iObject) const { return *Pointer(iObject); };
	DynamicContainer<Type> &operator=(DynamicContainer<Type> &coOriginal);
	// ����һ�������а����ж����ƶ�����
	void MoveContainer(DynamicContainer<Type> &coOther);

	/* Lock for getting indices. */
	void Lock(void);
	/* Unlock after getting indices. */
	void Unlock(void);
	// ���ݶ���ָ�뷵��������
	INDEX Index(Type *ptObject);
	// ��ȡ�����е�һ������(����ʱ��������������һ������)
	Type &GetFirst(void);
};

//--------------------------------------------------------------------------------------
// ����:��̬������������
// ˵��:
//--------------------------------------------------------------------------------------
template<class Type>
class DynamicContainerIterator 
{
private:
	INDEX dci_Index;               // ��ǰ��������
	DynamicContainer<Type> &dci_Array;   // �������Ķ�̬����������

public:
	inline DynamicContainerIterator(DynamicContainer<Type> &da);
	inline ~DynamicContainerIterator(void);

public:
	// ֻ��������
	inline void MoveToNext(void);
	inline BOOL IsPastEnd(void);

	// ��ȡ��ǰ����
	Type &Current(void) { return *dci_Array.Pointer(dci_Index); }
	Type &operator*(void) { return *dci_Array.Pointer(dci_Index); }
	operator Type *(void) { return dci_Array.Pointer(dci_Index); }
	Type *operator->(void) { return dci_Array.Pointer(dci_Index); }
};

}

#endif

