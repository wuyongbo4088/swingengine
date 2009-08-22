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

#ifndef Swing_IntrLine3Sphere3_H
#define Swing_IntrLine3Sphere3_H

#include "SEFoundationLIB.h"
#include "SEIntersector.h"
#include "SELine3.h"
#include "SESphere3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:IntrLine3Sphere3f��
// ˵��:
// ����:Sun Che
// ʱ��:20090203
//----------------------------------------------------------------------------
class SE_FOUNDATION_API IntrLine3Sphere3f
    : public Intersector<float, Vector3f>
{
public:
    IntrLine3Sphere3f(const Line3f& rLine, const Sphere3f& rSphere);

    // �������.
    const Line3f& GetLine(void) const;
    const Sphere3f& GetSphere(void) const;

    // test-intersection��ѯ.
    virtual bool Test(void);

    // find-intersection��ѯ.
    virtual bool Find(void);
    int GetCount(void) const;
    const Vector3f& GetPoint(int i) const;
    float GetLineT(int i) const;

private:
    // ��������.
    const Line3f* m_pLine;
    const Sphere3f* m_pSphere;

    // �ཻ�������Ϣ.
    int m_iCount;
    Vector3f m_aPoint[2];
    float m_afLineT[2];
};

}

#endif
