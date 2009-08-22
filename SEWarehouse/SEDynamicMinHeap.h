#ifndef Swing_DynamicMinHeap_H
#define Swing_DynamicMinHeap_H

#include "SESystem.h"
#include "SEDynamicArray.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:��̬��С����
// ˵��:m_HeapArray�����е�Ԫ��������ȫ�������Ĵ洢˳��:
//      ki <= k2i + 1(ki������) �� ki <= k2i + 2(ki���Һ���)
//      �����������һ����Ҷ�ӽڵ���: ki = (m_iCurrentSize - 2) / 2
//      ki�ڵ�ĸ��׽ڵ�kp = (ki - 1) / 2
// ����:Sun Che
// ʱ��:20070609
//----------------------------------------------------------------------------
template <class Type>
class DynamicMinHeap
{
public:
    DynamicMinHeap(void);
    // �ó�ʼ����Ԫ�ع�����,O(n*log(n))
    DynamicMinHeap(DynamicArray<Type>& rDA, int iUsed, 
                   int (*pCompare)(const void* p0, const void* p1) = 0,
                   void (*pOperation)(void* pObject, int iIndex) = 0);
    ~DynamicMinHeap(void);

public:
    inline void SetAllocationStep(int iStep);
    inline void Clear(void);

    DynamicMinHeap<Type>& operator = (const DynamicMinHeap<Type>& rOther);

    // �ڶ�β��׷��,֮�������,O(log(n))
    inline void Insert(const Type& rSrcObject);
    // ���߶���Ԫ��,֮�������,O(log(n))
    inline void Remove(int iPos, Type& rDesObject);
    // ���ö���Ԫ��,֮�������,O(log(n))
    inline void        Set(int iPos, const Type& rSrcObject);
	inline const Type& Get(int iPos) const;

    inline int GetCount(void) const;

    // �û�ָ���ıȽϺ���
    int (*UserCompareFunction)(const void* p1, const void* p2);
    // �û�ָ���Ķ����������,�ڶѵ���ʱ����,iIndex�Ƕ���������λ��
    void (*UserOperationFunction)(void* pObject, int iIndex);

private:
	DynamicArray<Type> m_HeapArray;
	int                m_iUsed;
    int                m_iAllocationStep;

    int CompareFunction(const void* p1, const void* p2);

	void AdjustDownward(int iStart, int iEnd);
	void AdjustUpward(int iStart);
};

#include "SEDynamicMinHeap.inl"

}

#endif