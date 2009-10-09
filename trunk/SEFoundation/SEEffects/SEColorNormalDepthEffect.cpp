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

#include "SEFoundationPCH.h"
#include "SEColorNormalDepthEffect.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, ColorNormalDepthEffect, ShaderEffect);
SE_IMPLEMENT_STREAM(ColorNormalDepthEffect);
SE_IMPLEMENT_DEFAULT_STREAM(ColorNormalDepthEffect, ShaderEffect);
SE_IMPLEMENT_DEFAULT_NAME_ID(ColorNormalDepthEffect, ShaderEffect);

//SE_REGISTER_STREAM(ColorNormalDepthEffect);

float ColorNormalDepthEffect::ms_fFarCilpDist = 0.0f;

//----------------------------------------------------------------------------
ColorNormalDepthEffect::ColorNormalDepthEffect()
    :
    ShaderEffect(1)
{
    m_VShader[0] = SE_NEW VertexShader("ColorNormalDepth.v_ColorNormalDepth");
    m_PShader[0] = SE_NEW PixelShader("ColorNormalDepth.p_ColorNormalDepth");
}
//----------------------------------------------------------------------------
ColorNormalDepthEffect::~ColorNormalDepthEffect()
{
}
//----------------------------------------------------------------------------
void ColorNormalDepthEffect::OnLoadPrograms(int, Program* pVProgram, Program*,
    Program*)
{
    UserConstant* pUC = pVProgram->GetUC("fFarClipDist");
    if( pUC )
        pUC->SetDataSource(&ms_fFarCilpDist);
}
//----------------------------------------------------------------------------
void ColorNormalDepthEffect::OnPreApplyEffect(Renderer*, bool)
{
    ms_fFarCilpDist = FarCilpDist;
}
//----------------------------------------------------------------------------