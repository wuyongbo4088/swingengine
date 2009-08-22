//----------------------------------------------------------------------------
template<class Type>
inline DynamicStackArray<Type>::DynamicStackArray() 
    : DynamicArray<Type>() 
{
    m_iUsed = 0;
    m_iAllocationStep = 16;

    DynamicArray<Type>::Lock();
}
//----------------------------------------------------------------------------
template<class Type>
inline DynamicStackArray<Type>::~DynamicStackArray() 
{
    DynamicArray<Type>::Unlock();
}
//----------------------------------------------------------------------------
template<class Type>
inline void DynamicStackArray<Type>::Clear() 
{
    DynamicArray<Type>::Clear(); 
    m_iUsed = 0; 
}
//----------------------------------------------------------------------------
template<class Type>
inline void DynamicStackArray<Type>::SetAllocationStep(INDEX iStep) 
{
    ASSERT( iStep > 0 );

    m_iAllocationStep = iStep;
}
//----------------------------------------------------------------------------
template<class Type>
inline Type& DynamicStackArray<Type>::Push() 
{
    // ��������ڴ�������,��̬����
    if( DynamicArray<Type>::GetCount() - m_iUsed < 1 ) 
    {
        DynamicArray<Type>::Append(m_iAllocationStep);
    }
  
    // �Ժ�Ӧѹ���¶���
    m_iUsed++;
	
    ASSERT( m_iUsed <= DynamicArray<Type>::GetCount() );
	
    return DynamicArray<Type>::operator[](m_iUsed - 1);
}
//----------------------------------------------------------------------------
template<class Type>
inline Type* DynamicStackArray<Type>::Push(INDEX iCount) 
{
    while( DynamicArray<Type>::GetCount() - m_iUsed < iCount ) 
    {
        DynamicArray<Type>::Append(m_iAllocationStep);
    }

    // �Ժ�Ӧѹ�������¶���
    m_iUsed += iCount;
	
    ASSERT( m_iUsed <= DynamicArray<Type>::GetCount() );
	
    return &DynamicArray<Type>::operator[](m_iUsed - iCount);
}
//----------------------------------------------------------------------------
template<class Type>
inline void DynamicStackArray<Type>::PopAll() 
{
    if( m_BlocksList.IsEmpty() || &m_BlocksList.GetHead() == &m_BlocksList.GetTail() ) 
    {
        m_iUsed = 0;
    } 
    else
    {
        // ��ס֮ǰ��������ڴ��С
        INDEX iUsedBefore = DynamicArray<Type>::GetCount();
        // �ͷŵ�֮ǰ����������ڴ�
        DynamicArray<Type>::Clear();
        // ���·���֮ǰ��С��һ���ڴ��
        DynamicArray<Type>::Append(iUsedBefore);
        m_iUsed = 0;
    }
}
//----------------------------------------------------------------------------
template<class Type>
inline Type& DynamicStackArray<Type>::operator[](INDEX iObject) 
{
    ASSERT( this != 0 );
    ASSERT( i < m_iUsed );     // ����Ƿ�Խ��

    return DynamicArray<Type>::operator[](iObject);
}
//----------------------------------------------------------------------------
template<class Type>
inline const Type& DynamicStackArray<Type>::operator[](INDEX iObject) const 
{
    ASSERT( this != 0 );
    ASSERT( i < m_iUsed );     // ����Ƿ�Խ��

    return DynamicArray<Type>::operator[](iObject);
}
//----------------------------------------------------------------------------