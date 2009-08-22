#ifndef Swing_DynamicStackArray_H
#define Swing_DynamicStackArray_H

#include "SEDynamicArray.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:��̬��ջ������
// ˵��:�ο���Serious Engine
// ����:Sun Che
// ʱ��:20070617
//----------------------------------------------------------------------------
template<class Type>
class DynamicStackArray : public DynamicArray<Type> 
{
public:
    DynamicStackArray(void);
    virtual ~DynamicStackArray(void);

public:
    inline void SetAllocationStep(INDEX iStep);
    inline void Clear(void);

    // Pushֻ���Ӷ�ջʹ�ü���,��û������ѹ���¶���,��Ҫ�����Ժ��ֶ�ѹ���¶���,
    // example:Push() = Type,û�в�������,ֱ�ӷ������鸳ֵ,Ч�ʽϸ�
    inline Type& Push(void);
    inline Type* Push(INDEX iCount);
    // �������ж���
    inline void PopAll(void);

    inline Type& operator [] (INDEX iObject);
    inline const Type& operator [] (INDEX iObject) const;

	INDEX GetCount(void) const;
	INDEX Index(Type* pObject);
	// ��ȡ����ָ������,���ڶ�ָ����������
	Type** GetArrayOfPointers(void);

	DynamicStackArray<Type>& operator = (DynamicStackArray<Type> &rOriginal);

protected:
    INDEX m_iUsed;            // ��ǰ��������
    INDEX m_iAllocationStep;  // ��ջ���ʱ�·�����ڴ�����
};

#include "SEDynamicStackArray.inl"

}

#endif

