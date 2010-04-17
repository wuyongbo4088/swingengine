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

#ifndef Swing_Texture_H
#define Swing_Texture_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEObject.h"
#include "SEBindable.h"
#include "SEColorRGBA.h"
#include "SEImage.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:������
// ˵��:֧��1D,2D,3D����
// ����:Sun Che
// ʱ��:20080630
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Texture : public SEObject, public Bindable
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    enum FilterType
    {
        NEAREST,          // nearest neighbor
        LINEAR,           // linear filtering
        NEAREST_NEAREST,  // nearest within image, nearest across images
        NEAREST_LINEAR,   // nearest within image, linear across images
        LINEAR_NEAREST,   // linear within image, nearest across images
        LINEAR_LINEAR,    // linear within image, linear across images
        MAX_FILTER_TYPES
    };

    enum WrapType
    {
        CLAMP,
        REPEAT,
        MIRRORED_REPEAT,
        CLAMP_BORDER,
        CLAMP_EDGE,
        MAX_WRAP_TYPES
    };

    enum DepthCompare
    {
        DC_NEVER,
        DC_LESS,
        DC_EQUAL,
        DC_LEQUAL,
        DC_GREATER,
        DC_NOTEQUAL,
        DC_GEQUAL,
        DC_ALWAYS,
        DC_COUNT
    };

    Texture(Image* pImage = 0);
    Texture(Image* pDepthImage, DepthCompare eCompare);
    virtual ~Texture(void);

    // ����image
    inline void SetImage(Image* pImage);
    inline Image* GetImage(void);
    inline const Image* GetImage(void) const;

    // �����������ģʽ,Ĭ����LINEAR
    inline void SetFilterType(FilterType eFType);
    inline FilterType GetFilterType(void) const;

    // ����wrapģʽ,����0 <= i < GetDimension(),Ĭ�϶���CLAMP_TO_EDGE
    inline void SetWrapType(int i, WrapType eWType);
    inline WrapType GetWrapType(int i) const;

    // border color�ǲ�������������ͼ��߽�ʱ,����ͼ��߽���ı߽���ɫ,
    // Ĭ����SEColorRGBA(0,0,0,1)
    inline void SetBorderColor(const SEColorRGBA& rBorderColor);
    inline SEColorRGBA GetBorderColor(void) const;

    // ֧����������(render target)
    inline bool IsOffscreenTexture(void) const;
    inline void SetOffscreenTexture(bool bOffscreenTexture);

protected:
    ImagePtr m_spImage;
    FilterType m_eFType;       // default = LINEAR
    WrapType m_eWrapTypes[3];  // default = CLAMP_EDGE
    SEColorRGBA m_BorderColor;   // default = BLACK

    // ֧�����ͼ����,Ĭ����DC_COUNT,
    // ��ʱ�����������ͼ����
    DepthCompare m_eCompare;

    // ֧����������,Ĭ����false
    bool m_bOffscreenTexture;

private:
    static const char* ms_pFilterType[MAX_FILTER_TYPES];
    static const char* ms_pWrapType[MAX_WRAP_TYPES];
    static const char* ms_pDepthCompare[DC_COUNT];
};

typedef SESmartPointer<Texture> TexturePtr;

#include "SETexture.inl"

}

#endif
