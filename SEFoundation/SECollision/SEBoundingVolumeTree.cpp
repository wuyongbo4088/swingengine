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

// #define _DEBUG_TEST

#include "SEFoundationPCH.h"
#include "SEBoundingVolumeTree.h"

using namespace Swing;

BoundingVolumeTree::CreatorM
BoundingVolumeTree::ms_aoCreateModelBound[BoundingVolume::BV_COUNT];

BoundingVolumeTree::CreatorW
BoundingVolumeTree::ms_aoCreateWorldBound[BoundingVolume::BV_COUNT];

//----------------------------------------------------------------------------
BoundingVolumeTree::BoundingVolumeTree(const TriMesh* pMesh)
{
    m_pMesh = pMesh;
    m_pLChild = 0;
    m_pRChild = 0;
    m_iTriangleCount = 0;
    m_aiTriangle = 0;
}
//----------------------------------------------------------------------------
BoundingVolumeTree::BoundingVolumeTree(int eBVType, const TriMesh* pMesh,
    int iMaxTrisPerLeaf, bool bStoreInteriorTris)
    :
    m_pMesh(pMesh)
{
    // ������������������ڲ������.
    // ��Щ���ı�ͶӰ��һ�����ڲ��������������ϲ��洢����.
    // ��ֻ�����ЩͶӰֵ���м�ֵ.
    int iTCount = m_pMesh->IBuffer->GetIndexCount() / 3;
    const int* aiIndex = m_pMesh->IBuffer->GetData();
    Vector3f* aCentroid = SE_NEW Vector3f[iTCount];
    const float fOneThird = 1.0f / 3.0f;
    int iT, i;
    for( iT = 0, i = 0; iT < iTCount; iT++ )
    {
        int i0 = aiIndex[i++];
        int i1 = aiIndex[i++];
        int i2 = aiIndex[i++];
        aCentroid[iT] = fOneThird*(
            pMesh->VBuffer->Position3(i0) +
            pMesh->VBuffer->Position3(i1) +
            pMesh->VBuffer->Position3(i2));
    }

    // ��ʼ�����ڴ洢�����������Ķ���������.
    // �����񱻲��ʱ,��Щ���������洢����������ֵ.
    int* aiISplit = SE_NEW int[iTCount];
    int* aiOSplit = SE_NEW int[iTCount];
    for( iT = 0; iT < iTCount; iT++ )
    {
        aiISplit[iT] = iT;
    }

    BuildTree(eBVType, iMaxTrisPerLeaf, bStoreInteriorTris, aCentroid, 0,
        iTCount - 1, aiISplit, aiOSplit);

    SE_DELETE[] aCentroid;
    SE_DELETE[] aiISplit;
    SE_DELETE[] aiOSplit;

#ifdef _DEBUG_TEST
    if( bStoreInteriorTris )
    {
        const float fEpsilon = 1e-05f;
        bool bSuccess = ContainsLeafData(pMesh->VBuffer, aiIndex, fEpsilon);
        SE_ASSERT( bSuccess );
    }
#endif
}
//----------------------------------------------------------------------------
BoundingVolumeTree::~BoundingVolumeTree()
{
    SE_DELETE[] m_aiTriangle;
    SE_DELETE m_pLChild;
    SE_DELETE m_pRChild;
}
//----------------------------------------------------------------------------
void BoundingVolumeTree::BuildTree(int eBVType, int iMaxTrisPerLeaf,
    bool bStoreInteriorTris, const Vector3f* aCentroid, int i0, int i1,
    int* aiISplit, int* aiOSplit)
{
    SE_ASSERT( i0 <= i1 );

    Line3f tempLine;
    m_spModelBound = ms_aoCreateModelBound[eBVType](m_pMesh, i0, i1, aiISplit,
        tempLine);
    m_spWorldBound = ms_aoCreateWorldBound[eBVType]();

    if( i1 - i0 < iMaxTrisPerLeaf )
    {
        // Ҷ�ӽڵ�.
        m_iTriangleCount = i1 - i0 + 1;
        m_aiTriangle = SE_NEW int[m_iTriangleCount];
        size_t uiSize = m_iTriangleCount * sizeof(int);
        System::SE_Memcpy(m_aiTriangle, uiSize, &aiISplit[i0], uiSize);

        m_pLChild = 0;
        m_pRChild = 0;
    }
    else
    {
        // �ڲ��ڵ�.
        if( bStoreInteriorTris )
        {
            m_iTriangleCount = i1 - i0 + 1;
            m_aiTriangle = SE_NEW int[m_iTriangleCount];
            size_t uiSize = m_iTriangleCount * sizeof(int);
            System::SE_Memcpy(m_aiTriangle, uiSize, &aiISplit[i0], uiSize);
        }
        else
        {
            m_iTriangleCount = 0;
            m_aiTriangle = 0;
        }

        int j0, j1;
        SplitTriangles(aCentroid, i0, i1, aiISplit, j0, j1, aiOSplit, tempLine);

        // ������������������������������������.
        m_pLChild = SE_NEW BoundingVolumeTree(m_pMesh);
        m_pLChild->BuildTree(eBVType, iMaxTrisPerLeaf, bStoreInteriorTris,
            aCentroid, i0, j0, aiOSplit, aiISplit);

        // �ұ����������������������������������.
        m_pRChild = SE_NEW BoundingVolumeTree(m_pMesh);
        m_pRChild->BuildTree(eBVType, iMaxTrisPerLeaf, bStoreInteriorTris,
            aCentroid, j1, i1, aiOSplit, aiISplit);
    }
}
//----------------------------------------------------------------------------
int BoundingVolumeTree::Compare(const void* pvElement0,
    const void* pvElement1)
{
    const ProjectionInfo* pInfo0 = (const ProjectionInfo*)pvElement0;
    const ProjectionInfo* pInfo1 = (const ProjectionInfo*)pvElement1;

    if( pInfo0->m_fProjection < pInfo1->m_fProjection )
    {
        return -1;
    }

    if( pInfo0->m_fProjection > pInfo1->m_fProjection )
    {
        return +1;
    }

    return 0;
}
//----------------------------------------------------------------------------
void BoundingVolumeTree::SplitTriangles(const Vector3f* aCentroid,
    int i0, int i1, int* aiISplit, int& rj0, int& rj1, int* aiOSplit,
    const Line3f& rLine)
{
    // ͶӰ��ָ��ֱ����.
    int iCount = i1 - i0 + 1;
    ProjectionInfo* aInfo = SE_NEW ProjectionInfo[iCount];
    int i, j;
    for( i = i0, j = 0; i <= i1; i++, j++ )
    {
        int iTriangle = aiISplit[i];
        Vector3f vec3fDiff = aCentroid[iTriangle] - rLine.Origin;
        aInfo[j].m_iTriangle = iTriangle;
        aInfo[j].m_fProjection = rLine.Direction.Dot(vec3fDiff);
    }

    // ͨ�������ҵ�����ͶӰֵ���м�ֵ.
    qsort(aInfo, iCount, sizeof(ProjectionInfo), Compare);
    int iMedian = (iCount - 1) / 2;

    // �����м�ֵ,��������������ֻ�.
    for( j = 0, rj0 = i0 - 1; j <= iMedian; j++ )
    {
        aiOSplit[++rj0] = aInfo[j].m_iTriangle;
    }
    for( rj1 = i1 + 1; j < iCount; j++ )
    {
        aiOSplit[--rj1] = aInfo[j].m_iTriangle;
    }

    // ���ֻ�������,[i0,rj0]Ϊ�м�ֵ��ߵ�����,[rj1,i1]Ϊ�м�ֵ�ұߵ�����.

    SE_DELETE[] aInfo;
}
//----------------------------------------------------------------------------
void BoundingVolumeTree::UpdateWorldBound()
{
    m_spModelBound->TransformBy(m_pMesh->World, m_spWorldBound);
}
//----------------------------------------------------------------------------

#ifdef _DEBUG_TEST

bool BoundingVolumeTree::ContainsLeafData(const VertexBuffer* pVBuffer,
    const int* aiIndex, float fEpsilon) const
{
    if( m_pLChild )
    {
        if( !m_pLChild->ContainsLeafData(pVBuffer, aiIndex, fEpsilon) )
        {
            return false;
        }
    }

    if( m_pRChild )
    {
        if( !m_pRChild->ContainsLeafData(pVBuffer, aiIndex, fEpsilon) )
        {
            return false;
        }
    }

    for( int iT = 0; iT < m_iTriangleCount; iT++ )
    {
        int j = 3 * m_aiTriangle[iT];
        for( int i = 0; i < 3; i++ )
        {
            Vector3f vec3fPoint = pVBuffer->Position3(aiIndex[j++]);
            if( !m_spModelBound->Contains(vec3fPoint) )
            {
                return false;
            }
        }
    }

    return true;
}

#endif
