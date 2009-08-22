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

#ifndef Swing_LinComp_H
#define Swing_LinComp_H

#include "SEPlatforms.h"
#include "SEMath.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:LinComp�����
// ˵��:
// ����:Sun Che
// ʱ��:20081219
//----------------------------------------------------------------------------
template <class Real>
class LinComp
{
public:
    // �����
    virtual ~LinComp(void);

    // linear component����ʾΪP + t*D����ʽ,����P��ԭ��,D�ǵ�λ���ȷ�������.
    // �û�����ȷ����������.tֵ���������ֱ��,����,�߶�,��,�ռ��Ժ�����.

    // ����.
    enum
    {
        CT_EMPTY,
        CT_POINT,
        CT_SEGMENT,
        CT_RAY,
        CT_LINE
    };

    int GetType(void) const;

    // tֵ���������,�����������������.
    // ����SetInterval(min, max)��������tֵ����.
    // ����������Ĳ��Դ�����ֿ��ܵĴ���ֵ:
    //   CT_LINE:
    //     [-MAX_REAL,MAX_REAL]
    //   CT_RAY:
    //     [min,MAX_REAL], ����min��finiteֵ
    //     [-MAX_REAL,max], ����max��finiteֵ
    //   CT_SEGMENT:
    //     [min,max], ����min��max��finiteֵ, ��min < max
    //   CT_POINT:
    //     [min,max], ����min��max��finiteֵ, �� min = max
    //   CT_EMPTY:
    //     [min,max], ����min > max��min = max = MAX_REAL��
    //                min = max = -MAX_REAL
    void SetInterval(Real fMin, Real fMax);

    // ���贴��LinComp����Ϳɵ�֪������������.
    static int GetTypeFromInterval(Real fMin, Real fMax);

    // ����linear component���͵Ĺ淶������Ϊ:
    // ֱ����[-MAX_REAL,MAX_REAL],������[0,MAX_REAL],�߶���[-e,e],����e > 0,
    // ����[0,0],�ռ���[MAX_REAL,-MAX_REAL].�������������[min,max],
    // �������µ���.
    //
    // CT_RAY: ���max��MAX_REAL��min����,��P�㱻��ΪP' = P + min*D,
    // �Ӷ����߱���ʾΪP' + t*D,����t >= 0.
    // ���min��-MAX_REAL��max��finiteֵ,��P���D��������ΪP' = P + max*D
    // ��D' = -D.
    //
    // CT_SEGMENT: ���min������-max,��P����Ϊ
    // P' = P + ((min + max)/2)*D ���߶���չ������e' = (max - min)/2.
    //
    // CT_POINT: ���min��������, ��P�㱻��ΪP' = P + min*D.
    //
    // CT_EMPTY: ����maxΪ-MAX_REAL��minΪMAX_REAL.
    //
    // ��һ���������麯��,��Ϊ����Ĺ淶�����������������ռ��ά��.
    virtual void MakeCanonical(void) = 0;
    bool IsCanonical(void) const;

    // ������� [min,max].
    Real GetMin(void) const;
    Real GetMax(void) const;

    // �ж�ָ���Ĳ����Ƿ��ڵ�ǰ������.
    bool Contains(Real fParam) const;

protected:
    LinComp(void);  // default : CT_NONE

    // ��ֵ���������.
    LinComp& operator = (const LinComp& rComponent);

    // component type.
    int m_iType;

    // tֵ��������.
    Real m_fMin, m_fMax;
};

#include "SELinComp.inl"

typedef LinComp<float> LinCompf;
typedef LinComp<double> LinCompd;

}

#endif
