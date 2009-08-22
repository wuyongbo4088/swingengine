#ifndef Swing_DynamicArray_CPP
#define Swing_DynamicArray_CPP

#include "SEDynamicArray.h"

namespace Swing
{

// ����������̬����
// ע��:�ڽ��ж�̬����Ԫ�����/ɾ������ǰ,����궨��ĵ����������ȱ�destroy
#define FOREACHINDYNAMICARRAY(array, type, iter) \
  for( DynamicArrayIterator<type> iter(array); !iter.IsPastEnd(); iter.MoveToNext() )

//----------------------------------------------------------------------------
template<class Type>
DynamicArray<Type>::DynamicArray(void) 
{
#if CHECKARRAYLOCKING
    m_iLockCt = 0;
#endif
	// ��ʼ����Ϊ��
    m_Pointers = 0;
    m_iCount = 0;
}
//----------------------------------------------------------------------------
template<class Type>
DynamicArray<Type>::DynamicArray(DynamicArray<Type>& rOriginal)
{
#if CHECKARRAYLOCKING
    m_iLockCt = 0;
#endif
    // ��ʼ����Ϊ��
    m_Pointers = 0;
    m_iCount = 0;

    (*this) = rOriginal;
}
//----------------------------------------------------------------------------
template<class Type>
DynamicArray<Type>::~DynamicArray(void) 
{
    Clear();
}
//----------------------------------------------------------------------------
template<class Type>
void DynamicArray<Type>::Clear(void) 
{
    ASSERT( this != 0 );
  
    // ���������ָ�����鼰��ָ������ݿ�
  
    if( m_iCount != 0 ) 
    {
        // Serious Engine:ע��:���Ǳ�����ʽ���ͷŶ���,��Ϊɾ�����鲻����ö�����������
        // ������������ָ��
        //for( INDEX iPointer = 0; iPointer < m_iCount; iPointer++ ) 
        //{
        //    // destroy��ָ��Ķ���
        //    ::Clear(*m_Pointers[iPointer]);
        //}

        // �ͷ�ָ������
        free((char*)m_Pointers);

        m_Pointers = 0;
        m_iCount = 0;
    } 
    else 
    {
        ASSERT( m_Pointers == 0 );
        // û��ʲô���ͷŵ�
    }

    // �ͷ������Ѵ������ڴ��
    FORDELETELIST(DABlockInfo, NestedListNode, m_BlocksList, itBlock) 
    {
		// �ͷŵ�ǰblock info�ڵ��������ڴ��(Serious Engine:�Ⲣ������ÿ��ڶ������������ - ����������˵��!)
        delete[] (Type*)itBlock->BlockMemory;
		// �ͷŵ�ǰblock info
        delete &itBlock.Current();
    }
}
//----------------------------------------------------------------------------
template<class Type>
void DynamicArray<Type>::GrowPointers(INDEX iCount) 
{
    ASSERT( this != 0 && iCount > 0 );
  
    if( m_iCount == 0 ) 
    {
        // �����û�д���ָ������
        ASSERT( m_Pointers == 0 );
		// ����ָ������
        m_iCount = iCount;
        m_Pointers = (Type**)malloc(m_iCount * sizeof(Type*));
    } 
    else 
    {
        // ָ����������
        m_iCount += iCount;
        m_Pointers = (Type**)realloc((void*)m_Pointers, m_iCount * sizeof(Type*));
    }
}
//----------------------------------------------------------------------------
template<class Type>
void DynamicArray<Type>::ShrinkPointers(INDEX iCount) 
{
    ASSERT( this != 0 && iCount > 0 );
    ASSERT( m_Pointers != 0);

    m_iCount -= iCount;
    ASSERT( m_iCount >= 0 );

    if( m_iCount == 0 ) 
	{
        // �������Ϊ0,���ͷ�ָ������
        free(m_Pointers);
        m_Pointers = 0;
    } 
    else 
    {
        // ָ����������
        m_Pointers = (Type**)realloc((void*)m_Pointers, m_iCount * sizeof(Type*));
    }
}
//----------------------------------------------------------------------------
template<class Type>
Type* DynamicArray<Type>::AllocBlock(INDEX iCount) 
{
    ASSERT( this != 0 && iCount > 0 );
    Type* pBlock;
    DABlockInfo* pBlockInfo;

    // �����ڴ����ݿ鲢�������(+1 for cache-prefetch opt)
    pBlock = new Type[iCount + 1];
    // �������Ϣ
    pBlockInfo = new DABlockInfo;
    // �ѿ���Ϣ����block list
    m_BlocksList.AddTail(pBlockInfo->NestedListNode);
    // ����Ϣָ�����ڴ����ݿ�
    pBlockInfo->BlockMemory = pBlock;

    return pBlock;
}
//----------------------------------------------------------------------------
template<class Type>
Type* DynamicArray<Type>::Append(INDEX iCount) 
{
    ASSERT( this != NULL && iCount >= 0 );

    if( iCount == 0 ) 
    {
        return 0;
    }

    Type* pBlock;
    INDEX iOldCount = m_iCount;

    // ����ָ������
    GrowPointers(iCount);
    // �������ڴ��
    pBlock = AllocBlock(iCount);
    // ��������ָ���Ա���ָ����������
    for( INDEX iNewMember = 0; iNewMember < iCount; iNewMember++ ) 
    {
        m_Pointers[iOldCount + iNewMember] = pBlock + iNewMember;
    }

    return pBlock;
}
//----------------------------------------------------------------------------
template<class Type>
void DynamicArray<Type>::Delete(Type* pMember) 
{
    ASSERT( this != 0 );

#if CHECKARRAYLOCKING
    ASSERT( m_iLockCt == 0 );
#endif

    // ��������Type��Clear����
    ::Clear(*pMember);

    INDEX iMember = GetIndex(pMember);
    // ���������һ��Ԫ�طŹ���
    m_Pointers[iMember] = m_Pointers[m_iCount-1];
    // ָ����������1
    ShrinkPointers(1);
    // ���ͷ��ڴ�
    //!!!!
}
//----------------------------------------------------------------------------
template<class Type>
Type* DynamicArray<Type>::Pointer(INDEX iMember) 
{
    ASSERT( this != 0 );
    // ��ֹ����Խ��
    ASSERT( iMember >= 0 && iMember < m_iCount );

#if CHECKARRAYLOCKING
    ASSERT( m_iLockCt > 0 );
#endif

    return m_Pointers[iMember];
}
//----------------------------------------------------------------------------
template<class Type>
const Type* DynamicArray<Type>::Pointer(INDEX iMember) const 
{
    ASSERT( this != 0 );
    // ��ֹ����Խ��
    ASSERT( iMember >= 0 && iMember < m_iCount );

#if CHECKARRAYLOCKING
	ASSERT( m_iLockCt > 0 );
#endif

    return m_Pointers[iMember];
}
//----------------------------------------------------------------------------
template<class Type>
void DynamicArray<Type>::Lock() 
{
    ASSERT( this != 0 );

#if CHECKARRAYLOCKING
    ASSERT( m_iLockCt >= 0 );
    // ���Ӽ�������
    m_iLockCt++;
#endif
}
//----------------------------------------------------------------------------
template<class Type>
void DynamicArray<Type>::Unlock() 
{
    ASSERT( this != 0 );

#if CHECKARRAYLOCKING
    // ���ټ�������
    m_iLockCt--;
    ASSERT( m_iLockCt >= 0 );
#endif
}
//----------------------------------------------------------------------------
template<class Type>
INDEX DynamicArray<Type>::Index(Type* pMember) 
{
    ASSERT( this != 0 );

#if CHECKARRAYLOCKING
    ASSERT( m_iLockCt > 0 );
#endif

    return GetIndex(pMember);
}
//----------------------------------------------------------------------------
template<class Type>
INDEX DynamicArray<Type>::GetIndex(Type* pMember) 
{
    ASSERT( this != 0 );
    // �����Ĳ���
    for( INDEX iMember = 0; iMember < m_iCount; iMember++ ) 
    {
        if( m_Pointers[iMember] == pMember ) 
        {
            return iMember;
        }
    }
	
    ASSERTALWAYS("DynamicArray<>::GetIndex(): Not a member of this array!");

    return 0;
}
//----------------------------------------------------------------------------
template<class Type>
INDEX DynamicArray<Type>::GetCount(void) const 
{
    ASSERT( this != 0 );

    return m_iCount;
}
//----------------------------------------------------------------------------
template<class Type>
DynamicArray<Type>& DynamicArray<Type>::operator=(DynamicArray<Type>& rOriginal)
{
    ASSERT( this!= 0 );
    ASSERT(&rOriginal != 0 );
    ASSERT( this!= &rOriginal );
    // ���֮ǰ������
    Clear();

    INDEX ctOriginal = rOriginal.GetCount();
    if( ctOriginal == 0 ) 
	{
        return *this;
	}

    // ���������ڴ�
    Type* pNew = Append(ctOriginal);

    // ��������
    rOriginal.Lock();
    for( INDEX iNew = 0; iNew < ctOriginal; iNew++ ) 
    {
        pNew[iNew] = rOriginal[iNew];
    }
    rOriginal.Unlock();

    return *this;
}
//----------------------------------------------------------------------------
template<class Type>
void DynamicArray<Type>::MoveArray(DynamicArray<Type>& rOther)
{
    ASSERT( this != 0 && &rOther != 0 );

#if CHECKARRAYLOCKING
    ASSERT( m_iLockCt == 0 && rOther.m_iLockCt == 0 );
#endif

    // û������
    if( rOther.m_iCount == 0 ) 
    {
        return;
    }

    INDEX iOldCount = m_iCount;
	GrowPointers(rOther.m_iCount);

    // ��ָ������ȫ�������ȸ��ƹ���
    for( INDEX iOtherPointer = 0; iOtherPointer < rOther.m_iCount; iOtherPointer++ ) 
    {
        // ���ڵ�ǰָ����������֮��
        m_Pointers[iOldCount + iOtherPointer] = rOther.m_Pointers[iOtherPointer];
    }
    
    // �ͷ���ָ������
    rOther.ShrinkPointers(rOther.m_iCount);
	// ������block list���ӹ���
    m_BlocksList.MoveList(rOther.m_BlocksList);
}
//----------------------------------------------------------------------------

}

#endif

