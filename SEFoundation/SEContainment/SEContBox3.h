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

#ifndef Swing_ContBox3_H
#define Swing_ContBox3_H

#include "SEFoundationLIB.h"
#include "SEBox3.h"

namespace Swing
{

// ������������ɶ���,��������СAABB.
SE_FOUNDATION_API Box3f ContAABBf(int iCount, const Vector3f* aPoint);

// ������������ɶ���,������OBB,OBB�����Ƕ���ƽ������,
// OBB���Ƕ���Э����������������.
SE_FOUNDATION_API Box3f ContOBBf(int iCount, const Vector3f* aPoint);

// ����ָ�������Ƿ�����ں�����.
SE_FOUNDATION_API bool IsInBoxf(const Vector3f& rPoint, const Box3f& rBox);

// �����������OBB����OBB,��OBB���ܱ�֤��С.
SE_FOUNDATION_API Box3f MergeBoxesf(const Box3f& rBox0, const Box3f& rBox1);

}

#endif