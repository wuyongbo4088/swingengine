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
#include "SEContBox3.h"
#include "SEApprGaussPointsFit3.h"
#include "SEQuaternion.h"

using namespace Swing;

//----------------------------------------------------------------------------
Box3f Swing::ContAABBf(int iCount, const Vector3f* aPoint)
{
    Vector3f vec3fMinPoint, vec3fMaxPoint;
    Vector3f::ComputeExtremes(iCount, aPoint, vec3fMinPoint, vec3fMaxPoint);

    Box3f tempBox;
    tempBox.Center = 0.5f * (vec3fMinPoint + vec3fMaxPoint);

    tempBox.Axis[0].X = 1.0f;
    tempBox.Axis[0].Y = 0.0f;
    tempBox.Axis[0].Z = 0.0f;

    tempBox.Axis[1].X = 0.0f;
    tempBox.Axis[1].Y = 1.0f;
    tempBox.Axis[1].Z = 0.0f;

    tempBox.Axis[2].X = 0.0f;
    tempBox.Axis[2].Y = 0.0f;
    tempBox.Axis[2].Z = 1.0f;

    Vector3f vec3fHalfDiagonal = 0.5f * (vec3fMaxPoint - vec3fMinPoint);
    tempBox.Extent[0] = vec3fHalfDiagonal[0];
    tempBox.Extent[1] = vec3fHalfDiagonal[1];
    tempBox.Extent[2] = vec3fHalfDiagonal[2];

    return tempBox;
}
//----------------------------------------------------------------------------
Box3f Swing::ContOBBf(int iCount, const Vector3f* aPoint)
{
    Box3f tempBox = GaussPointsFit3f(iCount, aPoint);

    Vector3f vec3fDiff = aPoint[0] - tempBox.Center;
    Vector3f vec3fMinPoint(vec3fDiff.Dot(tempBox.Axis[0]), 
        vec3fDiff.Dot(tempBox.Axis[1]), vec3fDiff.Dot(tempBox.Axis[2]));
    Vector3f vec3fMaxPoint = vec3fMinPoint;
    for( int i = 1; i < iCount; i++ )
    {
        vec3fDiff = aPoint[i] - tempBox.Center;
        for( int j = 0; j < 3; j++ )
        {
            float fDot = vec3fDiff.Dot(tempBox.Axis[j]);
            if( fDot < vec3fMinPoint[j] )
            {
                vec3fMinPoint[j] = fDot;
            }
            else if( fDot > vec3fMaxPoint[j] )
            {
                vec3fMaxPoint[j] = fDot;
            }
        }
    }

    tempBox.Center +=
        (0.5f*(vec3fMinPoint[0] + vec3fMaxPoint[0]))*tempBox.Axis[0] +
        (0.5f*(vec3fMinPoint[1] + vec3fMaxPoint[1]))*tempBox.Axis[1] +
        (0.5f*(vec3fMinPoint[2] + vec3fMaxPoint[2]))*tempBox.Axis[2];

    tempBox.Extent[0] = 0.5f * (vec3fMaxPoint[0] - vec3fMinPoint[0]);
    tempBox.Extent[1] = 0.5f * (vec3fMaxPoint[1] - vec3fMinPoint[1]);
    tempBox.Extent[2] = 0.5f * (vec3fMaxPoint[2] - vec3fMinPoint[2]);

    return tempBox;
}
//----------------------------------------------------------------------------
bool Swing::IsInBoxf(const Vector3f& rPoint, const Box3f& rBox)
{
    Vector3f vec3fDiff = rPoint - rBox.Center;
    for( int i = 0; i < 3; i++ )
    {
        float fCoeff = vec3fDiff.Dot(rBox.Axis[i]);
        if( Math<float>::FAbs(fCoeff) > rBox.Extent[i] )
        {
            return false;
        }
    }

    return true;
}
//----------------------------------------------------------------------------
Box3f Swing::MergeBoxesf(const Box3f& rBox0, const Box3f& rBox1)
{
    Box3f tempBox;

    // ���ȳ����²���box����.
    // �Ժ󵱴����box�Ķ���ȫ��ͶӰ��Ϻ�,�����´�ֵ.
    tempBox.Center = 0.5f * (rBox0.Center + rBox1.Center);

    // box�������������ɱ���Ϊ���������,�Ӷ�����һ����ת����.
    // �����box��������ת��Ϊ��Ӧ����ת�������Ԫ��.
    // ����������Ԫ��,������ƽ����Ԫ��,֮��淶��Ϊ��λ����.
    // �����Ԫ�����Ǵ����������Ԫ����slerp,����tΪ1/2.
    // Ȼ���ٰѽ����Ԫ��ת���������Ӧ����ת����.�þ�������н���Ϊ��box������.
    Quaternionf Q0, Q1;
    Q0.FromRotationMatrix(rBox0.Axis);
    Q1.FromRotationMatrix(rBox1.Axis);
    if( Q0.Dot(Q1) < 0.0f )
    {
        Q1 = -Q1;
    }

    Quaternionf Q = Q0 + Q1;
    float fInvLength = Math<float>::InvSqrt(Q.Dot(Q));
    Q = fInvLength * Q;
    Q.ToRotationMatrix(tempBox.Axis);

    // �Ѵ����box�Ķ���ȫ��ͶӰ��merged-box������.
    // �ڰ�����ǰbox�е�C��ÿ����D[i]�϶�����СͶӰֵpmin[i]�����ͶӰֵpmax[i].
    // ��ͶӰֵ���Ӧ�����϶˵�ΪC+pmin[i]*D[i]��C+pmax[i]*D[i].
    // ��ǰC�㲻һ����ÿ��ͶӰ������е�.
    // ���,ʵ�ʵ���box�е㽫������ΪC':
    //   C' = C + sum_{i=0}^2 0.5*(pmin[i]+pmax[i])*D[i].
    // ��box��extentΪ:
    //   e[i] = 0.5*(pmax[i]-pmin[i]).

    int i, j;
    float fDot;
    Vector3f aVertex[8], vec3fDiff;
    Vector3f vec3fMin = Vector3f::ZERO;
    Vector3f vec3fMax = Vector3f::ZERO;

    rBox0.ComputeVertices(aVertex);
    for( i = 0; i < 8; i++ )
    {
        vec3fDiff = aVertex[i] - tempBox.Center;
        for( j = 0; j < 3; j++ )
        {
            fDot = vec3fDiff.Dot(tempBox.Axis[j]);
            if( fDot > vec3fMax[j] )
            {
                vec3fMax[j] = fDot;
            }
            else if( fDot < vec3fMin[j] )
            {
                vec3fMin[j] = fDot;
            }
        }
    }

    rBox1.ComputeVertices(aVertex);
    for( i = 0; i < 8; i++ )
    {
        vec3fDiff = aVertex[i] - tempBox.Center;
        for( j = 0; j < 3; j++ )
        {
            fDot = vec3fDiff.Dot(tempBox.Axis[j]);
            if( fDot > vec3fMax[j] )
            {
                vec3fMax[j] = fDot;
            }
            else if( fDot < vec3fMin[j] )
            {
                vec3fMin[j] = fDot;
            }
        }
    }

    // [vec3fMin,vec3fMax]��merged box�����ṹ�ɵ�������ϵ�µ�AABB.
    // ������box���е�C,���������е����extent.
    for( j = 0; j < 3; j++ )
    {
        tempBox.Center += (0.5f*(vec3fMax[j] + vec3fMin[j])) * tempBox.Axis[j];
        tempBox.Extent[j] = 0.5f*(vec3fMax[j] - vec3fMin[j]);
    }

    return tempBox;
}
//----------------------------------------------------------------------------