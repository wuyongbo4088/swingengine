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

#include "SEOGLES2RendererPCH.h"
#include "SEOGLES2Renderer.h"

using namespace Swing;

// 注意:
// OpenGL ES2 只支持GL_POLYGON_OFFSET_FILL模式.
// 因此用户有责任避免使用SEPolygonOffsetState的其他模式.

//----------------------------------------------------------------------------
void SEOGLES2Renderer::SetPolygonOffsetState(SEPolygonOffsetState* pState)
{
    SERenderer::SetPolygonOffsetState(pState);

    if( pState->FillEnabled )
    {
        glEnable(GL_POLYGON_OFFSET_FILL);
    }
    else
    {
        glDisable(GL_POLYGON_OFFSET_FILL);
    }

    glPolygonOffset(pState->Scale, pState->Bias);
}
//----------------------------------------------------------------------------
