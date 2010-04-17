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

#ifndef Swing_DistLine3Triangle3_H
#define Swing_DistLine3Triangle3_H

#include "SEFoundationLIB.h"
#include "SEDistance.h"
#include "SELine3.h"
#include "SETriangle3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:DistLine3Triangle3f��
// ˵��:
// ����:Sun Che
// ʱ��:20090114
//----------------------------------------------------------------------------
class SE_FOUNDATION_API DistLine3Triangle3f : public Distance<float, SEVector3f>
{
public:
    DistLine3Triangle3f(const SELine3f& rLine, const SETriangle3f& rTriangle);

    // �������.
    const SELine3f& GetLine(void) const;
    const SETriangle3f& GetTriangle(void) const;

    // static distance��ѯ.
    virtual float Get(void);
    virtual float GetSquared(void);

    // ����dynamic distance��ѯ��convex function����.
    virtual float Get(float fT, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);
    virtual float GetSquared(float fT, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);

    // ����������Ϣ.
    float GetLineParameter(void) const;
    float GetTriangleBary(int i) const;

private:
    const SELine3f* m_pLine;
    const SETriangle3f* m_pTriangle;

    // ����������Ϣ.
    float m_fLineParameter;  // closest0 = line.origin+param*line.direction
    float m_afTriangleBary[3];  // closest1 = sum_{i=0}^2 bary[i]*tri.vertex[i]
};

}

#endif
