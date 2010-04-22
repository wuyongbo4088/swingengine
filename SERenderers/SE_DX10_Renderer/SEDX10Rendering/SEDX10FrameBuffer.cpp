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
#include "SEDX10FrameBuffer.h"
#include "SEDX10Resources.h"

using namespace Swing;

SE_IMPLEMENT_INITIALIZE(SEDX10FrameBuffer);

//SE_REGISTER_INITIALIZE(SEDX10FrameBuffer);

//----------------------------------------------------------------------------
void SEDX10FrameBuffer::Initialize()
{
    ms_aoCreator[SERenderer::DIRECTX10] = &SEDX10FrameBuffer::Create;
    ms_aoDestroyer[SERenderer::DIRECTX10] = &SEDX10FrameBuffer::Destroy;
}
//----------------------------------------------------------------------------
SEFrameBuffer* SEDX10FrameBuffer::Create(FormatType eFormat, DepthType eDepth,
    StencilType eStencil, BufferingType eBuffering,
    MultisamplingType eMultisampling, SERenderer* pRenderer, int iTCount,
    SETexture** apTargets)
{
    if( pRenderer && apTargets )
    {
        SEDX10FrameBuffer* pBuffer = SE_NEW SEDX10FrameBuffer(eFormat,
            eDepth, eStencil, eBuffering, eMultisampling, pRenderer, iTCount,
            apTargets);

        if( pBuffer->InitializeBuffer() )
        {
            return pBuffer;
        }

        SE_DELETE pBuffer;
    }

    return 0;
}
//----------------------------------------------------------------------------
void SEDX10FrameBuffer::Destroy(SEFrameBuffer* pBuffer)
{
    ((SEDX10FrameBuffer*)pBuffer)->TerminateBuffer();
    SE_DELETE pBuffer;
}
//----------------------------------------------------------------------------
SEDX10FrameBuffer::SEDX10FrameBuffer(FormatType eFormat, DepthType eDepth,
    StencilType eStencil, BufferingType eBuffering,
    MultisamplingType eMultisampling, SERenderer* pRenderer, int iTCount, 
    SETexture** apTargets)
    :
    SEFrameBuffer(eFormat, eDepth, eStencil, eBuffering, eMultisampling, 
        pRenderer, iTCount, apTargets)
{
    m_TargetItems.resize(iTCount);
    ((SEDX10Renderer*)pRenderer)->m_FrameBuffers.push_back(this);
}
//----------------------------------------------------------------------------
SEDX10FrameBuffer::~SEDX10FrameBuffer()
{
    SEDX10Renderer* pRenderer = (SEDX10Renderer*)m_pRenderer;
    std::vector<SEDX10FrameBuffer*>& rFrameBuffers = pRenderer->m_FrameBuffers;

    int iCount = (int)pRenderer->m_FrameBuffers.size();
    for( int i = 0; i < iCount; i++ )
    {
        if( rFrameBuffers[i] == this )
        {
            // �����Ҫ�����������Ԫ�ظ��ǵ�ǰԪ��.
            if( i < --iCount )
            {
                rFrameBuffers[i] = rFrameBuffers[iCount];
            }

            // ɾ�����Ԫ��.
            rFrameBuffers.pop_back();

            return;
        }
    }
}
//----------------------------------------------------------------------------
bool SEDX10FrameBuffer::InitializeBuffer()
{
    //SEDX10Renderer* pRenderer = (SEDX10Renderer*)m_pRenderer;
    //HRESULT hResult;

    //for( int i = 0; i < m_iCount; i++ )
    //{
    //    SEImage* pImage = m_apTargets[i]->GetImage();
    //    if( pImage->IsCubeImage() )
    //    {
    //        // ��ʵ��.
    //        // ��δ֧��cube map.
    //        TerminateBuffer();
    //        return false;
    //    }

    //    // ȷ������frame buffer�������Ѿ�װ�����Դ�.
    //    SEResourceIdentifier* pID = m_apTargets[i]->GetIdentifier(m_pRenderer);
    //    SE_ASSERT( pID );
    //    SETextureID* pResource = (SETextureID*)pID;
    //    m_TargetItems[i].TargetID = (IDirect3DTexture9*)pResource->ID;

    //    if( m_apTargets[i]->IsDepthTexture() )
    //    {
    //        hResult = m_TargetItems[i].TargetID->GetSurfaceLevel(0, 
    //            &m_TargetItems[i].DepthStencilSurface);
    //        if( !SUCCEEDED(hResult) )
    //        {
    //            m_TargetItems[i].DepthStencilSurface = 0;

    //            TerminateBuffer();
    //            return false;
    //        }
    //    }
    //    else
    //    {
    //        hResult = m_TargetItems[i].TargetID->GetSurfaceLevel(0, 
    //            &m_TargetItems[i].TextureSurface);
    //        if( !SUCCEEDED(hResult) )
    //        {
    //            m_TargetItems[i].TextureSurface = 0;

    //            TerminateBuffer();
    //            return false;
    //        }
    //    }
    //}

    //// ������ʽ��ID3DXRenderToSurface�ӿ�,ʹ�øýӿ�ʱ,
    //// ֻ����ʹ��һ��render target,�������ȡtarget0.
    //D3DSURFACE_DESC tempDesc;
    //hResult = m_TargetItems[0].TextureSurface->GetDesc(&tempDesc);
    //if( !SUCCEEDED(hResult) )
    //{
    //    SE_DX10_SAFE_RELEASE(m_TargetItems[0].TextureSurface);

    //    TerminateBuffer();
    //    return false;
    //}
    //hResult = D3DXCreateRenderToSurface(pRenderer->m_pDXDevice,
    //    tempDesc.Width, tempDesc.Height, tempDesc.Format, TRUE, 
    //    D3DFMT_D24S8, &m_pRenderToTexture);
    //if( !SUCCEEDED(hResult) )
    //{
    //    SE_DX10_SAFE_RELEASE(m_TargetItems[0].TextureSurface);
    //    m_pRenderToTexture = 0;

    //    TerminateBuffer();
    //    return false;
    //}

    return true;
}
//----------------------------------------------------------------------------
void SEDX10FrameBuffer::TerminateBuffer()
{
    //SE_DX10_SAFE_RELEASE(m_pRenderToTexture);

    //for( int i = 0; i < m_iCount; i++ )
    //{
    //    if( m_apTargets[i]->IsDepthTexture() )
    //    {
    //        SE_DX10_SAFE_RELEASE(m_TargetItems[i].DepthStencilSurface);
    //    }
    //    else
    //    {
    //        SE_DX10_SAFE_RELEASE(m_TargetItems[i].TextureSurface);
    //    }

    //    SE_DX10_SAFE_RELEASE(m_TargetItems[i].CopySurface);
    //    SE_DX10_SAFE_RELEASE(m_TargetItems[i].CopyTexture);
    //}
}
//----------------------------------------------------------------------------
void SEDX10FrameBuffer::Enable()
{
    //SEDX10Renderer* pRenderer = (SEDX10Renderer*)m_pRenderer;
    //HRESULT hResult;

    //if( pRenderer->m_bBeginSceneActive )
    //{
    //    for( int i = 0; i < m_iCount; i++ )
    //    {
    //        if( m_apTargets[i]->IsDepthTexture() )
    //        {
    //            hResult = pRenderer->m_pDXDevice->GetDepthStencilSurface(
    //                &m_TargetItems[i].SaveDepthStencilSurface);
    //            SE_ASSERT( SUCCEEDED(hResult) );

    //            hResult = pRenderer->m_pDXDevice->SetDepthStencilSurface(
    //                m_TargetItems[i].DepthStencilSurface);
    //            SE_ASSERT( SUCCEEDED(hResult) );
    //        }
    //        else
    //        {
    //            hResult = pRenderer->m_pDXDevice->GetRenderTarget(i,
    //                &m_TargetItems[i].SaveTextureSurface);
    //            if( !SUCCEEDED(hResult) )
    //                m_TargetItems[i].SaveTextureSurface = 0;

    //            hResult = pRenderer->m_pDXDevice->SetRenderTarget(i,
    //                m_TargetItems[i].TextureSurface);
    //            SE_ASSERT( SUCCEEDED(hResult) );
    //        }
    //    }
    //}
    //else
    //{
    //    // ID3DXRenderToSurface::BeginScene��֧��Ƕ�׵���,Ҳ��֧��MRT.
    //    SE_ASSERT( m_iCount == 1 );

    //    hResult = m_pRenderToTexture->BeginScene(
    //        m_TargetItems[0].TextureSurface, 0);
    //    SE_ASSERT( SUCCEEDED(hResult) );
    //}

    //pRenderer->OnFrustumChange();
    //pRenderer->OnFrameChange();

    //SEImage* pImage = m_apTargets[0]->GetImage();
    //D3DVIEWPORT9 tempViewport;
    //tempViewport.X = 0;
    //tempViewport.Y = 0;
    //tempViewport.Width = (DWORD)pImage->GetBound(0);
    //tempViewport.Height = (DWORD)pImage->GetBound(1);
    //tempViewport.MinZ = 0.0f;
    //tempViewport.MaxZ = 1.0f;
    //hResult = pRenderer->m_pDXDevice->SetViewport(&tempViewport);
    //SE_ASSERT( SUCCEEDED(hResult) );
}
//----------------------------------------------------------------------------
void SEDX10FrameBuffer::Disable()
{
    //SEDX10Renderer* pRenderer = (SEDX10Renderer*)m_pRenderer;
    //HRESULT hResult;

    //if( pRenderer->m_bBeginSceneActive )
    //{
    //    for( int i = 0; i < m_iCount; i++ )
    //    {
    //        if( m_apTargets[i]->IsDepthTexture() )
    //        {
    //            hResult = pRenderer->m_pDXDevice->SetDepthStencilSurface(
    //                m_TargetItems[i].SaveDepthStencilSurface);
    //            SE_ASSERT( SUCCEEDED(hResult) );

    //            m_TargetItems[i].SaveDepthStencilSurface->Release();
    //        }
    //        else
    //        {
    //            hResult = pRenderer->m_pDXDevice->SetRenderTarget(i,
    //                m_TargetItems[i].SaveTextureSurface);
    //            SE_ASSERT( SUCCEEDED(hResult) );

    //            if( m_TargetItems[i].SaveTextureSurface )
    //                m_TargetItems[i].SaveTextureSurface->Release();
    //        }
    //    }
    //}
    //else
    //{
    //    // ID3DXRenderToSurface::EndScene��֧��Ƕ�׵���,Ҳ��֧��MRT.
    //    SE_ASSERT( m_iCount == 1 );

    //    hResult = m_pRenderToTexture->EndScene(0);
    //    SE_ASSERT( SUCCEEDED(hResult) );
    //}

    //pRenderer->OnViewportChange();
    //pRenderer->OnFrustumChange();
    //pRenderer->OnFrameChange();
}
//----------------------------------------------------------------------------
void SEDX10FrameBuffer::CopyToTexture(int)
{
    //SE_ASSERT( i >= 0 && i < m_iCount );
    //i = i < 0 ? 0 : (i >= m_iCount ? m_iCount - 1 : i );

    //// ��ʵ��. 
    //// DX9��,Ŀǰû����֪�������԰����ͼ���ݴ�VRAM���Ƶ�ϵͳ�ڴ�.
    //if( !m_apTargets[i]->IsDepthTexture() )
    //{
    //    SEImage* pImage = m_apTargets[i]->GetImage();
    //    unsigned char* aucDst = pImage->GetData();
    //    int iCount = pImage->GetCount();
    //    int iByteCount = pImage->GetBytesPerPixel() * iCount;
    //    unsigned char* aucRDst = SE_NEW unsigned char[iByteCount];
    //    int j, iDstBase = 0, iRDstBase = 0;

    //    SEDX10Renderer* pRenderer = (SEDX10Renderer*)m_pRenderer;
    //    HRESULT hResult;

    //    // D3D APIҪ����봴��һ����ϵͳ�ڴ��е�D3D����,
    //    // ������ȡrender target surface����.
    //    if( !m_TargetItems[i].CopyTexture )
    //    {
    //        hResult = D3DXCreateTexture(pRenderer->m_pDXDevice,
    //            pImage->GetBound(0), pImage->GetBound(1), 0,
    //            0, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM,
    //            &m_TargetItems[i].CopyTexture);
    //        SE_ASSERT( SUCCEEDED(hResult) );

    //        hResult = m_TargetItems[i].CopyTexture->GetSurfaceLevel(0,
    //            &m_TargetItems[i].CopySurface);
    //        SE_ASSERT( SUCCEEDED(hResult) );

    //        hResult = pRenderer->m_pDXDevice->GetRenderTargetData(
    //            m_TargetItems[i].TextureSurface, m_TargetItems[i].CopySurface);
    //        SE_ASSERT( SUCCEEDED(hResult) );
    //    }

    //    // ��ʵ��.
    //    // Ŀǰ���Ա������ҵĿ����豸�Ͽ�������������������,
    //    // �ھ�ʽ�豸���Ƿ���Ҫͨ��tempLockRect.Pitch�����ж�ȡԴ��������?
    //    D3DLOCKED_RECT tempLockRect;
    //    hResult = m_TargetItems[i].CopySurface->LockRect(&tempLockRect, 0, 0);
    //    SE_ASSERT( SUCCEEDED(hResult) );
    //    memcpy(aucRDst, tempLockRect.pBits, iByteCount);
    //    hResult = m_TargetItems[i].CopySurface->UnlockRect();
    //    SE_ASSERT( SUCCEEDED(hResult) );

    //    switch( m_eFormat )
    //    {
    //    case FT_FORMAT_RGB:
    //        for( j = 0; j < iCount; j++, iDstBase += 3, iRDstBase += 4 )
    //        {
    //            aucDst[iDstBase    ] = aucRDst[iRDstBase + 2];
    //            aucDst[iDstBase + 1] = aucRDst[iRDstBase + 1];
    //            aucDst[iDstBase + 2] = aucRDst[iRDstBase    ];
    //        }
    //        break;

    //    case FT_FORMAT_RGBA:
    //        for( j = 0; j < iCount; j++, iDstBase += 4, iRDstBase += 4 )
    //        {
    //            aucDst[iDstBase    ] = aucRDst[iRDstBase + 2];
    //            aucDst[iDstBase + 1] = aucRDst[iRDstBase + 1];
    //            aucDst[iDstBase + 2] = aucRDst[iRDstBase    ];
    //            aucDst[iDstBase + 3] = aucRDst[iRDstBase + 3];
    //        }
    //        break;

    //    case FT_FORMAT_DEPTH:
    //        memcpy(aucDst, aucRDst, iByteCount);
    //        break;

    //    default:
    //        SE_ASSERT( false );
    //        break;
    //    }

    //    SE_DELETE[] aucRDst;
    //}
}
//----------------------------------------------------------------------------