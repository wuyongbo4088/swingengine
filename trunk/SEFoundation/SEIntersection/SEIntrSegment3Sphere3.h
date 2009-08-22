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

#ifndef Swing_IntrSegment3Sphere3_H
#define Swing_IntrSegment3Sphere3_H

#include "SEFoundationLIB.h"
#include "SEIntersector.h"
#include "SESegment3.h"
#include "SESphere3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// 名称:IntrSegment3Sphere3f类
// 说明:
// 作者:Sun Che
// 时间:20081220
//----------------------------------------------------------------------------
class SE_FOUNDATION_API IntrSegment3Sphere3f
    : public Intersector<float, Vector3f>
{
public:
    IntrSegment3Sphere3f(const Segment3f& rSegment, const Sphere3f& rSphere);

    // 对象访问.
    const Segment3f& GetSegment(void) const;
    const Sphere3f& GetSphere(void) const;

    // test-intersection查询.
    virtual bool Test(void);

    // find-intersection查询.
    virtual bool Find(void);
    int GetCount(void) const;
    const Vector3f& GetPoint(int i) const;
    float GetSegmentT(int i) const;

    // dynamic test-intersection查询.
    virtual bool Test(float fTMax, const Vector3f& rVelocity0,
        const Vector3f& rVelocity1);

    // dynamic find-intersection查询.
    virtual bool Find(float fTMax, const Vector3f& rVelocity0,
        const Vector3f& rVelocity1);

    float ZeroThreshold;  // default = Math<float>::ZERO_TOLERANCE

private:
    // 待检查对象.
    const Segment3f* m_pSegment;
    const Sphere3f* m_pSphere;

    // 相交集相关信息.
    int m_iCount;
    Vector3f m_aPoint[2];
    float m_afSegmentT[2];
};

}

#endif
