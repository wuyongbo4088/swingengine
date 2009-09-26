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

#ifndef Swing_Mapper2_H
#define Swing_Mapper2_H

#include "SEFoundationLIB.h"
#include "SEVector2.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:Mapper2f��
// ˵��:
// ����:Sun Che
// ʱ��:20081202
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Mapper2f
{
public:
    // �����㴫�붥�㼯�ϵ�ά��ʱ,epsilonֵ����������.
    Mapper2f(int iVCount, const Vector2f* aVertex, float fEpsilon);
    ~Mapper2f(void);

    // ���붥�㼯�ϵ�AABBox.
    const Vector2f& GetMin(void) const;
    const Vector2f& GetMax(void) const;
    float GetMaxRange(void) const;

    // ���ϵ�ά��(0, 1, ��2).
    int GetDimension(void) const;

    // ����ϵͳ.�����κ�ά��d,origin����Ч.
    // ��λ����������0 <= i < d��Χ����Ч.
    // extreme index�봫�붥�������й�,��0 <= i < d��Χ����Ч.
    // ���d = 0,�����ж��㶼��ͬ,����ʹ����epsilon,��extreme index���ܲ���0.
    // ���d = 1,�����ж��㶼��һ���߶���.extreme index��ʾ�����߶εĶ˵�.
    // ���d = 2,��ǰ����extreme index��ʾһ���߶�.
    // ֮���extreme index��ʾ�ڴ�ֱ�������߶εķ�����,������߶���Զ�Ķ���.
    const Vector2f& GetOrigin(void) const;
    const Vector2f& GetDirection(int i) const;
    int GetExtremeIndex(int i) const;

    // ���d = 2,��������{U0,U1}������������ϵ.
    // ������ֵ���㹹��һ��������.�����������������Ƿ�Ϊ��ʱ������˳��.
    bool GetExtremeCCW(void) const;

private:
    // ���붥���AABBox.
    // maximum range��max[0]-min[0]��max[1]-min[1]�еĽϴ���.
    Vector2f m_Min, m_Max;
    float m_fMaxRange;

    // ���붥�㼯�ϵĹ���ά��.
    // ���ж�ά��ʱ,�������캯���Ĳ���fEpsilon�����ṩһ��������ֵ.
    int m_iDimension;

    // ��������maximum dimensional extents������ֵ.
    // m_aiExtreme[0]��m_aiExtreme[1]��������������,
    // ���������㶨���˾߱����extent����������.
    // �������ά����2,��m_aiExtreme[2]����һ����Ч�Ķ�������,
    // �ö��㶨���˴�ֱ��m_aiExtreme[0]��m_aiExtreme[1]���������ϵ����extent.
    // ������<V[extreme0],V[extreme1],V[extreme2]>������˳ʱ�������ʱ������˳��,
    // ��m_bExtremeCCW����.
    int m_aiExtreme[3];
    bool m_bExtremeCCW;

    Vector2f m_Origin;
    Vector2f m_aDirection[2];
};

}

#endif