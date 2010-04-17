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

#ifndef Swing_IntrRay3Sphere3_H
#define Swing_IntrRay3Sphere3_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEIntersector.h"
#include "SERay3.h"
#include "SESphere3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:IntrRay3Sphere3f��
// ˵��:
// ����:Sun Che
// ʱ��:20081219
//----------------------------------------------------------------------------
class SE_FOUNDATION_API IntrRay3Sphere3f
    : public Intersector<float, SEVector3f>
{
public:
    IntrRay3Sphere3f(const SERay3f& rRay, const SESphere3f& rSphere);

    // �������.
    const SERay3f& GetRay(void) const;
    const SESphere3f& GetSphere(void) const;

    // test-intersection��ѯ.
    virtual bool Test(void);

    // find-intersection��ѯ.
    virtual bool Find(void);
    int GetCount(void) const;
    const SEVector3f& GetPoint(int i) const;
    float GetRayT(int i) const;

private:
    // ������Ƿ��ཻ�Ķ���.
    const SERay3f* m_pRay;
    const SESphere3f* m_pSphere;

    // �ཻ�������Ϣ.
    int m_iCount;
    SEVector3f m_aPoint[2];
    float m_afRayT[2];
};

}

#endif
