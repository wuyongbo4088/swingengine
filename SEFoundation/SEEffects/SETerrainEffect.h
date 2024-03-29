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

#ifndef Swing_TerrainEffect_H
#define Swing_TerrainEffect_H

#include "SEShaderEffect.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Author:Sun Che
// Date:20081020
//----------------------------------------------------------------------------
class SETerrainEffect : public SEShaderEffect
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    // fog factor计算为f = exp(-(d*z)^2),其中d是fog density,
    // z是view-space顶点位置的z分量.
    SETerrainEffect(const char* acBaseName, const char* acDetailName,
        const SEColorRGB& rFogColor, float fFogDensity);
    virtual ~SETerrainEffect(void);

    inline void SetFogColor(const SEColorRGB& rFogColor);
    inline SEColorRGB GetFogColor(void) const;

    inline void SetFogDensity(float fFogDensity);
    inline float GetFogDensity(void) const;

    // 设置user-defined常量使用本地存储位置.
    virtual void OnLoadPrograms(int iPass, SEProgram* pVProgram,
        SEProgram* pPProgram, SEProgram* pGProgram);

protected:
    // streaming
    SETerrainEffect(void);

    // fog color存储在前三个分量中(R = 0, G = 1, B = 2).
    // fog density存储在最后一个分量中.
    static float ms_afFogColorDensity[4];
    static bool ms_bFogColorDensityUCInitialized;
};

typedef SESmartPointer<SETerrainEffect> SETerrainEffectPtr;

#include "SETerrainEffect.inl"

}

#endif
