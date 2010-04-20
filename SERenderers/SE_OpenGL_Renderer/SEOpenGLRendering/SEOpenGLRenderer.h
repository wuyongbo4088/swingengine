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

class SEBitmapFont;
class SEOpenGLProgramInterfaceCatalog;

//----------------------------------------------------------------------------
// ˵��:
// ����:Sun Che
// ʱ��:20080919
//----------------------------------------------------------------------------
class SE_RENDERER_API SEOpenGLRenderer : public SERenderer
{
    SE_DECLARE_INITIALIZE;
    SE_DECLARE_TERMINATE;

public:
    // RTTI.
    virtual int GetType(void) const { return SERenderer::OPENGL; }

    // �����.
    // ��OpenGL context������,�������๹�캯������InitializeState����.
    virtual ~SEOpenGLRenderer(void);
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

    // Cg runtime��غ���.
    CGcontext GetCgContext(void) const;
    CGprofile GetCgLatestVertexProfile(void) const;
    CGprofile GetCgLatestPixelProfile(void) const;
    CGprofile GetCgLatestGeometryProfile(void) const;

protected:
    SEOpenGLRenderer(SEFrameBuffer::FormatType eFormat,
        SEFrameBuffer::DepthType eDepth, SEFrameBuffer::StencilType eStencil,
        SEFrameBuffer::BufferingType eBuffering,
        SEFrameBuffer::MultisamplingType eMultisampling, int iWidth,
        int iHeight);

    // ���������.
    friend class SEOpenGLFrameBuffer;
    virtual void OnViewportChange(void);
    virtual void OnDepthRangeChange(void);

    // ȫ����Ⱦ״̬����.
    virtual void SetAlphaState(SEAlphaState* pState);
    virtual void SetCullState(SECullState* pState);
    virtual void SetPolygonOffsetState(SEPolygonOffsetState* pState);
    virtual void SetStencilState(SEStencilState* pState);
    virtual void SetWireframeState(SEWireframeState* pState);
    virtual void SetZBufferState(SEZBufferState* pState);

    // per-geometry pre/post-draw�������.
    virtual void OnPreDrawGeometry(void);
    virtual void OnPostDrawGeometry(void);

    // per-pass pre/post-draw�������,
    virtual void OnPreDrawPass(SEShaderEffect* pEffect, int iPass,
        bool bPrimaryEffect);
    virtual void OnPostDrawPass(SEShaderEffect* pEffect, int iPass,
        bool bPrimaryEffect);

    // ��Ⱦ����������Ⱦ�������.
    virtual void DrawElements(void);

    // ��Ⱦ����Դװ�����ͷ�.
    // ����Դ�,��Ҫ����ͼ��API����ʵ��.
    virtual void OnLoadVProgram(SEResourceIdentifier*& rpID,
        SEVertexProgram* pVProgram);
    virtual void OnReleaseVProgram(SEResourceIdentifier* pID);
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

    // �ı���Ⱦ
    void DrawCharacter(const SEBitmapFont& rFont, char cChar);

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

    static SEOpenGLProgramInterfaceCatalog* ms_pProgramInterfaceCatalog;

    // ��������������OpenGL��Ⱦ�����������ӳ���.
    static GLenum ms_aeObjectType[SEGeometry::GT_MAX_COUNT];
    static GLenum ms_aeAlphaSrcBlend[SEAlphaState::SBF_COUNT];
    static GLenum ms_aeAlphaDstBlend[SEAlphaState::DBF_COUNT];
    static GLenum ms_aeAlphaTest[SEAlphaState::TF_COUNT];
    static GLenum ms_aeFrontFace[SECullState::FT_COUNT];
    static GLenum ms_aeCullFace[SECullState::CT_COUNT];
    static GLenum ms_aeStencilCompare[SEStencilState::CF_COUNT];
    static GLenum ms_aeStencilOperation[SEStencilState::OT_COUNT];
    static GLenum ms_aeZBufferCompare[SEZBufferState::CF_COUNT];
    static GLenum ms_aeTextureMipmap[SETexture::MAX_FILTER_TYPES];
    static GLenum ms_aeDepthCompare[SETexture::DC_COUNT];
    static GLenum ms_aeWrapMode[SETexture::MAX_WRAP_TYPES];
    static GLenum ms_aeImageComponents[SEImage::IT_COUNT];
    static GLenum ms_aeImageFormats[SEImage::IT_COUNT];
    static GLenum ms_aeImageTypes[SEImage::IT_COUNT];
    static GLenum ms_aeSamplerTypes[SESamplerInformation::MAX_SAMPLER_TYPES];
};

}

#endif
