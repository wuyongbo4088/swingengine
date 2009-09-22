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

#ifndef Swing_ColorRGBA_H
#define Swing_ColorRGBA_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SESystem.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:RGBAֵ��
// ˵��:
// ����:Sun Che
// ʱ��:20080312
//----------------------------------------------------------------------------
class SE_FOUNDATION_API ColorRGBA
{
public:
    union
    {
        struct
        {
            float R;
            float G;
            float B;
            float A;     
        };
        float m_fData[4];
    };

public:
    // RGBAֵ��Χ��[0,1]֮��
    ColorRGBA(void);  // ��ʼ��Ϊ(0,0,0)
    ColorRGBA(float fR, float fG, float fB, float fA);
    ColorRGBA(const float* pData);
    ColorRGBA(const ColorRGBA& rColor);

    inline operator const float* (void) const;
    inline operator float* (void);
    inline float operator [] (int i) const;
    inline float& operator [] (int i);

    ColorRGBA& operator = (const ColorRGBA& rColor);

    bool operator == (const ColorRGBA& rColor) const;
    bool operator != (const ColorRGBA& rColor) const;
    bool operator <  (const ColorRGBA& rColor) const;
    bool operator <= (const ColorRGBA& rColor) const;
    bool operator >  (const ColorRGBA& rColor) const;
    bool operator >= (const ColorRGBA& rColor) const;

    ColorRGBA operator + (const ColorRGBA& rColor) const;
    ColorRGBA operator - (const ColorRGBA& rColor) const;
    ColorRGBA operator * (const ColorRGBA& rColor) const;
    ColorRGBA operator * (float fScalar) const;
    SE_FOUNDATION_API friend ColorRGBA operator * (float fScalar, const ColorRGBA& rColor);

    ColorRGBA& operator += (const ColorRGBA& rColor);
    ColorRGBA& operator -= (const ColorRGBA& rColor);
    ColorRGBA& operator *= (const ColorRGBA& rColor);
    ColorRGBA& operator *= (float fScalar);

    // ����ɫֵ�任��[0,1]����,
    // Clamp�����Ѹ����ü���0,����1�����ü���1,
    // ScaleByMax�����ٶ�������ɫͨ���Ǹ�,�ҵ�RGB������,����RGBֵ���Ը����ֵ,
    // Aֵ�ü���[0,1]����
    void Clamp(void);
    void ScaleByMax(void);

    static const ColorRGBA SE_RGBA_BLACK;   // = (0,0,0,1) 
    static const ColorRGBA SE_RGBA_WHITE;   // = (1,1,1,1)
    static const ColorRGBA SE_RGBA_RED;     // = (1,0,0,1) 
    static const ColorRGBA SE_RGBA_GREEN;   // = (0,1,0,1)
    static const ColorRGBA SE_RGBA_BLUE;    // = (0,0,1,1)
    static const ColorRGBA SE_RGBA_INVALID; // = (-1,-1,-1,-1)

private:
    inline int CompareData(const ColorRGBA& rColor) const;
};

#include "SEColorRGBA.inl"

}

#endif