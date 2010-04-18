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

#ifndef Swing_Intersector1_H
#define Swing_Intersector1_H

#include "SEFoundationLIB.h"
#include "SEMath.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Author:Sun Che
// Date:20081223
//----------------------------------------------------------------------------
template <class Real>
class SE_FOUNDATION_API SEIntersector1
{
public:
    // ����1ά����[u0,u1]��[v0,v1]��intersection��.
    // �˵�˳�������: u0 <= u1 �� v0 <= v1.ֵ������MAX_REAL��-MAX_REAL,
    // ͬʱҲ�����˻�������: u0 = u1 �� v0 = v1.
    SEIntersector1(Real fU0, Real fU1, Real fV0, Real fV1);
    SEIntersector1(Real afU[2], Real afV[2]);
    virtual ~SEIntersector1(void);

    // �������.
    Real GetU(int i) const;
    Real GetV(int i) const;

    // static intersection��ѯ.
    virtual bool Test(void);
    virtual bool Find(void);

    // dynamic intersection��ѯ.Find��ѯ����first contact set.
    virtual bool Test(Real fTMax, Real fSpeedU, Real fSpeedV);
    virtual bool Find(Real fTMax, Real fSpeedU, Real fSpeedV);

    // dynamic intersection��ѯ������first/last contact time..
    Real GetFirstTime(void) const;
    Real GetLastTime(void) const;

    // �ཻ�������Ϣ.
    int GetCount(void) const;
    Real GetOverlap(int i) const;

protected:
    // ����ѯ����.
    Real m_afU[2], m_afV[2];

    // �ཻ�������Ϣ.
    Real m_fFirstTime, m_fLastTime;
    int m_iCount;
    Real m_afOverlap[2];
};

typedef SEIntersector1<float> SEIntersector1f;
typedef SEIntersector1<double> SEIntersector1d;

}

#endif
