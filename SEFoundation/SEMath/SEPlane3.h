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

#ifndef Swing_Plane_H
#define Swing_Plane_H

#include "SEVector3.h"

// ��3άƽ��ķ�������Ϊ��λ����N,����֪ƽ����һ��P0,ƽ��������һ��P,
// ��ƽ�淽�̿ɱ�ʾΪDot(N,(P - P0)) = 0;��һ����ʾΪDot(N,P) = c.

// ƽ�������������ϵ������ԭ��,��ƽ����3��ΪP0,P1,P2,��ƽ�淨��N��
// ����Ϊ����˳ʱ������P0,P1,P2�Ĳ����������.
// ע��DirectX��������˳ʱ������,��OpenGL����������ʱ������,
// ��DirectX��ʽ��������<P0,P1,P2>,��OpenGL��ӦΪ<P0,P2,P1>.

namespace Swing
{

//----------------------------------------------------------------------------
// ����:3άƽ����
// ˵��:
// ����:Sun Che
// ʱ��:20070522
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Plane3f
{
public:
    Plane3f(void);
    Plane3f(const Plane3f& rPlane);
    // �õ�λ�������ߺͳ�������
    Plane3f(const Vector3f& rNormal, float fConstant);
    // �õ�λ�������ߺ�ƽ����һ�㹹��
    Plane3f(const Vector3f& rNormal, const Vector3f& rP0);
    // ��ƽ����3�㹹��
    Plane3f(const Vector3f& rP0, const Vector3f& rP1, const Vector3f& rP2);

    Plane3f& operator = (const Plane3f& rPlane);

    // ��P��ƽ�����淵��1,��ƽ���Ϸ���0,��ƽ�渺�淵��-1
    inline int OnWhichSide(const Vector3f& rP) const;
    // �����P��ƽ��ľ���
    inline float GetDistance(const Vector3f& rP) const;

public:
    Vector3f Normal;
    float Constant;
};

#include "SEPlane3.inl"

}

#endif