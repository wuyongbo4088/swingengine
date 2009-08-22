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

#ifndef Swing_IntrBox3Box3_H
#define Swing_IntrBox3Box3_H

#include "SEFoundationLIB.h"
#include "SEIntersector.h"
#include "SEBox3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:IntrBox3Box3f��
// ˵��:
// ����:Sun Che
// ʱ��:20081230
//----------------------------------------------------------------------------
class SE_FOUNDATION_API IntrBox3Box3f
    : public Intersector<float, Vector3f>
{
public:
    IntrBox3Box3f(const Box3f& rBox0, const Box3f& rBox1);

    // �������.
    const Box3f& GetBox0(void) const;
    const Box3f& GetBox1(void) const;

    // static test-intersection��ѯ.
    virtual bool Test(void);

    // dynamic test-intersection��ѯ.����first contact time(����еĻ�),
    // ��û���κι���contact set����Ϣ.
    virtual bool Test(float fTMax, const Vector3f& rVelocity0,
        const Vector3f& rVelocity1);

private:
    // ����֧��dynamic��ѯ.  
    // �����������: ����box��һ��Ǳ�ڷ������ϵĸ���ͶӰ����,
    // �������˶�������ٶ�ֵ,�Լ���ѯ��ָ�������ʱ��ֵ.
    // �����������: first/last contact time.
    // �����������Բ�ͬ��Ǳ�ڷ����ᱻƵ������ʱ,��Ӧ���������Ҳ�����ϸ���.
    // �����first contact timeֻ���ڴ��������first contact timeʱ�Ÿ���.
    // �����last contact timeֻ����С�������last contact timeʱ�Ÿ���.
    bool IsSeparated(float fMin0, float fMax0, float fMin1, float fMax1,
        float fSpeed, float fTMax, float& rfTFirst, float& rfTLast);

    // ��������.
    const Box3f* m_pBox0;
    const Box3f* m_pBox1;
};

}

#endif
