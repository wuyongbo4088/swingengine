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

#ifndef Swing_GaussianBlurHEffect_H
#define Swing_GaussianBlurHEffect_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEShaderEffect.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:Gaussian blur horizontally effect��
// ˵��:
// ����:Sun Che
// ʱ��:20090310
//----------------------------------------------------------------------------
class SE_FOUNDATION_API GaussianBlurHEffect : public ShaderEffect
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    GaussianBlurHEffect(const std::string& rBaseName);
    virtual ~GaussianBlurHEffect(void);

    // ����color buffer�ߴ����texel kernel.
    static void GenerateTexelKernel(int iWidth, int iHeight);

protected:
    // streaming
    GaussianBlurHEffect(void);

    virtual void OnLoadPrograms(int iPass, SEProgram* pVProgram,
        SEProgram* pPProgram, SEProgram* pGProgram);

    // ���������Kernel���ֵ
    enum { KERNEL_COUNT = 13 };
    // ��Ը�profile��ʵ�ʿ�������.
	enum 
    { 
        PS_2_0_COUNT = 12,
        PS_3_0_COUNT = 13,
        ARBFP1_COUNT = 12,
        FP40_COUNT = 13
    };
    static SEVector2f ms_aTexelKernel[KERNEL_COUNT];
    static SEVector2f ms_aPixelKernelH[KERNEL_COUNT];
    static bool ms_bTexelKernelUCInitialized;
};

typedef SESmartPointer<GaussianBlurHEffect> GaussianBlurHEffectPtr;

}

#endif
