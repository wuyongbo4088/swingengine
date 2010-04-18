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

#ifndef Swing_DistTriangle3Triangle3_H
#define Swing_DistTriangle3Triangle3_H

#include "SEFoundationLIB.h"
#include "SEDistance.h"
#include "SETriangle3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ˵��:
// ����:Sun Che
// ʱ��:20090119
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SEDistTriangle3Triangle3f : public SEDistance<float, 
    SEVector3f>
{
public:
    SEDistTriangle3Triangle3f(const SETriangle3f& rTriangle0,
        const SETriangle3f& rTriangle1);

    // �������.
    const SETriangle3f& GetTriangle0(void) const;
    const SETriangle3f& GetTriangle1(void) const;

    // static distance��ѯ.
    virtual float Get(void);
    virtual float GetSquared(void);

    // ����dynamic distance��ѯ��convex function����.
    virtual float Get(float fT, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);
    virtual float GetSquared(float fT, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);

    // ����������Ϣ.
    float GetTriangleBary0(int i) const;
    float GetTriangleBary1(int i) const;

private:
    const SETriangle3f* m_pTriangle0;
    const SETriangle3f* m_pTriangle1;

    // ����������Ϣ.
    float m_afTriangleBary0[3];  // closest0 = sum_{i=0}^2 bary0[i]*vertex0[i]
    float m_afTriangleBary1[3];  // closest1 = sum_{i=0}^2 bary1[i]*vertex1[i]
};

}

#endif
