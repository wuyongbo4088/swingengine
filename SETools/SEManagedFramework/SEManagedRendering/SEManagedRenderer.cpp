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

#include "SEManagedFrameworkPCH.h"
#include "SEManagedRenderer.h"

using namespace Swing;
using namespace Swing::Tools::ManagedFramework;

//---------------------------------------------------------------------------
ManagedRenderer::ManagedRenderer(IntPtr hWnd, int iWidth, int iHeight)
{
    m_pRenderer = SE_NEW DX9Renderer((HWND)(int)hWnd, 
        FrameBuffer::FT_FORMAT_RGBA, 
        FrameBuffer::DT_DEPTH_24, 
        FrameBuffer::ST_STENCIL_8, 
        FrameBuffer::BT_BUFFERED_DOUBLE, 
        FrameBuffer::MT_SAMPLING_NONE, 
        iWidth, iHeight);
}
//---------------------------------------------------------------------------
ManagedRenderer::~ManagedRenderer()
{
    SE_DELETE m_pRenderer;
    m_pRenderer = 0;
}
//---------------------------------------------------------------------------
void ManagedRenderer::SetCamera(ManagedCamera^ thCamera)
{
    m_thCamera = thCamera;

    if( thCamera )
    {
        m_pRenderer->SetCamera(thCamera->GetNativeCamera());
    }
    else
    {
        m_pRenderer->SetCamera(0);
    }
}
//---------------------------------------------------------------------------
ManagedCamera^ ManagedRenderer::GetCamera()
{
    return m_thCamera;
}
//---------------------------------------------------------------------------
void ManagedRenderer::SetClearColor(ManagedColorRGBA^ thClearColor)
{
#if defined(_DEBUG)
    if( !thClearColor )
    {
        throw gcnew ArgumentNullException("thClearColor");
    }
#endif

    ColorRGBA tempColor;
    thClearColor->ToColorRGBA(tempColor);
    m_pRenderer->SetClearColor(tempColor);
}
//---------------------------------------------------------------------------
ManagedColorRGBA^ ManagedRenderer::GetClearColor()
{
    ManagedColorRGBA^ thClearColor = gcnew ManagedColorRGBA;
    thClearColor->FromColorRGBA(m_pRenderer->GetClearColor());

    return thClearColor;
}
//---------------------------------------------------------------------------
bool ManagedRenderer::BeginScene()
{
    return m_pRenderer->BeginScene();
}
//---------------------------------------------------------------------------
void ManagedRenderer::EndScene()
{
    m_pRenderer->EndScene();
}
//---------------------------------------------------------------------------
void ManagedRenderer::DrawSceneFromCuller(ManagedCuller^ thCuller)
{
#if defined(_DEBUG)
    if( !thCuller )
    {
        throw gcnew ArgumentNullException("thCuller");
    }
#endif

    m_pRenderer->DrawScene(thCuller->GetNativeCuller()->GetVisibleSet());
}
//---------------------------------------------------------------------------
void ManagedRenderer::ClearBackBuffer()
{
    m_pRenderer->ClearBackBuffer();
}
//---------------------------------------------------------------------------
void ManagedRenderer::ClearZBuffer()
{
    m_pRenderer->ClearZBuffer();
}
//---------------------------------------------------------------------------
void ManagedRenderer::ClearStencilBuffer()
{
    m_pRenderer->ClearStencilBuffer();
}
//---------------------------------------------------------------------------
void ManagedRenderer::ClearBuffers()
{
    m_pRenderer->ClearBuffers();
}
//---------------------------------------------------------------------------
void ManagedRenderer::DisplayBackBuffer()
{
    m_pRenderer->DisplayBackBuffer();
}
//---------------------------------------------------------------------------
Renderer* ManagedRenderer::GetNativeRenderer()
{
    return m_pRenderer;
}
//---------------------------------------------------------------------------