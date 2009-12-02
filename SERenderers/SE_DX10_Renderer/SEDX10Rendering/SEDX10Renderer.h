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

#ifndef Swing_DX10Renderer_H
#define Swing_DX10Renderer_H

#include "SEDX10RendererLIB.h"
#include "SEDX10Utility.h"
#include "SEDX10FrameBuffer.h"
#include "SERenderer.h"
#include "SEGeometry.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Name:DirectX10 Renderer class
// Description:
// Author:Sun Che
// Date:20090519
//----------------------------------------------------------------------------
class SE_RENDERER_API DX10Renderer : public Renderer
{
public:
    // RTTI.
    virtual int GetType(void) const { return Renderer::DIRECTX10; }

    DX10Renderer(HWND hWnd, FrameBuffer::FormatType eFormat,
        FrameBuffer::DepthType eDepth, FrameBuffer::StencilType eStencil,
        FrameBuffer::BufferingType eBuffering,
        FrameBuffer::MultisamplingType eMultisampling, int iWidth,
        int iHeight);

    virtual ~DX10Renderer(void);

    virtual void ToggleFullscreen(void);
    virtual bool BeginScene(void);
    virtual void EndScene(void);

    // ȫ����buffer����.
    virtual void ClearBackBuffer(void);
    virtual void ClearZBuffer(void);
    virtual void ClearStencilBuffer(void);
    virtual void ClearBuffers(void);
    virtual void DisplayBackBuffer(void);

    // �ض�����buffer����.
    virtual void ClearBackBuffer(int iXPos, int iYPos, int iWidth,
        int iHeight);
    virtual void ClearZBuffer(int iXPos, int iYPos, int iWidth, int iHeight);
    virtual void ClearStencilBuffer(int iXPos, int iYPos, int iWidth,
        int iHeight);
    virtual void ClearBuffers(int iXPos, int iYPos, int iWidth, int iHeight);

    // ������Ⱦ.
    virtual bool SelectFont(int iFontID);
    int LoadFont(const char* acFace, int iSize, bool bBold, bool bItalic);
    void UnloadFont(int iFontID);
    virtual void Draw(int iX, int iY, const ColorRGBA& rColor,
        const char* acText);
    virtual void Draw(const unsigned char* aucBuffer);

    // color mask����,������Щ��ɫͨ������д��color buffer.
    virtual void SetColorMask(bool bAllowRed, bool bAllowGreen,
        bool bAllowBlue, bool bAllowAlpha);

    // ����/�ر��û��Զ���ü�ƽ��.
    // ����ƽ�������ģ�Ϳռ�ƽ��,�����ڲ����任��������ռ����clip�ռ�ü�.
    virtual void EnableUserClipPlane(int i, const Plane3f& rPlane);
    virtual void DisableUserClipPlane(int i);

    // windowsƽ̨�µĴ���Ӧ�ó��򿪷���ʹ��.
    const ID3D10Device* GetDevice(void);

protected:
    // �豸����.
    void OnLostDevice(void);
    void OnResetDevice(void);

    // ���������.
    virtual void OnViewportChange(void);
    virtual void OnDepthRangeChange(void);

    // ȫ����Ⱦ״̬����.
    void GenerateBlendState(const RenderStateBlock* pRStateBlock,
        ID3D10BlendState*& rpDX10BState);
    void GenerateDepthStencilState(const RenderStateBlock* pRStateBlock, 
        ID3D10DepthStencilState*& rpDX10DSState);
    void GenerateRasterizerState(const RenderStateBlock* pRStateBlock, 
        ID3D10RasterizerState*& rpDX10RState);

    // per-geometry pre/post-draw�������.
    virtual void OnPreDrawGeometry(void);
    virtual void OnPostDrawGeometry(void);

    // per-pass pre/post-draw�������,
    virtual void OnPreDrawPass(ShaderEffect* pEffect, int iPass,
        bool bPrimaryEffect);
    virtual void OnPostDrawPass(ShaderEffect* pEffect, int iPass,
        bool bPrimaryEffect);

    // ��Ⱦ����������Ⱦ�������.
    virtual void DrawElements(void);

