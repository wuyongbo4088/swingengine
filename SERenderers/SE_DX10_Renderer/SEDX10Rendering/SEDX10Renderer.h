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

class SEDX10ProgramInterfaceCatalog;

//----------------------------------------------------------------------------
// Description:
// Author:Sun Che
// Date:20090519
//----------------------------------------------------------------------------
class SE_RENDERER_API DX10Renderer : public SERenderer
{
    SE_DECLARE_INITIALIZE;
    SE_DECLARE_TERMINATE;

public:
    // RTTI.
    virtual int GetType(void) const { return SERenderer::DIRECTX10; }

    DX10Renderer(HWND hWnd, SEFrameBuffer::FormatType eFormat,
        SEFrameBuffer::DepthType eDepth, SEFrameBuffer::StencilType eStencil,
        SEFrameBuffer::BufferingType eBuffering,
        SEFrameBuffer::MultisamplingType eMultisampling, int iWidth,
        int iHeight);

    virtual ~DX10Renderer(void);

    virtual void ToggleFullscreen(void);

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
    virtual void Draw(int iX, int iY, const SEColorRGBA& rColor,
        const char* acText);
    virtual void Draw(const unsigned char* aucBuffer);

    // color mask����,������Щ��ɫͨ������д��color buffer.
    virtual void SetColorMask(bool bAllowRed, bool bAllowGreen,
        bool bAllowBlue, bool bAllowAlpha);

    // ����/�ر��û��Զ���ü�ƽ��.
    // ����ƽ�������ģ�Ϳռ�ƽ��,�����ڲ����任��������ռ����clip�ռ�ü�.
    virtual void EnableUserClipPlane(int i, const SEPlane3f& rPlane);
    virtual void DisableUserClipPlane(int i);

    // windowsƽ̨�µĴ���Ӧ�ó��򿪷���ʹ��.
    const ID3D10Device* GetDevice(void);

    // Cg runtime stuff.
    CGcontext GetCgContext(void) const;
    CGprofile GetCgLatestVertexProfile(void) const;
    CGprofile GetCgLatestPixelProfile(void) const;
    CGprofile GetCgLatestGeometryProfile(void) const;

protected:
    // Camera update.
    virtual void OnViewportChange(void);
    virtual void OnDepthRangeChange(void);

    // Render state block stuff(DirectX10 specific functions).
    void GenerateBlendState(const SERenderStateBlock* pRStateBlock,
        ID3D10BlendState*& rpDX10BState);
    void GenerateDepthStencilState(const SERenderStateBlock* pRStateBlock, 
        ID3D10DepthStencilState*& rpDX10DSState);
    void GenerateRasterizerState(const SERenderStateBlock* pRStateBlock, 
        ID3D10RasterizerState*& rpDX10RState);

    // Per-geometry pre/post-draw entry point.
    virtual void OnPreDrawGeometry(void);
    virtual void OnPostDrawGeometry(void);

    // Per-pass pre/post-draw entry point.
    virtual void OnPreDrawPass(SEShaderEffect* pEffect, int iPass,
        bool bPrimaryEffect);
    virtual void OnPostDrawPass(SEShaderEffect* pEffect, int iPass,
        bool bPrimaryEffect);

    // Rendering entry point.
    virtual void DrawElements(void);

