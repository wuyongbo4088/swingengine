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

#ifndef Swing_ColorRGB_H
#define Swing_ColorRGB_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SESystem.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Author:Sun Che
// Date:20080312
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SEColorRGB
{
public:
    union
    {
        struct
		{
            float R;
            float G;
            float B;
		};
        float m_fData[3];
    };

public:
    // RGB值范围在[0,1]之间
    SEColorRGB(void);  // 初始化为(0,0,0)
    SEColorRGB(float fR, float fG, float fB);
    SEColorRGB(const float* pData);
    SEColorRGB(const SEColorRGB& rColor);

    // 成员访问
    inline operator const float* (void) const;
    inline operator float* (void);
    inline float operator [] (int i) const;
    inline float& operator [] (int i);

    SEColorRGB& operator = (const SEColorRGB& rColor);

    bool operator == (const SEColorRGB& rColor) const;
    bool operator != (const SEColorRGB& rColor) const;
    bool operator <  (const SEColorRGB& rColor) const;
    bool operator <= (const SEColorRGB& rColor) const;
    bool operator >  (const SEColorRGB& rColor) const;
    bool operator >= (const SEColorRGB& rColor) const;

    SEColorRGB operator + (const SEColorRGB& rColor) const;
    SEColorRGB operator - (const SEColorRGB& rColor) const;
    SEColorRGB operator * (const SEColorRGB& rColor) const;
    SEColorRGB operator * (float fScalar) const;
    SE_FOUNDATION_API friend SEColorRGB operator * (float fScalar, const 
        SEColorRGB& rColor);

    SEColorRGB& operator += (const SEColorRGB& rColor);
    SEColorRGB& operator -= (const SEColorRGB& rColor);
    SEColorRGB& operator *= (const SEColorRGB& rColor);
    SEColorRGB& operator *= (float fScalar);

    // 把颜色值变换到[0,1]区间,
    // Clamp函数把负数裁剪到0,大于1的数裁剪到1,
    // ScaleByMax函数假定所有颜色通道非负,找到最大分量,并把所有值除以该最大值
    void Clamp(void);
    void ScaleByMax(void);

    static const SEColorRGB SE_RGB_BLACK;    // = (0,0,0) 
    static const SEColorRGB SE_RGB_WHITE;    // = (1,1,1)
    static const SEColorRGB SE_RGB_RED;      // = (1,0,0)
    static const SEColorRGB SE_RGB_GREEN;    // = (0,1,0)
    static const SEColorRGB SE_RGB_BLUE;     // = (0,0,1)
    static const SEColorRGB SE_RGB_INVALID;  // = (-1,-1,-1)

private:
    inline int CompareData(const SEColorRGB& rColor) const;
};

#include "SEColorRGB.inl"

}

#endif
