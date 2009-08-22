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

#ifndef Swing_ContSphere3_H
#define Swing_ContSphere3_H

#include "SEFoundationLIB.h"
#include "SESphere3.h"
#include "SEVector3.h"
#include "SEMath.h"

namespace Swing
{

// ������������ɶ���,��������������СAABBox��BV��.
SE_FOUNDATION_API Sphere3f ContSphereOfAABBf(int iCount, const Vector3f* aPoint);

// ������������ɶ���,������BV��,�����Ƕ����ƽ��ֵ.
SE_FOUNDATION_API Sphere3f ContSphereAveragef(int iCount, const Vector3f* aPoint);

// ����ָ�������Ƿ����������.
SE_FOUNDATION_API bool IsInSpheref(const Vector3f& rPoint, const Sphere3f& rSphere);

// ��������������BV��.
SE_FOUNDATION_API Sphere3f MergeSpheresf(const Sphere3f& rSphere0, 
    const Sphere3f& rSphere1);

// ������������ɶ���,��������СBV��,
// �㷨����������붥������˳��,��˹���ʱ��Ԥ��ΪO(n).
class SE_FOUNDATION_API MinSphere3f
{
public:
    MinSphere3f(int iCount, const Vector3f* aPoint, Sphere3f& rMinimal, 
        float fEpsilon = 1.0e-03f);

private:
    // ��ǰ��С��Ķ�������֧��.
    class SE_FOUNDATION_API Support
    {
    public:
        // �������붥���Ƿ�����support����,
        // ������붥����뼯����ĳ�����С�ڸ���epsilon,����Ϊ���ڼ���.
        bool Contains(int iIndex, Vector3f** apPoint, float fEpsilon)
        {
            for( int i = 0; i < Count; i++ )
            {
                Vector3f vec3fDiff = *apPoint[iIndex] - *apPoint[Index[i]];
                if( vec3fDiff.GetSquaredLength() < fEpsilon )
                {
                    return true;
                }
            }

            return false;
        }

        int Count;
        int Index[4];
    };

    // ���Զ���P�Ƿ��������S��.
    bool Contains(const Vector3f& rPoint, const Sphere3f& rSphere, 
        float& rfDistDiff);

    Sphere3f ExactSphere1(const Vector3f& rPoint);
    Sphere3f ExactSphere2(const Vector3f& rPoint0, const Vector3f& rPoint1);
    Sphere3f ExactSphere3(const Vector3f& rPoint0, const Vector3f& rPoint1, 
        const Vector3f& rPoint2);
    Sphere3f ExactSphere4(const Vector3f& rPoint0, const Vector3f& rPoint1, 
        const Vector3f& rPoint2, const Vector3f& rPoint3);

    Sphere3f UpdateSupport1(int i, Vector3f** apPermute, Support& rSupport);
    Sphere3f UpdateSupport2(int i, Vector3f** apPermute, Support& rSupport);
    Sphere3f UpdateSupport3(int i, Vector3f** apPermute, Support& rSupport);
    Sphere3f UpdateSupport4(int i, Vector3f** apPermute, Support& rSupport);

    typedef Sphere3f (MinSphere3f::*UpdateFunction)(int, Vector3f**, Support&);

    float m_fEpsilon, m_fOnePlusEpsilon;
    UpdateFunction m_aoUpdate[5];
};

}

#endif