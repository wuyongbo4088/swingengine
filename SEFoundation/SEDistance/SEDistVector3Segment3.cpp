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
#include "SEDistVector3Segment3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistVector3Segment3f::SEDistVector3Segment3f(const SEVector3f& rVector,
    const SESegment3f& rSegment)
    :
    m_pVector(&rVector),
    m_pSegment(&rSegment)
{
}
//----------------------------------------------------------------------------
const SEVector3f& SEDistVector3Segment3f::GetVector() const
{
    return *m_pVector;
}
//----------------------------------------------------------------------------
const SESegment3f& SEDistVector3Segment3f::GetSegment() const
{
    return *m_pSegment;
}
//----------------------------------------------------------------------------
float SEDistVector3Segment3f::Get()
{
    float fSqrDist = GetSquared();

    return SEMath<float>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistVector3Segment3f::GetSquared()
{
    SEVector3f vec3fDiff = *m_pVector - m_pSegment->Origin;
    m_fSegmentParameter = m_pSegment->Direction.Dot(vec3fDiff);

    if( -m_pSegment->Extent < m_fSegmentParameter )
    {
        if( m_fSegmentParameter < m_pSegment->Extent )
        {
            m_ClosestPoint1 = m_pSegment->Origin +
                m_fSegmentParameter*m_pSegment->Direction;
        }
        else
        {
            m_ClosestPoint1 = m_pSegment->Origin +
                m_pSegment->Extent*m_pSegment->Direction;
        }
    }
    else
    {
        m_ClosestPoint1 = m_pSegment->Origin -
            m_pSegment->Extent*m_pSegment->Direction;
    }

    m_ClosestPoint0 = *m_pVector;
    vec3fDiff = m_ClosestPoint1 - m_ClosestPoint0;

    return vec3fDiff.GetSquaredLength();
}
//----------------------------------------------------------------------------
float SEDistVector3Segment3f::Get(float fT, const SEVector3f& rVelocity0, 
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMVector = *m_pVector + fT*rVelocity0;
    SEVector3f vec3fMOrigin = m_pSegment->Origin + fT*rVelocity1;
    SESegment3f tempMSegment(vec3fMOrigin, m_pSegment->Direction,
        m_pSegment->Extent);

    return SEDistVector3Segment3f(vec3fMVector, tempMSegment).Get();
}
//----------------------------------------------------------------------------
float SEDistVector3Segment3f::GetSquared(float fT, const SEVector3f& 
    rVelocity0, const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMVector = *m_pVector + fT*rVelocity0;
    SEVector3f vec3fMOrigin = m_pSegment->Origin + fT*rVelocity1;
    SESegment3f tempMSegment(vec3fMOrigin, m_pSegment->Direction,
        m_pSegment->Extent);

    return SEDistVector3Segment3f(vec3fMVector, tempMSegment).GetSquared();
}
//----------------------------------------------------------------------------
float SEDistVector3Segment3f::GetSegmentParameter() const
{
    return m_fSegmentParameter;
}
//----------------------------------------------------------------------------
