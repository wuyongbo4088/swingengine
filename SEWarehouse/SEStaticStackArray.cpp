#ifndef Swing_StaticStackArray_CPP
#define Swing_StaticStackArray_CPP

#include "SEStaticStackArray.h"
#include "SEStaticArray.cpp"

namespace Swing
{

template<class Type>
StaticStackArray<Type>::StaticStackArray(void) : StaticArray<Type>() 
{
	sa_UsedCount=0;
	sa_ctAllocationStep = 16;
}

template<class Type>
StaticStackArray<Type>::~StaticStackArray(void) 
{
}

template<class Type>
inline void StaticStackArray<Type>::Clear(void) 
{
	if (StaticArray<Type>::Count()!=0) 
		Delete(); 
}

template<class Type>
inline void StaticStackArray<Type>::SetAllocationStep(INDEX ctStep) 
{
	ASSERT(ctStep>0);

	sa_ctAllocationStep = ctStep;
}

template<class Type>
inline void StaticStackArray<Type>::Append(INDEX iCount) 
{
	StaticArray<Type>::Append(iCount);
	sa_UsedCount = 0;
}

template<class Type>
inline void StaticStackArray<Type>::Delete(void) 
{
	StaticArray<Type>::Delete();
	sa_UsedCount = 0;
}

template<class Type>
inline Type &StaticStackArray<Type>::Push(void) 
{
	// ���ȶ�ջʹ�ü���+1
	sa_UsedCount++;
	// �����ջ�ռ�Ҫ�����,�������ڴ�
	if (sa_UsedCount>StaticArray<Type>::Count()) 
	{
		Expand(StaticArray<Type>::Count()+sa_ctAllocationStep);
	}

	// ȷ����ջ�ռ��Ѿ�������
	ASSERT(sa_UsedCount <= StaticArray<Type>::Count());
	// �����¶�ջ�����ռ������
	return StaticArray<Type>::operator[](sa_UsedCount-1);
}

template<class Type>
inline Type *StaticStackArray<Type>::Push(INDEX ct) 
{
	sa_UsedCount+=ct;
	if(sa_UsedCount>StaticArray<Type>::Count()) 
	{
		ASSERT(ct>0);

		const INDEX ctAllocSteps = (ct-1)/sa_ctAllocationStep +1;
		Expand( StaticArray<Type>::Count() + sa_ctAllocationStep*ctAllocSteps);
	}

	ASSERT(sa_UsedCount <= StaticArray<Type>::Count());
	return &StaticArray<Type>::operator[](sa_UsedCount-ct);
}

template<class Type>
inline Type &StaticStackArray<Type>::Pop(void)
{
	ASSERT(sa_UsedCount>0);
	sa_UsedCount--;
	return StaticArray<Type>::operator[](sa_UsedCount);
}

template<class Type>
inline void StaticStackArray<Type>::PopUntil(INDEX iNewTop)
{
	ASSERT(iNewTop < sa_UsedCount);
	sa_UsedCount = iNewTop+1;
}

template<class Type>
inline void StaticStackArray<Type>::PopAll(void) 
{
	sa_UsedCount = 0;
}

template<class Type>
inline Type &StaticStackArray<Type>::operator[](INDEX i) 
{
	// ȷ���ڶ�ջ�������ݷ�Χ�ڵ�����
	ASSERT(this!=NULL && i<sa_UsedCount);

	return StaticArray<Type>::operator[](i);
}

template<class Type>
inline const Type &StaticStackArray<Type>::operator[](INDEX i) const 
{
	// ȷ���ڶ�ջ�������ݷ�Χ�ڵ�����
	ASSERT(this!=NULL && i<sa_UsedCount);

	return StaticArray<Type>::operator[](i);
}

template<class Type>
INDEX StaticStackArray<Type>::Count(void) const 
{
	ASSERT(this!=NULL);
	
	return sa_UsedCount;
}

template<class Type>
INDEX StaticStackArray<Type>::Index(Type *ptMember) 
{
	ASSERT(this!=NULL);

	// ����ȷ��������Χ�ھ�̬�����ڴ淶Χ��
	INDEX i = StaticArray<Type>::Index(ptMember);
	// Ȼ��ȷ��������Χ�ڶ�ջ��ǰ����Χ��
	ASSERTMSG(i<sa_UsedCount, "StaticStackArray<>::Index(): Not a member of this array!");

	return i;
}

template<class Type>
StaticStackArray<Type> &StaticStackArray<Type>::operator=(const StaticStackArray<Type> &arOriginal)
{
	ASSERT(this!=NULL && &arOriginal!=NULL && this!=&arOriginal);

	StaticArray<Type>::operator=(arOriginal);
	sa_UsedCount = arOriginal.sa_UsedCount;

	return *this;
}


template<class Type>
void StaticStackArray<Type>::MoveArray(StaticStackArray<Type> &arOther)
{
	ASSERT(this!=NULL && &arOther!=NULL && this!=&arOther);

	// ���֮ǰ�����ж���
	Clear();
	if (arOther.Count()==0) 
	{
		return;
	}

	// �ӹ��¶�������ָ��
	StaticArray<Type>::MoveArray(arOther);
	sa_UsedCount = arOther.sa_UsedCount;
	sa_ctAllocationStep = arOther.sa_ctAllocationStep;
	arOther.sa_UsedCount = 0;
}

}

#endif

