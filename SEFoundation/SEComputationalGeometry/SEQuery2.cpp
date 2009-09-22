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
#include "SEQuery2.h"

using namespace Swing;

//----------------------------------------------------------------------------
Query2f::Query2f(int iVCount, const Vector2f* aVertex)
{
    SE_ASSERT( iVCount > 0 && aVertex );

    m_iVCount = iVCount;
    m_aVertex = aVertex;
}
//----------------------------------------------------------------------------
Query2f::~Query2f()
{
}
//----------------------------------------------------------------------------
Query::Type Query2f::GetType() const
{
    return Query::QT_REAL;
}
//----------------------------------------------------------------------------
int Query2f::GetCount() const
{
    return m_iVCount;
}
//----------------------------------------------------------------------------
const Vector2f* Query2f::GetVertices() const
{
    return m_aVertex;
}
//----------------------------------------------------------------------------
int Query2f::ToLine(int i, int iV0, int iV1) const
{
    return ToLine(m_aVertex[i], iV0, iV1);
}
//----------------------------------------------------------------------------
int Query2f::ToLine(const Vector2f& rP, int iV0, int iV1) const
{
    const Vector2f& rV0 = m_aVertex[iV0];
    const Vector2f& rV1 = m_aVertex[iV1];

    float fX0 = rP[0] - rV0[0];
    float fY0 = rP[1] - rV0[1];
    float fX1 = rV1[0] - rV0[0];
    float fY1 = rV1[1] - rV0[1];

    float fDet2 = Det2(fX0, fY0, fX1, fY1);

    return (fDet2 > 0.0f ? +1 : (fDet2 < 0.0f ? -1 : 0));
}
//----------------------------------------------------------------------------
int Query2f::ToTriangle(int i, int iV0, int iV1, int iV2) const
{
    return ToTriangle(m_aVertex[i], iV0, iV1, iV2);
}
//----------------------------------------------------------------------------
int Query2f::ToTriangle(const Vector2f& rP, int iV0, int iV1, int iV2) const
{
    int iSign0 = ToLine(rP, iV1, iV2);
    if( iSign0 > 0 )
    {
        return +1;
    }

    int iSign1 = ToLine(rP, iV0, iV2);
    if( iSign1 < 0 )
    {
        return +1;
    }

    int iSign2 = ToLine(rP, iV0, iV1);
    if( iSign2 > 0 )
    {
        return +1;
    }

    return ((iSign0 && iSign1 && iSign2) ? -1 : 0);
}
//----------------------------------------------------------------------------
int Query2f::ToCircumcircle(int i, int iV0, int iV1, int iV2) const
{
    return ToCircumcircle(m_aVertex[i], iV0, iV1, iV2);
}
//----------------------------------------------------------------------------
int Query2f::ToCircumcircle(const Vector2f& rP, int iV0, int iV1, int iV2) 
    const
{
    const Vector2f& rV0 = m_aVertex[iV0];
    const Vector2f& rV1 = m_aVertex[iV1];
    const Vector2f& rV2 = m_aVertex[iV2];

    float fS0x = rV0[0] + rP[0];
    float fD0x = rV0[0] - rP[0];
    float fS0y = rV0[1] + rP[1];
    float fD0y = rV0[1] - rP[1];
    float fS1x = rV1[0] + rP[0];
    float fD1x = rV1[0] - rP[0];
    float fS1y = rV1[1] + rP[1];
    float fD1y = rV1[1] - rP[1];
    float fS2x = rV2[0] + rP[0];
    float fD2x = rV2[0] - rP[0];
    float fS2y = rV2[1] + rP[1];
    float fD2y = rV2[1] - rP[1];
    float fZ0 = fS0x*fD0x + fS0y*fD0y;
    float fZ1 = fS1x*fD1x + fS1y*fD1y;
    float fZ2 = fS2x*fD2x + fS2y*fD2y;
    float fDet3 = Det3(fD0x, fD0y, fZ0, fD1x, fD1y, fZ1, fD2x, fD2y, fZ2);

    return (fDet3 < 0.0f ? 1 : (fDet3 > 0.0f ? -1 : 0));
}
//----------------------------------------------------------------------------
float Query2f::Dot(float fX0, float fY0, float fX1, float fY1)
{
    return fX0*fX1 + fY0*fY1;
}
//----------------------------------------------------------------------------
float Query2f::Det2(float fX0, float fY0, float fX1, float fY1)
{
    return fX0*fY1 - fX1*fY0;
}
//----------------------------------------------------------------------------
float Query2f::Det3(float fX0, float fY0, float fZ0, float fX1, float fY1,
    float fZ1, float fX2, float fY2, float fZ2)
{
    float fC00 = fY1*fZ2 - fY2*fZ1;
    float fC01 = fY2*fZ0 - fY0*fZ2;
    float fC02 = fY0*fZ1 - fY1*fZ0;

    return fX0*fC00 + fX1*fC01 + fX2*fC02;
}
//----------------------------------------------------------------------------