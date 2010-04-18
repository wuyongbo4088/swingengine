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

#ifndef Swing_SphereBV_H
#define Swing_SphereBV_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEBoundingVolume.h"
#include "SESphere3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ˵��:
// ����:Sun Che
// ʱ��:20080726
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SESphereBV : public SEBoundingVolume
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    SESphereBV(void);  // center (0,0,0), radius 0
    SESphereBV(const SESphere3f& rSphere);

    virtual int GetBVType(void) const;

    // ����BV��Ҫ�����е�Ͱ뾶.
    virtual void SetCenter(const SEVector3f& rCenter);
    virtual void SetRadius(float fRadius);
    virtual SEVector3f GetCenter(void) const;
    virtual float GetRadius(void) const;

    inline SESphere3f& Sphere(void);
    inline const SESphere3f& GetSphere(void) const;

    // ���ݴ��붥�㼯�ϴ���BV.
    virtual void ComputeFromData(const SEVector3fArray* pVertices);
    virtual void ComputeFromData(const SEVertexBuffer* pVBuffer);

    // �任BV(��ģ�Ϳռ䵽����ռ�).
    virtual void TransformBy(const SETransformation& rTransform, SEBoundingVolume* pResult);

    // �ж�BV�Ƿ���ƽ������ռ�(ƽ�淨����ָ��Ŀռ�),�ཻ,����ռ�,
    // ��Ӧ�ķ���ֵΪ+1,0,-1.
    virtual int OnWhichSide(const SEPlane3f& rPlane) const;

    // ����BV�Ƿ�������ཻ,�����㽻��,���߷������Ϊ��λ����.
    virtual bool TestIntersection(const SERay3f& rRay) const;

    // �����Ƿ����һ��BV�ཻ.
    virtual bool TestIntersection(const SEBoundingVolume* pInput) const;

    // ����һ��BV���Ƴ��Լ�.
    virtual void CopyFrom(const SEBoundingVolume* pInput);

    // ��ǰBV����,���������BV��֮ǰ���Լ�.
    virtual void GrowToContain(const SEBoundingVolume* pInput);

    // �Ƿ���������.
    virtual bool Contains(const SEVector3f& rPoint) const;

protected:
    SESphere3f m_Sphere;
};

typedef SESmartPointer<SESphereBV> SESphereBVPtr;

#include "SESphereBV.inl"

}

#endif
