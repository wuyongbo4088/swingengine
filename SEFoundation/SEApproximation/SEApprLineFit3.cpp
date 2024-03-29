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
#include "SEApprLineFit3.h"
#include "SEEigen.h"

using namespace Swing;

//----------------------------------------------------------------------------
SELine3f Swing::OrthogonalLineFit3f(int iCount, const SEVector3f* aPoint)
{
    SELine3f tempLine(SEVector3f::ZERO, SEVector3f::ZERO);

    // compute the mean of the points
    tempLine.Origin = aPoint[0];
    int i;
    for( i = 1; i < iCount; i++ )
    {
        tempLine.Origin += aPoint[i];
    }
    float fInvCount = 1.0f / iCount;
    tempLine.Origin *= fInvCount;

    // compute the covariance matrix of the points
    float fSumXX = 0.0f, fSumXY = 0.0f, fSumXZ = 0.0f;
    float fSumYY = 0.0f, fSumYZ = 0.0f, fSumZZ = 0.0f;
    for( i = 0; i < iCount; i++ ) 
    {
        SEVector3f vec3fDiff = aPoint[i] - tempLine.Origin;
        fSumXX += vec3fDiff.X * vec3fDiff.X;
        fSumXY += vec3fDiff.X * vec3fDiff.Y;
        fSumXZ += vec3fDiff.X * vec3fDiff.Z;
        fSumYY += vec3fDiff.Y * vec3fDiff.Y;
        fSumYZ += vec3fDiff.Y * vec3fDiff.Z;
        fSumZZ += vec3fDiff.Z * vec3fDiff.Z;
    }

    fSumXX *= fInvCount;
    fSumXY *= fInvCount;
    fSumXZ *= fInvCount;
    fSumYY *= fInvCount;
    fSumYZ *= fInvCount;
    fSumZZ *= fInvCount;

    // set up the eigensolver
    SEEigen tempES(3);
    tempES(0,0) = fSumYY + fSumZZ;
    tempES(0,1) = -fSumXY;
    tempES(0,2) = -fSumXZ;
    tempES(1,0) = tempES(0, 1);
    tempES(1,1) = fSumXX + fSumZZ;
    tempES(1,2) = -fSumYZ;
    tempES(2,0) = tempES(0, 2);
    tempES(2,1) = tempES(1, 2);
    tempES(2,2) = fSumXX + fSumYY;

    // compute eigenstuff, smallest eigenvalue is in last position
    tempES.DecrSortEigenStuff3();

    // unit-length direction for best-fit line
    tempES.GetEigenvector(2, tempLine.Direction);

    return tempLine;
}
//----------------------------------------------------------------------------
