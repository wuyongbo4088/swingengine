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

#ifndef Swing_InterfaceDescriptor_H
#define Swing_InterfaceDescriptor_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SESystem.h"
#include "SEObject.h"
#include "SEString.h"
#include "SEDescriptorItem.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:interface descriptor��
// ˵��:����Program��װ�غ���shader interface�ĳ���,ʹ����ɶ�̬ƴ�Ӳ�����.
// ����:Sun Che
// ʱ��:20090418
//----------------------------------------------------------------------------
class SE_FOUNDATION_API InterfaceDescriptor : public Object
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    InterfaceDescriptor(void);
    ~InterfaceDescriptor(void);

    // descriptor item����.
    inline int GetCount(void) const;
    void AttachItem(DescriptorItem* pItem);
    void DetachItem(DescriptorItem* pItem);
    DescriptorItem* GetItem(int i) const;

    // ��ȡ�ӿ�����.
    void GetDescription(String& rDesc) const;

protected:
    std::vector<DescriptorItemPtr> m_DescriptorItems;
};

typedef SmartPointer<InterfaceDescriptor> InterfaceDescriptorPtr;

#include "SEInterfaceDescriptor.inl"

}

#endif
