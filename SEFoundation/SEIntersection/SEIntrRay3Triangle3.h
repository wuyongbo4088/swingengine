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

#ifndef Swing_IntrRay3Triangle3_H
#define Swing_IntrRay3Triangle3_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEIntersector.h"
#include "SERay3.h"
#include "SETriangle3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:IntrRay3Triangle3f��
// ˵��:
// ����:Sun Che
// ʱ��:20081220
//----------------------------------------------------------------------------
class SE_FOUNDATION_API IntrRay3Triangle3f
    : public Intersector<float, Vector3f>
{
public:
    IntrRay3Triangle3f(const Ray3f& rRay, const Triangle3f& rTriangle);

    // �������.
    const Ray3f& GetRay(void) const;
    const Triangle3f& GetTriangle(void) const;

    // test-intersection��ѯ.
    virtual bool Test(void);

    // Find-intersection��ѯ.�ཻ��Ϊ
    //   P = origin + t*direction = b0*V0 + b1*V1 + b2*V2
    virtual bool Find(void);
    float GetRayT(void) const;
    float GetTriB0(void) const;
    float GetTriB1(void) const;
    float GetTriB2(void) const;

private:
    // ������Ƿ��ཻ�Ķ���.
    const Ray3f* m_pRay;
    const Triangle3f* m_pTriangle;

    // �ཻ�������Ϣ.
    float m_fRayT, m_fTriB0, m_fTriB1, m_fTriB2;
};

}

#endif
