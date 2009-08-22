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

#ifndef Swing_DistVector3Segment3_H
#define Swing_DistVector3Segment3_H

#include "SEFoundationLIB.h"
#include "SEDistance.h"
#include "SESegment3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// 名称:DistVector3Segment3f类
// 说明:
// 作者:Sun Che
// 时间:20090116
//----------------------------------------------------------------------------
class SE_FOUNDATION_API DistVector3Segment3f
    : public Distance<float, Vector3f>
{
public:
    DistVector3Segment3f(const Vector3f& rVector, const Segment3f& rSegment);

    // 对象访问.
    const Vector3f& GetVector(void) const;
    const Segment3f& GetSegment(void) const;

    // static distance查询.
    virtual float Get(void);
    virtual float GetSquared(void);

    // 用于dynamic distance查询的convex function计算.
    virtual float Get(float fT, const Vector3f& rVelocity0,
        const Vector3f& rVelocity1);
    virtual float GetSquared(float fT, const Vector3f& rVelocity0,
        const Vector3f& rVelocity1);

    // 线段最近点相关信息.
    float GetSegmentParameter(void) const;

private:
    const Vector3f* m_pVector;
    const Segment3f* m_pSegment;

    // 线段最近点相关信息.
    float m_fSegmentParameter;  // closest1 = seg.origin+param*seg.direction
};

}

#endif
