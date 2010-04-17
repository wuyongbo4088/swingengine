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

#ifndef Swing_ConvexPolyhedron3_H
#define Swing_ConvexPolyhedron3_H

#include "SEFoundationLIB.h"
#include "SEPolyhedron3.h"
#include "SEPlane3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:Convex Polyhedron��
// ˵��:
// ����:Sun Che
// ʱ��:20081124
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SEConvexPolyhedron3f : public SEPolyhedron3f
{
public:
    // ������������ȷ����������ݹ���͹������.
    // �����ۿ�meshʱ,�����������˳��ӦΪ˳ʱ�뷽��.
    // bOwnerΪtrueʱ,convex polyhedronӵ�д������������Ȩ.
    // ����Ϊfalseʱ,�ɵ����߸���ɾ����������.
    //
    // ����洢���������Ӧ��ƽ������,ƽ�淨��ָ��������ڲ�.
    // ����ͨ�����캯������ƽ������,�Ӷ�bOwnerҲ��������������Ȩ.
    // ���û�д���ƽ������,��������Լ����𴴽�������,�Ӷ�����bOwnerӰ��.
    SEConvexPolyhedron3f(int iVCount, SEVector3f* aVertex,
        int iTCount, int* aiIndex, SEPlane3f* aPlane, bool bOwner);

    // ��������polyhedronӵ������������,��Ϊ'this'����������������,
    // �Ӷ�ʹ'this'����Ҳӵ������������.
    // ��������polyhedron��ӵ������������,��'this'����Ҳ��ӵ��.ֻ����ָ�빲��.
    SEConvexPolyhedron3f(const SEConvexPolyhedron3f& rPoly);

    virtual ~SEConvexPolyhedron3f(void);

    // ��������polyhedronӵ������������,��Ϊ'this'����������������,
    // �Ӷ�ʹ'this'����Ҳӵ������������.
    // ��������polyhedron��ӵ������������,��'this'����Ҳ��ӵ��.ֻ����ָ�빲��.
    SEConvexPolyhedron3f& operator = (const SEConvexPolyhedron3f& rPoly);

    const SEPlane3f* GetPlanes(void) const;
    const SEPlane3f& GetPlane(int i) const;

    // ����Զ�������޸�.
    // �����߱���ȷ��mesh��Ϊconvex ployhedron.
    // �޸Ķ������Ҫ����UpdatePlanes()����.
    // ��������޸Ķ����������ͨ��SetVertex������ɵ�,
    // ��ֻ���ܵ�Ӱ����������plane�ᱻ���¼���.
    // ���������ͨ��GetVertices�����޸ĵĶ���,
    // ���ཫ�޷���֪�����޸ĵ���Ϣ,�Ӷ�ʹUpdatePlanes()��������ȫ��plane.
    virtual void SetVertex(int i, const SEVector3f& rV);
    void UpdatePlanes(void);

    // ͹�����Լ��.
    // ���������������������������,
    // ����֤���ж����嶥�㶼�����ڸ�������ķǸ��ռ�.
    // ʹ�ô����ŵľ������,�Ӷ���һ�����㴦�������渺��ռ�ʱ,
    // ���������d < 0.
    // ͬʱΪ�˱�����ֵ�����ɵĴ����ж�,����ʹ��һ�������ٽ�ֵt,
    // �Ӷ�ʹ�����жϳ�Ϊ d < t < 0.
    bool IsConvex(float fThreshold = 0.0f) const;

    // Point-in-polyhedron����.
    // �ٽ�ֵt��IsConvex�����е���;һ��.
    bool ContainsPoint(const SEVector3f& rP, float fThreshold = 0.0f)
        const;

protected:
    SEPlane3f* m_aPlane;
    bool m_bPlaneOwner;

    // ֧�ָ�Ч����ƽ������.
    // set�洢�˸��Ķ�������ʱ��Ӱ�쵽�������������.
    // �Ӷ�����ֻ����޸Ĺ�����������и���.
    void UpdatePlane(int i, const SEVector3f& rAverage);
    std::set<int> m_TModified;
};

}

#endif
