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
#include "SEQuery2TInteger.h"

using namespace Swing;

//----------------------------------------------------------------------------
Query2TIntegerf::Query2TIntegerf(int iVCount, const Vector2f* aVertex)
    :
    Query2f(iVCount, aVertex)
{
}
//----------------------------------------------------------------------------
Query::Type Query2TIntegerf::GetType() const
{
    return Query::QT_INTEGER;
}
//----------------------------------------------------------------------------
int Query2TIntegerf::ToLine(const Vector2f& rP, int iV0, int iV1) const
{
    const Vector2f& rV0 = m_aVertex[iV0];
    const Vector2f& rV1 = m_aVertex[iV1];

    TInteger<2> tempX0((int)rP[0] - (int)rV0[0]);
    TInteger<2> tempY0((int)rP[1] - (int)rV0[1]);
    TInteger<2> tempX1((int)rV1[0] - (int)rV0[0]);
    TInteger<2> tempY1((int)rV1[1] - (int)rV0[1]);

    TInteger<2> tempDet2 = Det2(tempX0,tempY0,tempX1, tempY1);

    return (tempDet2 > 0 ? +1 : (tempDet2 < 0 ? -1 : 0));
}
//----------------------------------------------------------------------------
int Query2TIntegerf::ToCircumcircle(const Vector2f& rP, int iV0,
    int iV1, int iV2) const
{
    const Vector2f& rV0 = m_aVertex[iV0];
    const Vector2f& rV1 = m_aVertex[iV1];
    const Vector2f& rV2 = m_aVertex[iV2];

    int aiP[2] = { (int)rP[0], (int)rP[1] };
    int aiV0[2] = { (int)rV0[0], (int)rV0[1] };
    int aiV1[2] = { (int)rV1[0], (int)rV1[1] };
    int aiV2[2] = { (int)rV2[0], (int)rV2[1] };

    TInteger<4> tempS0x(aiV0[0] + aiP[0]);
    TInteger<4> tempD0x(aiV0[0] - aiP[0]);
    TInteger<4> tempS0y(aiV0[1] + aiP[1]);
    TInteger<4> tempD0y(aiV0[1] - aiP[1]);
    TInteger<4> tempS1x(aiV1[0] + aiP[0]);
    TInteger<4> tempD1x(aiV1[0] - aiP[0]);
    TInteger<4> tempS1y(aiV1[1] + aiP[1]);
    TInteger<4> tempD1y(aiV1[1] - aiP[1]);
    TInteger<4> tempS2x(aiV2[0] + aiP[0]);
    TInteger<4> tempD2x(aiV2[0] - aiP[0]);
    TInteger<4> tempS2y(aiV2[1] + aiP[1]);
    TInteger<4> tempD2y(aiV2[1] - aiP[1]);
    TInteger<4> tempZ0 = tempS0x*tempD0x + tempS0y*tempD0y;
    TInteger<4> tempZ1 = tempS1x*tempD1x + tempS1y*tempD1y;
    TInteger<4> tempZ2 = tempS2x*tempD2x + tempS2y*tempD2y;
    TInteger<4> tempDet3 = Det3(tempD0x, tempD0y, tempZ0, 
        tempD1x, tempD1y, tempZ1, tempD2x, tempD2y, tempZ2);

    return (tempDet3 < 0 ? 1 : (tempDet3 > 0 ? -1 : 0));
}
//----------------------------------------------------------------------------
TInteger<2> Query2TIntegerf::Dot(TInteger<2>& rX0, TInteger<2>& rY0,
    TInteger<2>& rX1, TInteger<2>& rY1)
{
    return rX0*rX1 + rY0*rY1;
}
//----------------------------------------------------------------------------
TInteger<2> Query2TIntegerf::Det2(TInteger<2>& rX0, TInteger<2>& rY0,
    TInteger<2>& rX1, TInteger<2>& rY1)
{
    return rX0*rY1 - rX1*rY0;
}
//----------------------------------------------------------------------------
TInteger<4> Query2TIntegerf::Det3(TInteger<4>& rX0, TInteger<4>& rY0,
    TInteger<4>& rZ0, TInteger<4>& rX1, TInteger<4>& rY1,
    TInteger<4>& rZ1, TInteger<4>& rX2, TInteger<4>& rY2,
    TInteger<4>& rZ2)
{
    TInteger<4> tempC00 = rY1*rZ2 - rY2*rZ1;
    TInteger<4> tempC01 = rY2*rZ0 - rY0*rZ2;
    TInteger<4> tempC02 = rY0*rZ1 - rY1*rZ0;

    return rX0*tempC00 + rX1*tempC01 + rX2*tempC02;
}
//----------------------------------------------------------------------------