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

#ifndef Swing_IntrLine3Box3_H
#define Swing_IntrLine3Box3_H

#include "SEFoundationLIB.h"
#include "SEIntersector.h"
#include "SELine3.h"
#include "SEBox3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:IntrLine3Box3f��
// ˵��:
// ����:Sun Che
// ʱ��:20081230
//----------------------------------------------------------------------------
class SE_FOUNDATION_API IntrLine3Box3f
    : public Intersector<float, SEVector3f>
{
public:
    IntrLine3Box3f(const SELine3f& rLine, const SEBox3f& rBox);

    // �������.
    const SELine3f& GetLine(void) const;
    const SEBox3f& GetBox(void) const;

    // static intersection��ѯ.
    virtual bool Test(void); // ʹ��Ǳ�ڷ������Minkowski difference�㷨
    virtual bool Find(void);

    // �ཻ����.
    int GetCount(void) const;
    const SEVector3f& GetPoint(int i) const;

private:
    static bool Clip(float fDenom, float fNumer, float& rfT0, float& rfT1);

    // ������Ƿ��ཻ�Ķ���.
    const SELine3f* m_pLine;
    const SEBox3f* m_pBox;

    // �ཻ�������Ϣ.
    int m_iCount;
    SEVector3f m_aPoint[2];

// �ڲ�ʹ��(�����IntrRay3Box3f��IntrSegment3Box3f)
public:
    // ʹ�����Ѷ�-Barsky�ü��㷨,����linear component��box�Ľ���.
    static bool DoClipping(float fT0, float fT1, const SEVector3f& rOrigin,
        const SEVector3f& rDirection, const SEBox3f& rBox, bool bSolid, 
        int& riCount, SEVector3f aPoint[2], int& riIntrType);
};

}

#endif
