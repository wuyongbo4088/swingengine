#ifndef Swing_StaticArray_CPP
#define Swing_StaticArray_CPP

#include "SEStaticArray.h"

namespace Swing
{

//----------------------------------------------------------------------------
template<class Type>
StaticArray<Type>::StaticArray() 
{
    m_iCount = 0;
    m_iArray = 0;
}
//----------------------------------------------------------------------------
template<class Type>
StaticArray<Type>::~StaticArray() 
{
    if( m_iCount != 0 ) 
    {
        Delete();
    }
}
//----------------------------------------------------------------------------
template<class Type>
INDEX StaticArray<Type>::GetCount() const 
{
    ASSERT( this != 0 );
	
    return m_iCount;
}
//----------------------------------------------------------------------------
template<class Type>
INDEX StaticArray<Type>::Index(Type* pObject) 
{
    ASSERT( this != 0 );
    // �õ�ǰ����ָ���ȥ�����׵�ַָ����Կ��ٵó���ǰ�����Ƿ��������еĽ���
    INDEX i = pObject - m_pArray;
    // ȷ����ǰ���������鷶Χ��
    ASSERT( i >= 0 && i < m_iCount );

    return i;
}
//----------------------------------------------------------------------------
template<class Type>
void StaticArray<Type>::CopyArray(const StaticArray<Type>& rOriginal)
{
    ASSERT( this != 0 && &rOriginal != 0 && this != &rOriginal );

    // ��������Լ�֮ǰ�Ķ����ͷ��ڴ�
    Clear();
    // ��ȡ�³ߴ�
    INDEX iOriginal = rOriginal.GetCount();
    if( iOriginal == 0 ) 
    {
        return;
    }

    // �����³ߴ�����ڴ�
    Append(ctOriginal);
    // �������ж������
    for( INDEX iNew = 0; iNew < iOriginal; iNew++ ) 
    {
        m_pArray[iNew] = rOriginal[iNew];
    }
}
//----------------------------------------------------------------------------
template<class Type>
void StaticArray<Type>::MoveArray(StaticArray<Type>& rOther)
{
    ASSERT( this != 0 && &rOther != 0 && this != &rOther );

    // ��������Լ�֮ǰ�Ķ����ͷ��ڴ�
    Clear();
    if( rOther.GetCount() == 0 ) 
    {
        return;
    }

    // ֱ�ӽӹ������������ָ��
    m_iCount = rOther.m_iCount;
    m_pArray = rOther.m_pArray;
    rOther.m_iCount = 0;
    rOther.m_pArray = 0;
}
//----------------------------------------------------------------------------

}

#endif

