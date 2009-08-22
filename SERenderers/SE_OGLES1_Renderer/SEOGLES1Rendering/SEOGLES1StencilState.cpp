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

GLenum OGLES1Renderer::ms_aeStencilCompare[StencilState::CF_COUNT] =
{
    GL_NEVER,     // StencilState::CF_NEVER
    GL_LESS,      // StencilState::CF_LESS
    GL_EQUAL,     // StencilState::CF_EQUAL
    GL_LEQUAL,    // StencilState::CF_LEQUAL
    GL_GREATER,   // StencilState::CF_GREATER
    GL_NOTEQUAL,  // StencilState::CF_NOTEQUAL
    GL_GEQUAL,    // StencilState::CF_GEQUAL
    GL_ALWAYS     // StencilState::CF_ALWAYS
};

GLenum OGLES1Renderer::ms_aeStencilOperation[StencilState::OT_COUNT] =
{
    GL_KEEP,      // StencilState::OT_KEEP
    GL_ZERO,      // StencilState::OT_ZERO
    GL_REPLACE,   // StencilState::OT_REPLACE
    GL_INCR,      // StencilState::OT_INCREMENT
    GL_DECR,      // StencilState::OT_DECREMENT
    GL_INVERT     // StencilState::OT_INVERT
};

//----------------------------------------------------------------------------
void OGLES1Renderer::SetStencilState(StencilState* pState)
{
    Renderer::SetStencilState(pState);

    if( pState->Enabled )
    {
        glEnable(GL_STENCIL_TEST);

        glStencilFunc(ms_aeStencilCompare[pState->Compare],
            (GLint)pState->Reference, pState->Mask);

        glStencilMask(pState->WriteMask);

        glStencilOp(ms_aeStencilOperation[pState->OnFail],
            ms_aeStencilOperation[pState->OnZFail],
            ms_aeStencilOperation[pState->OnZPass]);
    }
    else
    {
        glDisable(GL_STENCIL_TEST);
    }
}
//----------------------------------------------------------------------------
