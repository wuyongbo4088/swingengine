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

#ifndef Swing_Frustum3_H
#define Swing_Frustum3_H

#include "SEFoundationLIB.h"
#include "SEVector3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Author:Sun Che
// Date:20090115
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SEFrustum3f
{
public:
    // �淶������Ͷ��.��EΪԭ��,RΪ������,UΪ������,DΪ��������.
    // ��r > 0��u > 0,�ֱ�ΪR���U�᷽�����չ����.
    // ��n��fΪD�᷽�����չ����,��0 < n < f.
    // ����Ͷ��(near plane)�ϵ��ĽǶ����ʾΪE + s0*r*R + s1*u*U + n*D,
    // ���� |s0| = |s1| = 1(��ʾ����ѡ��).
    // Զ��Ͷ��(far plane)�ϵ��ĽǶ����ʾΪE + (f/n)*(s0*r*R + s1*u*U) + f*D,
    // ���� |s0| = |s1| = 1(��ʾ����ѡ��).

    // Ĭ��ֵΪ: E = (0,0,0), R = (1,0,0), U = (0,1,0), D = (0,0,1),
    // RBound = 1, UBound = 1, DMin = 1, DMax = 2.
    SEFrustum3f(void);
    SEFrustum3f(const SEVector3f& rOrigin, const SEVector3f& rRVector,
        const SEVector3f& rUVector, const SEVector3f& rDVector,
        float fRBound, float fUBound, float fDMin, float fDMax);

    // ��RBound,UBound,Dmin,Dmax�����ı�ʱ,�������Update()����.
    // DRatio,MTwoUF,MTwoRF����֮�ı�,���Ӧ��Update()֮���������������.
    void Update(void);
    float GetDRatio(void) const;
    float GetMTwoUF(void) const;
    float GetMTwoRF(void) const;

    void ComputeVertices(SEVector3f aVertex[8]) const;

    SEVector3f Origin, RVector, UVector, DVector;
    float RBound, UBound, DMin, DMax;

protected:
    // ��ص�������.
    float m_fDRatio, m_fMTwoUF, m_fMTwoRF;
};

}

#endif