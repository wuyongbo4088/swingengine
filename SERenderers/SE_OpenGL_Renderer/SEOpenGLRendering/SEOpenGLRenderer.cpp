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

#include "SEOpenGLRendererPCH.h"
#include "SEOpenGLRenderer.h"
#include "SECamera.h"
#include "SEOpenGLProgram.h"
#include "SEOpenGLProgramInterfaceCatalog.h"

using namespace Swing;

SEOpenGLProgramInterfaceCatalog* SEOpenGLRenderer::ms_pProgramInterfaceCatalog 
    = 0;

SE_IMPLEMENT_INITIALIZE(SEOpenGLRenderer);
SE_IMPLEMENT_TERMINATE(SEOpenGLRenderer);

//SE_REGISTER_INITIALIZE(SEOpenGLRenderer);
//SE_REGISTER_TERMINATE(SEOpenGLRenderer);

//----------------------------------------------------------------------------
void SEOpenGLRenderer::Initialize()
{
    ms_pProgramInterfaceCatalog = 
        SE_NEW SEOpenGLProgramInterfaceCatalog("Main");
    SEOpenGLProgramInterfaceCatalog::SetActive(ms_pProgramInterfaceCatalog);
}
//----------------------------------------------------------------------------
void SEOpenGLRenderer::Terminate()
{
    if( SEOpenGLProgramInterfaceCatalog::GetActive() == 
        ms_pProgramInterfaceCatalog )
    {
        SEOpenGLProgramInterfaceCatalog::SetActive(0);
    }
    SE_DELETE ms_pProgramInterfaceCatalog;
}
//----------------------------------------------------------------------------
SEOpenGLRenderer::SEOpenGLRenderer(SEFrameBuffer::FormatType eFormat,
    SEFrameBuffer::DepthType eDepth, SEFrameBuffer::StencilType eStencil,
    SEFrameBuffer::BufferingType eBuffering,
    SEFrameBuffer::MultisamplingType eMultisampling, int iWidth, int iHeight)
    :
    SERenderer(eFormat, eDepth, eStencil, eBuffering, eMultisampling, iWidth, 
        iHeight)
{
}
//----------------------------------------------------------------------------
SEOpenGLRenderer::~SEOpenGLRenderer()
{
    // �ͷ�Cg context.
    cgDestroyContext(m_CgContext);

    // If this assertion is triggered, then most likely there are some Cg
    // runtime resources haven't been released. For example, maybe a geometry
    // object is still alive(which should be released already), and it is 
    // using a shader effect which itself is handling a Cg shader resource.
    SE_GL_DEBUG_CG_PROGRAM;
}
//----------------------------------------------------------------------------
void SEOpenGLRenderer::InitializeState()
{
    // �����������Ǵ���.
    glEnableClientState(GL_VERTEX_ARRAY);

    // �ر���ɫ����,��ǰ��ɫĬ��Ϊ��ɫ.
    glDisableClientState(GL_COLOR_ARRAY);
    glColor4fv((const float*)SEColorRGBA::SE_RGBA_WHITE);
    glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2],
        m_ClearColor[3]);

    // �رշ�������.
    glDisableClientState(GL_NORMAL_ARRAY);

    // ��ѯ�豸��������.

    // ��ȡvertex program���������.
    m_iMaxVShaderImages = 0;
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &m_iMaxVShaderImages);

    // ��ʵ��.
    // ��ȡgeometry program���������.
    m_iMaxGShaderImages = 0;

    // ��ȡpixel program���������.
    m_iMaxPShaderImages = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_iMaxPShaderImages);

    // ��ʵ��.
    // GLSL��,"texture image units"��"texture coordinate sets"���������Ƕ�����.
    // Ҳ����˵,iMaxTextures��iMaxTCoords��ֵ���Բ�ͬ.
    m_iMaxTCoords = 0;
    glGetIntegerv(GL_MAX_TEXTURE_COORDS, &m_iMaxTCoords);

    // OpenGLֻ֧��primary��secondary���鶥����ɫ.
    m_iMaxColors = 2;

    GLint iMaxLights;
    glGetIntegerv(GL_MAX_LIGHTS, &iMaxLights);
    SE_ASSERT( iMaxLights > 0 );
    m_iMaxLights = (int)iMaxLights;
    m_aspLight = SE_NEW SEObjectPtr[m_iMaxLights];

    // ����lighting model.�ر�lighting.
    // ��ʵ��:  ����һ��shader-based����,���б�Ҫ����Щ��?
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, 
        (const float*)SEColorRGBA::SE_RGBA_BLACK);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
    glDisable(GL_LIGHTING);

    // ��ȡstencil bufferλ��.
    GLint iBits = 0;
    glGetIntegerv(GL_STENCIL_BITS, &iBits);
    m_iMaxStencilIndices = (iBits > 0 ? (1 << iBits) : 0);

    GLint iMaxUseClipPlanes = 0;
    glGetIntegerv(GL_MAX_CLIP_PLANES, &iMaxUseClipPlanes);
    m_iMaxUserClipPlanes = (int)iMaxUseClipPlanes;

    // ��ȡvertex program profile.
    m_iMaxVShaderProfile = SE_ExistsGlNvVertexProgram3 ? SERenderer::VP40 :
        SERenderer::ARBVP1;

    // ��ʵ��.
    // ��ȡgeometry program profile.
    m_iMaxGShaderProfile = SERenderer::GS_UNSUPPORTED;

    // ��ȡfragment program profile.
    m_iMaxPShaderProfile = SE_ExistsGlNvFragmentProgram2 ? SERenderer::FP40 :
        SERenderer::ARBFP1;

    if( !SE_ExistsGlExtFrameBufferObject )
    {
        m_iMaxRenderTargets = 0;
    }
    else
    {
        glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &m_iMaxRenderTargets);
    }

    m_iMaxActiveSamplerCount = m_iMaxVShaderImages + m_iMaxPShaderImages;
    if( m_iMaxActiveSamplerCount > 0 )
    {
        m_apActiveSamplers =
            SE_NEW SESamplerInformation*[m_iMaxActiveSamplerCount];

        memset(m_apActiveSamplers, 0, m_iMaxActiveSamplerCount*
            sizeof(SESamplerInformation*));
    }

    // Cg runtime���.

    // ������Ⱦ����Cg context.
    m_CgContext = cgCreateContext();
    SE_GL_DEBUG_CG_PROGRAM;
    cgSetParameterSettingMode(m_CgContext, CG_DEFERRED_PARAMETER_SETTING);
    SE_GL_DEBUG_CG_PROGRAM;

    // �ر�cg��GL debug����.
    cgGLSetDebugMode(CG_FALSE);
    SE_GL_DEBUG_CG_PROGRAM;
    m_CgLatestVProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
    m_CgLatestPProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);

    // �ر����߻���ģʽ.
    glDisable(GL_LINE_STIPPLE);

    // ��ʼ��ȫ����Ⱦ״̬Ϊ����Ĭ������.
    SetGlobalState(SEGlobalState::Default);
}
//----------------------------------------------------------------------------
void SEOpenGLRenderer::ClearBackBuffer()
{
    glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2],
        m_ClearColor[3]);

    glClear(GL_COLOR_BUFFER_BIT);
}
//----------------------------------------------------------------------------
void SEOpenGLRenderer::ClearZBuffer()
{
    glClearDepth((double)m_fClearDepth);

    glClear(GL_DEPTH_BUFFER_BIT);
}
//----------------------------------------------------------------------------
void SEOpenGLRenderer::ClearStencilBuffer()
{
    glClearStencil((GLint)m_uiClearStencil);

    glClear(GL_STENCIL_BUFFER_BIT);
}
//----------------------------------------------------------------------------
void SEOpenGLRenderer::ClearBuffers()
{
    glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2],
        m_ClearColor[3]);
    glClearDepth((double)m_fClearDepth);
    glClearStencil((GLint)m_uiClearStencil);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
}
//----------------------------------------------------------------------------
void SEOpenGLRenderer::ClearBackBuffer(int iXPos, int iYPos, int iWidth,
    int iHeight)
{
    glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2],
        m_ClearColor[3]);

    glEnable(GL_SCISSOR_TEST);
    glScissor(iXPos, iYPos, iWidth, iHeight);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
}
//----------------------------------------------------------------------------
void SEOpenGLRenderer::ClearZBuffer(int iXPos, int iYPos, int iWidth,
    int iHeight)
{
    glClearDepth((double)m_fClearDepth);

    glEnable(GL_SCISSOR_TEST);
    glScissor(iXPos, iYPos, iWidth, iHeight);
    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
}
//----------------------------------------------------------------------------
void SEOpenGLRenderer::ClearStencilBuffer(int iXPos, int iYPos, int iWidth,
    int iHeight)
{
    glClearStencil((GLint)m_uiClearStencil);

    glEnable(GL_SCISSOR_TEST);
    glScissor(iXPos, iYPos, iWidth, iHeight);
    glClear(GL_STENCIL_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
}
//----------------------------------------------------------------------------
void SEOpenGLRenderer::ClearBuffers(int iXPos, int iYPos, int iWidth,
    int iHeight)
{
    glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2],
        m_ClearColor[3]);
    glClearDepth((double)m_fClearDepth);
    glClearStencil((GLint)m_uiClearStencil);

    glEnable(GL_SCISSOR_TEST);
    glScissor(iXPos, iYPos, iWidth, iHeight);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glStencilMask((GLuint)~0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
}
//----------------------------------------------------------------------------
void SEOpenGLRenderer::SetColorMask(bool bAllowRed, bool bAllowGreen,
    bool bAllowBlue, bool bAllowAlpha)
{
    SERenderer::SetColorMask(bAllowRed, bAllowGreen, bAllowBlue, bAllowAlpha);

    glColorMask((GLboolean)bAllowRed, (GLboolean)bAllowGreen,
        (GLboolean)bAllowBlue, (GLboolean)bAllowAlpha);
}
//----------------------------------------------------------------------------
void SEOpenGLRenderer::EnableUserClipPlane(int i, const SEPlane3f& rPlane)
{
    GLdouble adPlane[4] =
    {
        (double)rPlane.Normal.X,
        (double)rPlane.Normal.Y,
        (double)rPlane.Normal.Z,
        (double)-rPlane.Constant
    };
    glClipPlane(GL_CLIP_PLANE0 + i, adPlane);
    glEnable(GL_CLIP_PLANE0 + i);
}
//----------------------------------------------------------------------------
void SEOpenGLRenderer::DisableUserClipPlane(int i)
{
    glDisable(GL_CLIP_PLANE0 + i);
}
//----------------------------------------------------------------------------
void SEOpenGLRenderer::OnPreDrawGeometry()
{
    SERenderStateBlock* pRStateBlock = m_pGeometry->RStateBlock;
    SE_ASSERT( pRStateBlock );

    SetGlobalState(pRStateBlock->States);
}
//----------------------------------------------------------------------------
void SEOpenGLRenderer::OnPostDrawGeometry()
{
    SERenderStateBlock* pRStateBlock = m_pGeometry->RStateBlock;
    SE_ASSERT( pRStateBlock );

    RestoreGlobalState(pRStateBlock->States);
}
//----------------------------------------------------------------------------
void SEOpenGLRenderer::OnPreDrawPass(SEShaderEffect* pEffect, int iPass,
    bool bPrimaryEffect)
{
    pEffect->SetGlobalState(iPass, this, bPrimaryEffect);
}
//----------------------------------------------------------------------------
void SEOpenGLRenderer::OnPostDrawPass(SEShaderEffect* pEffect, int iPass,
    bool bPrimaryEffect)
{
    pEffect->RestoreGlobalState(iPass, this, bPrimaryEffect);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Cg runtime���
//----------------------------------------------------------------------------
CGcontext SEOpenGLRenderer::GetCgContext() const
{
    return m_CgContext;
}
//----------------------------------------------------------------------------
CGprofile SEOpenGLRenderer::GetCgLatestVertexProfile() const
{
    return m_CgLatestVProfile;
}
//----------------------------------------------------------------------------
CGprofile SEOpenGLRenderer::GetCgLatestPixelProfile() const
{
    return m_CgLatestPProfile;
}
//----------------------------------------------------------------------------
CGprofile SEOpenGLRenderer::GetCgLatestGeometryProfile() const
{
    return m_CgLatestGProfile;
}
//----------------------------------------------------------------------------