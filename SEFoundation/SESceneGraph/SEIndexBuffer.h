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

#ifndef Swing_IndexBuffer_H
#define Swing_IndexBuffer_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEObject.h"
#include "SEBindable.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Author:Sun Che
// Date:20080621
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SEIndexBuffer : public SEObject, public SEBindable
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    SEIndexBuffer(int iICount);
    SEIndexBuffer(const SEIndexBuffer* pIBuffer);
    virtual ~SEIndexBuffer(void);

    inline int operator [] (int i) const;
    inline int& operator [] (int i);

    // ��ȡ��������,�������ֽ���ΪGetIndexCount()*sizeof(int)
    inline int GetIndexCount(void) const;
    inline int* GetData(void);
    inline const int* GetData(void) const;

    // �û�����������������"active count",ʹ�ô˺���������,
    // ����û�иı�ʵ�����ݴ洢,ֻ������m_iICount,
    // �û������α���ʵ��������Ŀ,����һϵ�в�����ɺ�ָ�ʵ��������Ŀ,
    // active count���ܳ���ʵ��������Ŀ
    inline void SetIndexCount(int iICount);

protected:
    // streaming support
    SEIndexBuffer(void);

	// ��������������
    int m_iICount;
    int* m_pIndex;
};

typedef SESmartPointer<SEIndexBuffer> SEIndexBufferPtr;

#include "SEIndexBuffer.inl"

}

#endif
