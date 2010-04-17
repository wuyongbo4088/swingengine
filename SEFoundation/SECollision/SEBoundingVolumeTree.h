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

#ifndef Swing_BoundingVolumeTree_H
#define Swing_BoundingVolumeTree_H

#include "SEFoundationLIB.h"
#include "SEBoundingVolume.h"
#include "SELine3.h"
#include "SETriMesh.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:Bounding volume tree����
// ˵��:
// ����:Sun Che
// ʱ��:20081223
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SEBoundingVolumeTree
{
public:
    // �����.
    virtual ~SEBoundingVolumeTree(void);

    // ������.
    inline SEBoundingVolumeTree* GetLChild(void);
    inline SEBoundingVolumeTree* GetRChild(void);
    inline bool IsInteriorNode(void) const;
    inline bool IsLeafNode(void) const;

    // ��Ա����.
    inline const TriMesh* GetMesh(void) const;
    inline const BoundingVolume* GetWorldBound(void) const;
    inline int GetTriangleCount(void) const;
    inline int GetTriangle(int i) const;
    inline const int* GetTriangles(void) const;

    void UpdateWorldBound(void);

protected:
    SEBoundingVolumeTree(const TriMesh* pMesh);
    SEBoundingVolumeTree(int eBVType, const TriMesh* pMesh,
        int iMaxTrisPerLeaf = 1, bool bStoreInteriorTris = false);

    void BuildTree(int eBVType, int iMaxTrisPerLeaf, bool bStoreInteriorTris,
        const SEVector3f* aCentroid, int i0, int i1, int* aiISplit,
        int* aiOSplit);

    static void SplitTriangles(const SEVector3f* aCentroid, int i0, int i1,
        int* aiISplit, int& rj0, int& rj1, int* aiOSplit,
        const SELine3f& rLine);

    // ������β��һ����������:
    // ������������������ڲ������.
    // ��Щ���ı�ͶӰ��һ�����ڲ��������������ϲ��洢����.
    // ��ֻ�����ЩͶӰֵ���м�ֵ.�ö�Ӧ�м�ֵ��������,��������ж��ֻ�.

    // ����������������ͶӰ���ϵ�ͶӰ��Ŀ�������.
    class SE_FOUNDATION_API ProjectionInfo
    {
    public:
        int m_iTriangle;
        float m_fProjection;
    };
    static int Compare(const void* pvElement0, const void* pvElement1);

    // ģ�Ϳռ�BV�Ĺ���������.
    typedef BoundingVolume* (*CreatorM)(const TriMesh*, int, int, int*, SELine3f&);
    static CreatorM ms_aoCreateModelBound[BoundingVolume::BV_COUNT];

    // ����ռ�BV�Ĺ���������.
    typedef BoundingVolume* (*CreatorW)(void);
    static CreatorW ms_aoCreateWorldBound[BoundingVolume::BV_COUNT];

    // ������BV.
    const TriMesh* m_pMesh;
    BoundingVolumePtr m_spModelBound;
    BoundingVolumePtr m_spWorldBound;

    // ��������ʾ.
    SEBoundingVolumeTree* m_pLChild;
    SEBoundingVolumeTree* m_pRChild;

    // ���캯�������bStoreInteriorTris����Ϊfalse,
    // �������ڲ��ڵ�����m_iTriangleCountΪ��,����m_aiTriangleΪ��.
    // ����Ҷ�ӽڵ�洢ʵ�������ĸýڵ���������(iMaxTrianglesPerLeaf����Ϊ1ʱ,
    // ��ʵ������ҲΪ1),���ҷ�����֮��Ӧ��С������,���ڴ洢��Ӧ�������ڶ���
    // ���캯������ĳ�ʼ�����е�����ֵ.
    //
    // ���bStoreInteriorTris����Ϊtrue,
    // �������ڲ��ڵ�Ҳ���洢�����Ӧ����Ӧ����������������ֵ����.
    int m_iTriangleCount;
    int* m_aiTriangle;

#ifdef _DEBUG_TEST
    // ���ÿ���ڵ�����ʾ���������ȫ�������Ƿ��ڸýڵ��ģ�Ϳռ�BV��.
    // ֻ�ж�����_DEBUG_TEST��bStoreInteriorTrisΪtrueʱ����Ч.
    bool ContainsLeafData(const VertexBuffer* pVBuffer, const int* aiIndex,
        float fEpsilon) const;
#endif
};

#include "SEBoundingVolumeTree.inl"

}

#endif
