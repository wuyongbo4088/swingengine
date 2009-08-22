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

#ifndef Swing_Intersector_H
#define Swing_Intersector_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SELinComp.h"
#include "SEVector2.h"
#include "SEVector3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// 名称:Intersector虚基类
// 说明:
// 作者:Sun Che
// 时间:20081219
//----------------------------------------------------------------------------
template <class Real, class TVector>
class SE_FOUNDATION_API Intersector
{
public:
    // 虚基类
    virtual ~Intersector(void);

    // 静态相交查询.默认实现返回false.
    // Find查询返回一个相交集.派生类有责任提供访问该集合的功能,
    // 因为该集合的性质由具体的对象类型所决定.
    virtual bool Test(void);
    virtual bool Find(void);

    // 动态相交查询.默认实现返回false.
    // Find查询返回一个first contact set.派生类有责任提供访问该集合的功能,
    // 因为该集合的性质由具体的对象类型所决定.
    virtual bool Test(Real fTMax, const TVector& rVelocity0,
        const TVector& rVelocity1);
    virtual bool Find(Real fTMax, const TVector& rVelocity0,
        const TVector& rVelocity1);

    // 动态相交查询时,两个对象第一次接触的时间.
    Real GetContactTime(void) const;

    // 相交集的信息.
    enum
    {
        IT_EMPTY = LinComp<Real>::CT_EMPTY,
        IT_POINT = LinComp<Real>::CT_POINT,
        IT_SEGMENT = LinComp<Real>::CT_SEGMENT,
        IT_RAY = LinComp<Real>::CT_RAY,
        IT_LINE = LinComp<Real>::CT_LINE,
        IT_POLYGON,
        IT_PLANE,
        IT_POLYHEDRON,
        IT_OTHER
    };
    int GetIntersectionType(void) const;

protected:
    Intersector(void);

    Real m_fContactTime;
    int m_iIntersectionType;
};

typedef Intersector<float, Vector2f> Intersector2f;
typedef Intersector<float, Vector3f> Intersector3f;
//typedef Intersector<double, Vector2d> Intersector2d;
//typedef Intersector<double, Vector3d> Intersector3d;

}

#endif
