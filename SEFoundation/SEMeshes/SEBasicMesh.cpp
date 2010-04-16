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

#include "SEFoundationPCH.h"
#include "SEBasicMesh.h"
#include "SEEdgeKey.h"

using namespace Swing;

//----------------------------------------------------------------------------
BasicMesh::BasicMesh(int iVCount, const void* aPoint, int iTCount,
    const int* aiIndex)
{
    SE_ASSERT( iVCount > 0 && iTCount > 0 && aiIndex );

    if( iVCount <= 0 || iTCount <= 0 || !aiIndex )
    {
        m_iVCount = 0;
        m_iECount = 0;
        m_iTCount = 0;
        m_aVertex = 0;
        m_aEdge = 0;
        m_aTriangle = 0;
        m_aPoint = 0;
        m_aiIndex = 0;
        m_bIsValid = false;

        return;
    }

    m_iVCount = iVCount;
    m_iECount = 0;
    m_iTCount = iTCount;
    m_aPoint = aPoint;
    m_aiIndex = aiIndex;
    m_bIsValid = true;

    // ���ݴ������ݶ�̬����triangle mesh.
    m_aVertex = SE_NEW Vertex[m_iVCount];
    m_aEdge = SE_NEW Edge[3*m_iTCount];
    m_aTriangle = SE_NEW Triangle[m_iTCount];
    std::map<EdgeKey, int> tempEMap;
    for( int iT = 0; iT < m_iTCount; iT++ )
    {
        // ����triangle.
        Triangle& rTriangle = m_aTriangle[iT];
        rTriangle.V[0] = *aiIndex++;
        rTriangle.V[1] = *aiIndex++;
        rTriangle.V[2] = *aiIndex++;

        // Ϊmesh���edges.
        for( int i0 = 2, i1 = 0; i1 < 3; i0 = i1++ )
        {
            // ����vertices.
            m_aVertex[rTriangle.V[i1]].InsertTriangle(iT);

            EdgeKey tempKey(rTriangle.V[i0], rTriangle.V[i1]);
            std::map<EdgeKey, int>::iterator tempEIter = tempEMap.find(tempKey);
            if( tempEIter == tempEMap.end() )
            {
                // ��һ��������edge.
                tempEMap[tempKey] = m_iECount;

                // ����edge.
                Edge& rEdge = m_aEdge[m_iECount];
                rEdge.V[0] = rTriangle.V[i0];
                rEdge.V[1] = rTriangle.V[i1];
                rEdge.T[0] = iT;

                // ����vertices.
                m_aVertex[rEdge.V[0]].InsertEdge(rEdge.V[1], m_iECount);
                m_aVertex[rEdge.V[1]].InsertEdge(rEdge.V[0], m_iECount);

                // ����triangle.
                rTriangle.E[i0] = m_iECount;

                m_iECount++;
            }
            else
            {
                // �ڶ���������edge.
                int iE = tempEIter->second;
                Edge& rEdge = m_aEdge[iE];

                // ����edge.
                SE_ASSERT( rEdge.T[1] == -1 );  // ֻ�ܱ����������ι���
                if( rEdge.T[1] != -1 )
                {
                    SE_DELETE[] m_aVertex;
                    SE_DELETE[] m_aEdge;
                    SE_DELETE[] m_aTriangle;
                    m_iVCount = 0;
                    m_iECount = 0;
                    m_iTCount = 0;
                    m_aVertex = 0;
                    m_aEdge = 0;
                    m_aTriangle = 0;
                    m_aPoint = 0;
                    m_aiIndex = 0;
                    m_bIsValid = false;
                }
                rEdge.T[1] = iT;

                // ����triangles.
                int iAdj = rEdge.T[0];
                Triangle& rAdj = m_aTriangle[iAdj];
                for( int j = 0; j < 3; j++ )
                {
                    if( rAdj.E[j] == iE )
                    {
                        rAdj.T[j] = iT;
                        break;
                    }
                }
                rTriangle.E[i0] = iE;
                rTriangle.T[i0] = iAdj;
            }
        }
    }
}
//----------------------------------------------------------------------------
BasicMesh::~BasicMesh()
{
    SE_DELETE[] m_aVertex;
    SE_DELETE[] m_aEdge;
    SE_DELETE[] m_aTriangle;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// BasicMesh::Vertex
//----------------------------------------------------------------------------
BasicMesh::Vertex::Vertex()
{
    VCount = 0;
    V = 0;
    E = 0;
    TCount = 0;
    T = 0;
}
//----------------------------------------------------------------------------
BasicMesh::Vertex::~Vertex()
{
    SE_DELETE[] V;
    SE_DELETE[] E;
    SE_DELETE[] T;
}
//----------------------------------------------------------------------------
void BasicMesh::Vertex::InsertEdge(int iV, int iE)
{
    // ����µ����ڶ���/��.

    // ������ڶ����Ƿ��Ѿ�����,
    // �Ӷ��ж��Ƿ��Ѿ��������edge.
    for( int i = 0; i < VCount; i++ )
    {
        if( iV == V[i] )
        {
            return;
        }
    }

    if( (VCount % MV_CHUNK) == 0 )
    {
        size_t uiDstSize = (VCount + MV_CHUNK)*sizeof(int);
        size_t uiSrcSize = VCount * sizeof(int);

        int* aiSave = V;
        V = SE_NEW int[VCount + MV_CHUNK];
        if( aiSave )
        {
            SESystem::SE_Memcpy(V, uiDstSize, aiSave, uiSrcSize);
            SE_DELETE[] aiSave;
        }

        aiSave = E;
        E = SE_NEW int[VCount + MV_CHUNK];
        if( aiSave )
        {
            SESystem::SE_Memcpy(E, uiDstSize, aiSave, uiSrcSize);
            SE_DELETE[] aiSave;
        }
    }

    V[VCount] = iV;
    E[VCount] = iE;
    VCount++;
}
//----------------------------------------------------------------------------
void BasicMesh::Vertex::InsertTriangle(int iT)
{
    // ����µ�����������.

    // ��������������Ƿ��Ѿ�����.
    for( int i = 0; i < TCount; i++ )
    {
        if( iT == T[i] )
        {
            return;
        }
    }

    if( (TCount % MV_CHUNK) == 0 )
    {
        int* aiSave = T;
        T = SE_NEW int[TCount + MV_CHUNK];
        if( aiSave )
        {
            size_t uiDstSize = (TCount + MV_CHUNK)*sizeof(int);
            size_t uiSrcSize = TCount * sizeof(int);
            SESystem::SE_Memcpy(T, uiDstSize, aiSave, uiSrcSize);
            SE_DELETE[] aiSave;
        }
    }

    T[TCount] = iT;
    TCount++;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// BasicMesh::Edge
//----------------------------------------------------------------------------
BasicMesh::Edge::Edge()
{
    for( int i = 0; i < 2; i++ )
    {
        V[i] = -1;
        T[i] = -1;
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// BasicMesh::Triangle
//----------------------------------------------------------------------------
BasicMesh::Triangle::Triangle()
{
    for( int i = 0; i < 3; i++ )
    {
        V[i] = -1;
        E[i] = -1;
        T[i] = -1;
    }
}
//----------------------------------------------------------------------------
