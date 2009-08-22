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

#ifndef Swing_IntrSphere3Sphere3_H
#define Swing_IntrSphere3Sphere3_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEIntersector.h"
#include "SESphere3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:IntrSphere3Sphere3f��
// ˵��:
// ����:Sun Che
// ʱ��:20081219
//----------------------------------------------------------------------------
class SE_FOUNDATION_API IntrSphere3Sphere3f
    : public Intersector<float, Vector3f>
{
public:
    IntrSphere3Sphere3f(const Sphere3f& rSphere0, const Sphere3f& rSphere1);

    // �������.
    const Sphere3f& GetSphere0(void) const;
    const Sphere3f& GetSphere1(void) const;

    // ��̬test-intersection��ѯ.
    virtual bool Test(void);

    // ��̬find-intersection��ѯ.
    virtual bool Find(void);

    // ��̬find-intersection��ѯ���ཻ��.
    const Vector3f& GetCenter(void) const;
    const Vector3f& GetUAxis(void) const;
    const Vector3f& GetVAxis(void) const;
    const Vector3f& GetNormal(void) const;
    float GetRadius(void) const;

    // ��̬test-intersection��ѯ.
    virtual bool Test(float fTMax, const Vector3f& rVelocity0,
        const Vector3f& rVelocity1);

    // ��̬find-intersection��ѯ.
    virtual bool Find(float fTMax, const Vector3f& rVelocity0,
        const Vector3f& rVelocity1);

    // ��̬find-intersection��ѯ���ཻ��.
    const Vector3f& GetContactPoint(void) const;

private:
    using Intersector<float, Vector3f>::m_fContactTime;

    // ������Ƿ��ཻ�Ķ���.
    const Sphere3f* m_pSphere0;
    const Sphere3f* m_pSphere1;

    // ��̬����ཻԲ.
    // Բ����C,Բ����ƽ����������λ����U��Vȷ��.
    // ƽ�淨���ǵ�λ����N.Բ�뾶��R.
    Vector3f m_Center, m_UAxis, m_VAxis, m_Normal;
    float m_fRadius;

    // ��̬����ĽӴ���.
    Vector3f m_ContactPoint;
};

}

#endif