    // ��Ⱦ����Դװ�����ͷ�.
    // ����Դ�,��Ҫ����ͼ��API����ʵ��.
    virtual void OnLoadVProgram(SEResourceIdentifier*& rpID,
        SEVertexProgram* pVProgram);
    virtual void OnReleaseVProgram(SEResourceIdentifier* pID);
    virtual void OnLoadGProgram(SEResourceIdentifier*& rpID,
        SEGeometryProgram* pGProgram);
    virtual void OnReleaseGProgram(SEResourceIdentifier* pID);
    virtual void OnLoadPProgram(SEResourceIdentifier*& rpID,
        SEPixelProgram* pPProgram);
    virtual void OnReleasePProgram(SEResourceIdentifier* pID);
    virtual void OnLoadTexture(SEResourceIdentifier*& rpID,
        SETexture* pTexture);
    virtual void OnReleaseTexture(SEResourceIdentifier* pID);
    virtual void OnLoadVBuffer(SEResourceIdentifier*& rpID,
        const SEAttributes& rIAttr, const SEAttributes& rOAttr,
        SEVertexBuffer* pVBuffer, SEVertexProgram* pVProgram);
    virtual void OnReleaseVBuffer(SEResourceIdentifier* pID);
    virtual void OnLoadIBuffer(SEResourceIdentifier*& rpID,
        SEIndexBuffer* pIBuffer);
    virtual void OnReleaseIBuffer(SEResourceIdentifier* pID);
    // DirectX 10��Ⱦ����Ҫ�������º���.
    virtual void OnLoadRenderStateBlock(SEResourceIdentifier*& rpID,
        SERenderStateBlock* pRStateBlock);
    virtual void OnReleaseRenderStateBlock(SEResourceIdentifier* pID);

    // ��Ⱦ����Դ������ر�.
    virtual void SetVProgramRC(SERendererConstant* pRC);
    virtual void SetVProgramUC(SEUserConstant* pUC);
    virtual void SetGProgramRC(SERendererConstant* pRC);
    virtual void SetGProgramUC(SEUserConstant* pUC);
    virtual void SetPProgramRC(SERendererConstant* pRC);
    virtual void SetPProgramUC(SEUserConstant* pUC);
    virtual void UpdateVProgramConstants(SEVertexProgram* pVProgram);
    virtual void UpdateGProgramConstants(SEGeometryProgram* pGProgram);
    virtual void UpdatePProgramConstants(SEPixelProgram* pPProgram);
    virtual void OnEnableVProgram(SEResourceIdentifier* pID);
    virtual void OnDisableVProgram(SEResourceIdentifier* pID);
    virtual void OnEnableGProgram(SEResourceIdentifier* pID);
    virtual void OnDisableGProgram(SEResourceIdentifier* pID);
    virtual void OnEnablePProgram(SEResourceIdentifier* pID);
    virtual void OnDisablePProgram(SEResourceIdentifier* pID);
    virtual void OnEnableTexture(SEResourceIdentifier* pID);
    virtual void OnDisableTexture(SEResourceIdentifier* pID);
    virtual void OnEnableVBuffer(SEResourceIdentifier* pID, 
        SEVertexProgram* pVProgram);
    virtual void OnDisableVBuffer(SEResourceIdentifier* pID, 
        SEVertexProgram* pVProgram);
    virtual void OnEnableIBuffer(SEResourceIdentifier* pID);
    virtual void OnDisableIBuffer(SEResourceIdentifier* pID);
    // DirectX 10��Ⱦ����Ҫ�������º���.
    virtual void OnEnableRenderStateBlock(SEResourceIdentifier* pID);
    virtual void OnDisableRenderStateBlock(SEResourceIdentifier* pID);

// ��Ա����
protected:
    friend class SEDX10FrameBuffer;

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
    std::vector<SEDX10FrameBuffer*> m_FrameBuffers;

    // �����.
    std::vector<ID3DX10Font*> m_FontArray;

    // Cg runtime�������.
    CGcontext m_CgContext;
    CGprofile m_CgLatestVProfile;
    CGprofile m_CgLatestGProfile;
    CGprofile m_CgLatestPProfile;

    // ���������쳣��ȡ.
    static HRESULT ms_hResult;

    static SEDX10ProgramInterfaceCatalog* ms_pProgramInterfaceCatalog;

	// test
    ID3D10Resource* pTestTexture;

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
    static DXGI_FORMAT ms_aeImageFormat[];
};

}

#endif
