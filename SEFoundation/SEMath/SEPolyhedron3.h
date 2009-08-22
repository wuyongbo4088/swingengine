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

#ifndef Swing_Polyhedron3_H
#define Swing_Polyhedron3_H

#include "SEFoundationLIB.h"
#include "SEVector3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:Polyhedron��
// ˵��:
// ����:Sun Che
// ʱ��:20081124
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Polyhedron3f
{
public:
    // simple polyhedron:ÿ��edge��౻���������ι���,������û�����ཻ�����.

    // ������������ȷ����������ݹ���simple polyhedron.
    // �����ۿ�meshʱ,�����������˳��ӦΪ˳ʱ�뷽��.
    // bOwnerΪtrueʱ,polyhedronӵ�д������������Ȩ.
    // ����Ϊfalseʱ,�ɵ����߸���ɾ����������.
    Polyhedron3f(int iVCount, Vector3f* aVertex, int iTCount,
        int* aiIndex, bool bOwner);

    // ��������polyhedronӵ������������,��Ϊ'this'����������������,
    // �Ӷ�ʹ'this'����Ҳӵ������������.
    // ��������polyhedron��ӵ������������,��'this'����Ҳ��ӵ��.ֻ����ָ�빲��.
    Polyhedron3f(const Polyhedron3f& rPoly);

    virtual ~Polyhedron3f(void);

    // ��������polyhedronӵ������������,��Ϊ'this'����������������,
    // �Ӷ�ʹ'this'����Ҳӵ������������.
    // ��������polyhedron��ӵ������������,��'this'����Ҳ��ӵ��.ֻ����ָ�빲��.
    Polyhedron3f& operator = (const Polyhedron3f& rPoly);

    int GetVCount(void) const;
    const Vector3f* GetVertices(void) const;
    const Vector3f& GetVertex(int i) const;
    int GetTCount(void) const;
    const int* GetIndices(void) const;
    const int* GetTriangle(int i) const;

    // ����Զ�������޸�.
    // ���������κμ�������У��,����֤����simple polyhedron.
    virtual void SetVertex(int i, const Vector3f& rV);
    Vector3f* GetVertices(void);

    // ����polyhedron�ĸ��������Ϣ.
    Vector3f ComputeVertexAverage(void) const; // ƽ����������
    float ComputeSurfaceArea(void) const; // �����
    float ComputeVolume(void) const; // ���

protected:
    int m_iVCount;
    Vector3f* m_aVertex;
    int m_iTCount;
    int* m_aiIndex;
    bool m_bOwner;
};

}

#endif
