// Swing Engine Version 1 Source Code 
// Most of techniques in the engine are mainly based on David Eberly's
// Wild Magic 4 open-source code.The author of Swing Engine learned a lot
// from Eberly's experience of architecture and algorithm.
// Several sub-systems are totally new,and others are re-implimented or
// re-organized based on Wild Magic 4's sub-systems.
// Copyright (c) 2007-2010.  All Rights Reserved
//
// Eberly's permission:
// Geometric Tools, Inc.
// http://www.geometrictools.com
// Copyright (c) 1998-2006.  All Rights Reserved
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.  The license is available for reading at
// the location:
// http://www.gnu.org/copyleft/lgpl.html

#ifndef Swing_HashTable_H
#define Swing_HashTable_H

// TKey����ʵ�ֵĺ���:
//   TKey::TKey()
//   TKey& TKey::operator=(const TKey&)
//   bool TKey::operator==(const TKey&) const
//   bool TKey::operator!=(const TKey&) const
//   TKey::operator unsigned int() const
// ��ʽ����ת����unsigned int�������ɹ�ϣ������,
// ����ֵ��һ��Ҫ�ڹ�ϣ��������Χ��,��ϣ��ͨ��ȡģ������ӳ�䵽�Ϸ���Χ
// TValue����ʵ�ֵĺ���:
//   TValue::TValue()
//   TValue& TValue::operator=(const TValue&)

#include "SESystem.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:��ϣ����
// ˵��:
// ����:Sun Che
// ʱ��:20080308
//----------------------------------------------------------------------------
template <class TKey, class TValue>
class HashTable
{
public:
    HashTable(int iTableSize);
    ~HashTable(void);

    int GetCount(void) const;

    bool Insert(const TKey& rKey, const TValue& rValue);
    TValue* Find(const TKey& rKey) const;
    bool Remove(const TKey& rKey);
    void RemoveAll(void);

    TValue* GetFirst(TKey* pKey) const;
    TValue* GetNext(TKey* pKey) const;

    // �û��Զ���Ĺ�ϣ����
    int (*UserHashFunction)(const TKey&);

private:
    class HTItem
    {
    public:
        TKey m_Key;
        TValue m_Value;
        HTItem* m_pNextHashItem;
    };

    // Ĭ�Ϲ�ϣ����,�ɱ��û���������
    int HashFunction(const TKey& rKey) const;

    // ��ϣ��
    int m_iTableSize;
    int m_iCount;
    HTItem** m_ppTable;

    // �������б���ʱ�ĸ�������
    mutable int m_iIndex;
    mutable HTItem* m_pHashItem;
};

#include "SEHashTable.inl"

}

#endif
