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

#ifndef Swing_WglES2_Renderer_H
#define Swing_WglES2_Renderer_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <windows.h>
#include <EGL/egl.h>

#include "SEOGLES2RendererLIB.h"
#include "SEOGLES2Renderer.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:Microsoft Windows OpenGL ES2 Renderer��
// ˵��:
// ����:Sun Che
// ʱ��:20090701
//----------------------------------------------------------------------------
class SE_RENDERER_API WglES2Renderer : public OGLES2Renderer
{
public:
    WglES2Renderer(EGLNativeWindowType hWnd, FrameBuffer::FormatType eFormat,
        FrameBuffer::DepthType eDepth, FrameBuffer::StencilType eStencil,
        FrameBuffer::BufferingType eBuffering,
        FrameBuffer::MultisamplingType eMultisampling, int iWidth,
        int iHeight);

    virtual ~WglES2Renderer(void);

    virtual void ToggleFullscreen(void);
    virtual void DisplayBackBuffer(void);
    virtual int LoadFont(const char* acFace, int iSize, bool bBold,
        bool bItalic);

    // EGL��ز���.
    EGLNativeWindowType GetWindow(void){ return m_hWnd; }
    EGLDisplay GetDisplay(void){ return m_eglDisplay; }
    EGLContext GetContext(void){ return m_eglContext; }
    EGLSurface GetSurface(void){ return m_eglSurface; }

protected:
    // EGL��ز���.
    EGLNativeWindowType m_hWnd;
    EGLDisplay m_eglDisplay;
    EGLContext m_eglContext;
    EGLSurface m_eglSurface;

    // ����֧��fullscreen/window
    int m_iSaveWidth, m_iSaveHeight;
};

}

#endif
