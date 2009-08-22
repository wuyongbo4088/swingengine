#ifndef Swing_DynamicArray_H
#define Swing_DynamicArray_H

#include "SESystem.h"
#include "SELists.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:��̬�����ڴ��ṹ
// ˵��:�ο���Serious Engine
//      ÿ�ζ�̬��������ʱ,��������ṹ����ʵ���ڵ�
// ����:Sun Che
// ʱ��:20070608
//----------------------------------------------------------------------------
struct DABlockInfo 
{
    // ����׷��ά����ǰlist
    ListNode NestedListNode;
    // ָ����������ڴ�ռ��׵�ַ
    void*    BlockMemory;
};

//----------------------------------------------------------------------------
// ����:��̬������
// ˵��:�ο���Serious Engine
// ����:Sun Che
// ʱ��:20070608
//----------------------------------------------------------------------------
template<class Type>
class DynamicArray 
{
public:
    DynamicArray(void);
    DynamicArray(DynamicArray<Type>& rOriginal);
    virtual ~DynamicArray(void);

public:
    // ��̬��������,һ����СΪiCount*sizeof(Type)�������ݿ鱻���ӽ���
    Type* Append(INDEX iCount = 1);
    // ��ͼɾ��һ��Ԫ��,�����ͷ������ݿ��ڴ�
    void  Delete(Type* pObject);
    void  Clear(void);

    // �����Ա����
    Type* Pointer(INDEX iObject);
    const Type* Pointer(INDEX iObject) const;
    inline Type& operator [] (INDEX iObject);
    inline const Type& operator [] (INDEX iObject) const;

    DynamicArray<Type>& operator = (DynamicArray<Type>& rOriginal);

    void MoveArray(DynamicArray<Type>& rOther);

    void Lock(void);
    void Unlock(void);
    INDEX Index(Type* pObject);
    INDEX GetCount(void) const;

protected:
    // ʵ�ʶ�̬����Ķ����ڴ�ռ��ǲ�������,Ϊ��ʵ��random access,
    // ʹ�����������ָ���ڴ�ռ��ӷ���m_BlocksList�еĶ���
    // ÿ�����鶯̬����,�µ��ڴ汻����,ָ��ռ�Ҳ��������,
    // ÿ���¼����ָ�붼ָ���·�����ڴ�block���Լ���Ӧ��Type��С���ڴ�
    ListHead m_BlocksList;  // �ѷ����ڴ������
    Type**   m_Pointers;    // ����ָ������
    INDEX    m_iCount;      // �����С

    // Ϊ֧�ֶ��߳�,��ǰ�������ڱ����ʵļ�������
#if CHECKARRAYLOCKING
    INDEX m_iLockCt;
#endif

    // ָ����������iCount
	void GrowPointers(INDEX iCount);
    // ָ����������iCount
	void ShrinkPointers(INDEX iCount);
    // ����һ�����ڴ��
    Type* AllocBlock(INDEX iCount);
    // ����ָ�������Ƿ��ڵ�ǰ������,ʱ�临�Ӷ�O(n)
    INDEX GetIndex(Type* pMember);
};

//----------------------------------------------------------------------------
// ����:��̬�����������
// ˵��:�ο���Serious Engine
// ����:Sun Che
// ʱ��:20070608
//----------------------------------------------------------------------------
template<class Type>
class DynamicArrayIterator 
{
public:
	inline DynamicArrayIterator(DynamicArray<Type>& rDA);
	inline ~DynamicArrayIterator(void);

public:
    inline void MoveToNext(void);
    inline bool IsPastEnd(void);

    Type& Current(void) { return *m_rDynamicArray.Pointer(m_iIndex); }
    Type& operator * (void) { return *m_rDynamicArray.Pointer(m_iIndex); }
    operator Type * (void) { return m_rDynamicArray.Pointer(m_iIndex); }
    Type* operator -> (void) { return m_rDynamicArray.Pointer(m_iIndex); }

private:
	INDEX               m_iIndex;          // ��ǰ��������
	DynamicArray<Type>& m_rDynamicArray;   // �����������������
};

#include "SEDynamicArray.inl"

}

#endif

