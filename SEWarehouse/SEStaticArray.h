#ifndef Swing_StaticArray_H
#define Swing_StaticArray_H

#include "SESystem.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:��̬������
// ˵��:�ο���Serious Engine
// ����:Sun Che
// ʱ��:20070618
//----------------------------------------------------------------------------
template<class Type>
class StaticArray 
{
public:
    StaticArray(void);
    virtual ~StaticArray(void);

public:
    void operator = (const StaticArray<Type>& rOriginal);

    inline void Append(INDEX iCount);
    // ����ߴ絫���־��ж���
    inline void Expand(INDEX iNewCount);
    // �ͷ��ڴ�,�ָ���ʼ��״̬
    inline void Delete(void);
    // ����Delete�ͷ��ڴ�,�ָ���ʼ��״̬
    inline void Clear(void);

    inline Type& operator [] (INDEX iObject);
    inline const Type& operator [] (INDEX iObject) const;
    
    INDEX GetCount(void) const;
    INDEX Index(Type* pObject);

    // ������һ��������������ݹ���
    void CopyArray(const StaticArray<Type>& rOriginal);
    // ����һ�����������ָ��ֱ�ӽӹ���
    void MoveArray(StaticArray<Type>& rOther);

public:
	INDEX m_iCount;
	Type* m_pArray;
};

//----------------------------------------------------------------------------
// ����:��̬�����������
// ˵��:�ο���Serious Engine
// ����:Sun Che
// ʱ��:20070618
//----------------------------------------------------------------------------
template<class Type>
class StaticArrayIterator
{
public:
    inline StaticArrayIterator(StaticArray<Type>& rSA);
    inline ~StaticArrayIterator(void);

public:
    // ֻ��������
    inline void MoveToNext(void);
    inline bool IsPastEnd(void);

    // ��ȡ��ǰ����
    Type& Current(void) { return m_rStaticArray[m_iIndex]; }
    Type& operator * (void) { return m_rStaticArray[m_iIndex]; }
    operator Type* (void) { return &m_rStaticArray[m_iIndex]; }
    Type* operator -> (void) { return &m_rStaticArray[m_iIndex]; }

private:
    INDEX m_iIndex;                      // ��ǰ��������
    StaticArray<Type>& m_rStaticArray;   // �������ľ�̬���������
};

#include "SEStaticArray.inl"

#define FOREACHINSTATICARRAY(array, type, iter) \
  for(StaticArrayIterator<type> iter(array); !iter.IsPastEnd(); iter.MoveToNext() )

}

#endif

