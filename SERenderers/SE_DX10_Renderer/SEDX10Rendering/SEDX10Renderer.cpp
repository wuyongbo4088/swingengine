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

#include "SEDX10RendererPCH.h"
#include "SEDX10Renderer.h"
#include "SECamera.h"

using namespace Swing;

HRESULT DX10Renderer::ms_hResult = 0;

//----------------------------------------------------------------------------
DX10Renderer::DX10Renderer(HWND hWnd, FrameBuffer::FormatType eFormat,
    FrameBuffer::DepthType eDepth, FrameBuffer::StencilType eStencil,
    FrameBuffer::BufferingType eBuffering,
    FrameBuffer::MultisamplingType eMultisampling, int iWidth, int iHeight)
    :
    Renderer(eFormat, eDepth, eStencil, eBuffering, eMultisampling, iWidth, 
        iHeight)
{
    m_SwapChainDesc.BufferDesc.Width = iWidth;
    m_SwapChainDesc.BufferDesc.Height = iHeight;
    m_SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    m_SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    m_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    m_SwapChainDesc.BufferDesc.ScanlineOrdering = 
        DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    m_SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    m_SwapChainDesc.BufferCount = 1;
    m_SwapChainDesc.OutputWindow = hWnd;
    m_SwapChainDesc.Windowed = true;
    m_SwapChainDesc.Flags = 0;

    // ����multisampling.
    m_SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //multisampling����
    m_SwapChainDesc.SampleDesc.Count = 1;
    m_SwapChainDesc.SampleDesc.Quality = 0;
    if( m_eMultisampling == FrameBuffer::MT_SAMPLING_4 )
    {
        // ��ʵ��.
        // ����豸�Ƿ�֧��multisampling 4 samples.
        m_SwapChainDesc.SampleDesc.Count = 4;
    }
    else if( m_eMultisampling == FrameBuffer::MT_SAMPLING_2 )
    {
        // ��ʵ��.
        // ����豸�Ƿ�֧��multisampling 2 samples.
        m_SwapChainDesc.SampleDesc.Count = 2;
    }

    UINT uiCreateDeviceFlags = 0;
#if defined(_DEBUG)  
    uiCreateDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif

    ms_hResult = D3D10CreateDeviceAndSwapChain(
        0,  //default adapter
        D3D10_DRIVER_TYPE_HARDWARE,
        0,  // no software device
        uiCreateDeviceFlags, 
        D3D10_SDK_VERSION,
        &m_SwapChainDesc,
        &m_pDXGISwapChain,
        &m_pDX10Device);
    SE_ASSERT( SUCCEEDED(ms_hResult) );

    // ����back buffer��d3d10 render target view.
    ID3D10Texture2D* pDX10BackBuffer;
    ms_hResult = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), 
        reinterpret_cast<void**>(&pDX10BackBuffer));
    SE_ASSERT( SUCCEEDED(ms_hResult) );
    ms_hResult = m_pDX10Device->CreateRenderTargetView(pDX10BackBuffer, 0, 
        &m_pDX10RTView);
    SE_DX10_SAFE_RELEASE(pDX10BackBuffer);

    // ����depth/stencil buffer����view.
    // ��ʵ��,Ŀǰֻ�Ǵ���һ���������D24S8��ʽ������Ϊ��buffer.
    D3D10_TEXTURE2D_DESC tempDSDesc;
    tempDSDesc.Width = iWidth;
    tempDSDesc.Height = iHeight;
    tempDSDesc.MipLevels = 1;
    tempDSDesc.ArraySize = 1;
    tempDSDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    // multisampling must match swap chain values.
    tempDSDesc.SampleDesc.Count = m_SwapChainDesc.SampleDesc.Count;
    tempDSDesc.SampleDesc.Quality = m_SwapChainDesc.SampleDesc.Quality;
    tempDSDesc.Usage = D3D10_USAGE_DEFAULT;
    tempDSDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
    tempDSDesc.CPUAccessFlags = 0; 
    tempDSDesc.MiscFlags = 0;

    ms_hResult = m_pDX10Device->CreateTexture2D(&tempDSDesc, 0, &m_pDX10DSBuffer);
    SE_ASSERT( SUCCEEDED(ms_hResult) );
    ms_hResult = m_pDX10Device->CreateDepthStencilView(m_pDX10DSBuffer, 0, 
        &m_pDX10DSView);
    SE_ASSERT( SUCCEEDED(ms_hResult) );

    // bind��ǰrender target view��depth/stencil view.
    m_pDX10Device->OMSetRenderTargets(1, &m_pDX10RTView, m_pDX10DSView);

    // ����Ĭ���ӿڱ任.
    D3D10_VIEWPORT tempViewport;
    tempViewport.TopLeftX = 0;
    tempViewport.TopLeftY = 0;
    tempViewport.Width = iWidth;
    tempViewport.Height = iHeight;
    tempViewport.MinDepth = 0.0f;
    tempViewport.MaxDepth = 1.0f;
    m_pDX10Device->RSSetViewports(1, &tempViewport);

    // ����Ĭ������.
    m_iFontID = LoadFont("Arial", 18, false, false);
    SE_ASSERT( m_iFontID == 0 );

    // ����֧��DX10���豸��������ѭͳһ���豸����,���DX10û�в�ѯ�豸������֧�ֺ���.

    // OpenGLֻ֧��primary��secondary���鶥����ɫ.
    // ������DirectX��Ⱦ�����䱣��һ��.
    m_iMaxColors = 2;
    // ��ʵ��.
    // ��Щ������ο�DX10 SDK�ĵ�,ĿǰΪ����ֵ.
    m_iMaxTCoords = 8;
    // VS_4_0֧��vertex shader texture.
    m_iMaxVShaderImages = 4;
    m_iMaxGShaderImages = 4;
    m_iMaxPShaderImages = m_iMaxTCoords;
    m_iMaxUserClipPlanes = 8;
    m_iMaxStencilIndices = 8;
    m_iMaxVShaderProfile = Renderer::VS_4_0;
    m_iMaxGShaderProfile = Renderer::GS_4_0;
    m_iMaxPShaderProfile = Renderer::PS_4_0;
    m_iMaxRenderTargets = 8;

    // ����ڹ̶�����T&L����,shader-based engine������������,
    // Swing EngineĿǰֻ֧��8.
    m_iMaxLights = 8;
    m_aspLight = SE_NEW ObjectPtr[m_iMaxLights];

    // Cg runtime���.
    cgD3D10SetDevice(m_CgContext, m_pDX10Device);
    SE_DEBUG_CG_PROGRAM;
    m_LatestVProfile = cgD3D10GetLatestVertexProfile();
    m_LatestGProfile = cgD3D10GetLatestGeometryProfile();
    m_LatestPProfile = cgD3D10GetLatestPixelProfile();

    // ����������Ĭ��rasterizer state.
    // Disable culling
    D3D10_RASTERIZER_DESC tempRSDesc;
    tempRSDesc.FillMode = D3D10_FILL_SOLID;
    tempRSDesc.CullMode = D3D10_CULL_BACK;
    tempRSDesc.FrontCounterClockwise = false;
    tempRSDesc.DepthBias = 0;
    tempRSDesc.DepthBiasClamp = 0.0f;
    tempRSDesc.SlopeScaledDepthBias = 0.0f;
    tempRSDesc.ScissorEnable = false;
    tempRSDesc.MultisampleEnable = false;
    tempRSDesc.AntialiasedLineEnable = false;

    ms_hResult = m_pDX10Device->CreateRasterizerState(&tempRSDesc, 
        &m_pDX10RasterizerState);
    SE_ASSERT( SUCCEEDED(ms_hResult) );

    m_pDX10Device->RSSetState(m_pDX10RasterizerState);

    //// ��ʼ��ȫ����Ⱦ״̬Ϊ����Ĭ������.
    //SetGlobalState(GlobalState::Default);

    m_bCursorVisible = true;
    m_bDeviceLost = false;
    m_bBeginSceneActive = false;
}
//----------------------------------------------------------------------------
DX10Renderer::~DX10Renderer()
{
    // �ͷ���������.
    for( int i = 0; i < (int)m_FontArray.size(); i++ )
    {
        UnloadFont(i);
    }

    // ����cursor.
    if( !m_bCursorVisible )
    {
        ShowCursor(true);
    }

    m_pDX10RasterizerState->Release();
    m_pDX10Device->ClearState();

    cgD3D10SetDevice(m_CgContext, 0);
    SE_DEBUG_CG_PROGRAM;
}
//----------------------------------------------------------------------------
void DX10Renderer::ToggleFullscreen()
{
    //Renderer::ToggleFullscreen();
    //m_Present.Windowed = !m_Present.Windowed;

    //if( m_Present.Windowed )
    //{
    //    m_Present.FullScreen_RefreshRateInHz = 0;
    //}
    //else
    //{
    //    GetWindowRect(m_Present.hDeviceWindow, &m_rcWindow);
    //    m_Present.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    //}

    //OnLostDevice();
    //OnResetDevice(); 

    //if( m_Present.Windowed )
    //{
    //    SetWindowPos(m_Present.hDeviceWindow, 
    //        NULL, 
    //        m_rcWindow.left,
    //        m_rcWindow.top, 
    //        m_rcWindow.right - m_rcWindow.left,
    //        m_rcWindow.bottom - m_rcWindow.top,
    //        SWP_NOZORDER | SWP_DRAWFRAME | SWP_SHOWWINDOW);
    //}
}
//----------------------------------------------------------------------------
bool DX10Renderer::BeginScene()
{
    //ms_hResult = m_pDXDevice->TestCooperativeLevel();
    //
    //switch( ms_hResult )
    //{
    //case D3DERR_DEVICELOST:
    //    if( !m_bDeviceLost )
    //    {
    //        m_bDeviceLost = true;
    //        OnLostDevice();
    //    }
    //    return false;
    //case D3DERR_DEVICENOTRESET:
    //    OnResetDevice();
    //    m_bDeviceLost = false;
    //    break;
    //}

    //ms_hResult = m_pDXDevice->BeginScene();
    //SE_ASSERT( SUCCEEDED(ms_hResult) );

    //m_bBeginSceneActive = true;

    return true;
}
//----------------------------------------------------------------------------
void DX10Renderer::EndScene()
{
    //ms_hResult = m_pDXDevice->EndScene();
    //SE_ASSERT( SUCCEEDED(ms_hResult) );

    //m_bBeginSceneActive = false;
}
//----------------------------------------------------------------------------
void DX10Renderer::OnLostDevice()
{
    //// release font.
    //m_FontArray[0]->OnLostDevice();

    //// release framebuffer.
    //int iFrameBufferCount = (int)m_FrameBuffers.size();
    //for( int i = 0; i < iFrameBufferCount; i++ )
    //{
    //    m_FrameBuffers[i]->OnLostDevice();
    //}
}
//----------------------------------------------------------------------------
void DX10Renderer::OnResetDevice()
{
    //ms_hResult = m_pDXDevice->Reset(&m_Present);
    //SE_ASSERT( SUCCEEDED(ms_hResult) );

    //// reset font.
    //m_FontArray[0]->OnResetDevice();

    //// reset framebuffer.
    //int iFrameBufferCount = (int)m_FrameBuffers.size();
    //for( int i = 0; i < iFrameBufferCount; i++ )
    //{
    //    m_FrameBuffers[i]->OnResetDevice();
    //}

    //OnFrustumChange();
    //OnViewportChange();
    //OnFrameChange();
}
//----------------------------------------------------------------------------
void DX10Renderer::ClearBackBuffer()
{
    //D3DXCOLOR tempClearColor(m_ClearColor.R, m_ClearColor.G, m_ClearColor.B, 
    //    m_ClearColor.A);

    m_pDX10Device->ClearRenderTargetView(m_pDX10RTView, m_ClearColor);
}
//----------------------------------------------------------------------------
void DX10Renderer::ClearZBuffer()
{
    m_pDX10Device->ClearDepthStencilView(m_pDX10DSView, D3D10_CLEAR_DEPTH,
        m_fClearDepth, 0);
}
//----------------------------------------------------------------------------
void DX10Renderer::ClearStencilBuffer()
{
    m_pDX10Device->ClearDepthStencilView(m_pDX10DSView, D3D10_CLEAR_STENCIL, 
        1.0f, (UINT8)m_uiClearStencil);
}
//----------------------------------------------------------------------------
void DX10Renderer::ClearBuffers()
{
    m_pDX10Device->ClearRenderTargetView(m_pDX10RTView, m_ClearColor);
    m_pDX10Device->ClearDepthStencilView(m_pDX10DSView, D3D10_CLEAR_DEPTH | 
        D3D10_CLEAR_STENCIL, m_fClearDepth, (UINT8)m_uiClearStencil);
}
//----------------------------------------------------------------------------
void DX10Renderer::ClearBackBuffer(int, int, int, int)
{
    // ��ʵ��.
}
//----------------------------------------------------------------------------
void DX10Renderer::ClearZBuffer(int, int, int, int)
{
    // ��ʵ��.
}
//----------------------------------------------------------------------------
void DX10Renderer::ClearStencilBuffer(int, int, int, int)
{
    // ��ʵ��.
}
//----------------------------------------------------------------------------
void DX10Renderer::ClearBuffers(int, int, int, int)
{
    // ��ʵ��.
}
//----------------------------------------------------------------------------
void DX10Renderer::DisplayBackBuffer()
{
    // ��ʵ��.
    // ��鵱ǰ�豸״̬,�����Ƿ��Ѷ�ʧ.
    m_pDXGISwapChain->Present(0, 0);
} 
//----------------------------------------------------------------------------
void DX10Renderer::SetColorMask(bool bAllowRed, bool bAllowGreen,
    bool bAllowBlue, bool bAllowAlpha)
{
    Renderer::SetColorMask(bAllowRed, bAllowGreen, bAllowBlue, bAllowAlpha);
}
//----------------------------------------------------------------------------
void DX10Renderer::EnableUserClipPlane(int, const Plane3f&)
{
    // ��ʵ��.
}
//----------------------------------------------------------------------------
void DX10Renderer::DisableUserClipPlane(int)
{
    // ��ʵ��.
}
//----------------------------------------------------------------------------
const ID3D10Device* DX10Renderer::GetDevice()
{
    return m_pDX10Device;
}
//----------------------------------------------------------------------------
void DX10Renderer::OnPreDrawGeometry()
{
    SE_ASSERT( m_pGeometry->RStateBlock );

    EnableRenderStateBlock(m_pGeometry->RStateBlock);
}
//----------------------------------------------------------------------------
void DX10Renderer::OnPostDrawGeometry()
{
    // �����κβ���.
}
//----------------------------------------------------------------------------
void DX10Renderer::OnPreDrawPass(ShaderEffect* pEffect, int iPass, bool)
{
    RenderStateBlock* pRStateBlock = pEffect->GetRStateBlock(iPass);
    SE_ASSERT( pRStateBlock );

    EnableRenderStateBlock(pRStateBlock);
}
//----------------------------------------------------------------------------
void DX10Renderer::OnPostDrawPass(ShaderEffect*, int, bool)
{
    // �����κβ���.
}
//----------------------------------------------------------------------------