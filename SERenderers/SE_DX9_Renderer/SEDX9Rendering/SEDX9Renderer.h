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

#ifndef Swing_DX9Renderer_H
#define Swing_DX9Renderer_H

#include "SEDX9RendererLIB.h"
#include "SEDX9Utility.h"
#include "SEDX9FrameBuffer.h"
#include "SERenderer.h"
#include "SEGeometry.h"
#include "SEProgram.h"

namespace Swing
{

class DX9ProgramInterfaceCatalog;

//----------------------------------------------------------------------------
// ����:DirectX9 Renderer��
// ˵��:
// ����:Sun Che
// ʱ��:20080827
//----------------------------------------------------------------------------
class SE_RENDERER_API DX9Renderer : public Renderer
{
    SE_DECLARE_INITIALIZE;
    SE_DECLARE_TERMINATE;

public:
    // RTTI.
    virtual int GetType(void) const { return Renderer::DIRECTX9; }

    // װ��shader����ʱ�������Ⱦ�������Ϣ.
    virtual const char* GetExtension(void) const { return "dx9"; }
    virtual char GetCommentCharacter(void) const { return '/'; }

    DX9Renderer(HWND hWnd, FrameBuffer::FormatType eFormat,
        FrameBuffer::DepthType eDepth, FrameBuffer::StencilType eStencil,
        FrameBuffer::BufferingType eBuffering,
        FrameBuffer::MultisamplingType eMultisampling, int iWidth,
        int iHeight);

    virtual ~DX9Renderer(void);

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
    const LPDIRECT3DDEVICE9 GetDevice(void);

    // Cg runtime��غ���.
    CGcontext GetCgContext(void) const;
    CGprofile GetCgLatestVertexProfile(void) const;
    CGprofile GetCgLatestPixelProfile(void) const;
    CGprofile GetCgLatestGeometryProfile(void) const;

protected:
    // �豸����.
    void OnLostDevice(void);
    void OnResetDevice(void);

    // ���������.
    virtual void OnViewportChange(void);
    virtual void OnDepthRangeChange(void);

    // ȫ����Ⱦ״̬����.
    virtual void SetAlphaState(AlphaState* pState);
    virtual void SetCullState(CullState* pState);
    virtual void SetPolygonOffsetState(PolygonOffsetState* pState);
    virtual void SetStencilState(StencilState* pState);
    virtual void SetWireframeState(WireframeState* pState);
    virtual void SetZBufferState(ZBufferState* pState);

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

    // ��Ⱦ����Դ������ر�.
    virtual void SetVProgramRC(RendererConstant* pRC);
    virtual void SetVProgramUC(UserConstant* pUC);
    virtual void SetGProgramRC(RendererConstant* pRC);
    virtual void SetGProgramUC(UserConstant* pUC);
    virtual void SetPProgramRC(RendererConstant* pRC);
    virtual void SetPProgramUC(UserConstant* pUC);
    virtual void UpdateVProgramConstants(VertexProgram* pVProgram);
    virtual void UpdateGProgramConstants(GeometryProgram* pGProgram);
    virtual void UpdatePProgramConstants(PixelProgram* pPProgram);
    virtual void OnEnableVProgram(ResourceIdentifier* pID);
    virtual void OnDisableVProgram(ResourceIdentifier* pID);
    virtual void OnEnablePProgram(ResourceIdentifier* pID);
    virtual void OnDisablePProgram(ResourceIdentifier* pID);
    virtual void OnEnableTexture(ResourceIdentifier* pID);
    virtual void OnDisableTexture(ResourceIdentifier* pID);
    virtual void OnEnableVBuffer(ResourceIdentifier* pID, 
        VertexProgram* pVProgram);
    virtual void OnDisableVBuffer(ResourceIdentifier* pID, 
        VertexProgram* pVProgram);
    virtual void OnEnableIBuffer(ResourceIdentifier* pID);
    virtual void OnDisableIBuffer(ResourceIdentifier* pID);

// ��Ա����
protected:
    friend class DX9FrameBuffer;

    LPDIRECT3D9 m_pDXMain;
    LPDIRECT3DDEVICE9 m_pDXDevice;
    D3DPRESENT_PARAMETERS m_Present;
    RECT m_rcWindow;
    bool m_bDeviceLost;
    bool m_bCursorVisible;
    bool m_bBeginSceneActive;

    // ׷��������Ӧ�ó��򴴽���frame buffer.
    // D3DҪ����device lostʱ�ͷ����VRAM��Դ(��������ΪD3DPOOL_DEFAULT),
    // ��device resetʱ����װ����ЩVRAM��Դ.
    // Ŀǰֻ��frame buffer������offscreen texture�Ǹ�������Դ.
    std::vector<DX9FrameBuffer*> m_FrameBuffers;

    // �����.
    std::vector<LPD3DXFONT> m_FontArray;

    // Cg runtime�������.
    CGcontext m_CgContext;
    CGprofile m_CgLatestVProfile;
    CGprofile m_CgLatestGProfile;
    CGprofile m_CgLatestPProfile;

    // ���������쳣��ȡ.
    static HRESULT ms_hResult;

    static DX9ProgramInterfaceCatalog* ms_pProgramInterfaceCatalog;

    // ��������������DirectX��Ⱦ�����������ӳ���.
    static D3DPRIMITIVETYPE ms_aeObjectType[];
    static DWORD ms_adwAlphaSrcBlend[];
    static DWORD ms_adwAlphaDstBlend[];
    static DWORD ms_adwAlphaTest[];
    static DWORD ms_adwCullType[];
    static DWORD ms_adwStencilCompare[];
    static DWORD ms_adwStencilOperation[];
    static DWORD ms_adwZBufferCompare[];
    static DWORD ms_adwTexMinFilter[];
    static DWORD ms_adwTexMipFilter[];
    static DWORD ms_adwTexWrapMode[];
    static D3DFORMAT ms_aeImageFormat[];
};

}

#endif
