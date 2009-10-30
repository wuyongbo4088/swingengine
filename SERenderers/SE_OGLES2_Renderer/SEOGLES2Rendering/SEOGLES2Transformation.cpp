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
#include "SECamera.h"

using namespace Swing;

//----------------------------------------------------------------------------
void OGLES2Renderer::OnViewportChange()
{
    float fPortL, fPortR, fPortT, fPortB;
    if( m_pCamera )
    {
        // 3DӦ�ó���ʹ��camera.
        m_pCamera->GetViewport(fPortL, fPortR, fPortT, fPortB);
    }
    else
    {
        // 2DӦ�ó�����camera.
        fPortL = 0.0f;
        fPortR = 1.0f;
        fPortT = 1.0f;
        fPortB = 0.0f;
    }

    // OpenGL��viewport����ԭ�㶨������Ļ���½�.
    // fPortBָ��viewport�������Ļ�·��ľ���.
    // Swing EngineĿǰҲʹ�����Լ��.
    // ��DirectX��ͬ(iY����DirectXʱ��ͬ).
    GLint iX = (GLint)(fPortL*m_iWidth);
    GLint iY = (GLint)(fPortB*m_iHeight);
    GLsizei iW = (GLsizei)((fPortR - fPortL)*m_iWidth);
    GLsizei iH = (GLsizei)((fPortT - fPortB)*m_iHeight);
    glViewport(iX, iY, iW, iH);
}
//----------------------------------------------------------------------------
void OGLES2Renderer::OnDepthRangeChange()
{
    if( m_pCamera )
    {
        float fPortN, fPortF;
        m_pCamera->GetDepthRange(fPortN, fPortF);
        glDepthRangef((GLclampf)fPortN, (GLclampf)fPortF);
    }
    else
    {
        glDepthRangef(0.0, 1.0);
    }
}
//----------------------------------------------------------------------------