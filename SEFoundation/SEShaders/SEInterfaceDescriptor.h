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
#include "SEDescriptorItem.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:帮助SEProgram类装载含有shader interface的程序,使其完成动态拼接
//     并编译.
// Author:Sun Che
// Date:20090418
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SEInterfaceDescriptor : public SEObject
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    SEInterfaceDescriptor(void);
    ~SEInterfaceDescriptor(void);

    // descriptor item访问.
    inline int GetCount(void) const;
    void AttachItem(SEDescriptorItem* pItem);
    void DetachItem(SEDescriptorItem* pItem);
    SEDescriptorItem* GetItem(int i) const;

    // 获取接口描述.
    void GetDescription(std::string& rDesc) const;

protected:
    std::vector<SEDescriptorItemPtr> m_DescriptorItems;
};

typedef SESmartPointer<SEInterfaceDescriptor> SEInterfaceDescriptorPtr;

#include "SEInterfaceDescriptor.inl"

}

#endif
