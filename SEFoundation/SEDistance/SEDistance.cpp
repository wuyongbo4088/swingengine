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

#include "SEFoundationPCH.h"
#include "SEDistance.h"

namespace Swing
{
//----------------------------------------------------------------------------
template <class Real, class TVector>
Distance<Real, TVector>::Distance()
{
    MaximumIterations = 8;
    ZeroThreshold = Math<Real>::ZERO_TOLERANCE;
    SetDifferenceStep((Real)1e-03);

    m_fContactTime = Math<Real>::MAX_REAL;
    m_bHasMultipleClosestPoints0 = false;
    m_bHasMultipleClosestPoints1 = false;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
Distance<Real, TVector>::~Distance()
{
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
Real Distance<Real, TVector>::GetDifferenceStep() const
{
    return m_fDifferenceStep;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
Real Distance<Real, TVector>::GetContactTime() const
{
    return m_fContactTime;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
const TVector& Distance<Real, TVector>::GetClosestPoint0() const
{
    return m_ClosestPoint0;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
const TVector& Distance<Real, TVector>::GetClosestPoint1() const
{
    return m_ClosestPoint1;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
bool Distance<Real, TVector>::HasMultipleClosestPoints0() const
{
    return m_bHasMultipleClosestPoints0;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
bool Distance<Real, TVector>::HasMultipleClosestPoints1() const
{
    return m_bHasMultipleClosestPoints1;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
void Distance<Real, TVector>::SetDifferenceStep(Real fDifferenceStep)
{
    SE_ASSERT( fDifferenceStep > (Real)0.0 );

    if( fDifferenceStep > (Real)0.0 )
    {
        m_fDifferenceStep = fDifferenceStep;
    }
    else
    {
        m_fDifferenceStep = (Real)1e-03;
    }

    m_fInvTwoDifferenceStep = ((Real)0.5)/m_fDifferenceStep;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
Real Distance<Real, TVector>::GetDerivative(Real fT, 
    const TVector& rVelocity0, const TVector& rVelocity1)
{
    // �����޲�ֹ��㵼��: f'(t) = (f(t + h) - f(t - h))/(2 * h).
    Real fFp = Get(fT + m_fDifferenceStep, rVelocity0, rVelocity1);
    Real fFm = Get(fT - m_fDifferenceStep, rVelocity0, rVelocity1);
    Real fDFApprox = m_fInvTwoDifferenceStep*(fFp - fFm);

    return fDFApprox;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
Real Distance<Real, TVector>::GetDerivativeSquared(Real fT, 
    const TVector& rVelocity0, const TVector& rVelocity1)
{
    // ����������и���ķ����������ƽ���ĵ���,��Ӧ�����������.
    Real fDistance = Get(fT, rVelocity0, rVelocity1);
    Real fDerivative = GetDerivative(fT, rVelocity0, rVelocity1);

    return ((Real)2.0)*fDistance*fDerivative;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
Real Distance<Real, TVector>::Get(Real fTMin, Real fTMax, 
    const TVector& rVelocity0, const TVector& rVelocity1)
{
    // ������ں���f(t)��convex function��ǰ����.
    // ���f'(tmin) >= 0,����Сֵ��tmin��.
    // ���f'(tmax) <= 0,����Сֵ��tmax��.
    // ����,f'(tmin) < 0��f'(tmax) > 0,��Сֵ��(tmin,tmax)������.

    Real fT0 = fTMin;
    Real fF0 = Get(fT0, rVelocity0, rVelocity1);
    if( fF0 <= ZeroThreshold )
    {
        // ���뼸��Ϊ��.
        // �����ʼʱ�̽Ӵ�.
        m_fContactTime = fT0;

        return (Real)0.0;
    }
    Real fDF0 = GetDerivative(fT0, rVelocity0, rVelocity1);
    if( fDF0 >= (Real)0.0 )
    {
        // ������[tmin, tmax]�ϳ�������.
        m_fContactTime = fT0;

        return fF0;
    }

    Real fT1 = fTMax;
    Real fF1 = Get(fT1, rVelocity0, rVelocity1);
    if( fF1 <= ZeroThreshold )
    {
        // ���뼸��Ϊ��.
        m_fContactTime = fT1;

        return (Real)0.0;
    }
    Real fDF1 = GetDerivative(fT1, rVelocity0, rVelocity1);
    if( fDF1 <= (Real)0.0 )
    {
        // ������[tmin, tmax]�ϳ�����С.
        m_fContactTime = fT1;

        return fF1;
    }

    // ��ţ�ٷ����㺯��ֵΪ0ʱ���Ա���tֵ.
    // �ڸù�����,������벻�ܴﵽ0ֵ,���л���һ��numerical minimizer.
    int i;
    for( i = 0; i < MaximumIterations; i++ )
    {
        // ������һ��ţ�ٵ���λ��.
        Real fT = fT0 - fF0/fDF0;
        if( fT >= fTMax )
        {
            // ����ͼ�ε�͹��ȷ���������������ʱ,����������ֵ.
            // �л���һ��numerical minimizer.
            break;
        }

        Real fF = Get(fT, rVelocity0, rVelocity1);
        if( fF <= ZeroThreshold )
        {
            // ���뼸��Ϊ��.
            m_fContactTime = fT;

            return (Real)0.0;
        }

        Real fDF = GetDerivative(fT, rVelocity0, rVelocity1);
        if( fDF >= (Real)0.0 )
        {
            // ����ͼ�ε�͹��ȷ���������������ʱ,����������ֵ.
            // �л���һ��numerical minimizer.
            break;
        }

        fT0 = fT;
        fF0 = fF;
        fDF0 = fDF;
    }

    if( i == MaximumIterations )
    {
        // ��ָ�����������ں���û��������0ֵ����.
        // ��������ʱ,�����ĵ����ض���Ϊ��ֵ,
        // ��˰����һ�ε���ʱ��t�ĺ���ֵ��Ϊ����ֵ����.
        m_fContactTime = fT0;

        return fF0;
    }

    // ����������ֵ.
    // �ö��ַ����ҵ������ĸ�.
    Real fTm = fT0;
    for( i = 0; i < MaximumIterations; i++ )
    {
        fTm = ((Real)0.5)*(fT0 + fT1);
        Real fDFm = GetDerivative(fTm, rVelocity0, rVelocity1);
        Real fProduct = fDFm*fDF0;
        if( fProduct < -ZeroThreshold )
        {
            fT1 = fTm;
            fDF1 = fDFm;
        }
        else if( fProduct > ZeroThreshold )
        {
            fT0 = fTm;
            fDF0 = fDFm;
        }
        else
        {
            break;
        }
    }

    // ���Ǻ���������Сֵ��tֵ,������contact time.
    // �洢��ֵ����debugging.
    m_fContactTime = fTm;
    Real fFm = Get(fTm, rVelocity0, rVelocity1);

    return fFm;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
Real Distance<Real, TVector>::GetSquared(Real fTMin, Real fTMax, 
    const TVector& rVelocity0, const TVector& rVelocity1)
{
    // ������ں���f(t)��convex function��ǰ����.
    // ���f'(tmin) >= 0,����Сֵ��tmin��.
    // ���f'(tmax) <= 0,����Сֵ��tmax��.
    // ����,f'(tmin) < 0��f'(tmax) > 0,��Сֵ��(tmin,tmax)������.

    Real fT0 = fTMin;
    Real fF0 = GetSquared(fT0, rVelocity0, rVelocity1);
    if( fF0 <= ZeroThreshold )
    {
        // ���뼸��Ϊ��.
        // �����ʼʱ�̽Ӵ�.
        m_fContactTime = fT0;

        return (Real)0.0;
    }
    Real fDF0 = GetDerivativeSquared(fT0, rVelocity0, rVelocity1);
    if( fDF0 >= (Real)0.0 )
    {
        // ������[tmin, tmax]�ϳ�������.
        m_fContactTime = fT0;

        return fF0;
    }

    Real fT1 = fTMax;
    Real fF1 = GetSquared(fT1, rVelocity0, rVelocity1);
    if( fF1 <= ZeroThreshold )
    {
        // ���뼸��Ϊ��.
        m_fContactTime = fT1;

        return (Real)0.0;
    }
    Real fDF1 = GetDerivativeSquared(fT1, rVelocity0, rVelocity1);
    if( fDF1 <= (Real)0.0 )
    {
        // ������[tmin, tmax]�ϳ�����С.
        m_fContactTime = fT1;

        return fF1;
    }

    // ��ţ�ٷ����㺯��ֵΪ0ʱ���Ա���tֵ.
    // �ڸù�����,������벻�ܴﵽ0ֵ,���л���һ��numerical minimizer.
    int i;
    for( i = 0; i < MaximumIterations; i++ )
    {
        // ������һ��ţ�ٵ���λ��.
        Real fT = fT0 - fF0/fDF0;
        if( fT >= fTMax )
        {
            // ����ͼ�ε�͹��ȷ���������������ʱ,����������ֵ.
            // �л���һ��numerical minimizer.
            break;
        }

        Real fF = GetSquared(fT, rVelocity0, rVelocity1);
        if( fF <= ZeroThreshold )
        {
            // ���뼸��Ϊ��.
            m_fContactTime = fT;

            return (Real)0.0;
        }

        Real fDF = GetDerivativeSquared(fT, rVelocity0, rVelocity1);
        if( fDF >= (Real)0.0 )
        {
            // ����ͼ�ε�͹��ȷ���������������ʱ,����������ֵ.
            // �л���һ��numerical minimizer.
            break;
        }

        fT0 = fT;
        fF0 = fF;
        fDF0 = fDF;
    }

    if( i == MaximumIterations )
    {
        // ��ָ�����������ں���û��������0ֵ����.
        // ��������ʱ,�����ĵ����ض���Ϊ��ֵ,
        // ��˰����һ�ε���ʱ��t�ĺ���ֵ��Ϊ����ֵ����.
        m_fContactTime = fT0;

        return fF0;
    }

    // ����������ֵ.
    // �ö��ַ����ҵ������ĸ�.
    Real fTm = fT0;
    for( i = 0; i < MaximumIterations; i++ )
    {
        fTm = ((Real)0.5)*(fT0 + fT1);
        Real fDFm = GetDerivativeSquared(fTm, rVelocity0, rVelocity1);
        Real fProduct = fDFm*fDF0;
        if( fProduct < -ZeroThreshold )
        {
            fT1 = fTm;
            fDF1 = fDFm;
        }
        else if( fProduct > ZeroThreshold )
        {
            fT0 = fTm;
            fDF0 = fDFm;
        }
        else
        {
            break;
        }
    }

    // ���Ǻ���������Сֵ��tֵ,������contact time.
    // �洢��ֵ����debugging.
    m_fContactTime = fTm;
    Real fFm = GetSquared(fTm, rVelocity0, rVelocity1);

    return fFm;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// explicit instantiation
//----------------------------------------------------------------------------
template class Distance<float, Vector2f>;
template class Distance<float, Vector3f>;

//template class Distance<double, Vector2d>;
//template class Distance<double, Vector3d>;
//----------------------------------------------------------------------------
}
