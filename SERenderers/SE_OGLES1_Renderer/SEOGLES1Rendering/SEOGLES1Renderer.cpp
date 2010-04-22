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

#include "SEOGLES1RendererPCH.h"
#include "SEOGLES1Renderer.h"
#include "SEOGLES1FixedEffect.h"
#include "SECamera.h"
#include "SEUnculledSet.h"
#include "SESamplerInformation.h"

using namespace Swing;

SE_IMPLEMENT_INITIALIZE(SEOGLES1Renderer);

//SE_REGISTER_INITIALIZE(SEOGLES1Renderer);

//----------------------------------------------------------------------------
void SEOGLES1Renderer::Initialize()
{
    SESamplerInformation::OnReleaseID = 
        &SEOGLES1Renderer::OnReleaseSamplerInformationID;
}
//----------------------------------------------------------------------------
SEOGLES1Renderer::SEOGLES1Renderer(SEFrameBuffer::FormatType eFormat,
    SEFrameBuffer::DepthType eDepth, SEFrameBuffer::StencilType eStencil,
    SEFrameBuffer::BufferingType eBuffering,
    SEFrameBuffer::MultisamplingType eMultisampling, int iWidth, int iHeight)
    :
    SERenderer(eFormat, eDepth, eStencil, eBuffering, eMultisampling, iWidth, 
        iHeight)
{
}
//----------------------------------------------------------------------------
SEOGLES1Renderer::~SEOGLES1Renderer()
{
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::InitializeState()
{
    // �����������Ǵ���.
    glEnableClientState(GL_VERTEX_ARRAY);

    // �ر���ɫ����,��ǰ��ɫĬ��Ϊ��ɫ.
    glDisableClientState(GL_COLOR_ARRAY);
    const SEColorRGBA& rDefaultColor = SEColorRGBA::SE_RGBA_WHITE;
    glColor4f(rDefaultColor.R, rDefaultColor.G, rDefaultColor.B,
        rDefaultColor.A);
    glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2],
        m_ClearColor[3]);

    // �رշ�������.
    glDisableClientState(GL_NORMAL_ARRAY);

    // ��ѯ�豸��������.

    // ��ȡvertex program���������.
    m_iMaxVShaderImages = 0;

    // ��ȡgeometry program���������.
    m_iMaxGShaderImages = 0;

    // ��ȡpixel program���������.
    m_iMaxPShaderImages = 0;

    m_iMaxTCoords = 0;

    // OpenGL ES1ֻ֧��һ�鶥����ɫ.
    m_iMaxColors = 1;

    GLint iMaxLights;
    glGetIntegerv(GL_MAX_LIGHTS, &iMaxLights);
    SE_ASSERT( iMaxLights > 0 );
    m_iMaxLights = (int)iMaxLights;
    m_aspLight = SE_NEW SEObjectPtr[m_iMaxLights];

    // ����lighting model.�ر�lighting.
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, 
        (const float*)SEColorRGBA::SE_RGBA_BLACK);
    glDisable(GL_LIGHTING);

    // ��ȡstencil bufferλ��.
    GLint iBits = 0;
    glGetIntegerv(GL_STENCIL_BITS, &iBits);
    m_iMaxStencilIndices = (iBits > 0 ? (1 << iBits) : 0);

    GLint iMaxUseClipPlanes = 0;
    glGetIntegerv(GL_MAX_CLIP_PLANES, &iMaxUseClipPlanes);
    m_iMaxUserClipPlanes = (int)iMaxUseClipPlanes;

    // ��ȡvertex program profile.
    m_iMaxVShaderProfile = -1;

    // ��ȡgeometry program profile.
    m_iMaxGShaderProfile = -1;

    // ��ȡfragment program profile.
    m_iMaxPShaderProfile = -1;

    // OpenGL ES1��֧��MRT.	
    m_iMaxRenderTargets = 1;

    // ��ʼ��ȫ����Ⱦ״̬Ϊ����Ĭ������.
    SetGlobalState(SEGlobalState::Default);
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::ClearBackBuffer()
{
    glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2],
        m_ClearColor[3]);

    glClear(GL_COLOR_BUFFER_BIT);
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::ClearZBuffer()
{
    glClearDepthf((GLclampf)m_fClearDepth);

    glClear(GL_DEPTH_BUFFER_BIT);
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::ClearStencilBuffer()
{
    glClearStencil((GLint)m_uiClearStencil);

    glClear(GL_STENCIL_BUFFER_BIT);
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::ClearBuffers()
{
    glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2],
        m_ClearColor[3]);
    glClearDepthf((GLclampf)m_fClearDepth);
    glClearStencil((GLint)m_uiClearStencil);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::ClearBackBuffer(int iXPos, int iYPos, int iWidth,
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
void SEOGLES1Renderer::ClearZBuffer(int iXPos, int iYPos, int iWidth,
    int iHeight)
{
    glClearDepthf((GLclampf)m_fClearDepth);

    glEnable(GL_SCISSOR_TEST);
    glScissor(iXPos, iYPos, iWidth, iHeight);
    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::ClearStencilBuffer(int iXPos, int iYPos, int iWidth,
    int iHeight)
{
    glClearStencil((GLint)m_uiClearStencil);

    glEnable(GL_SCISSOR_TEST);
    glScissor(iXPos, iYPos, iWidth, iHeight);
    glClear(GL_STENCIL_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::ClearBuffers(int iXPos, int iYPos, int iWidth,
    int iHeight)
{
    glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2],
        m_ClearColor[3]);
    glClearDepthf((GLclampf)m_fClearDepth);
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
void SEOGLES1Renderer::SetColorMask(bool bAllowRed, bool bAllowGreen,
    bool bAllowBlue, bool bAllowAlpha)
{
    SERenderer::SetColorMask(bAllowRed, bAllowGreen, bAllowBlue, bAllowAlpha);

    glColorMask((GLboolean)bAllowRed, (GLboolean)bAllowGreen,
        (GLboolean)bAllowBlue, (GLboolean)bAllowAlpha);
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::EnableUserClipPlane(int i, const SEPlane3f& rPlane)
{
    GLfloat afPlane[4] =
    {
        rPlane.Normal.X,
        rPlane.Normal.Y,
        rPlane.Normal.Z,
        -rPlane.Constant
    };
    glClipPlanef(GL_CLIP_PLANE0 + i, afPlane);
    glEnable(GL_CLIP_PLANE0 + i);
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::DisableUserClipPlane(int i)
{
    glDisable(GL_CLIP_PLANE0 + i);
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::OnReleaseSamplerInformationID(void* pID)
{
    SESamplerInformationID* pSIID = (SESamplerInformationID*)pID;
    SE_DELETE pSIID;
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::OnPreDrawGeometry()
{
    SERenderStateBlock* pRStateBlock = m_pGeometry->RStateBlock;
    SE_ASSERT( pRStateBlock );

    SetGlobalState(pRStateBlock->States);
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::OnPostDrawGeometry()
{
    SERenderStateBlock* pRStateBlock = m_pGeometry->RStateBlock;
    SE_ASSERT( pRStateBlock );

    RestoreGlobalState(pRStateBlock->States);
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::OnPreDrawPass(SEShaderEffect* pEffect, int iPass,
    bool bPrimaryEffect)
{
    pEffect->SetGlobalState(iPass, this, bPrimaryEffect);
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::OnPostDrawPass(SEShaderEffect* pEffect, int iPass,
    bool bPrimaryEffect)
{
    pEffect->RestoreGlobalState(iPass, this, bPrimaryEffect);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ������Ⱦ���
//----------------------------------------------------------------------------
void SEOGLES1Renderer::DrawScene(SEUnculledSet& rVisibleSet)
{
    const int iVisibleCount = rVisibleSet.GetCount();
    SEUnculledObject* pVisibleSet = rVisibleSet.GetUnculled();
    for( int i = 0; i < iVisibleCount; i++ )
    {
        if( pVisibleSet[i].IsRenderable() )
        {
            Draw((SEGeometry*)pVisibleSet[i].Object);
        }
    }
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::ApplyFixedEffect(SEOGLES1FixedEffect* pEffect, bool&)
{
    // ����light.
    int iLCount = pEffect->GetLightCount();
    if( iLCount > 0 )
    {
        EnableLightingFixed();

        for( int i = 0; i < iLCount; i++ )
        {
            SetLightingStateFixed(i, pEffect->GetLight(i));
        }
    }

    // Keep track of the current sampler to be used in enabling the textures.
    m_iCurrentSampler = 0;

    // ����texture.
    int iTCount = pEffect->GetTextureCount();
    for( int i = 0; i < iTCount; i++ )
    {
        SETexture* pTexture = pEffect->GetTexture(i);
        if( pTexture )
        {
            EnableTexture(pTexture);
            m_iCurrentSampler++;
        }
    }

    // ���ù̶�����WVP�任����.
    SetWVPMatrixFixed();

    // ��Ⱦ.
    DrawElements();

    // Keep track of the current sampler to be used in enabling the textures.
    m_iCurrentSampler = 0;

    // �ر�texture.
    for( int i = 0; i < iTCount; i++ )
    {
        SETexture* pTexture = pEffect->GetTexture(i);
        if( pTexture )
        {
            DisableTexture(pTexture);
            m_iCurrentSampler++;
        }
    }

    // �ر�light.
    if( iLCount > 0 )
    {
        for( int i = 0; i < iLCount; i++ )
        {
            RestoreLightingStateFixed(i, pEffect->GetLight(i));
        }

        DisableLightingFixed();
    }
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::Draw(SEGeometry* pGeometry)
{
    // ���õ�ǰ����Ⱦ����,����������ʹ��.
    m_pGeometry = pGeometry;

    // per-geometry pre-draw,��Ⱦ���������Զ�����Ϊ���.
    OnPreDrawGeometry();

    // ��������任����.
    SetWorldTransformation();

    // ������ǰ����Ⱦ�����IB.
    // ����Ӧ���ڵ�ǰ����Ⱦ�����effect��ÿ��effect������pass����ʹ�ø�IB.
    EnableIBuffer();

    // ������ǰ����Ⱦ�����VB.
    const SEAttributes& rAttr = m_pGeometry->VBuffer->GetAttributes();
    SEResourceIdentifier* pID = EnableVBuffer(rAttr, rAttr, 0);

    bool bPrimaryEffect = true;
    const int iMin = m_pGeometry->GetStartEffect();
    const int iMax = m_pGeometry->GetEffectCount();
    for( int i = iMin; i < iMax; i++ )
    {
        // effect������OpenGL ES1 fixed effect.
        SEOGLES1FixedEffect* pEffect = 
            DynamicCast<SEOGLES1FixedEffect>(m_pGeometry->GetEffect(i));
        ApplyFixedEffect(pEffect, bPrimaryEffect);
    }

    // �رյ�ǰ����Ⱦ�����VB.
    DisableVBuffer(pID);

    // �رյ�ǰ����Ⱦ�����IB.
    DisableIBuffer();

    // �ָ�֮ǰ������任����.
    RestoreWorldTransformation();

    // per-geometry post-draw,��Ⱦ���������Զ�����Ϊ���.
    OnPostDrawGeometry();

    // ��Ⱦ���,������õ�ǰ����Ⱦ����.
    m_pGeometry = 0;
}
//----------------------------------------------------------------------------
