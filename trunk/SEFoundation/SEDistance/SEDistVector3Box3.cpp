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
#include "SEDistVector3Box3.h"

using namespace Swing;

//----------------------------------------------------------------------------
DistVector3Box3f::DistVector3Box3f(const Vector3f& rVector, const Box3f& rBox)
    :
    m_pVector(&rVector),
    m_pBox(&rBox)
{
}
//----------------------------------------------------------------------------
const Vector3f& DistVector3Box3f::GetVector() const
{
    return *m_pVector;
}
//----------------------------------------------------------------------------
const Box3f& DistVector3Box3f::GetBox() const
{
    return *m_pBox;
}
//----------------------------------------------------------------------------
float DistVector3Box3f::Get()
{
    return Math<float>::Sqrt(GetSquared());
}
//----------------------------------------------------------------------------
float DistVector3Box3f::GetSquared()
{
    // ��box��������ϵ�¼���.
    Vector3f vec3fDiff = *m_pVector - m_pBox->Center;

    // ����squared distance��box�ϵ������.
    float fSqrDistance = 0.0f, fDelta;
    Vector3f vec3fClosest;
    int i;
    for( i = 0; i < 3; i++ )
    {
        vec3fClosest[i] = vec3fDiff.Dot(m_pBox->Axis[i]);
        if( vec3fClosest[i] < -m_pBox->Extent[i] )
        {
            fDelta = vec3fClosest[i] + m_pBox->Extent[i];
            fSqrDistance += fDelta*fDelta;
            vec3fClosest[i] = -m_pBox->Extent[i];
        }
        else if( vec3fClosest[i] > m_pBox->Extent[i] )
        {
            fDelta = vec3fClosest[i] - m_pBox->Extent[i];
            fSqrDistance += fDelta*fDelta;
            vec3fClosest[i] = m_pBox->Extent[i];
        }
    }

    m_ClosestPoint0 = *m_pVector;
    m_ClosestPoint1 = m_pBox->Center;
    for( i = 0; i < 3; i++ )
    {
        m_ClosestPoint1 += vec3fClosest[i]*m_pBox->Axis[i];
    }

    return fSqrDistance;
}
//----------------------------------------------------------------------------
float DistVector3Box3f::Get(float fT, const Vector3f& rVelocity0,
    const Vector3f& rVelocity1)
{
    Vector3f vec3fMVector = *m_pVector + fT*rVelocity0;
    Vector3f vec3fMCenter = m_pBox->Center + fT*rVelocity1;
    Box3f tempMBox(vec3fMCenter, m_pBox->Axis,m_pBox->Extent);

    return DistVector3Box3f(vec3fMVector, tempMBox).Get();
}
//----------------------------------------------------------------------------
float DistVector3Box3f::GetSquared(float fT, const Vector3f& rVelocity0, 
    const Vector3f& rVelocity1)
{
    Vector3f vec3fMVector = *m_pVector + fT*rVelocity0;
    Vector3f vec3fMCenter = m_pBox->Center + fT*rVelocity1;
    Box3f tempMBox(vec3fMCenter, m_pBox->Axis,m_pBox->Extent);

    return DistVector3Box3f(vec3fMVector, tempMBox).GetSquared();
}
//----------------------------------------------------------------------------