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

#ifndef Swing_OpenGLRenderer_H
#define Swing_OpenGLRenderer_H

#include "SEglExtensions.h"
// ����ȷ��cg��ͷ�ļ����������ǵ�OpenGL��װ���ĺ���
#include <Cg/Cg.h>
#include <Cg/cgGL.h>

#include "SEOpenGLRendererLIB.h"
#include "SEOpenGLFrameBuffer.h"
#include "SERenderer.h"
#include "SEGeometry.h"
#include "SETexture.h"

//----------------------------------------------------------------------------
#ifdef _DEBUG
#define SE_GL_DEBUG_CG_PROGRAM \
{ \
    CGerror tempError; \
    cgGetLastErrorString(&tempError); \
    SE_ASSERT( tempError == CG_NO_ERROR ); \
}
#else
#define SE_GL_DEBUG_CG_PROGRAM
#endif
//----------------------------------------------------------------------------

namespace Swing
{

class BitmapFont;
class OpenGLProgramInterfaceCatalog;

//----------------------------------------------------------------------------
// ����:OpenGL2.0 Renderer�����
// ˵��:
// ����:Sun Che
// ʱ��:20080919
//----------------------------------------------------------------------------
class SE_RENDERER_API OpenGLRenderer : public Renderer
{
    SE_DECLARE_INITIALIZE;
    SE_DECLARE_TERMINATE;

public:
    // RTTI.
    virtual int GetType(void) const { return Renderer::OPENGL; }

    // �����.
    // ��OpenGL context������,�������๹�캯������InitializeState����.
    virtual ~OpenGLRenderer(void);
    void InitializeState(void);

    // ȫ����buffer����.
    virtual void ClearBackBuffer(void);
    virtual void ClearZBuffer(void);
    virtual void ClearStencilBuffer(void);
    virtual void ClearBuffers(void);
    virtual void DisplayBackBuffer(void) = 0;

    // �ض�����buffer����.
    virtual void ClearBackBuffer(int iXPos, int iYPos, int iWidth,
        int iHeight);
    virtual void ClearZBuffer(int iXPos, int iYPos, int iWidth,
        int iHeight);
    virtual void ClearStencilBuffer(int iXPos, int iYPos, int iWidth,
        int iHeight);
    virtual void ClearBuffers(int iXPos, int iYPos, int iWidth,
        int iHeight);

    // ������Ⱦ.
    virtual bool SelectFont(int iFontID);
    virtual void UnloadFont(int iFontID);
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

    // Cg runtime��غ���.
    CGcontext GetCgContext(void) const;
    CGprofile GetCgLatestVertexProfile(void) const;
    CGprofile GetCgLatestPixelProfile(void) const;
    CGprofile GetCgLatestGeometryProfile(void) const;

protected:
    OpenGLRenderer(FrameBuffer::FormatType eFormat,
        FrameBuffer::DepthType eDepth, FrameBuffer::StencilType eStencil,
        FrameBuffer::BufferingType eBuffering,
        FrameBuffer::MultisamplingType eMultisampling, int iWidth,
        int iHeight);

    // ���������.
    friend class OpenGLFrameBuffer;
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

    // �ı���Ⱦ
    void DrawCharacter(const BitmapFont& rFont, char cChar);

    // �����������display listʵ��.
    class DisplayListInfo
    {
    public:
        int Count;     // display list��Ŀ,����glGenLists����
        int Start;     // start index,��glGenLists�������ص���ʼ����
        int Base;      // base index,����glListBase
    };
    std::vector<DisplayListInfo> m_DLInfo;

    // Cg runtime�������.
    CGcontext m_CgContext;
    CGprofile m_CgLatestVProfile;
    CGprofile m_CgLatestGProfile;
    CGprofile m_CgLatestPProfile;

    static OpenGLProgramInterfaceCatalog* ms_pProgramInterfaceCatalog;

    // ��������������OpenGL��Ⱦ�����������ӳ���.
    static GLenum ms_aeObjectType[Geometry::GT_MAX_COUNT];
    static GLenum ms_aeAlphaSrcBlend[AlphaState::SBF_COUNT];
    static GLenum ms_aeAlphaDstBlend[AlphaState::DBF_COUNT];
    static GLenum ms_aeAlphaTest[AlphaState::TF_COUNT];
    static GLenum ms_aeFrontFace[CullState::FT_COUNT];
    static GLenum ms_aeCullFace[CullState::CT_COUNT];
    static GLenum ms_aeStencilCompare[StencilState::CF_COUNT];
    static GLenum ms_aeStencilOperation[StencilState::OT_COUNT];
    static GLenum ms_aeZBufferCompare[ZBufferState::CF_COUNT];
    static GLenum ms_aeTextureMipmap[Texture::MAX_FILTER_TYPES];
    static GLenum ms_aeDepthCompare[Texture::DC_COUNT];
    static GLenum ms_aeWrapMode[Texture::MAX_WRAP_TYPES];
    static GLenum ms_aeImageComponents[Image::IT_COUNT];
    static GLenum ms_aeImageFormats[Image::IT_COUNT];
    static GLenum ms_aeImageTypes[Image::IT_COUNT];
    static GLenum ms_aeSamplerTypes[SamplerInformation::MAX_SAMPLER_TYPES];
};

}

#endif
