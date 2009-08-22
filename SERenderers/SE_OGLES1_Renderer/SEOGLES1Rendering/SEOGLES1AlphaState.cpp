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

#include "SEOGLES1RendererPCH.h"
#include "SEOGLES1Renderer.h"

using namespace Swing;

// ע��:
// OpenGL ES1��֧��constant color��ص�alphaö��ֵ.
// �����ص�������������ӳ���Ϊ0���û������α���ʹ����Щ������������.

GLenum OGLES1Renderer::ms_aeAlphaSrcBlend[AlphaState::SBF_COUNT] =
{
    GL_ZERO,
    GL_ONE,
    GL_DST_COLOR,
    GL_ONE_MINUS_DST_COLOR,
    GL_SRC_ALPHA,
    GL_ONE_MINUS_SRC_ALPHA,
    GL_DST_ALPHA,
    GL_ONE_MINUS_DST_ALPHA,
    GL_SRC_ALPHA_SATURATE,
    0,    // ��֧��
    0,    // ��֧��
    0,    // ��֧��
    0     // ��֧��
};

GLenum OGLES1Renderer::ms_aeAlphaDstBlend[AlphaState::DBF_COUNT] =
{
    GL_ZERO,
    GL_ONE,
    GL_SRC_COLOR,
    GL_ONE_MINUS_SRC_COLOR,
    GL_SRC_ALPHA,
    GL_ONE_MINUS_SRC_ALPHA,
    GL_DST_ALPHA,
    GL_ONE_MINUS_DST_ALPHA,
    0,    // ��֧��
    0,    // ��֧��
    0,    // ��֧��
    0     // ��֧��
};

GLenum OGLES1Renderer::ms_aeAlphaTest[AlphaState::TF_COUNT] =
{
    GL_NEVER,
    GL_LESS,
    GL_EQUAL,
    GL_LEQUAL,
    GL_GREATER,
    GL_NOTEQUAL,
    GL_GEQUAL,
    GL_ALWAYS
};

//----------------------------------------------------------------------------
void OGLES1Renderer::SetAlphaState(AlphaState* pState)
{
    Renderer::SetAlphaState(pState);

    if( pState->BlendEnabled )
    {
        glEnable(GL_BLEND);
        glBlendFunc(ms_aeAlphaSrcBlend[pState->SrcBlend],
            ms_aeAlphaDstBlend[pState->DstBlend]);
    }
    else
    {
        glDisable(GL_BLEND);
    }

    if( pState->TestEnabled )
    {
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(ms_aeAlphaTest[pState->Test], pState->Reference);
    }
    else
    {
        glDisable(GL_ALPHA_TEST);
    }
}
//----------------------------------------------------------------------------
