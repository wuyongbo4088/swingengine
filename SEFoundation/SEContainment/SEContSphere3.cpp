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
#include "SEContSphere3.h"

using namespace Swing;

//----------------------------------------------------------------------------
Sphere3f Swing::ContSphereOfAABBf(int iCount, const Vector3f* aPoint)
{
    Vector3f vec3fMinPoint, vec3fMaxPoint;
    Vector3f::ComputeExtremes(iCount, aPoint, vec3fMinPoint, vec3fMaxPoint);

    Sphere3f tempSphere;
    tempSphere.Center = 0.5f * (vec3fMaxPoint + vec3fMinPoint);
    Vector3f vec3fHalfDiagonal = 0.5f * (vec3fMaxPoint - vec3fMinPoint);
    tempSphere.Radius = vec3fHalfDiagonal.GetLength();
    
    return tempSphere;
}
//----------------------------------------------------------------------------
Sphere3f Swing::ContSphereAveragef(int iCount, const Vector3f* aPoint)
{
    Sphere3f tempSphere;

    tempSphere.Center = aPoint[0];
	tempSphere.Radius = 0.0f;

    int i;
    for( i = 1; i < iCount; i++ )
    {
        tempSphere.Center += aPoint[i];
    }
    tempSphere.Center /= (float)iCount;

    for( i = 0; i < iCount; i++ )
    {
        Vector3f vec3fDiff = aPoint[i] - tempSphere.Center;
        float fRadiusSqr = vec3fDiff.GetSquaredLength();

        if( fRadiusSqr > tempSphere.Radius )
        {
            tempSphere.Radius = fRadiusSqr;
        }
    }

    tempSphere.Radius = Math<float>::Sqrt(tempSphere.Radius);

    return tempSphere;
}
//----------------------------------------------------------------------------
bool Swing::IsInSpheref(const Vector3f& rPoint, const Sphere3f& rSphere)
{
    Vector3f vec3fDiff = rPoint - rSphere.Center;

    return vec3fDiff.GetSquaredLength() <= rSphere.Radius * rSphere.Radius;
}
//----------------------------------------------------------------------------
Sphere3f Swing::MergeSpheresf(const Sphere3f& rSphere0, 
    const Sphere3f& rSphere1)
{
    Vector3f vec3fCenterDiff = rSphere1.Center - rSphere0.Center;
    float fLSqr = vec3fCenterDiff.GetSquaredLength();
    float fRadiusDiff = rSphere1.Radius - rSphere0.Radius;
    float fRadiusDiffSqr = fRadiusDiff * fRadiusDiff;

    if( fRadiusDiffSqr >= fLSqr )
    {
        return ( fRadiusDiff >= 0.0f ? rSphere1 : rSphere0 );
    }

    float fLength = Math<float>::Sqrt(fLSqr);
    Sphere3f tempSphere;

    if( fLength > Math<float>::ZERO_TOLERANCE )
    {
        float fCoeff = (fLength + fRadiusDiff) / (2.0f * fLength);
        tempSphere.Center = rSphere0.Center + fCoeff*vec3fCenterDiff;
    }
    else
    {
        tempSphere.Center = rSphere0.Center;
    }

    tempSphere.Radius = 0.5f * (fLength + rSphere0.Radius + rSphere1.Radius);

    return tempSphere;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// MinSphere3f
//
// ������С����ڲ����㺯������Sphere3f�������ݴ�ƽ���뾶,ֻ��������ʱ�ſ���.
//----------------------------------------------------------------------------
MinSphere3f::MinSphere3f(int iCount, const Vector3f* aPoint, 
    Sphere3f& rMinimal, float fEpsilon)
{
    m_fEpsilon = fEpsilon;
    m_aoUpdate[0] = 0;
    m_aoUpdate[1] = &MinSphere3f::UpdateSupport1;
    m_aoUpdate[2] = &MinSphere3f::UpdateSupport2;
    m_aoUpdate[3] = &MinSphere3f::UpdateSupport3;
    m_aoUpdate[4] = &MinSphere3f::UpdateSupport4;

    Support tempSupp;
    float fDistDiff;

    if( iCount >= 1 )
    {
        // ������ͬ�����붥����¶��㼯ָ������.
        Vector3f** apPermute = new Vector3f*[iCount];
        int i;
        for( i = 0; i < iCount; i++ )
        {
            apPermute[i] = (Vector3f*)&aPoint[i];
        }

        // ����������ж���˳��.
        for( i = iCount - 1; i > 0; i-- )
        {
            int j = rand() % (i + 1);
            if( j != i )
            {
                Vector3f* pTemp = apPermute[i];
                apPermute[i] = apPermute[j];
                apPermute[j] = pTemp;
            }
        }

        rMinimal = ExactSphere1(*apPermute[0]);
		tempSupp.Count = 1;
        tempSupp.Index[0] = 0;
        i = 1;
        while( i < iCount )
        {
            if( !tempSupp.Contains(i, apPermute, m_fEpsilon) )
            {
                // �����ǰsupport���ϲ�������ǰ����i.
 
                if( !Contains(*apPermute[i], rMinimal, fDistDiff) )
                {
                    // �����ǰ��С�򲻰�����ǰ����i.

					UpdateFunction oUpdate = m_aoUpdate[tempSupp.Count];
                    // �����µĵ�ǰ��С��,
                    // ʹ���ܹ�ͬʱ������ǰsupport���Ϻ͵�ǰ����i.
                    Sphere3f tempSphere =(this->*oUpdate)(i, apPermute, tempSupp);
                    if( tempSphere.Radius > rMinimal.Radius )
                    {
                        rMinimal = tempSphere;
                        i = 0;

                        continue;
                    }
                }
            }
            i++;
        }

        delete[] apPermute;
    }
    else
    {
        SE_ASSERT( false );
    }

    rMinimal.Radius = Math<float>::Sqrt(rMinimal.Radius);
}
//----------------------------------------------------------------------------
bool MinSphere3f::Contains(const Vector3f& rPoint, const Sphere3f& rSphere, 
    float& rfDistDiff)
{
    Vector3f vec3fDiff = rPoint - rSphere.Center;
    float fTest = vec3fDiff.GetSquaredLength();

    // �����sphere�뾶��ƽ���뾶.
    rfDistDiff = fTest - rSphere.Radius;

    return rfDistDiff <= 0.0f;
}
//----------------------------------------------------------------------------
Sphere3f MinSphere3f::ExactSphere1(const Vector3f& rPoint)
{
    Sphere3f tempMinimal;
    tempMinimal.Center = rPoint;
    tempMinimal.Radius = 0.0f;
    
    return tempMinimal;
}
//----------------------------------------------------------------------------
Sphere3f MinSphere3f::ExactSphere2(const Vector3f& rPoint0, 
    const Vector3f& rPoint1)
{
    Sphere3f tempMinimal;
    tempMinimal.Center = 0.5f * (rPoint0 + rPoint1);
    Vector3f vec3fDiff = rPoint1 - rPoint0;
    tempMinimal.Radius = 0.25f * vec3fDiff.GetSquaredLength();
    
    return tempMinimal;
}
//----------------------------------------------------------------------------
Sphere3f MinSphere3f::ExactSphere3(const Vector3f& rPoint0, 
    const Vector3f& rPoint1, const Vector3f& rPoint2)
{
    // ���㾭����������p0,p1,p2����,����C���������������,
    // ��K = u0*p0 + u1*p1 + u2*p2,����u0 + u1 + u2 = 1,
    // ��|K - p0| = |K - p1| = |K - p2| = �뾶r
    // �ɴ˵õ�:
    //   K - p0 = (u0*A + u1*B) - A
    //   K - p1 = (u0*A + u1*B) - B
    //   K - p2 = (u0*A + u1*B)
    // ���� A = p0 - p2, B = p1 - p2,
    // �ɴ˵õ�:
    //   r^2 = |u0*A + u1*B|^2 - 2*Dot(A, u0*A + u1*B) + |A|^2
    //   r^2 = |u0*A + u1*B|^2 - 2*Dot(B, u0*A + u1*B) + |B|^2
    //   r^2 = |u0*A + u1*B|^2
    // ��ǰ��ʽ��ȥ���ʽ,�õ����Է�����:
    // +-                 -++   -+       +-        -+
    // | Dot(A,A) Dot(A,B) || u0 | = 0.5 | Dot(A,A) |
    // | Dot(B,A) Dot(B,B) || u1 |       | Dot(B,B) |
    // +-                 -++   -+       +-        -+
    //
    // ���´��������Է�����,�õ�u0,u1,Ȼ��õ�r,
    // ע�����Է������޽�����.

    Vector3f vec3fA = rPoint0 - rPoint2;
    Vector3f vec3fB = rPoint1 - rPoint2;
    float fAdA = vec3fA.Dot(vec3fA);
    float fAdB = vec3fA.Dot(vec3fB);
    float fBdB = vec3fB.Dot(vec3fB);
    float fDet = fAdA*fBdB - fAdB*fAdB;

    Sphere3f tempMinimal;

    if( Math<float>::FAbs(fDet) > m_fEpsilon )
    {
        float fHalfInvDet = 0.5f / fDet;
        float fU0 = fHalfInvDet * fBdB * (fAdA - fAdB);
        float fU1 = fHalfInvDet * fAdA * (fBdB - fAdB);
        float fU2 = 1.0f - fU0 - fU1;
        
        tempMinimal.Center = fU0*rPoint0 + fU1*rPoint1 + fU2*rPoint2;
        Vector3f vec3fTemp = fU0*vec3fA + fU1*vec3fB;
        tempMinimal.Radius = vec3fTemp.GetSquaredLength();
    }
    else
    {
        tempMinimal.Center = Vector3f::ZERO;
        tempMinimal.Radius = Math<float>::MAX_REAL;
    }

    return tempMinimal;
}
//----------------------------------------------------------------------------
Sphere3f MinSphere3f::ExactSphere4(const Vector3f& rPoint0, 
    const Vector3f& rPoint1, const Vector3f& rPoint2, const Vector3f& rPoint3)
{
    // ���㾭���ĸ�����p0,p1,p2,p3����,����C���ĸ����������,
    // ��K = u0*p0 + u1*p1 + u2*p2 + u3*p3,����u0 + u1 + u2 + u3 = 1,
    // ��|K - p0| = |K - p1| = |K - p2| = |K - p3| = �뾶r
    // �ɴ˵õ�:
    //   K - p0 = u0*A + u1*B + u2*C - A
    //   K - p1 = u0*A + u1*B + u2*C - B
    //   K - p2 = u0*A + u1*B + u2*C - C
    //   K - p3 = u0*A + u1*B + u2*C
    // ���� A = p0 - p3, B = p1 - p3, C = p2 - p3
    // �ɴ˵õ�:
    //   r^2 = |u0*A + u1*B+u2*C|^2 - 2*Dot(A, u0*A + u1*B+u2*C) + |A|^2
    //   r^2 = |u0*A + u1*B+u2*C|^2 - 2*Dot(B, u0*A + u1*B+u2*C) + |B|^2
    //   r^2 = |u0*A + u1*B+u2*C|^2 - 2*Dot(C, u0*A + u1*B+u2*C) + |C|^2
    //   r^2 = |u0*A + u1*B+u2*C|^2
    // ��ǰ��ʽ��ȥ���ʽ,�õ����Է�����
    //
    // +-                          -++   -+       +-        -+
    // | Dot(A,A) Dot(A,B) Dot(A,C) || u0 | = 0.5 | Dot(A,A) |
    // | Dot(B,A) Dot(B,B) Dot(B,C) || u1 |       | Dot(B,B) |
    // | Dot(C,A) Dot(C,B) Dot(C,C) || u2 |       | Dot(C,C) |
    // +-                          -++   -+       +-        -+
    //
    // ���´��������Է�����,�õ�u0,u1,u2Ȼ��õ�r,
    // ע�����Է������޽�����.

    Vector3f vec3fE10 = rPoint0 - rPoint3;
    Vector3f vec3fE20 = rPoint1 - rPoint3;
    Vector3f vec3fE30 = rPoint2 - rPoint3;

    float aafA[3][3];
    aafA[0][0] = vec3fE10.Dot(vec3fE10);
    aafA[0][1] = vec3fE10.Dot(vec3fE20);
    aafA[0][2] = vec3fE10.Dot(vec3fE30);
    aafA[1][0] = aafA[0][1];
    aafA[1][1] = vec3fE20.Dot(vec3fE20);
    aafA[1][2] = vec3fE20.Dot(vec3fE30);
    aafA[2][0] = aafA[0][2];
    aafA[2][1] = aafA[1][2];
    aafA[2][2] = vec3fE30.Dot(vec3fE30);

    float afB[3];
    afB[0] = 0.5f * aafA[0][0];
    afB[1] = 0.5f * aafA[1][1];
    afB[2] = 0.5f * aafA[2][2];

    float aafAInv[3][3];
    aafAInv[0][0] = aafA[1][1]*aafA[2][2] - aafA[1][2]*aafA[2][1];
    aafAInv[0][1] = aafA[0][2]*aafA[2][1] - aafA[0][1]*aafA[2][2];
    aafAInv[0][2] = aafA[0][1]*aafA[1][2] - aafA[0][2]*aafA[1][1];
    aafAInv[1][0] = aafA[1][2]*aafA[2][0] - aafA[1][0]*aafA[2][2];
    aafAInv[1][1] = aafA[0][0]*aafA[2][2] - aafA[0][2]*aafA[2][0];
    aafAInv[1][2] = aafA[0][2]*aafA[1][0] - aafA[0][0]*aafA[1][2];
    aafAInv[2][0] = aafA[1][0]*aafA[2][1] - aafA[1][1]*aafA[2][0];
    aafAInv[2][1] = aafA[0][1]*aafA[2][0] - aafA[0][0]*aafA[2][1];
    aafAInv[2][2] = aafA[0][0]*aafA[1][1] - aafA[0][1]*aafA[1][0];
    float fDet = aafA[0][0]*aafAInv[0][0] + aafA[0][1]*aafAInv[1][0] + 
        aafA[0][2]*aafAInv[2][0];

    Sphere3f tempMinimal;

    if( Math<float>::FAbs(fDet) > m_fEpsilon )
    {
        float fInvDet = 1.0f / fDet;
        int iRow, iCol;
        for( iRow = 0; iRow < 3; iRow++ )
        {
            for( iCol = 0; iCol < 3; iCol++ )
            {
                aafAInv[iRow][iCol] *= fInvDet;
            }
        }
        
        float afU[4];
        for( iRow = 0; iRow < 3; iRow++ )
        {
            afU[iRow] = 0.0f;
            for( iCol = 0; iCol < 3; iCol++ )
            {
                afU[iRow] += aafAInv[iRow][iCol] * afB[iCol];
            }
        }
        afU[3] = 1.0f - afU[0] - afU[1] - afU[2];
        
        tempMinimal.Center = afU[0]*rPoint0 + afU[1]*rPoint1 + afU[2]*rPoint2 + 
            afU[3]*rPoint3;
        Vector3f vec3fTemp = afU[0]*vec3fE10 + afU[1]*vec3fE20 + afU[2]*vec3fE30;
        tempMinimal.Radius = vec3fTemp.GetSquaredLength();
    }
    else
    {
        tempMinimal.Center = Vector3f::ZERO;
        tempMinimal.Radius = Math<float>::MAX_REAL;
    }

    return tempMinimal;
}
//----------------------------------------------------------------------------
Sphere3f MinSphere3f::UpdateSupport1(int i, Vector3f** apPermute, 
    Support& rSupport)
{
    const Vector3f& rPoint0 = *apPermute[rSupport.Index[0]];
    const Vector3f& rPoint1 = *apPermute[i];

    Sphere3f tempMinimal = ExactSphere2(rPoint0, rPoint1);
	rSupport.Count = 2;
    rSupport.Index[1] = i;

    return tempMinimal;
}
//----------------------------------------------------------------------------
Sphere3f MinSphere3f::UpdateSupport2(int i, Vector3f** apPermute, 
    Support& rSupport)
{
    const Vector3f& rPoint0 = *apPermute[rSupport.Index[0]];
    const Vector3f& rPoint1 = *apPermute[rSupport.Index[1]];
    const Vector3f& rPoint2 = *apPermute[i];

    Sphere3f tempSpheres[3];
    float fMinRSqr = Math<float>::MAX_REAL;
    float fDistDiff;
    int iIndex = -1;

    tempSpheres[0] = ExactSphere2(rPoint0, rPoint2);
    if( Contains(rPoint1, tempSpheres[0], fDistDiff) )
    {
        fMinRSqr = tempSpheres[0].Radius;
        iIndex = 0;
    }

    tempSpheres[1] = ExactSphere2(rPoint1, rPoint2);
    if( tempSpheres[1].Radius < fMinRSqr )
    {
        if( Contains(rPoint0, tempSpheres[1], fDistDiff) )
        {
            fMinRSqr = tempSpheres[1].Radius;
            iIndex = 1;
        }
    }

    Sphere3f tempMinimal;

    if( iIndex != -1 )
    {
        tempMinimal = tempSpheres[iIndex];
        rSupport.Index[1 - iIndex] = i;
    }
    else
    {
        tempMinimal = ExactSphere3(rPoint0, rPoint1, rPoint2);
        SE_ASSERT( tempMinimal.Radius <= fMinRSqr );
		rSupport.Count = 3;
        rSupport.Index[2] = i;
    }

    return tempMinimal;
}
//----------------------------------------------------------------------------
Sphere3f MinSphere3f::UpdateSupport3(int i, Vector3f** apPermute, 
    Support& rSupport)
{
    const Vector3f& rPoint0 = *apPermute[rSupport.Index[0]];
    const Vector3f& rPoint1 = *apPermute[rSupport.Index[1]];
    const Vector3f& rPoint2 = *apPermute[rSupport.Index[2]];
    const Vector3f& rPoint3 = *apPermute[i];

    Sphere3f tempSpheres[6];
    float fMinRSqr = Math<float>::MAX_REAL;
    float fDistDiff;
    int iIndex = -1;

    tempSpheres[0] = ExactSphere2(rPoint0, rPoint3);
    if( Contains(rPoint1, tempSpheres[0], fDistDiff) && 
        Contains(rPoint2, tempSpheres[0], fDistDiff) )
    {
        fMinRSqr = tempSpheres[0].Radius;
        iIndex = 0;
    }

    tempSpheres[1] = ExactSphere2(rPoint1, rPoint3);
    if( tempSpheres[1].Radius < fMinRSqr && 
        Contains(rPoint0, tempSpheres[1], fDistDiff) && 
        Contains(rPoint2, tempSpheres[1], fDistDiff) )
    {
        fMinRSqr = tempSpheres[1].Radius;
        iIndex = 1;
    }

    tempSpheres[2] = ExactSphere2(rPoint2, rPoint3);
    if( tempSpheres[2].Radius < fMinRSqr && 
        Contains(rPoint0, tempSpheres[2], fDistDiff) && 
        Contains(rPoint1, tempSpheres[2], fDistDiff) )
    {
        fMinRSqr = tempSpheres[2].Radius;
        iIndex = 2;
    }

    tempSpheres[3] = ExactSphere3(rPoint0, rPoint1, rPoint3);
    if( tempSpheres[3].Radius < fMinRSqr && 
        Contains(rPoint2, tempSpheres[3], fDistDiff) )
    {
        fMinRSqr = tempSpheres[3].Radius;
        iIndex = 3;
    }

    tempSpheres[4] = ExactSphere3(rPoint0, rPoint2, rPoint3);
    if( tempSpheres[4].Radius < fMinRSqr && 
        Contains(rPoint1, tempSpheres[4], fDistDiff) )
    {
        fMinRSqr = tempSpheres[4].Radius;
        iIndex = 4;
    }

    tempSpheres[5] = ExactSphere3(rPoint1, rPoint2, rPoint3);
    if( tempSpheres[5].Radius < fMinRSqr && 
        Contains(rPoint0, tempSpheres[5], fDistDiff) )
    {
        fMinRSqr = tempSpheres[5].Radius;
        iIndex = 5;
    }

    Sphere3f tempMinimal;

    switch( iIndex )
    {
    case 0:
        tempMinimal = tempSpheres[0];
		rSupport.Count = 2;
        rSupport.Index[1] = i;
        break;
    case 1:
        tempMinimal = tempSpheres[1];
        rSupport.Count = 2;
        rSupport.Index[0] = i;
        break;
    case 2:
        tempMinimal = tempSpheres[2];
        rSupport.Count = 2;
        rSupport.Index[0] = rSupport.Index[2];
        rSupport.Index[1] = i;
        break;
    case 3:
        tempMinimal = tempSpheres[3];
        rSupport.Index[2] = i;
        break;
    case 4:
        tempMinimal = tempSpheres[4];
        rSupport.Index[1] = i;
        break;
    case 5:
        tempMinimal = tempSpheres[5];
        rSupport.Index[0] = i;
        break;
    default:
        tempMinimal = ExactSphere4(rPoint0, rPoint1, rPoint2, rPoint3);
        SE_ASSERT( tempMinimal.Radius <= fMinRSqr );
        rSupport.Count = 4;
        rSupport.Index[3] = i;
        break;
    }

    return tempMinimal;
}
//----------------------------------------------------------------------------
Sphere3f MinSphere3f::UpdateSupport4(int i, Vector3f** apPermute, 
    Support& rSupport)
{
    const Vector3f* aPoint[4] =
    {
        apPermute[rSupport.Index[0]],
        apPermute[rSupport.Index[1]],
        apPermute[rSupport.Index[2]],
        apPermute[rSupport.Index[3]]
    };

    const Vector3f& rPoint4 = *apPermute[i];

    // ��������1.
    int aiT1[4][4] =
    {
        {0, /*4*/ 1,2,3},
        {1, /*4*/ 0,2,3},
        {2, /*4*/ 0,1,3},
        {3, /*4*/ 0,1,2}
    };

    // ��������2.
    int aiT2[6][4] =
    {
        {0,1, /*4*/ 2,3},
        {0,2, /*4*/ 1,3},
        {0,3, /*4*/ 1,2},
        {1,2, /*4*/ 0,3},
        {1,3, /*4*/ 0,2},
        {2,3, /*4*/ 0,1}
    };

    // ��������3.
    int aiT3[4][4] =
    {
        {0,1,2, /*4*/ 3},
        {0,1,3, /*4*/ 2},
        {0,2,3, /*4*/ 1},
        {1,2,3, /*4*/ 0}
    };

    Sphere3f tempSpheres[14];
    float fMinRSqr = Math<float>::MAX_REAL;
    int iIndex = -1;
    float fDistDiff, fMinDistDiff = Math<float>::MAX_REAL;
    int iMinIndex = -1;
    int k = 0;  // �������

    // ��������1.
    int j;
    for( j = 0; j < 4; j++, k++ )
    {
        tempSpheres[k] = ExactSphere2(*aPoint[aiT1[j][0]], rPoint4);
        if( tempSpheres[k].Radius < fMinRSqr )
        {
            if( Contains(*aPoint[aiT1[j][1]], tempSpheres[k], fDistDiff) &&
                Contains(*aPoint[aiT1[j][2]], tempSpheres[k], fDistDiff) &&
                Contains(*aPoint[aiT1[j][3]], tempSpheres[k], fDistDiff) )
            {
                fMinRSqr = tempSpheres[k].Radius;
                iIndex = k;
            }
            else if( fDistDiff < fMinDistDiff )
            {
                fMinDistDiff = fDistDiff;
                iMinIndex = k;
            }
        }
    }

    // ��������2.
    for( j = 0; j < 6; j++, k++ )
    {
        tempSpheres[k] = ExactSphere3(*aPoint[aiT2[j][0]], *aPoint[aiT2[j][1]], 
            rPoint4);
        if( tempSpheres[k].Radius < fMinRSqr )
        {
            if( Contains(*aPoint[aiT2[j][2]], tempSpheres[k], fDistDiff) &&  
                Contains(*aPoint[aiT2[j][3]], tempSpheres[k], fDistDiff) )
            {
                fMinRSqr = tempSpheres[k].Radius;
                iIndex = k;
            }
            else if( fDistDiff < fMinDistDiff )
            {
                fMinDistDiff = fDistDiff;
                iMinIndex = k;
            }
        }
    }

    // ��������3.
    for( j = 0; j < 4; j++, k++ )
    {
        tempSpheres[k] = ExactSphere4(*aPoint[aiT3[j][0]], *aPoint[aiT3[j][1]], 
            *aPoint[aiT3[j][2]], rPoint4);
        if( tempSpheres[k].Radius < fMinRSqr )
        {
            if( Contains(*aPoint[aiT3[j][3]], tempSpheres[k], fDistDiff) )
            {
                fMinRSqr = tempSpheres[k].Radius;
                iIndex = k;
            }
            else if( fDistDiff < fMinDistDiff )
            {
                fMinDistDiff = fDistDiff;
                iMinIndex = k;
            }
        }
    }

    // ��������˵,iIndex >= 0Ӧ�ûᷢ��,�����ڸ����������������������,
    // iIndex == -1Ҳ�п��ܷ���,
    // ��ʱѡ��߱���С��Χ����������.
    if( iIndex == -1 )
    {
        iIndex = iMinIndex;
    }

    Sphere3f tempMinimal = tempSpheres[iIndex];

    switch( iIndex )
    {
    case 0:
        rSupport.Count = 2;
        rSupport.Index[1] = i;
        break;
    case 1:
        rSupport.Count = 2;
        rSupport.Index[0] = i;
        break;
    case 2:
        rSupport.Count = 2;
        rSupport.Index[0] = rSupport.Index[2];
        rSupport.Index[1] = i;
        break;
    case 3:
        rSupport.Count = 2;
        rSupport.Index[0] = rSupport.Index[3];
        rSupport.Index[1] = i;
        break;
    case 4:
        rSupport.Count = 3;
        rSupport.Index[2] = i;
        break;
    case 5:
        rSupport.Count = 3;
        rSupport.Index[1] = i;
        break;
    case 6:
        rSupport.Count = 3;
        rSupport.Index[1] = rSupport.Index[3];
        rSupport.Index[2] = i;
        break;
    case 7:
        rSupport.Count = 3;
        rSupport.Index[0] = i;
        break;
    case 8:
        rSupport.Count = 3;
        rSupport.Index[0] = rSupport.Index[3];
        rSupport.Index[2] = i;
        break;
    case 9:
        rSupport.Count = 3;
        rSupport.Index[0] = rSupport.Index[3];
        rSupport.Index[1] = i;
        break;
    case 10:
        rSupport.Index[3] = i;
        break;
    case 11:
        rSupport.Index[2] = i;
        break;
    case 12:
        rSupport.Index[1] = i;
        break;
    case 13:
        rSupport.Index[0] = i;
        break;
    }

    return tempMinimal;
}
//----------------------------------------------------------------------------