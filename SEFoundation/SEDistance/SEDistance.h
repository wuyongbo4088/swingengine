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

#ifndef Swing_Distance_H
#define Swing_Distance_H

#include "SEFoundationLIB.h"
#include "SEVector2.h"
#include "SEVector3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:Distance�����
// ˵��:
// ����:Sun Che
// ʱ��:20090104
//----------------------------------------------------------------------------
template <class Real, class TVector>
class SE_FOUNDATION_API Distance
{
public:
    // �����.
    virtual ~Distance(void);

    // static distance��ѯ.
    virtual Real Get(void) = 0;     // distance
    virtual Real GetSquared(void) = 0;  // squared distance

    // ����dynamic distance��ѯ��convex function����.
    virtual Real Get(Real fT, const TVector& rVelocity0,
        const TVector& rVelocity1) = 0;
    virtual Real GetSquared(Real fT, const TVector& rVelocity0,
        const TVector& rVelocity1) = 0;

    // ����dynamic distance��ѯ�ĵ�������.
    // Ĭ��ʹ�����޲�ֹ��㵼��.
    // f'(t) = (f(t+h)-f(t-h))/(2*h).
    // ����h = DifferenceStep.  ���������������Щ����,
    // �Ӷ�ʵ��һ������Ҫh�ľ�ȷ�������㷽��.
    virtual Real GetDerivative(Real fT, const TVector& rVelocity0,
        const TVector& rVelocity1);
    // g(t) = f(t)^2, g'(t) = 2*f(t)*f'(t).
    virtual Real GetDerivativeSquared(Real fT, const TVector& rVelocity0,
        const TVector& rVelocity1);

    // Dynamic distance��ѯ.
    // ����������ڸ���ʱ������[tmin,tmax]��,����������С����.
    virtual Real Get(Real fTMin, Real fTMax, const TVector& rVelocity0,
        const TVector& rVelocity1);
    virtual Real GetSquared(Real fTMin, Real fTMax,
        const TVector& rVelocity0, const TVector& rVelocity1);

    // ����Newton's method��inverse parabolic interpolation.
    int MaximumIterations;  // default = 8
    Real ZeroThreshold;     // default = SEMath<Real>::ZERO_TOLERANCE

    // ���ڵ�������.
    void SetDifferenceStep(Real fDifferenceStep);  // default = 1e-03
    Real GetDifferenceStep(void) const;

    // dynamic distance��ѯʱ,�ﵽ��С�����ʱ��.
    Real GetContactTime(void) const;

    // �������ϵ������.����static��dynmaic��ѯ����Ч.
    // һ�������ϵ�����������һ������,��ʱ,������Ա��������Ϊtrue.
    // ���������ʵ��֧����������㼯�ϵĲ�ѯ.
    const TVector& GetClosestPoint0(void) const;
    const TVector& GetClosestPoint1(void) const;
    bool HasMultipleClosestPoints0(void) const;
    bool HasMultipleClosestPoints1(void) const;

protected:
    Distance(void);

    Real m_fContactTime;
    TVector m_ClosestPoint0;
    TVector m_ClosestPoint1;
    bool m_bHasMultipleClosestPoints0;
    bool m_bHasMultipleClosestPoints1;
    Real m_fDifferenceStep, m_fInvTwoDifferenceStep;
};

typedef Distance<float, SEVector2f> Distance2f;
typedef Distance<float, SEVector3f> Distance3f;
//typedef Distance<double, SEVector2d> Distance2d;
//typedef Distance<double, SEVector3d> Distance3d;

}

#endif
