#ifndef Swing_DynamicMaxHeap_CPP
#define Swing_DynamicMaxHeap_CPP

#include "SEDynamicMaxHeap.h"
#include "SEDynamicArray.cpp"

namespace Swing
{

//----------------------------------------------------------------------------
template <class Type>
DynamicMaxHeap<Type>::DynamicMaxHeap()
{
    m_iUsed = 0;
    m_iAllocationStep = 16;
    
    UserCompareFunction = 0;
    UserOperationFunction = 0;

	m_HeapArray.Lock();
}
//----------------------------------------------------------------------------
template <class Type>
DynamicMaxHeap<Type>::DynamicMaxHeap(DynamicArray<Type>& rDA, int iUsed, 
                                     int (*pCompare)(const void* p0, const void* p1),
                                     void (*pOperation)(void* pObject, int iIndex))
{
    ASSERT( rDA.GetCount() >= iUsed );

    m_HeapArray.MoveArray(rDA);

    m_iUsed = iUsed;
    m_iAllocationStep = 16;
    
    UserCompareFunction = pCompare;
    UserOperationFunction = pOperation;

    if( UserOperationFunction )
        for( int i = 0; i < m_iUsed; i++ )
            (*UserOperationFunction)((void*)m_HeapArray.Pointer(i), i);

    // ��ȡ���һ����Ҷ�ӽڵ��λ��
    int CurrentPos = (m_iUsed - 2) >> 1;
    while( CurrentPos >= 0 )
    {
        AdjustDownward(CurrentPos, m_iUsed - 1);
        CurrentPos--;
    }

	m_HeapArray.Lock();
}
//----------------------------------------------------------------------------
template <class Type>
DynamicMaxHeap<Type>::~DynamicMaxHeap()
{ 
    m_HeapArray.Clear();

	m_HeapArray.Unlock();
}
//----------------------------------------------------------------------------
template <class Type>
DynamicMaxHeap<Type>& DynamicMaxHeap<Type>::operator=(const DynamicMaxHeap<Type>& rOther)
{
    m_HeapArray = rOther.m_HeapArray;
    m_iUsed = rOther.m_iUsed;

    return *this;
}
//----------------------------------------------------------------------------
template <class Type>
void DynamicMaxHeap<Type>::AdjustDownward(int iStart, int iEnd)
{
    int i = iStart, j = (i << 1) + 1;
    // �ݴ浱ǰ���ƶ��Ķ���
    Type TempObject = m_HeapArray[i];

    while( j <= iEnd )
    {
        // ָ���������нϴ���
		if( j < iEnd && 
            CompareFunction((void*)m_HeapArray.Pointer(j), (void*)m_HeapArray.Pointer(j + 1)) < 0 )
			j++;

        // �������Ҫ����
        if( CompareFunction((void*)&TempObject, (void*)m_HeapArray.Pointer(j)) >= 0 )
            break;
        else
        {
            m_HeapArray[i] = m_HeapArray[j];
            if( UserOperationFunction )
                (*UserOperationFunction)((void*)m_HeapArray.Pointer(i), i);

            i = j;
            j = (j << 1) + 1;
        }
    }

    m_HeapArray[i] = TempObject;
    if( UserOperationFunction )
        (*UserOperationFunction)((void*)m_HeapArray.Pointer(i), i);
}
//----------------------------------------------------------------------------
template <class Type>
void DynamicMaxHeap<Type>::AdjustUpward(int iStart)
{
    int j = iStart, i = (j - 1) >> 1;
    // �ݴ浱ǰ���ƶ��Ķ���
    Type TempObject = m_HeapArray[j];

    while( j > 0 )
    {
        // �������Ҫ����
        if( CompareFunction((void*)m_HeapArray.Pointer(i), (void*)&TempObject) >= 0 )
            break;
        else
        {
            m_HeapArray[j] = m_HeapArray[i];
            if( UserOperationFunction )
                (*UserOperationFunction)((void*)m_HeapArray.Pointer(j), j);

            j = i;
            i = (i - 1) >> 1;
        }
    }

    m_HeapArray[j] = TempObject;
    if( UserOperationFunction )
        (*UserOperationFunction)((void*)m_HeapArray.Pointer(j), j);
}
//----------------------------------------------------------------------------
template <class Type>
int DynamicMaxHeap<Type>::CompareFunction(const void* p1, const void* p2)
{
    ASSERT( p1 && p2 );

    if( UserCompareFunction )
        return (*UserCompareFunction)(p1, p2);

    return memcmp(p1, p2, sizeof(Type));
}
//----------------------------------------------------------------------------

}

#endif