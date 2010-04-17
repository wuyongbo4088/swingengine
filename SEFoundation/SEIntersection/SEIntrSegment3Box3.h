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

#ifndef Swing_IntrSegment3Box3_H
#define Swing_IntrSegment3Box3_H

#include "SEFoundationLIB.h"
#include "SEIntersector.h"
#include "SESegment3.h"
#include "SEBox3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:IntrSegment3Box3f��
// ˵��:
// ����:Sun Che
// ʱ��:20090203
//----------------------------------------------------------------------------
class SE_FOUNDATION_API IntrSegment3Box3f
    : public Intersector<float, SEVector3f>
{
public:
    IntrSegment3Box3f(const SESegment3f& rSegment, const SEBox3f& rBox, 
        bool bSolid);

    // �������.
    const SESegment3f& GetSegment(void) const;
    const SEBox3f& GetBox(void) const;

    // static intersection��ѯ.
    virtual bool Test(void);
    virtual bool Find(void);

    // �ཻ����.
    int GetCount(void) const;
    const SEVector3f& GetPoint(int i) const;

private:
    // ��������.
    const SESegment3f* m_pSegment;
    const SEBox3f* m_pBox;
    bool m_bSolid;

    // �ཻ�������Ϣ.
    int m_iCount;
    SEVector3f m_aPoint[2];
};

}

#endif
