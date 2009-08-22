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

#ifndef Swing_BoxBV_H
#define Swing_BoxBV_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEBoundingVolume.h"
#include "SEBox3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:OBB��Χ����
// ˵��:
// ����:Sun Che
// ʱ��:20080330
//----------------------------------------------------------------------------
class SE_FOUNDATION_API BoxBV : public BoundingVolume
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    BoxBV(void); // center(0,0,0), axes(1,0,0),(0,1,0),(0,0,1), extents 1,1,1
    BoxBV(const Box3f& rBox);
    virtual ~BoxBV(void);

    virtual int GetBVType(void) const;

    // ����BV��Ҫ�����е�Ͱ뾶.
    virtual void SetCenter(const Vector3f& rCenter);
    virtual Vector3f GetCenter(void) const;
    virtual void SetRadius(float fRadius);
    virtual float GetRadius(void) const;

    inline Box3f& Box(void);
    inline const Box3f& GetBox(void) const;

    // ���ݴ��붥�㼯�ϴ���BV.
    virtual void ComputeFromData(const Vector3fArray* pVertices);
    virtual void ComputeFromData(const VertexBuffer* pVBuffer);

    // �任BV(��ģ�Ϳռ䵽����ռ�).
    virtual void TransformBy(const Transformation& rTransform, 
        BoundingVolume* pResult);

    // �ж�BV�Ƿ���ƽ������ռ�(ƽ�淨����ָ��Ŀռ�),�ཻ,����ռ�,
    // ��Ӧ�ķ���ֵΪ+1,0,-1.
    virtual int OnWhichSide(const Plane3f& rPlane) const;

    // ����BV�Ƿ�������ཻ,�����㽻��,���߷������Ϊ��λ����.
    virtual bool TestIntersection(const Ray3f& rRay) const;

    // �����Ƿ����һ��BV�ཻ.
    virtual bool TestIntersection(const BoundingVolume* pInput) const;

    // ����һ��BV���Ƴ��Լ�.
    virtual void CopyFrom(const BoundingVolume* pInput);

    // ��ǰBV����,���������BV��֮ǰ���Լ�.
    virtual void GrowToContain(const BoundingVolume* pInput);

    // �Ƿ���������.
    virtual bool Contains(const Vector3f& rPoint) const;

protected:
    Box3f m_Box;
};

#include "SEBoxBV.inl"

typedef SmartPointer<BoxBV> BoxBVPtr;

}

#endif
