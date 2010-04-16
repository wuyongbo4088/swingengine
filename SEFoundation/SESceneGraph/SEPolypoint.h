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

#ifndef Swing_Polypoint_H
#define Swing_Polypoint_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEGeometry.h"
#include "SEIntArray.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:��ͼԪ��
// ˵��:
// ����:Sun Che
// ʱ��:20080808
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Polypoint : public Geometry
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    Polypoint(VertexBuffer* pVBuffer);
    virtual ~Polypoint(void);

    void SetActiveCount(int iActiveCount);
    inline int GetActiveCount(void) const;

protected:
    Polypoint(void);

    // ����Ӧ�ó���ָ������ʵ�ʶ������Ķ����Ӽ�������Ⱦ.
    int m_iActiveCount;
};

typedef SESmartPointer<Polypoint> PolypointPtr;

#include "SEPolypoint.inl"

}

#endif
