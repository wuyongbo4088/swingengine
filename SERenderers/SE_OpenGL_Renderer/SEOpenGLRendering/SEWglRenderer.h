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

#ifndef Swing_Wgl_Renderer_H
#define Swing_Wgl_Renderer_H

#include "SEOpenGLRendererLIB.h"
#include "SEOpenGLRenderer.h"
#include "SEWglExtensions.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:Microsoft Windows OpenGL2.0 Renderer��
// ˵��:
// ����:Sun Che
// ʱ��:20080920
//----------------------------------------------------------------------------
class SE_RENDERER_API WglRenderer : public OpenGLRenderer
{
public:
    // ���캯����Ӧ�ó�����һ�ε���ʱ,�����pixel format�������Ե�.
    // ���multisampling������,��Ⱦ���ʹ�����뱻�ͷŵ�,
    // Ȼ������֧��multisampling��pixel format�����´�����Ⱦ���ʹ���.
    // ��SEWglApplication.cpp���Բ鿴�����������.
    WglRenderer(HWND hWnd, FrameBuffer::FormatType eFormat,
        FrameBuffer::DepthType eDepth, FrameBuffer::StencilType eStencil,
        FrameBuffer::BufferingType eBuffering,
        FrameBuffer::MultisamplingType eMultisampling, int iWidth,
        int iHeight, int iPixelFormat = 0);

    virtual ~WglRenderer(void);

    virtual void ToggleFullscreen(void);
    virtual void DisplayBackBuffer(void);
    virtual int LoadFont(const char* acFace, int iSize, bool bBold,
        bool bItalic);

    // �������.
    HWND GetWindow(void){ return m_hWnd; }
    HDC GetDC(void){ return m_hWindowDC; }
    HGLRC GetRC(void){ return m_hWindowRC; }

protected:
    // �������.
    HWND m_hWnd;
    HDC m_hWindowDC;
    HGLRC m_hWindowRC;
    HGDIOBJ m_hOldFont;

    // ����֧��fullscreen/window
    int m_iSaveWidth, m_iSaveHeight;
};

}

#endif