    // ��Ⱦ����Դװ�����ͷ�.
    // ����Դ�,��Ҫ����ͼ��API����ʵ��.
    virtual void OnLoadVProgram(ResourceIdentifier*& rpID,
        VertexProgram* pVProgram);
    virtual void OnReleaseVProgram(ResourceIdentifier* pID);
    virtual void OnLoadGProgram(ResourceIdentifier*& rpID,
        GeometryProgram* pGProgram);
    virtual void OnReleaseGProgram(ResourceIdentifier* pID);
    virtual void OnLoadPProgram(ResourceIdentifier*& rpID,
        PixelProgram* pPProgram);
    virtual void OnReleasePProgram(ResourceIdentifier* pID);
    virtual void OnLoadTexture(ResourceIdentifier*& rpID,
        Texture* pTexture);
    virtual void OnReleaseTexture(ResourceIdentifier* pID);
    virtual void OnLoadVBuffer(ResourceIdentifier*& rpID,
        const Attributes& rIAttr, const Attributes& rOAttr,
        VertexBuffer* pVBuffer, VertexProgram* pVProgram);
    virtual void OnReleaseVBuffer(ResourceIdentifier* pID);
    virtual void OnLoadIBuffer(ResourceIdentifier*& rpID,
        IndexBuffer* pIBuffer);
    virtual void OnReleaseIBuffer(ResourceIdentifier* pID);
    // DirectX 10��Ⱦ����Ҫ�������º���.
    virtual void OnLoadRenderStateBlock(ResourceIdentifier*& rpID,
        RenderStateBlock* pRStateBlock);
    virtual void OnReleaseRenderStateBlock(ResourceIdentifier* pID);

    // ��Ⱦ����Դ������ر�.
    virtual void OnEnableVProgram(ResourceIdentifier* pID);
    virtual void OnDisableVProgram(ResourceIdentifier* pID);
    virtual void OnEnableGProgram(ResourceIdentifier* pID);
    virtual void OnDisableGProgram(ResourceIdentifier* pID);
    virtual void OnEnablePProgram(ResourceIdentifier* pID);
    virtual void OnDisablePProgram(ResourceIdentifier* pID);
    virtual void OnEnableTexture(ResourceIdentifier* pID);
    virtual void OnDisableTexture(ResourceIdentifier* pID);
    virtual void OnEnableVBuffer(ResourceIdentifier* pID);
    virtual void OnDisableVBuffer(ResourceIdentifier* pID);
    virtual void OnEnableIBuffer(ResourceIdentifier* pID);
    virtual void OnDisableIBuffer(ResourceIdentifier* pID);
    // DirectX 10��Ⱦ����Ҫ�������º���.
    virtual void OnEnableRenderStateBlock(ResourceIdentifier* pID);
    virtual void OnDisableRenderStateBlock(ResourceIdentifier* pID);

// ��Ա����
protected:
    friend class DX10FrameBuffer;

    ID3D10Device* m_pDX10Device;
    IDXGISwapChain* m_pDXGISwapChain;
    ID3D10Texture2D* m_pDX10DSBuffer;
    ID3D10RenderTargetView* m_pDX10RTView;
    ID3D10DepthStencilView* m_pDX10DSView;
    DXGI_SWAP_CHAIN_DESC m_SwapChainDesc;

    // Ĭ��rasterizer state.
    ID3D10RasterizerState* m_pDX10RasterizerState;

    RECT m_rcWindow;
    bool m_bDeviceLost;
    bool m_bCursorVisible;
    bool m_bBeginSceneActive;

    // ׷��������Ӧ�ó��򴴽���frame buffer.
    // D3DҪ����device lostʱ�ͷ����VRAM��Դ(��������ΪD3DPOOL_DEFAULT),
    // ��device resetʱ����װ����ЩVRAM��Դ.
    // Ŀǰֻ��frame buffer������offscreen texture�Ǹ�������Դ.
    std::vector<DX10FrameBuffer*> m_FrameBuffers;

    // �����.
    std::vector<ID3DX10Font*> m_FontArray;

    // Cg runtime�������.
    CGcontext m_CgContext;
    CGprofile m_CgLatestVProfile;
    CGprofile m_CgLatestGProfile;
    CGprofile m_CgLatestPProfile;

    // ���������쳣��ȡ.
    static HRESULT ms_hResult;

    // ��������������DirectX��Ⱦ�����������ӳ���.
    static D3D10_PRIMITIVE_TOPOLOGY ms_aeObjectType[];
    static D3D10_BLEND ms_aeAlphaSrcBlend[];
    static D3D10_BLEND ms_aeAlphaDstBlend[];
    static D3D10_CULL_MODE ms_aeCullModel[];
    static D3D10_COMPARISON_FUNC ms_aeZBufferCompare[];
    static D3D10_COMPARISON_FUNC ms_aeStencilCompare[];
    static D3D10_STENCIL_OP ms_aeStencilOperation[];
    static DWORD ms_adwTexMinFilter[];
    static DWORD ms_adwTexMipFilter[];
    static DWORD ms_adwTexWrapMode[];
};

}

#endif
