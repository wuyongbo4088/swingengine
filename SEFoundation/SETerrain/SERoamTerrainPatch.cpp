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
#include "SERoamTerrainPatch.h"

using namespace Swing;

//----------------------------------------------------------------------------
RoamTerrainPatch::RoamTerrainPatch()
{
    m_pPage = 0;
    m_iPatchX = -1;
    m_iPatchY = -1;
    m_iMSpaceX = 0;
    m_iMSpaceY = 0;
    m_ausHeight = 0;
    m_pCurrentVariance = 0;
    m_bDirty = false;
    m_bVisible = false;
}
//----------------------------------------------------------------------------
RoamTerrainPatch::~RoamTerrainPatch()
{
}
//----------------------------------------------------------------------------
void RoamTerrainPatch::Initialize(unsigned short* ausHeight, 
    RoamTerrainPage* pPage, int iPatchX, int iPatchY)
{
    m_pPage = pPage;
    m_iPatchX = iPatchX;
    m_iPatchY = iPatchY;

    // ��ʼ���������ڵ��ϵ.
    m_BaseLeft.pRightNeighbor = 0;
    m_BaseLeft.pLeftNeighbor = 0;
    m_BaseLeft.pRightChild = 0;
    m_BaseLeft.pLeftChild = 0;

    m_BaseRight.pRightNeighbor = 0;
    m_BaseRight.pLeftNeighbor = 0;
    m_BaseRight.pRightChild = 0;
    m_BaseRight.pLeftChild = 0;

    // ���ڵ��ε���.
    m_BaseLeft.pPatch = this;
    m_BaseRight.pPatch = this;
	
    // ��ʼ��Base�����ι���Diamond.
    m_BaseLeft.pBaseNeighbor = &m_BaseRight;
    m_BaseRight.pBaseNeighbor = &m_BaseLeft;

    // Patch��ģ�Ϳռ�����.
    m_iMSpaceX = iPatchX * m_pPage->m_iPatchSizeM1;
    m_iMSpaceY = iPatchY * m_pPage->m_iPatchSizeM1;

    m_ausHeight = ausHeight;

    m_bDirty = true;
    m_bVisible = true;

    // left top����Ķ�������.
    int iLTIndex = (m_iMSpaceY + m_pPage->m_iPatchSizeM1)*m_pPage->m_iSize + 
        m_iMSpaceX;
    // right bottom����Ķ�������.
    int iRBIndex = m_iMSpaceY*m_pPage->m_iSize + m_iMSpaceX + 
        m_pPage->m_iPatchSizeM1;

    // �������ζ��������.
    m_BaseLeft.LeftIndex = iRBIndex;
    m_BaseLeft.RightIndex = iLTIndex;
    m_BaseLeft.ApexIndex = m_iMSpaceY*m_pPage->m_iSize + m_iMSpaceX;

    // �������ζ��������.
    m_BaseRight.LeftIndex = iLTIndex;
    m_BaseRight.RightIndex = iRBIndex;
    m_BaseRight.ApexIndex = (m_iMSpaceY + m_pPage->m_iPatchSizeM1)*
        m_pPage->m_iSize + m_iMSpaceX + m_pPage->m_iPatchSizeM1;
}
//----------------------------------------------------------------------------
void RoamTerrainPatch::Reset()
{
    m_bVisible = true;

    m_BaseLeft.pLeftChild = 0;
    m_BaseLeft.pRightChild = 0;
    m_BaseRight.pLeftChild = 0;
    m_BaseRight.pRightChild = 0;

    m_BaseLeft.pBaseNeighbor = &m_BaseRight;
    m_BaseRight.pBaseNeighbor = &m_BaseLeft;

    m_BaseLeft.pRightNeighbor = 0;
    m_BaseLeft.pLeftNeighbor = 0;
    m_BaseRight.pRightNeighbor = 0;
    m_BaseRight.pLeftNeighbor = 0;

    // ���ڵ��ε���.
    m_BaseLeft.pPatch = this;
    m_BaseRight.pPatch = this;
}
//----------------------------------------------------------------------------
void RoamTerrainPatch::Tessellate(const Camera* pCamera)
{
    m_pCurrentVariance = m_VarianceLeft;
    RecursTessellate(&m_BaseLeft, pCamera,
        m_iMSpaceX + m_pPage->m_iPatchSizeM1, m_iMSpaceY,
        m_iMSpaceX, m_iMSpaceY + m_pPage->m_iPatchSizeM1,
        m_iMSpaceX, m_iMSpaceY, 1);

    m_pCurrentVariance = m_VarianceRight;
    RecursTessellate(&m_BaseRight, pCamera,
        m_iMSpaceX, m_iMSpaceY + m_pPage->m_iPatchSizeM1,
        m_iMSpaceX + m_pPage->m_iPatchSizeM1, m_iMSpaceY,	
        m_iMSpaceX + m_pPage->m_iPatchSizeM1, m_iMSpaceY + 
        m_pPage->m_iPatchSizeM1, 1);
}
//----------------------------------------------------------------------------
void RoamTerrainPatch::Collect()
{	
    RecursCollect(&m_BaseLeft);
    RecursCollect(&m_BaseRight);
}
//----------------------------------------------------------------------------
void RoamTerrainPatch::ComputeVariance()
{
    // ��Base���ǿ�ʼ�ݹ����variance.

    m_pCurrentVariance = m_VarianceLeft;
    RecursComputeVariance(m_iMSpaceX + m_pPage->m_iPatchSizeM1, m_iMSpaceY,
        m_iMSpaceX, m_iMSpaceY + m_pPage->m_iPatchSizeM1,
        m_iMSpaceX, m_iMSpaceY,
        m_BaseLeft.LeftIndex, m_BaseLeft.RightIndex, m_BaseLeft.ApexIndex, 1);

    m_pCurrentVariance = m_VarianceRight;
    RecursComputeVariance(m_iMSpaceX, m_iMSpaceY + m_pPage->m_iPatchSizeM1,
        m_iMSpaceX + m_pPage->m_iPatchSizeM1, m_iMSpaceY,
        m_iMSpaceX + m_pPage->m_iPatchSizeM1, m_iMSpaceY + m_pPage->m_iPatchSizeM1,
        m_BaseRight.LeftIndex, m_BaseRight.RightIndex, m_BaseRight.ApexIndex, 1);

    m_bDirty = false;
}
//----------------------------------------------------------------------------
void RoamTerrainPatch::Split(RoamTriTreeNode* pTriTreeNode)
{
    SE_ASSERT( pTriTreeNode );

    // ����Ѿ�split��,�򷵻�.
    if( pTriTreeNode->pLeftChild )
        return;

    // �������diamond,����split base neighbor.
    if( pTriTreeNode->pBaseNeighbor && 
        (pTriTreeNode->pBaseNeighbor->pBaseNeighbor != pTriTreeNode) )
        Split(pTriTreeNode->pBaseNeighbor);

    // split��ǰ�ڵ�,Ϊ��ǰ�ڵ�������Һ��ӽڵ�.
    pTriTreeNode->pLeftChild  = m_pPage->AllocateTri();
    pTriTreeNode->pRightChild = m_pPage->AllocateTri();
    // ���ڵ��ε���.
    SE_ASSERT( pTriTreeNode->pLeftChild );
    pTriTreeNode->pLeftChild->pPatch = this;
    SE_ASSERT( pTriTreeNode->pRightChild );
    pTriTreeNode->pRightChild->pPatch = this;

    // ����ڴ����ʧ��,�򷵻�.
    // ע��m_pPage->m_iPoolSize�ߴ�,
    // ���С�ڵ�ǰPage����patch����ȫ�����������ڵ�����,
    // ���������п����ڴ����ʧ��.
    SE_ASSERT( pTriTreeNode->pLeftChild );
    if( !pTriTreeNode->pLeftChild )
        return;

    // Ϊ��split�����ӽڵ㽨���ڵ��ϵ.
    pTriTreeNode->pLeftChild->pBaseNeighbor = pTriTreeNode->pLeftNeighbor;
    pTriTreeNode->pLeftChild->pLeftNeighbor = pTriTreeNode->pRightChild;

    pTriTreeNode->pRightChild->pBaseNeighbor = pTriTreeNode->pRightNeighbor;
    pTriTreeNode->pRightChild->pRightNeighbor = pTriTreeNode->pLeftChild;

    // ���ھӽڵ�ָ����split�������ӽڵ�.
    if( pTriTreeNode->pLeftNeighbor != 0 )
    {
        if( pTriTreeNode->pLeftNeighbor->pBaseNeighbor == pTriTreeNode )
            pTriTreeNode->pLeftNeighbor->pBaseNeighbor = 
                pTriTreeNode->pLeftChild;
        else if( pTriTreeNode->pLeftNeighbor->pLeftNeighbor == pTriTreeNode )
            pTriTreeNode->pLeftNeighbor->pLeftNeighbor = 
                pTriTreeNode->pLeftChild;
        else if( pTriTreeNode->pLeftNeighbor->pRightNeighbor == pTriTreeNode )
            pTriTreeNode->pLeftNeighbor->pRightNeighbor = 
                pTriTreeNode->pLeftChild;
        else
        {
            // �Ƿ������ھӽڵ�
            //int iStopHere = 0;
            SE_ASSERT( false );
        }
    }

    // ���ھӽڵ�ָ����split�����Һ��ӽڵ�.
    if( pTriTreeNode->pRightNeighbor != 0 )
    {
        if( pTriTreeNode->pRightNeighbor->pBaseNeighbor == pTriTreeNode )
            pTriTreeNode->pRightNeighbor->pBaseNeighbor = 
                pTriTreeNode->pRightChild;
        else if( pTriTreeNode->pRightNeighbor->pRightNeighbor == pTriTreeNode )
            pTriTreeNode->pRightNeighbor->pRightNeighbor = 
                pTriTreeNode->pRightChild;
        else if( pTriTreeNode->pRightNeighbor->pLeftNeighbor == pTriTreeNode )
            pTriTreeNode->pRightNeighbor->pLeftNeighbor = 
                pTriTreeNode->pRightChild;
        else
        {
            // �Ƿ������ھӽڵ�
            //int iStopHere = 0;
            SE_ASSERT( false );
        }
    }

    // ���ھӽڵ�ָ����split���ĺ��ӽڵ�.
    if( pTriTreeNode->pBaseNeighbor != 0 )
    {
        if( pTriTreeNode->pBaseNeighbor->pLeftChild )
        {
            pTriTreeNode->pBaseNeighbor->pLeftChild->pRightNeighbor = 
                pTriTreeNode->pRightChild;
            pTriTreeNode->pBaseNeighbor->pRightChild->pLeftNeighbor = 
                pTriTreeNode->pLeftChild;
            pTriTreeNode->pLeftChild->pRightNeighbor = 
                pTriTreeNode->pBaseNeighbor->pRightChild;
            pTriTreeNode->pRightChild->pLeftNeighbor = 
                pTriTreeNode->pBaseNeighbor->pLeftChild;
        }
        else
        {
            Split(pTriTreeNode->pBaseNeighbor);
        }
    }
    else
    {
        // Page�߽��ϵ�������.
        pTriTreeNode->pLeftChild->pRightNeighbor = 0;
        pTriTreeNode->pRightChild->pLeftNeighbor = 0;
    }

    int iCenterIndex = (pTriTreeNode->LeftIndex + pTriTreeNode->RightIndex) >> 1;
  
    // �������������Ϣ.
	
    // ����
    pTriTreeNode->pLeftChild->ApexIndex = iCenterIndex;
    pTriTreeNode->pLeftChild->LeftIndex = pTriTreeNode->ApexIndex;
    pTriTreeNode->pLeftChild->RightIndex = pTriTreeNode->LeftIndex;

    // �Һ���
    pTriTreeNode->pRightChild->ApexIndex = iCenterIndex;
    pTriTreeNode->pRightChild->LeftIndex = pTriTreeNode->RightIndex;
    pTriTreeNode->pRightChild->RightIndex = pTriTreeNode->ApexIndex;
}
//----------------------------------------------------------------------------
void RoamTerrainPatch::RecursCollect(RoamTriTreeNode* pTriTreeNode)
{
    SE_ASSERT( pTriTreeNode );

    // ���з�Ҷ�ӽڵ㶼��Ȼ����������,ֻ����һ��.
    if( pTriTreeNode->pLeftChild )
    {
        RecursCollect(pTriTreeNode->pLeftChild);
        RecursCollect(pTriTreeNode->pRightChild);
    }
    else									
    {
        // ֻ�ռ�Ҷ�ӽڵ�.
        m_pPage->NumTrisRendered++;
        m_pPage->Triangles.push_back(pTriTreeNode);
    }
}
//----------------------------------------------------------------------------
void RoamTerrainPatch::RecursTessellate(RoamTriTreeNode* pTriTreeNode, 
    const Camera* pCamera, int iLeftX, int iLeftY, int iRightX, int iRightY, 
    int iApexX, int iApexY, int iNode)
{
    SE_ASSERT( pTriTreeNode );

    float fTriVariance = 0.0f;
    // ����б���е�.
    int iCenterX = (iLeftX + iRightX) >> 1;
    int iCenterY = (iLeftY + iRightY) >> 1;

    // ��ȡ�����������λ��.
    SEVector3f vec3fCLocWorld = pCamera->GetLocation();
    SEVector3f vec3fCLocLocal;
    // �������������λ�ñ任����ǰpatch����page��ģ�Ϳռ�.
    m_pPage->World.ApplyInverse(vec3fCLocWorld, vec3fCLocLocal);
    //vec3fCLocLocal.X = vec3fCLocWorld.X - m_pPage->m_Origin.X;
    //vec3fCLocLocal.Z = vec3fCLocWorld.Z - m_pPage->m_Origin.Y;
    float fCenterXLocal = m_pPage->GetX(iCenterX);
    float fCenterYLocal = m_pPage->GetZ(iCenterY);

    // ��ֹ����Խ��.
    if( iNode < (1 << VARIANCE_DEPTH) )
    {		
        float fSqrDistance = SE_SQR(fCenterXLocal - vec3fCLocLocal.X) + 
            SE_SQR(fCenterYLocal - vec3fCLocLocal.Z);

        float fInvDistance = SEMathf::FastInvSqrt(fSqrDistance);
        fTriVariance = ((float)m_pCurrentVariance[iNode] * 
            (m_pPage->m_iSize << 1)) * fInvDistance;
    }

    // ��ʹû�д�node��varianceֵ,����Ȼ�ݹ�tessellate������ϣ������split,
    // ֱ����ǰ�������׽Ǵﵽ�ٽ��������.
    if( (iNode >= (1 << VARIANCE_DEPTH)) ||	
         // ���ݶ�̬varianceֵ�Ƿ������ٽ�ֵ�ж��Ƿ����split,���������
         (fTriVariance - m_pPage->FrameVariance > 0.0f) )
    {
        Split(pTriTreeNode);
		
        if( pTriTreeNode->pLeftChild && 
            ((SEMathf::FAbs((float)(iLeftX - iRightX)) >= m_pPage->m_iSplitLevel) || 
            (SEMathf::FAbs((float)(iLeftY - iRightY)) >= m_pPage->m_iSplitLevel)) )
        {
            RecursTessellate(pTriTreeNode->pLeftChild, pCamera,
                iApexX, iApexY, iLeftX, iLeftY, iCenterX, iCenterY, iNode<<1);
            RecursTessellate(pTriTreeNode->pRightChild, pCamera,
                iRightX, iRightY, iApexX, iApexY, iCenterX, iCenterY, 1+(iNode<<1));
        }
    }
}
//----------------------------------------------------------------------------
float RoamTerrainPatch::RecursComputeVariance(int iLeftX, int iLeftY, 
    int iRightX, int iRightY, int iApexX, int iApexY, 
    int iLeftIndex, int iRightIndex, int iApexIndex, int iNodeIndex)
{
    int iCenterX = (iLeftX + iRightX) >> 1;
    int iCenterY = (iLeftY + iRightY) >> 1;
    int iCenterIndex = (iLeftIndex + iRightIndex) >> 1;
    float fMyVariance;

    // ��ȡб���е�ĸ߶�ֵ.
    float fCenterZ  = m_pPage->GetHeight(iCenterIndex);
    float fLeftZ = m_pPage->GetHeight(iLeftIndex);
    float fRightZ = m_pPage->GetHeight(iRightIndex);
    // variance���е�߶�ֵ���е�����Zֵ֮��.
    fMyVariance = SEMathf::FAbs(fCenterZ - (fLeftZ + fRightZ) * 0.5f);

    // ֻ�ݹ����variance���߶�ͼ��N*N������.
    if( (SEMathf::FAbs((float)(iLeftX - iRightX)) >= m_pPage->m_iVarianceLevel) ||
        (SEMathf::FAbs(float(iLeftY - iRightY)) >= m_pPage->m_iVarianceLevel) )
    {
        // ���յ�ǰ�ڵ��varianceȡ���Լ����������������variance.
        fMyVariance = SE_MAX(fMyVariance, RecursComputeVariance(iApexX, iApexY, 
            iLeftX, iLeftY, iCenterX, iCenterY, iApexIndex, iLeftIndex, 
            iCenterIndex, iNodeIndex<<1));
        fMyVariance = SE_MAX(fMyVariance, RecursComputeVariance(iRightX, iRightY, 
            iApexX, iApexY, iCenterX, iCenterY, iRightIndex, iApexIndex, 
            iCenterIndex, 1+(iNodeIndex<<1)));
    }

    // ��ֹ����Խ��,ͬʱvariance��СӦΪ1.
    if( iNodeIndex < (1<<VARIANCE_DEPTH) )
        m_pCurrentVariance[iNodeIndex] = 1 + fMyVariance;

    return fMyVariance;
}
//----------------------------------------------------------------------------
