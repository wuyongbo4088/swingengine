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

#ifndef Swing_Terrain_H
#define Swing_Terrain_H

#include "SEFoundationLIB.h"
#include "SENode.h"
#include "SETerrainPage.h"

namespace Swing
{

class SECamera;

//----------------------------------------------------------------------------
// ����:Terrain��
// ˵��:û�н���LOD.	
// ����:Sun Che
// ʱ��:20081014
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SETerrain : public SENode
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    SETerrain(const char* acHeightName, const char* acImageName,
        const SEAttributes& rAttr, SECamera* pCamera, float fUVBias,
        SEColorRGBA* pBorderColor);

    virtual ~SETerrain(void);

    int GetRowCount(void) const;
    int GetColCount(void) const;
    int GetSize(void) const;
    float GetMinElevation(void) const;
    float GetMaxElevation(void) const;
    float GetSpacing(void) const;
    float& UVBias(void);
    SEColorRGBA& BorderColor(void);
    void SetCamera(SECamera* pCamera);

    // page����.
    SETerrainPage* GetPage(int iRow, int iCol);
    SETerrainPage* GetCurrentPage(float fX, float fZ) const;
    float GetHeight(float fX, float fZ) const;

    // ����һ��page���滻��.
    // ����װ��һ����page.
    // Ȼ�󷵻ؾ���page,�Ӷ�����Ӧ�ó���cache����page,���ڶ���������ʹ����.
    SETerrainPagePtr ReplacePage(int iRow, int iCol, const char* acHeightName,
        const char* acHeightSuffix, const char* acImageName,
        const char* acImageSuffix);

    SETerrainPagePtr ReplacePage(int iRow, int iCol, SETerrainPage* pNewPage);

    // ����terrain pages��active set.
    void OnCameraMotion(void);

protected:
    // streaming support
    SETerrain(void);

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
    SETerrainPagePtr** m_aaspPage;  // ��1ά��row,��2ά��col

    // ����camera�ĵ�ǰpage.
    int m_iCameraRow, m_iCameraCol;
    SESmartPointer<SECamera> m_spCamera;

    // texture����.
    float m_fUVBias;
    SEColorRGBA m_BorderColor;
};

typedef SESmartPointer<SETerrain> SETerrainPtr;
#include "SETerrain.inl"

}

#endif
