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

#include "SEPhysicsPCH.h"
#include "SEExtremalQuery3BSP.h"
#include "SEBasicMesh.h"

using namespace Swing;

//----------------------------------------------------------------------------
ExtremalQuery3BSPf::ExtremalQuery3BSPf(const ConvexPolyhedron3f& rPolytope)
    :
    ExtremalQuery3f(rPolytope)
{
    // Ϊ�����崴���ڽ���Ϣ.
    int iVCount = m_pPolytope->GetVCount();
    const Vector3f* aVertex = m_pPolytope->GetVertices();
    int iTCount = m_pPolytope->GetTCount();
    const int* aiIndex = m_pPolytope->GetIndices();
    BasicMesh tempMesh(iVCount, aVertex, iTCount, aiIndex);

    // ����spherical arc����,���ڴ���BSP��.
    std::multiset<SphericalArc> tempArcs;
    CreateSphericalArcs(tempMesh, tempArcs);

    // ����BSP��,���ڼ�ֵ��ѯ.
    std::vector<SphericalArc> tempNodes;
    CreateBSPTree(tempArcs, tempNodes);

    // ��BSP�����Ƶ��̶���С�ļ�������.
    m_iNodeCount = (int)tempNodes.size();
    m_aNode = SE_NEW SphericalArc[m_iNodeCount];
    size_t uiSize = m_iNodeCount * sizeof(SphericalArc);
    SESystem::SE_Memcpy(m_aNode, uiSize, &tempNodes.front(), uiSize);
}
//----------------------------------------------------------------------------
ExtremalQuery3BSPf::~ExtremalQuery3BSPf()
{
    SE_DELETE[] m_aNode;
}
//----------------------------------------------------------------------------
void ExtremalQuery3BSPf::GetExtremeVertices(const Vector3f& rDirection, 
    int& riPositiveDirection, int& riNegativeDirection)
{
    // �ǵݹ�������Ȳ���BSP��,�жϰ���ָ����������D����������.
    int iCurrent = 0;  // BSP����
    while( iCurrent >= 0 )
    {
        SphericalArc& rCurrent = m_aNode[iCurrent];
        int iSign = (int)Mathf::Sign(rDirection.Dot(rCurrent.Normal));
        if( iSign >= 0 )
        {
            iCurrent = rCurrent.PosChild;
            if( iCurrent == -1 )
            {
                // ����Ҷ�ӽڵ�.
                riPositiveDirection = rCurrent.PosVertex;
            }
        }
        else
        {
            iCurrent = rCurrent.NegChild;
            if( iCurrent == -1 )
            {
                // ����Ҷ�ӽڵ�.
                riPositiveDirection = rCurrent.NegVertex;
            }
        }
    }

    // �ǵݹ�������Ȳ���BSP��,�жϰ���ָ����������D�ķ�������-D����������.
    iCurrent = 0;  // BSP����
    while( iCurrent >= 0 )
    {
        SphericalArc& rCurrent = m_aNode[iCurrent];
        int iSign = (int)-Mathf::Sign(rDirection.Dot(rCurrent.Normal));
        if( iSign >= 0 )
        {
            iCurrent = rCurrent.PosChild;
            if( iCurrent == -1 )
            {
                // ����Ҷ�ӽڵ�.
                riNegativeDirection = rCurrent.PosVertex;
            }
        }
        else
        {
            iCurrent = rCurrent.NegChild;
            if( iCurrent == -1 )
            {
                // ����Ҷ�ӽڵ�.
                riNegativeDirection = rCurrent.NegVertex;
            }
        }
    }
}
//----------------------------------------------------------------------------
int ExtremalQuery3BSPf::GetNodeCount() const
{
    return m_iNodeCount;
}
//----------------------------------------------------------------------------
int ExtremalQuery3BSPf::GetTreeDepth() const
{
    return m_iTreeDepth;
}
//----------------------------------------------------------------------------
void ExtremalQuery3BSPf::SortVertexAdjacents(BasicMesh& rMesh)
{
    // ָ���typecast������Զ�����������޸�.
    // ֻҪ�����㷨��ȷ,�޸Ķ���������ǰ�ȫ��.
    int iVCount = rMesh.GetVCount();
    BasicMesh::Vertex* aVertex = (BasicMesh::Vertex*)rMesh.GetVertices();

    const BasicMesh::Triangle* aTriangle = rMesh.GetTriangles();
    for( int i = 0; i < iVCount; i++ )
    {
        // This copy circumvents the constness of the mesh vertices, which
        // allows the sorting of the mesh triangles shared by a mesh vertex.
        BasicMesh::Vertex& rVertex = aVertex[i];

        // ���񹹳ɶ�����ʱ����������:�ö�����ڽӶ����������ڽ���������.
        SE_ASSERT( rVertex.VCount == rVertex.TCount );

        // һ���������˵�һ��Ҫ�����ڽ���Ϣ����Ķ�������ʼ�ڽ�������,
        // ���ǾͿ��ԴӸ��ڽ����������,Χ�Ÿö���"��һȦ",
        // ����ķ��ʵ��ö���������ڽ�������.
        // ���������Ķ�����������˳ʱ��,���ڽ����������η���Ҳ����˳ʱ��˳��.
        // Ҳ����˵�����������̵�ʱ����,ȡ�������ǵ���ѧϵͳ��������.
        // ����������,�Զ����ڽ���Ϣ�ĸ����ǰ�ȫ��,
        // ��Ϊ���ǵ�������Դȡ�������ڽ�������.
        int iTIndex = rVertex.T[0];
        const BasicMesh::Triangle* pTri = &aTriangle[iTIndex];

        for( int iAdj = 0; iAdj < rVertex.VCount; iAdj++ )
        {
            int iPrev, iCurr;
            for( iPrev = 2, iCurr = 0; iCurr < 3; iPrev = iCurr++ )
            {
                if( pTri->V[iCurr] == i )
                {
                    rVertex.V[iAdj] = pTri->V[iPrev];
                    rVertex.E[iAdj] = pTri->E[iPrev];
                    rVertex.T[iAdj] = iTIndex;

                    // ָ�򼴽����ʵ���һ���ڽ�������.
                    iTIndex = pTri->T[iPrev];
                    pTri = &aTriangle[iTIndex];

                    break;
                }
            }

            SE_ASSERT( iCurr < 3 );
        }
    }
}
//----------------------------------------------------------------------------
void ExtremalQuery3BSPf::CreateSphericalArcs(BasicMesh& rMesh,
    std::multiset<SphericalArc>& rArcs)
{
    int iECount = rMesh.GetECount();
    const BasicMesh::Edge* aEdge = rMesh.GetEdges();
    const BasicMesh::Triangle* aTriangle = rMesh.GetTriangles();

    const int aiPrev[3] = { 2, 0, 1 };
    const int aiNext[3] = { 1, 2, 0 };

    for( int i = 0; i < iECount; i++ )
    {
        const BasicMesh::Edge& rEdge = aEdge[i];

        SphericalArc tempArc;
        tempArc.N[0] = rEdge.T[0];
        tempArc.N[1] = rEdge.T[1];
        tempArc.Separation = 1;

        tempArc.Normal = m_aFaceNormal[tempArc.N[0]].Cross(
            m_aFaceNormal[tempArc.N[1]]);

        const BasicMesh::Triangle& rAdj = aTriangle[rEdge.T[0]];
        int j;
        for( j = 0; j < 3; j++ )
        {
            if( rAdj.V[j] != rEdge.V[0] && rAdj.V[j] != rEdge.V[1] )
            {
                tempArc.PosVertex = rAdj.V[aiPrev[j]];
                tempArc.NegVertex = rAdj.V[aiNext[j]];

                break;
            }
        }

        SE_ASSERT( j < 3 );

        rArcs.insert(tempArc);
    }

    CreateSphericalBisectors(rMesh, rArcs);
}
//----------------------------------------------------------------------------
void ExtremalQuery3BSPf::CreateSphericalBisectors(BasicMesh& rMesh,
    std::multiset<SphericalArc>& rArcs)
{
    // ���ÿ������,�����ڽ���Ϣ(��������ڽ������淨��)��������,
    // ʹ�������ڽ���Ϣ�õ����ڽ������淨��,
    // ��spherical dual�Ϲ���˳ʱ��˳�����������(������ۿ�).
    SortVertexAdjacents(rMesh);

    int iVCount = rMesh.GetVCount();
    const BasicMesh::Vertex* aVertex = rMesh.GetVertices();
    std::queue<std::pair<int, int> > tempQueue;
    for( int i = 0; i < iVCount; i++ )
    {
        const BasicMesh::Vertex& rVertex = aVertex[i];

        tempQueue.push(std::make_pair(0, rVertex.TCount));
        while( !tempQueue.empty() )
        {
            std::pair<int, int> tempArc = tempQueue.front();
            tempQueue.pop();
            int i0 = tempArc.first, i1 = tempArc.second;
            int iSeparation = i1 - i0;
            if( iSeparation > 1 && iSeparation != rVertex.TCount-1 )
            {
                if( i1 < rVertex.TCount )
                {
                    SphericalArc tempArc;
                    tempArc.N[0] = rVertex.T[i0]; // �����
                    tempArc.N[1] = rVertex.T[i1]; // �����
                    tempArc.Separation = iSeparation;

                    tempArc.Normal = m_aFaceNormal[tempArc.N[0]].Cross(
                        m_aFaceNormal[tempArc.N[1]]);

                    tempArc.PosVertex = i;
                    tempArc.NegVertex = i;
                    rArcs.insert(tempArc);
                }
                int iMid = (i0 + i1 + 1)/2;
                if( iMid != i1 )
                {
                    tempQueue.push(std::make_pair(i0, iMid));
                    tempQueue.push(std::make_pair(iMid, i1));
                }
            }
        }
    }
}
//----------------------------------------------------------------------------
void ExtremalQuery3BSPf::CreateBSPTree(std::multiset<SphericalArc>& rArcs, 
    std::vector<SphericalArc>& rNodes)
{
    // ������һ�������ڵ�.
    m_iTreeDepth = 1;

    std::multiset<SphericalArc>::reverse_iterator tempIter;
    for( tempIter = rArcs.rbegin(); tempIter != rArcs.rend(); tempIter++ )
    {
        InsertArc(*tempIter, rNodes);
    }

    // �ڱ���multiset����InsertArc����ʱ,Ҷ�ӽڵ�û�в�����ȼ���.
    // ��˱���������,����һ����ȼ���.
    m_iTreeDepth++;
}
//----------------------------------------------------------------------------
void ExtremalQuery3BSPf::InsertArc(const SphericalArc& rArc,
    std::vector<SphericalArc>& rNodes)
{
    // �����arc���洢�ڽڵ��������.
    if( rNodes.size() > 0 )
    {
        // Ϊ�˷��ô���arc,�Ե�ǰBSP�����ǵݹ�������Ȳ���.
        std::stack<int> tempCandidates;
        tempCandidates.push(0);  // ��BSP������ʼ
        while( !tempCandidates.empty() )
        {
            int iCurrent = tempCandidates.top();
            tempCandidates.pop();
            SphericalArc* pCurrent = &rNodes[iCurrent];

            int iSign0;
            if( rArc.N[0] == pCurrent->N[0]
            ||  rArc.N[0] == pCurrent->N[1] )
            {
                iSign0 = 0;
            }
            else
            {
                iSign0 = (int)Mathf::Sign(
                    m_aFaceNormal[rArc.N[0]].Dot(pCurrent->Normal));
            }

            int iSign1;
            if( rArc.N[1] == pCurrent->N[0]
            ||  rArc.N[1] == pCurrent->N[1] )
            {
                iSign1 = 0;
            }
            else
            {
                iSign1 = (int)Mathf::Sign(
                    m_aFaceNormal[rArc.N[1]].Dot(pCurrent->Normal));
            }

            int iDoTest = 0;
            if( iSign0*iSign1 < 0 )
            {
                // ����arc�뵱ǰarc�ཻ,��˰Ѵ���arcͬʱ���ݸ������ӽڵ�.
                iDoTest = 3;
            }
            else if( iSign0 > 0 || iSign1 > 0 )
            {
                // ����arc�ڵ�ǰarc��������ռ�.
                iDoTest = 1;
            }
            else if( iSign0 < 0 || iSign1 < 0 )
            {
                // ����arc�ڵ�ǰarc��������ռ�.
                iDoTest = 2;
            }
            // ���� iSign0 = iSign1 = 0,����������arc�뵱ǰarc����.
            // ��û�б�Ҫ�����arc��.

            int iDepth;

            if( iDoTest & 1 )
            {
                if( pCurrent->PosChild != -1 )
                {
                    tempCandidates.push(pCurrent->PosChild);
                    iDepth = (int)tempCandidates.size();
                    if( iDepth > m_iTreeDepth )
                    {
                        m_iTreeDepth = iDepth;
                    }
                }
                else
                {
                    pCurrent->PosChild = (int)rNodes.size();
                    rNodes.push_back(rArc);

                    // push_back�������vector���·����ڴ�,���ˢ��currentָ��.
                    pCurrent = &rNodes[iCurrent];
                }
            }

            if( iDoTest & 2 )
            {
                if( pCurrent->NegChild != -1 )
                {
                    tempCandidates.push(pCurrent->NegChild);
                    iDepth = (int)tempCandidates.size();
                    if( iDepth > m_iTreeDepth )
                    {
                        m_iTreeDepth = iDepth;
                    }
                }
                else
                {
                    pCurrent->NegChild = (int)rNodes.size();
                    rNodes.push_back(rArc);
                }
            }
        }
    }
    else
    {
        // ���ڵ�.
        rNodes.push_back(rArc);
    }
}
//----------------------------------------------------------------------------
