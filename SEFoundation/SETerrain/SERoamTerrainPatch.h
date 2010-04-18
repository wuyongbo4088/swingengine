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

#ifndef Swing_RoamTerrainPatch_H
#define Swing_RoamTerrainPatch_H

#include "SEFoundationLIB.h"
#include "SERoamTriTreeNode.h"
#include "SECamera.h"

namespace Swing
{

class SERoamTerrainPage;

//----------------------------------------------------------------------------
// ˵��:
// ����:Sun Che
// ʱ��:20070601
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SERoamTerrainPatch
{
public:
    SERoamTerrainPatch(void);
    virtual ~SERoamTerrainPatch(void);

    inline SERoamTriTreeNode*	GetBaseLeft(void);
    inline SERoamTriTreeNode*	GetBaseRight(void);
    inline bool GetDirty(void) const;
    inline void SetDirty(bool bDirty);
    inline bool IsVisible(void) const;
    inline void SetVisible(bool bVisible);

    void Initialize(unsigned short* ausHeight, SERoamTerrainPage* pPage,
        int iPatchX, int iPatchY);
    void Reset(void);
    void Tessellate(const SECamera* pCamera);
    void Collect(void);
    void ComputeVariance(void);

protected:
    void Split(SERoamTriTreeNode* pTriTreeNode);
    void RecursCollect(SERoamTriTreeNode* pTriTreeNode);
    void RecursTessellate(SERoamTriTreeNode* pTriTreeNode, const SECamera* pCamera,
        int iLeftX, int iLeftY, int iRightX, int iRightY, 
        int iApexX, int iApexY, int iNode);
    float RecursComputeVariance(int iLeftX, int iLeftY, int iRightX, int iRightY,
        int iApexX, int iApexY, int iLeftIndex, int iRightIndex, int iApexIndex, 
        int iNodeIndex);

    SERoamTerrainPage* m_pPage;
    int m_iPatchX, m_iPatchY;
    int m_iMSpaceX, m_iMSpaceY; // ģ�Ϳռ�����

    unsigned short* m_ausHeight;

    float* m_pCurrentVariance; // ��ǰ����ʹ�õ�variance��
    bool m_bDirty; // variance���Ƿ���Ҫ���¼���
    bool m_bVisible; // Patch��ǰ�Ƿ�ɼ�

    // variance�������:Ӧ�ýӽ� SQRT(PatchSize) + 1
	enum { VARIANCE_DEPTH = 8 };
    float m_VarianceLeft[1<<(VARIANCE_DEPTH)]; // ��variance��
    float m_VarianceRight[1<<(VARIANCE_DEPTH)]; // ��variance��
    SERoamTriTreeNode m_BaseLeft; // ��������������ڵ�
    SERoamTriTreeNode m_BaseRight; // �Ҷ������������ڵ�
};

#include "SERoamTerrainPatch.inl"

}

#endif