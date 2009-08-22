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

#ifndef Swing_RoamTerrainPage_H
#define Swing_RoamTerrainPage_H

#include "SEFoundationLIB.h"
#include "SETriMesh.h"
#include "SERoamTerrainPatch.h"
#include "SERoamTriTreeNode.h"

namespace Swing
{

class RoamTerrain;

//   Y 
//   |
//   |----------------
//   | \     | \     |
//   |   \   |   \   |
//   |     \ |     \ |
//   |----------------
//   | \  R  | \     |
//   |   \   |   \   |
//   | L   \ |     \ |
//   |--------------------X
//
// ���ǲ�����������ϵ,
// ����mesh�洢��model space��,
// ���ε�Y�ᾭ������任,Ӧ������������ϵ��Z��,���ε�X�ᾭ������任,
// ������������ϵ��X��,����Z��Ϊ�����߶���,��������任,������������ϵ��Y��,
// �߶�ͼΪ(2^N+1)*(2^N+1)�Ĳ���������,
// һ��Terrain page��Ӧһ���߶�ͼ,
// һ��Terrain page��N*N��Terrain patch���,
// һ��Terrain patch��Left base triangle��Right base triangle���,����ͼ.
//
// ROAM(Real-time Optimally Adapting Meshes) Terrain LOD:
// http://www.cognigraph.com/ROAM_homepage/
// ��ʵ�ֲ���ROAM��һ�������㷨,���㷨ÿframe��split��û��merge,
// û��frame-coherence,���ö����������ݹ��㷨,
// �����priority queue��ά��,ע���������priority queue�������CPU����,
// heap-based queue��ά�����۹���,��ʱӦ����bucket-based queue.
//
// ����GPU����������������Ϊ���,�ֽ׶�ROAM LOD��CPUƿ�������Ѿ�ͻ��,
// ��ĳЩ�豸��,ʹ��GPUֱ�Ӵ��������������Ч�ʿ��ܻ����.

//----------------------------------------------------------------------------
// ����:ROAM����ҳ����
// ˵��:
// ����:jazzboysc
// ʱ��:20070601
//----------------------------------------------------------------------------
class SE_FOUNDATION_API RoamTerrainPage : public TriMesh
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    RoamTerrainPage(const Attributes& rAttr, int iSize,
        unsigned short* ausHeight, const Vector2f& rOrigin,
        float fMinElevation, float fMaxElevation, float fSpacing,
        float fUVBias, int iPoolSize, int iPatchSize, int iSplitLevel, 
        int iVarianceLevel);

    virtual ~RoamTerrainPage(void);

    // �߶�ͼ����.
    inline int GetSize(void) const;
    inline const unsigned short* GetHeights(void) const;
    inline const Vector2f& GetOrigin(void) const;
    inline float GetMinElevation(void) const;
    inline float GetMaxElevation(void) const;
    inline float GetSpacing(void) const;

    // ��ȡ�߶�.
    // �������page��,�򷵻�INFINITY.
    float GetHeight(float fX, float fZ) const;

    // page�Ķ�������������Դ�VB�л�ȡ.
    // bias�Ӹú�����ȡ.
    inline float& UVBias(void);

    // ��ʼframe varience(Ӧ������Ϊ�ϸ�,���򽫻��Զ�������ֵ)
    float FrameVariance;
    
    // ÿframe�������Ķ���������tessellation����ֵ,
    // ������ʵ����Ⱦ������������,
    // ��ʵ����Ⱦ�������ε�2������
    static int DesiredTris;
    // ��ǰframe�Ѿ���Ⱦ������������
    static int NumTrisRendered;
    std::vector<RoamTriTreeNode*> Triangles;

protected:
    // streaming support.
    RoamTerrainPage(void);
    void InitializeDerivedData(void);

    // simplification.
    friend class RoamTerrain;
    void ResetPatches(void);
    void SimplifyPatches(const Camera* pCamera, Culler& rCuller);
    void CollectPatches(void);

    // page������Ϣ.
    inline void SetPageGrid(int iRow, int iCol, RoamTerrain* pTerrain);
    int m_iRow, m_iCol;
    RoamTerrain* m_pTerrain;  // ��������ָ�뻥ָ,����Ҫstreaming.

    // tessellation.
    inline float GetX(int iX) const;
    inline float GetZ(int iZ) const;
    inline float GetHeight(int iIndex) const;
    inline float GetTextureCoordinate(int iIndex) const;

    // �����ؽ�IB.
    virtual void GetUnculledSet(Culler& rCuller, bool bNoCull);
	
    inline int GetNextTriNode(void) const;
    inline void SetNextTriNode(int iNextNode);

    // �߶�ͼ����.
    int m_iSize, m_iSizeM1, m_iPatchSizeM1;
    unsigned short* m_ausHeight;
    Vector2f m_Origin;
    float m_fMinElevation, m_fMaxElevation, m_fSpacing;
    float m_fInvSpacing, m_fTextureSpacing, m_fMultiplier;

    // ROAM���page��Ϣ.
    int m_iPatchesPerSide;
    int m_iPoolSize;
    int m_iPatchSize;
    int m_iSplitLevel;
    int m_iVarianceLevel; // ��СΪ2
    int m_iMaxIndexCount;

    // ��Lodģʽ��Simplify��,��Ҫ����Ϊtrue,�Ӷ������ؽ�IB.
    bool m_bNeedsTessellation;

    // �л�����Lodģʽ��,��Ҫ����Ϊtrue,�Ӷ������ؽ�IB.
    bool m_bNeedsResetIB;

    // patch����,������n*n��patch.
    mutable std::vector<std::vector<RoamTerrainPatch> > m_Patches;

    friend class RoamTerrainPatch;
    RoamTriTreeNode* AllocateTri(void);
    int m_iNextTriNode;										
    std::vector<RoamTriTreeNode> m_TriPool;

    // texture����.
    float m_fUVBias;
};

typedef SmartPointer<RoamTerrainPage> RoamTerrainPagePtr;
#include "SERoamTerrainPage.inl"

}

#endif