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

#include "SEiPhoneOES2Renderer.h"

using namespace Swing;

//----------------------------------------------------------------------------
iPhoneOES2Renderer::iPhoneOES2Renderer(UIWindow* pWindow,
    FrameBuffer::FormatType eFormat, FrameBuffer::DepthType eDepth,
    FrameBuffer::StencilType eStencil, FrameBuffer::BufferingType eBuffering,
    FrameBuffer::MultisamplingType eMultisampling, int iX, int iY, int iWidth,
    int iHeight)
    :
    OGLES2Renderer(eFormat, eDepth, eStencil, eBuffering, eMultisampling, 
        iWidth, iHeight)
{
    SE_ASSERT( m_iWidth > 0 && m_iHeight > 0 );
    SE_ASSERT( pWindow );
    
    m_iSaveWidth = m_iWidth;
    m_iSaveHeight = m_iHeight;

    m_iX = iX;
    m_iY = iY;
    // �Ժ󴴽�EAGL2View.
    m_pEAGL2View = 0;
    m_pWindow = pWindow;
    m_bFullScreen = false;

    // Request a depth buffer.
    if( m_eDepth != FrameBuffer::DT_DEPTH_NONE )
    {
        // ��ʵ��.
        // Ŀǰֻ֧��GL_DEPTH_COMPONENT16_OES.
    }

    // Request a stencil buffer.
    if( m_eStencil == FrameBuffer::ST_STENCIL_8 )
    {
        // ��ʵ��.
    }

    // Request buffering.
    if( m_eBuffering == FrameBuffer::BT_BUFFERED_DOUBLE )
    {
        // ��ʵ��.
    }

    // Request sampling.
    if( m_eMultisampling != FrameBuffer::MT_SAMPLING_NONE )
    {
        // ��ʵ��.
    }

    // ����EAGL2View����EAGLContext.
    CGRect tempRect = [[UIScreen mainScreen] bounds];
    m_pEAGL2View = [[EAGL2View alloc] initWithFrame:tempRect 
        pixelFormat:GL_RGB565 depthFormat:GL_DEPTH_COMPONENT16 
        preserveBackbuffer:NO];

    // ��EAGL2View�󶨸�UIWindow.
    [m_pWindow addSubview:m_pEAGL2View];

    // ��ʼ��OpenGL����,��ȡ�豸����.
    InitializeState();
}
//----------------------------------------------------------------------------
iPhoneOES2Renderer::~iPhoneOES2Renderer()
{
    [m_pEAGL2View release];
}
//----------------------------------------------------------------------------
void iPhoneOES2Renderer::Resize(int iWidth, int iHeight)
{
    // ��ʵ��.
}
//----------------------------------------------------------------------------
void iPhoneOES2Renderer::ToggleFullscreen()
{
    // ��ʵ��.
}
//----------------------------------------------------------------------------
void iPhoneOES2Renderer::DisplayBackBuffer()
{
    [m_pEAGL2View swapBuffers];
}
//----------------------------------------------------------------------------
int iPhoneOES2Renderer::LoadFont(const char*, int, bool, bool)
{
    // ��ʵ��.
    return 0;
}
//----------------------------------------------------------------------------
EAGL2View* iPhoneOES2Renderer::GetView()
{
    return m_pEAGL2View;
}
//----------------------------------------------------------------------------
UIWindow* iPhoneOES2Renderer::GetWindow()
{
    return m_pWindow;
}
//----------------------------------------------------------------------------