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

#ifndef Swing_Segment2_H
#define Swing_Segment2_H

#include "SEFoundationLIB.h"
#include "SEVector2.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:2ά�߶���
// ˵��:
// ����:Sun Che
// ʱ��:20090113
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Segment2f
{
public:
    // �߶α�ʾΪ����������ʽ:P + t*D,����P���߶�ԭ��,D�ǵ�λ���ȷ�������,
    // |t| <= e,����e���߶���չ����.
    // �û�����ȷ��DΪ��λ����.
    // �߶ζ˵���P - e*D��P + e*D.
    // �߶εı�ʾ����������OBB.P���е�,D�ǵ�λ������,e��������չ����.

    Segment2f(void);
    Segment2f(const Vector2f& rOrigin, const Vector2f& rDirection,
        float fExtent);

    // �˵����.
    Vector2f GetPosEnd(void) const;  // P + e*D
    Vector2f GetNegEnd(void) const;  // P - e*D

    Vector2f Origin, Direction;
    float Extent;
};

}

#endif
