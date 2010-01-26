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

class Camera;

//----------------------------------------------------------------------------
// ����:Terrain��
// ˵��:û�н���LOD.	
// ����:Sun Che
// ʱ��:20081014
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Terrain : public Node
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    Terrain(const char* acHeightName, const char* acImageName,
        const Attributes& rAttr, Camera* pCamera, float fUVBias,
        ColorRGBA* pBorderColor);

    virtual ~Terrain(void);

    int GetRowCount(void) const;
    int GetColCount(void) const;
    int GetSize(void) const;
    float GetMinElevation(void) const;
    float GetMaxElevation(void) const;
    float GetSpacing(void) const;
    float& UVBias(void);
    ColorRGBA& BorderColor(void);
    void SetCamera(Camera* pCamera);

    // page����.
    TerrainPage* GetPage(int iRow, int iCol);
    TerrainPage* GetCurrentPage(float fX, float fZ) const;
    float GetHeight(float fX, float fZ) const;

    // ����һ��page���滻��.
    // ����װ��һ����page.
    // Ȼ�󷵻ؾ���page,�Ӷ�����Ӧ�ó���cache����page,���ڶ���������ʹ����.
    TerrainPagePtr ReplacePage(int iRow, int iCol, const char* acHeightName,
        const char* acHeightSuffix, const char* acImageName,
        const char* acImageSuffix);

    TerrainPagePtr ReplacePage(int iRow, int iCol, TerrainPage* pNewPage);

    // ����terrain pages��active set.
    void OnCameraMotion(void);

protected:
    // streaming support
    Terrain(void);

    void LoadHeader(const char* acHeightName);
    void LoadPage(int iRow, int iCol, const char* acHeightName,
        const char* acHeightSuffix, const char* acImageName,
        const char* acImageSuffix);

    // page����VB��attribute.
    Attributes m_Attr;

    // page��Ϣ.
    int m_iRows, m_iCols;
    int m_iSize;
    float m_fMinElevation, m_fMaxElevation, m_fSpacing;
    TerrainPagePtr** m_aaspPage;  // ��1ά��row,��2ά��col

    // ����camera�ĵ�ǰpage.
    int m_iCameraRow, m_iCameraCol;
    SmartPointer<Camera> m_spCamera;

    // texture����.
    float m_fUVBias;
    ColorRGBA m_BorderColor;
};

typedef SmartPointer<Terrain> TerrainPtr;
#include "SETerrain.inl"

}

#endif
