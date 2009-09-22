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

#ifndef Swing_ExtremalQuery3BSP_H
#define Swing_ExtremalQuery3BSP_H

#include "SEPhysicsLIB.h"
#include "SEExtremalQuery3.h"

namespace Swing
{

class BasicMesh;

class SE_PHYSICS_API ExtremalQuery3BSPf : public ExtremalQuery3f
{
public:
    ExtremalQuery3BSPf(const ConvexPolyhedron3f& rPolytope);
    virtual ~ExtremalQuery3BSPf(void);

    // ����ָ������,����������ڸ÷����ϵļ�ֵ����,
    // ��������Щ�����ڶ��������е�����.
    virtual void GetExtremeVertices(const Vector3f& rDirection,
        int& riPositiveDirection, int& riNegativeDirection);

    // tree statistics.
    int GetNodeCount(void) const;
    int GetTreeDepth(void) const;

private:
    class SE_PHYSICS_API SphericalArc
    {
    public:
        SphericalArc()
        {
            PosChild = -1;
            NegChild = -1;
        }

        // ��Ӧarc���˵�������淨�ߵ�����.
        int N[2];

        // �ӷ���N[0]���﷨��N[1]��·����������arc����.
        // �����������εı�arc,��ֵΪ1.
        // �����������ε�bisector arc,��ֵ���ڵ���2.
        int Separation;

        // �÷���Ϊ:Cross(FaceNormal[N[0]], FaceNormal[N[1]]).
        Vector3f Normal;

        // ������arc�������������δ�����Ը�arc������Ǳ�ڼ�ֵ����.
        // ����arc��N[0]�˵���N[1]�˵��ƶ�ʱ,��������ԭ��,
        // arc���ֱߵ�Ǳ�ڼ�ֵ����,��PosVertex����.
        // arc���ֱߵ�Ǳ�ڼ�ֵ����,��NegVertex����.
        int PosVertex, NegVertex;

        // BSP���ӽڵ�����.
        int PosChild, NegChild;

        // ʵ�������<������,�Ӷ����������ͱ�����stl��multiset.
        // ȫ��arc����separationֵ,������洢��multiset��.
        // ����multiset����BSP������ʱ,�������,separationֵ������ȴ���.
        // ��������ʽ�㷨ȷ����BSP����ƽ����,
        // �Ӷ��ڼ�ֵ��ѯʱ,����ڵ㾡���ܶ���޳�������arc.
        bool operator < (const SphericalArc& rArc) const
        {
            return Separation < rArc.Separation;
        }
    };

    void SortVertexAdjacents(BasicMesh& rMesh);
    void CreateSphericalArcs(BasicMesh& rMesh,
        std::multiset<SphericalArc>& rArcs);
    void CreateSphericalBisectors(BasicMesh& rMesh,
        std::multiset<SphericalArc>& rArcs);

    void CreateBSPTree(std::multiset<SphericalArc>& rArcs,
        std::vector<SphericalArc>& rNodes);
    void InsertArc(const SphericalArc& rArc,
        std::vector<SphericalArc>& rNodes);

    // BSP�ڵ�Ĺ̶���С�洢�ռ�.
    int m_iNodeCount;
    SphericalArc* m_aNode;
    int m_iTreeDepth;
};

}

#endif