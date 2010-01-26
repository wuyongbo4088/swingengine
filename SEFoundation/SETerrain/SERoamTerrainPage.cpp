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
#include "SERoamTerrainPage.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, RoamTerrainPage, TriMesh);
SE_IMPLEMENT_STREAM(RoamTerrainPage);
SE_IMPLEMENT_DEFAULT_NAME_ID(RoamTerrainPage, TriMesh);

//SE_REGISTER_STREAM(RoamTerrainPage);

//float RoamTerrainPage::FrameVariance = 50.0f;
int RoamTerrainPage::DesiredTris = 20000;
int RoamTerrainPage::NumTrisRendered = 0;

//----------------------------------------------------------------------------
RoamTerrainPage::RoamTerrainPage(const Attributes& rAttr, int iSize,
    unsigned short* ausHeight, const Vector2f& rOrigin,
    float fMinElevation, float fMaxElevation, float fSpacing,
    float fUVBias, int iPoolSize, int iPatchSize, int iSplitLevel, 
    int iVarianceLevel)
    :
    m_Origin(rOrigin)
{
    SE_ASSERT( iSize == 33 || iSize == 65 || iSize == 129 );

    m_iSize = iSize;
    m_ausHeight = ausHeight;
    m_fMinElevation = fMinElevation;
    m_fMaxElevation = fMaxElevation;
    m_fSpacing = fSpacing;
    m_iPoolSize = iPoolSize;
    m_iPatchSize = iPatchSize;
    m_iSplitLevel = iSplitLevel;
    m_iVarianceLevel = iVarianceLevel;
    FrameVariance = 50.0f;
    m_iNextTriNode = 0;

    // �Ժ�����캯������,��RoamTerrain�������SetPageGrid��������.
    m_iRow = -1;
    m_iCol = -1;
    m_pTerrain = 0;

    InitializeDerivedData();

    // ����mesh.
    StandardMesh tempSM(rAttr);

    float fExtent = m_fSpacing * m_iSizeM1;
    TriMesh* pMesh = tempSM.Rectangle(m_iSize, m_iSize, fExtent, fExtent);
    VBuffer = pMesh->VBuffer;
    IBuffer = pMesh->IBuffer;

    m_fUVBias = fUVBias;
    SE_DELETE pMesh;

    // �޸Ķ��㲢ʹ�ø߶�ͼ����.
    int iVCount = VBuffer->GetVertexCount();
    for( int i = 0; i < iVCount; i++ )
    {
        int iX = i % m_iSize;
        int iZ = i / m_iSize;
        VBuffer->Position3(i) = Vector3f(GetX(iX), GetHeight(i), GetZ(iZ));
    }

    UpdateMS();
}
//----------------------------------------------------------------------------
RoamTerrainPage::RoamTerrainPage()
    :
    m_Origin(Vector2f::ZERO)
{
    m_iSize = 0;
    m_ausHeight = 0;
    m_fMinElevation = 0.0f;
    m_fMaxElevation = 0.0f;
    m_fSpacing = 0.0f;
    m_iPoolSize = 0;
    m_iPatchSize = 0;
    m_iSplitLevel = 0;
    m_iVarianceLevel = 0;
    FrameVariance = 50.0f;
    m_fUVBias = 0.0f;
    m_iNextTriNode = 0;

    // ͨ��SetPageGrid��������.
    m_iRow = -1;
    m_iCol = -1;
    m_pTerrain = 0;

    // derived.
    m_iSizeM1 = 0;
    m_iPatchSizeM1 = 0;
    m_fInvSpacing = 0.0f;
    m_fTextureSpacing = 0.0f;
    m_fMultiplier = 0.0f;
    m_iPatchesPerSide = 0;
    m_iMaxIndexCount = 0;
    m_bNeedsTessellation = false;
    m_bNeedsResetIB = false;
}
//----------------------------------------------------------------------------
RoamTerrainPage::~RoamTerrainPage()
{
    SE_DELETE[] m_ausHeight;
}
//----------------------------------------------------------------------------
float RoamTerrainPage::GetHeight(float fX, float fZ) const
{
    float fTempHeight = 0.0f;
    // ������ռ�X,Z�任��terrain�ռ�.
    float fTerrainMSpaceX = fX - World.GetTranslate().X;
    float fTerrainMSpaceZ = fZ - World.GetTranslate().Z;
    // ��terrain�ռ�X,Z�任��terrain page�ռ�.
    float fMSpaceX = fTerrainMSpaceX - m_Origin.X;
    float fMSpaceZ = fTerrainMSpaceZ - m_Origin.Y;
    fMSpaceX *= m_fInvSpacing;
    fMSpaceZ *= m_fInvSpacing;

    SE_ASSERT( m_pTerrain );

    if( m_pTerrain->GetLod() )
    {
        // ��ȡ��������patch.
        float fInvPatchSizeM1 = 1.0f / (float)m_iPatchSizeM1;
        int iPatchX = (int)Mathf::Floor(fMSpaceX * fInvPatchSizeM1);
        int iPatchY = (int)Mathf::Floor(fMSpaceZ * fInvPatchSizeM1);
        if( iPatchX >= m_iPatchesPerSide )
            iPatchX = m_iPatchesPerSide - 1;
        if( iPatchX < 0 )
            iPatchX = 0;
        if( iPatchY >= m_iPatchesPerSide )
            iPatchY = m_iPatchesPerSide - 1;
        if( iPatchY < 0 )
            iPatchY = 0;
    
        // �ж�����base triangle.
        float fPatchLBX = (float)(iPatchX * m_iPatchSizeM1);
        float fPatchLBY = (float)(iPatchY * m_iPatchSizeM1);
        float fPatchRBX = fPatchLBX + (float)m_iPatchSizeM1;
        float fPatchRBY = fPatchLBY;
        float fdX = fMSpaceX - fPatchRBX;
        float fdY = fMSpaceZ - fPatchRBY;

        RoamTriTreeNode* pCurTriangle = 0;
        if( -fdX >= fdY )
        {
            pCurTriangle = m_Patches[iPatchY][iPatchX].GetBaseLeft();
        }
        else
        {
            pCurTriangle = m_Patches[iPatchY][iPatchX].GetBaseRight();
        }

        Vector2f Apex, Left, Right, CurPoint;
        Vector2f A, B, C;
        float fCdA, fCdB;

        CurPoint.X = fTerrainMSpaceX;
        CurPoint.Y = fTerrainMSpaceZ;

        // ��ȡ��ǰ������Сtriangle.
        while( pCurTriangle->pLeftChild != 0 )
        {
            Apex.X = (*(Vector3f*)VBuffer->PositionTuple(pCurTriangle->ApexIndex)).X;
            Apex.Y = (*(Vector3f*)VBuffer->PositionTuple(pCurTriangle->ApexIndex)).Z;
            Left.X = (*(Vector3f*)VBuffer->PositionTuple(pCurTriangle->LeftIndex)).X;
            Left.Y = (*(Vector3f*)VBuffer->PositionTuple(pCurTriangle->LeftIndex)).Z;
            Right.X = (*(Vector3f*)VBuffer->PositionTuple(pCurTriangle->RightIndex)).X;
            Right.Y = (*(Vector3f*)VBuffer->PositionTuple(pCurTriangle->RightIndex)).Z;

            A = Left - Apex;
            B = Right - Apex;
            C = CurPoint - Apex;
            A.Normalize();
            B.Normalize();

            fCdA = C.Dot(A);
            fCdB = C.Dot(B);

            if( fCdA >= fCdB )
            {
                pCurTriangle = pCurTriangle->pLeftChild;
            }
            else
            {
                pCurTriangle = pCurTriangle->pRightChild;
            }
        }

        // ��ֵ�������ڵ�߶�.
        Apex.X = (*(Vector3f*)VBuffer->PositionTuple(pCurTriangle->ApexIndex)).X;
        Apex.Y = (*(Vector3f*)VBuffer->PositionTuple(pCurTriangle->ApexIndex)).Z;
        Left.X = (*(Vector3f*)VBuffer->PositionTuple(pCurTriangle->LeftIndex)).X;
        Left.Y = (*(Vector3f*)VBuffer->PositionTuple(pCurTriangle->LeftIndex)).Z;
        Right.X = (*(Vector3f*)VBuffer->PositionTuple(pCurTriangle->RightIndex)).X;
        Right.Y = (*(Vector3f*)VBuffer->PositionTuple(pCurTriangle->RightIndex)).Z;

        A = Left - Apex;
        B = Right - Apex;
        C = CurPoint - Apex;
        float fCurTriSpacing = A.GetLength();
        float fInvCurTriSpacing = 1.0f / fCurTriSpacing;
        A.Normalize();
        B.Normalize();

        fCdA = C.Dot(A);
        fCdB = C.Dot(B);

        Vector3f Apex3, Left3, Right3, CurPoint3;
        Apex3.X = (*(Vector3f*)VBuffer->PositionTuple(pCurTriangle->ApexIndex)).X;
        Apex3.Y = (*(Vector3f*)VBuffer->PositionTuple(pCurTriangle->ApexIndex)).Z;
        Apex3.Z = (*(Vector3f*)VBuffer->PositionTuple(pCurTriangle->ApexIndex)).Y;
        Left3.X = (*(Vector3f*)VBuffer->PositionTuple(pCurTriangle->LeftIndex)).X;
        Left3.Y = (*(Vector3f*)VBuffer->PositionTuple(pCurTriangle->LeftIndex)).Z;
        Left3.Z = (*(Vector3f*)VBuffer->PositionTuple(pCurTriangle->LeftIndex)).Y;
        Right3.X = (*(Vector3f*)VBuffer->PositionTuple(pCurTriangle->RightIndex)).X;
        Right3.Y = (*(Vector3f*)VBuffer->PositionTuple(pCurTriangle->RightIndex)).Z;
        Right3.Z = (*(Vector3f*)VBuffer->PositionTuple(pCurTriangle->RightIndex)).Y;

        float fdLAHeight = Left3.Z - Apex3.Z;
        float fdRAHeight = Right3.Z - Apex3.Z;
        
        // ģ�Ϳռ�z�任������ռ��y.
        fTempHeight = Apex3.Z + fCdA*fInvCurTriSpacing*fdLAHeight + 
            fCdB*fInvCurTriSpacing*fdRAHeight;
    }
    else
    {
        int iCellX = (int)Mathf::Floor(fMSpaceX);
        int iCellY = (int)Mathf::Floor(fMSpaceZ);
        float fdX = fMSpaceX - iCellX;
        float fdY = fMSpaceZ - iCellY;

        float fLBHeight = 
            (*(Vector3f*)VBuffer->PositionTuple(iCellX + iCellY*m_iSize)).Y;
        float fRBHeight = 
            (*(Vector3f*)VBuffer->PositionTuple(iCellX + 1 + iCellY*m_iSize)).Y;
        float fLTHeight = 
            (*(Vector3f*)VBuffer->PositionTuple(iCellX + (iCellY + 1)*m_iSize)).Y;
        float fRTHeight = 
            (*(Vector3f*)VBuffer->PositionTuple(iCellX + 1 + (iCellY + 1)*m_iSize)).Y;

        if( fdY <= -fdX + 1 )
        {
            float fdBottomHeight = fRBHeight - fLBHeight;
            float fdLeftHeight = fLTHeight - fLBHeight;

            fTempHeight = fLBHeight + fdX*fdBottomHeight + fdY*fdLeftHeight;
        }
        else
        {
            float fdTopHeight = fLTHeight - fRTHeight;
            float fdRightHeight = fRBHeight - fRTHeight;
            
            fTempHeight = fRTHeight + (1.0f - fdX)*fdTopHeight + (1.0f - fdY)*fdRightHeight;
        }
    }

    return fTempHeight + World.GetTranslate().Y;
}
//----------------------------------------------------------------------------
ObjectPtr RoamTerrainPage::Clone()
{
    // TODO:
    // Impliment this function.
    return 0;
}
//----------------------------------------------------------------------------
void RoamTerrainPage::InitializeDerivedData()
{
    m_iSizeM1 = m_iSize - 1;
    m_iPatchSizeM1 = m_iPatchSize - 1;
    m_fInvSpacing = 1.0f / m_fSpacing;
    m_fTextureSpacing = 1.0f / (float)m_iSizeM1;
    m_fMultiplier = (m_fMaxElevation - m_fMinElevation) / 65535.0f;
    m_iPatchesPerSide = m_iSizeM1 / m_iPatchSizeM1;
    m_iMaxIndexCount = 6 * m_iSizeM1 * m_iSizeM1;
    m_bNeedsTessellation = true;
    m_bNeedsResetIB = true;

    // ����patch����.
    m_Patches.resize(m_iPatchesPerSide);
    for( int i = 0; i < m_iPatchesPerSide; i++ )
    {
        m_Patches[i].resize(m_iPatchesPerSide);
    }

    // ����triangle pool.
    m_TriPool.resize(m_iPoolSize);

    // ��ʼ��patches.
    for( int iPatchY = 0; iPatchY < m_iPatchesPerSide; iPatchY++ )
    {
        for( int iPatchX = 0; iPatchX < m_iPatchesPerSide; iPatchX++ )
        {
            m_Patches[iPatchY][iPatchX].Initialize(m_ausHeight, this, 
                iPatchX, iPatchY);
            m_Patches[iPatchY][iPatchX].ComputeVariance();
        }
    }
}
//----------------------------------------------------------------------------
void RoamTerrainPage::ResetPatches()
{
    // ����Pool�е�������Դ.
    SetNextTriNode(0);

    NumTrisRendered = 0;

    SE_ASSERT( m_pTerrain );

    for( int iPatchY = 0; iPatchY < m_iPatchesPerSide; iPatchY++ )
    {
        for( int iPatchX = 0; iPatchX < m_iPatchesPerSide; iPatchX++ )
        {			
            m_Patches[iPatchY][iPatchX].Reset();
            
            // ��ʵ��.
            // patch�޳�.

            // �����patch�߶�ͼ���ݱ��޸�,�������¹�����̬����variance��
            if( m_Patches[iPatchY][iPatchX].GetDirty() )
                m_Patches[iPatchY][iPatchX].ComputeVariance();

            if( m_Patches[iPatchY][iPatchX].IsVisible() )
            {
                // ��patch�໥��������.
                RoamTriTreeNode* pBaseLeft;
                RoamTriTreeNode* pBaseRight;

                if( iPatchX > 0 )
                {
                    pBaseLeft = m_Patches[iPatchY][iPatchX].GetBaseLeft();
                    pBaseLeft->pRightNeighbor = 
                        m_Patches[iPatchY][iPatchX - 1].GetBaseRight();
                }
                else
                {
                    if( m_iCol == 0 )
                    {
                        pBaseLeft = m_Patches[iPatchY][iPatchX].GetBaseLeft();
                        pBaseLeft->pRightNeighbor = 0;
                    }
                    else
                    {
                        // ����Ӧ�����ӵ�������ڵ�RoamTerrainPage.
                        RoamTerrainPage* pLeftPage = 
                            m_pTerrain->GetPage(m_iRow, m_iCol - 1);
                        pBaseLeft = m_Patches[iPatchY][iPatchX].GetBaseLeft();
                        pBaseLeft->pRightNeighbor = 
                            pLeftPage->m_Patches[iPatchY][m_iPatchesPerSide - 1].GetBaseRight();
                    }
                }

                if( iPatchX < m_iPatchesPerSide - 1 )
                {
                    pBaseRight = m_Patches[iPatchY][iPatchX].GetBaseRight();
                    pBaseRight->pRightNeighbor = 
                        m_Patches[iPatchY][iPatchX + 1].GetBaseLeft();
                }
                else
                {
                    if( m_iCol == m_pTerrain->GetColCount() - 1 )
                    {
                        pBaseRight = m_Patches[iPatchY][iPatchX].GetBaseRight();
                        pBaseRight->pRightNeighbor = 0;
                    }
                    else
                    {
                        // ����Ӧ�����ӵ��ұ����ڵ�RoamTerrainPage.
                        RoamTerrainPage* pRightPage = 
                            m_pTerrain->GetPage(m_iRow, m_iCol + 1);
                        pBaseRight = m_Patches[iPatchY][iPatchX].GetBaseRight();
                        pBaseRight->pRightNeighbor = 
                            pRightPage->m_Patches[iPatchY][0].GetBaseLeft();
                    }
                }

                if( iPatchY > 0 )
                {
                    pBaseLeft = m_Patches[iPatchY][iPatchX].GetBaseLeft();
                    pBaseLeft->pLeftNeighbor = 
                        m_Patches[iPatchY - 1][iPatchX].GetBaseRight();
                }
                else
                {
                    if( m_iRow == 0 )
                    {
                        pBaseLeft = m_Patches[iPatchY][iPatchX].GetBaseLeft();
                        pBaseLeft->pLeftNeighbor = 0;
                    }
                    else
                    {
                        // ����Ӧ�����ӵ��������ڵ�RoamTerrainPage.
                        RoamTerrainPage* pDownwardPage = 
                            m_pTerrain->GetPage(m_iRow - 1, m_iCol);
                        pBaseLeft = m_Patches[iPatchY][iPatchX].GetBaseLeft();
                        pBaseLeft->pLeftNeighbor = 
                            pDownwardPage->m_Patches[m_iPatchesPerSide - 1][iPatchX].GetBaseRight();
                    }
                }

                if( iPatchY < m_iPatchesPerSide - 1 )
                {
                    pBaseRight = m_Patches[iPatchY][iPatchX].GetBaseRight();
                    pBaseRight->pLeftNeighbor = 
                        m_Patches[iPatchY + 1][iPatchX].GetBaseLeft();
                }
                else
                {
                    if( m_iRow == m_pTerrain->GetRowCount() - 1 )
                    {
                        pBaseRight = m_Patches[iPatchY][iPatchX].GetBaseRight();
                        pBaseRight->pLeftNeighbor = 0;
                    }
                    else
                    {
                        // ����Ӧ�����ӵ��������ڵ�RoamTerrainPage.
                        RoamTerrainPage* pUpwardPage = 
                            m_pTerrain->GetPage(m_iRow + 1, m_iCol);
                        pBaseRight = m_Patches[iPatchY][iPatchX].GetBaseRight();
                        pBaseRight->pLeftNeighbor = 
                            pUpwardPage->m_Patches[0][iPatchX].GetBaseLeft();
                    }
                }
            }
        }
    }
}
//----------------------------------------------------------------------------
void RoamTerrainPage::SimplifyPatches(const Camera* pCamera, Culler& rCuller)
{
    bool bSingleVisible = rCuller.IsSingleInFrustum(WorldBound);
    if( !bSingleVisible )
        return;

    for( int i = 0; i < m_iPatchesPerSide; i++ )
    {
        for( int j = 0; j < m_iPatchesPerSide; j++ )
        {
            if( m_Patches[i][j].IsVisible() )
                m_Patches[i][j].Tessellate(pCamera);
        }
    }

    m_bNeedsTessellation = true;
}
//----------------------------------------------------------------------------
void RoamTerrainPage::CollectPatches()
{
    Triangles.clear();
    for( int i = 0; i < m_iPatchesPerSide; i++ )
    {
        for( int j = 0; j < m_iPatchesPerSide; j++ )
        {
            if( m_Patches[i][j].IsVisible() )
                m_Patches[i][j].Collect();
        }
    }
}
//----------------------------------------------------------------------------
void RoamTerrainPage::GetUnculledSet(Culler& rCuller, bool bNoCull)
{
    SE_ASSERT( m_pTerrain );

    if( m_pTerrain->GetLod() )
    {
        // �����ռ����Ķ�����������Ҷ�ӽڵ�������,�ؽ�IB.
        if( m_bNeedsTessellation )
        {
            m_bNeedsTessellation = false;

            CollectPatches();

            int iCount, iIndexCount;
            iCount = (int)Triangles.size();
            iIndexCount = 3 * iCount;
            SE_ASSERT( iIndexCount <= m_iMaxIndexCount );

            IBuffer->SetIndexCount(iIndexCount);
            int* aiIndex = IBuffer->GetData();
            for( int i = 0; i < iCount; i++ )
            {
                RoamTriTreeNode* pTriangle = Triangles[i];
                aiIndex[3*i    ] = pTriangle->LeftIndex;
                aiIndex[3*i + 1] = pTriangle->ApexIndex;
                aiIndex[3*i + 2] = pTriangle->RightIndex;
            }   
            IBuffer->Release();

            // �������������.
            if( Mathf::FAbs((float)(GetNextTriNode() - DesiredTris)) >= 50 )
            {
                FrameVariance += ((float)GetNextTriNode() - 
                    (float)DesiredTris)/(float)DesiredTris;
            }

            if( FrameVariance < 1.0f )
                FrameVariance = 1.0f;
        }
    }
    else
    {
        // �ؽ���LOD״̬��IB.
        if( m_bNeedsResetIB )
        {
            m_bNeedsResetIB = false;

            IBuffer->SetIndexCount(m_iMaxIndexCount);
            int* aiIndex = IBuffer->GetData();
            int i, i0, i1;
            for( i1 = 0, i = 0; i1 < m_iSizeM1; i1++ )
            {
                for( i0 = 0; i0 < m_iSizeM1; i0++ )
                {
                    int iV0 = i0 + m_iSize * i1;
                    int iV1 = iV0 + 1;
                    int iV2 = iV1 + m_iSize;
                    int iV3 = iV0 + m_iSize;
                    aiIndex[i++] = iV0;
                    aiIndex[i++] = iV2;
                    aiIndex[i++] = iV1;
                    aiIndex[i++] = iV0;
                    aiIndex[i++] = iV3;
                    aiIndex[i++] = iV2;
                }
            }
            IBuffer->Release();
        }
    }

    if( IBuffer->GetIndexCount() > 0 )
    {
        TriMesh::GetUnculledSet(rCuller, bNoCull);
    }
}
//----------------------------------------------------------------------------
RoamTriTreeNode* RoamTerrainPage::AllocateTri()
{
    RoamTriTreeNode* pTriTreeNode;
    if( m_iNextTriNode >= m_iPoolSize )
        return 0;

    pTriTreeNode = &(m_TriPool[m_iNextTriNode++]);
    pTriTreeNode->pLeftChild = 0;
    pTriTreeNode->pRightChild = 0;

    return pTriTreeNode;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// streaming
//----------------------------------------------------------------------------
void RoamTerrainPage::Load(Stream& rStream, Stream::Link* pLink)
{
    SE_BEGIN_DEBUG_STREAM_LOAD;

    TriMesh::Load(rStream, pLink);

    rStream.Read(m_iSize);
    int iVCount = m_iSize * m_iSize;
    m_ausHeight = SE_NEW unsigned short[iVCount];
    rStream.Read(iVCount, m_ausHeight);
    rStream.Read(m_Origin);
    rStream.Read(m_fMinElevation);
    rStream.Read(m_fMaxElevation);
    rStream.Read(m_fSpacing);
    rStream.Read(m_iPoolSize);
    rStream.Read(m_iPatchSize);
    rStream.Read(m_iSplitLevel);
    rStream.Read(m_iVarianceLevel);

    InitializeDerivedData();

    SE_END_DEBUG_STREAM_LOAD(RoamTerrainPage);
}
//----------------------------------------------------------------------------
void RoamTerrainPage::Link(Stream& rStream, Stream::Link* pLink)
{
    TriMesh::Link(rStream, pLink);
}
//----------------------------------------------------------------------------
bool RoamTerrainPage::Register(Stream& rStream) const
{
    return TriMesh::Register(rStream);
}
//----------------------------------------------------------------------------
void RoamTerrainPage::Save(Stream& rStream) const
{
    SE_BEGIN_DEBUG_STREAM_SAVE;

    TriMesh::Save(rStream);

    rStream.Write(m_iSize);
    int iVCount = VBuffer->GetVertexCount();
    rStream.Write(iVCount, m_ausHeight);
    rStream.Write(m_Origin);
    rStream.Write(m_fMinElevation);
    rStream.Write(m_fMaxElevation);
    rStream.Write(m_fSpacing);
    rStream.Write(m_iPoolSize);
    rStream.Write(m_iPatchSize);
    rStream.Write(m_iSplitLevel);
    rStream.Write(m_iVarianceLevel);

    SE_END_DEBUG_STREAM_SAVE(RoamTerrainPage);
}
//----------------------------------------------------------------------------
int RoamTerrainPage::GetDiskUsed(const StreamVersion& rVersion) const
{
    return TriMesh::GetDiskUsed(rVersion) +
        sizeof(m_iSize) +
        m_iSize*m_iSize*sizeof(m_ausHeight[0]) +
        sizeof(m_Origin) +
        sizeof(m_fMinElevation) +
        sizeof(m_fMaxElevation) +
        sizeof(m_fSpacing) +
        sizeof(m_iPoolSize) +
        sizeof(m_iPatchSize) +
        sizeof(m_iSplitLevel) +
        sizeof(m_iVarianceLevel);
}
//----------------------------------------------------------------------------
StringTree* RoamTerrainPage::SaveStrings(const char*)
{
    StringTree* pTree = SE_NEW StringTree;

    // strings
    pTree->Append(Format(&TYPE, GetName().c_str()));
    pTree->Append(Format("size =", m_iSize));
    pTree->Append(Format("origin =", m_Origin));
    pTree->Append(Format("min elevation =", m_fMinElevation));
    pTree->Append(Format("max elevation =", m_fMaxElevation));
    pTree->Append(Format("spacing =", m_fSpacing));
    pTree->Append(Format("pool size =", m_iPoolSize));
    pTree->Append(Format("patch size =", m_iPatchSize));
    pTree->Append(Format("split level =", m_iSplitLevel));
    pTree->Append(Format("variance level =", m_iVarianceLevel));
    pTree->Append(Format("uv bias =", m_fUVBias));

    // children
    pTree->Append(TriMesh::SaveStrings());

    return pTree;
}
//----------------------------------------------------------------------------
