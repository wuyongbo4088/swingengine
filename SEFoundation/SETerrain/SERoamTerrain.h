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

#ifndef Swing_RoamTerrain_H
#define Swing_RoamTerrain_H

#include "SEFoundationLIB.h"
#include "SENode.h"
#include "SECuller.h"
#include "SERoamTerrainPage.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:����SERoamTerrainPage��,SERoamTerrain���󹹳�һ����������,
//      ע�����ڵ�SERoamTerrainPage֮����Ҫ���з��,
//      �����νӴ������ܲ���T-junction.
// Author:Sun Che
// Date:20080104
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SERoamTerrain : public SENode
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    SERoamTerrain(const char* acHeightName, const char* acImageName,
        const SEAttributes& rAttr, SECamera* pCamera, float fUVBias,
        SEColorRGBA* pBorderColor);

    virtual ~SERoamTerrain(void);

    inline int GetRowCount(void) const;
    inline int GetColCount(void) const;
    inline int GetSize(void) const;
    inline float GetMinElevation(void) const;
    inline float GetMaxElevation(void) const;
    inline float GetSpacing(void) const;
    inline float& UVBias(void);
    inline SEColorRGBA& BorderColor(void);
    inline void SetCamera(SECamera* pCamera);

    inline int GetPoolSize(void) const;
    inline int GetPatchSize(void) const;
    inline int GetSplitLevel(void) const;
    inline int GetVarianceLevel(void) const;

    void SetLod(bool bLod);
    inline bool GetLod(void) const;

    // page����.
    SERoamTerrainPage* GetPage(int iRow, int iCol);
    SERoamTerrainPage* GetCurrentPage(float fX, float fZ) const;
    float GetHeight(float fX, float fZ) const;

    // ����һ��page���滻��.
    // ����Ѿ���page������,װ��һ����page�����·��.
    // Ȼ�󷵻ؾ���page,�Ӷ�����Ӧ�ó���cache����page,���ڶ���������ʹ����.
    SERoamTerrainPagePtr ReplacePage(int iRow, int iCol, const char* 
        acHeightName, const char* acHeightSuffix, const char* acImageName,
        const char* acImageSuffix);

    SERoamTerrainPagePtr ReplacePage(int iRow, int iCol, SERoamTerrainPage* 
        pNewPage);

    // LOD��ں���.
    void Simplify(SECuller& rCuller);

protected:
    // streaming support
    SERoamTerrain(void);

    void LoadHeader(const char* acHeightName);
    void LoadPage(int iRow, int iCol, const char* acHeightName,
        const char* acHeightSuffix, const char* acImageName,
        const char* acImageSuffix);

    // page����VB��attribute.
    SEAttributes m_Attr;

    // page��Ϣ.
    int m_iRows, m_iCols;
    int m_iSize;
    float m_fMinElevation, m_fMaxElevation, m_fSpacing;
    std::vector<std::vector<SERoamTerrainPagePtr> > m_Pages;

    // ROAM���page��Ϣ.
    int m_iPoolSize;
    int m_iPatchSize;
    int m_iSplitLevel;
    int m_iVarianceLevel; // ��СΪ2

    // LOD״̬.
    bool m_bLod;

    // ����camera�ĵ�ǰpage.
    int m_iCameraRow, m_iCameraCol;
    SESmartPointer<SECamera> m_spCamera;

    // texture����.
    float m_fUVBias;
    SEColorRGBA m_BorderColor;
};

typedef SESmartPointer<SERoamTerrain> SERoamTerrainPtr;
#include "SERoamTerrain.inl"

}

#endif