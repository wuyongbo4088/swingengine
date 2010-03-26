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

#include "SEOGLES2RendererPCH.h"
#include "SEOGLES2Resources.h"
#include "SEOGLES2FrameBuffer.h"
#include "SECamera.h"

using namespace Swing;

SE_IMPLEMENT_INITIALIZE(OGLES2FrameBuffer);

//SE_REGISTER_INITIALIZE(OGLES2FrameBuffer);

//----------------------------------------------------------------------------
void OGLES2FrameBuffer::Initialize()
{
    ms_aoCreator[Renderer::OPENGLES2] = &OGLES2FrameBuffer::Create;
    ms_aoDestroyer[Renderer::OPENGLES2] = &OGLES2FrameBuffer::Destroy;
}
//----------------------------------------------------------------------------
FrameBuffer* OGLES2FrameBuffer::Create(FormatType eFormat, DepthType eDepth,
    StencilType eStencil, BufferingType eBuffering,
    MultisamplingType eMultisampling, Renderer* pRenderer, int iTCount,
    Texture** apTargets)
{
    if( pRenderer && apTargets )
    {
        OGLES2FrameBuffer* pBuffer = SE_NEW OGLES2FrameBuffer(eFormat,
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
void OGLES2FrameBuffer::Destroy(FrameBuffer* pBuffer)
{
    ((OGLES2FrameBuffer*)pBuffer)->TerminateBuffer();
    SE_DELETE pBuffer;
}
//----------------------------------------------------------------------------
OGLES2FrameBuffer::OGLES2FrameBuffer(FormatType eFormat, DepthType eDepth,
    StencilType eStencil, BufferingType eBuffering,
    MultisamplingType eMultisampling, Renderer* pRenderer, int iTCount, 
    Texture** apTargets)
    :
    FrameBuffer(eFormat, eDepth, eStencil, eBuffering, eMultisampling, 
        pRenderer, iTCount, apTargets)
{
    // OpenGL ES2ֻ֧��һ��color render target.
    SE_ASSERT( iTCount == 1 );
	
    m_TargetItems.resize(iTCount);
    m_pAttachments = SE_NEW GLenum[iTCount];
}
//----------------------------------------------------------------------------
OGLES2FrameBuffer::~OGLES2FrameBuffer()
{
    SE_DELETE[] m_pAttachments;
}
//----------------------------------------------------------------------------
bool OGLES2FrameBuffer::InitializeBuffer()
{
    // ������render buffer�ߴ�.
    Image* pImage = m_apTargets[0]->GetImage();
    SE_ASSERT( pImage );
    int iWidth = pImage->GetBound(0);
    int iHeight = pImage->GetBound(1);
    GLint iMaxRenderbufferSize;
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &iMaxRenderbufferSize);
    if( iMaxRenderbufferSize <= iWidth || iMaxRenderbufferSize <= iHeight )
    {
        SE_ASSERT( false );
        return false;
    }

    // ֧��Ƕ�׵���FBO,�ݴ浱ǰ����ʹ�õ�FBO,�Ժ����ڻָ�.
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&m_uiSaveFrameBufferID);

    // ����һ��FBO����.
    glGenFramebuffers(1, &m_uiFrameBufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_uiFrameBufferID);

    // ΪFBO���󴴽�depth buffer.
    glGenRenderbuffers(1, &m_uiDepthBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_uiDepthBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16,
        (GLsizei)iWidth, (GLsizei)iHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
        GL_RENDERBUFFER, m_uiDepthBufferID);

    // ΪFBO�����������.
    if( pImage->IsCubeImage() )
    {
        // ��ʵ��.
        // ��δ֧��cube map.
        TerminateBuffer();
        glBindFramebuffer(GL_FRAMEBUFFER, m_uiSaveFrameBufferID);
        return false;
    }

    // ȷ������buffer�������Ѿ�װ�����Դ�.
    ResourceIdentifier* pID = m_apTargets[0]->GetIdentifier(m_pRenderer);
    SE_ASSERT( pID );
    TextureID* pResource = (TextureID*)pID;
    m_TargetItems[0].TargetID = pResource->ID;
    glBindTexture(GL_TEXTURE_2D, m_TargetItems[0].TargetID);

    if( !pImage->IsDepthImage() )
    {
        m_pAttachments[0] = GL_COLOR_ATTACHMENT0;
        glFramebufferTexture2D(GL_FRAMEBUFFER, m_pAttachments[0],
            GL_TEXTURE_2D, m_TargetItems[0].TargetID, 0);
    }
    else
    {
        m_pAttachments[0] = GL_DEPTH_ATTACHMENT;
        glFramebufferTexture2D(GL_FRAMEBUFFER, m_pAttachments[0],
            GL_TEXTURE_2D, m_TargetItems[0].TargetID, 0);
    }

    GLenum uiStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    int iStopHere;
    switch( uiStatus )
    {
    case GL_FRAMEBUFFER_COMPLETE:
        iStopHere = 0;
        break;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
    case GL_FRAMEBUFFER_UNSUPPORTED:

        TerminateBuffer();
        glBindFramebuffer(GL_FRAMEBUFFER, m_uiSaveFrameBufferID);
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, m_uiSaveFrameBufferID);

    return true;
}
//----------------------------------------------------------------------------
void OGLES2FrameBuffer::TerminateBuffer()
{
    glDeleteFramebuffers(1, &m_uiFrameBufferID);
    glDeleteRenderbuffers(1, &m_uiDepthBufferID);
}
//----------------------------------------------------------------------------
void OGLES2FrameBuffer::Enable()
{
    OGLES2Renderer* pRenderer = (OGLES2Renderer*)m_pRenderer;

    glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&m_uiSaveFrameBufferID);

    glBindTexture(GL_TEXTURE_2D, 0); // ȷ������ʹ�õ��������Ѱ󶨵�targets
    glBindFramebuffer(GL_FRAMEBUFFER, m_uiFrameBufferID);

    pRenderer->OnFrustumChange();

    if( pRenderer->m_pCamera->IsPerspective() )
    {
        pRenderer->m_ProjectionMatrix[1][1] = 
            -pRenderer->m_ProjectionMatrix[1][1];
        pRenderer->m_ProjectionMatrix[2][1] = 
            -pRenderer->m_ProjectionMatrix[2][1];
    }
    else
    {
        pRenderer->m_ProjectionMatrix[1][1] = 
            -pRenderer->m_ProjectionMatrix[1][1];
        pRenderer->m_ProjectionMatrix[3][1] = 
            -pRenderer->m_ProjectionMatrix[3][1];
    }

    // ����ʹ��FBOʱҪ��תcull state,Ƕ���е�FBO�����ٴη�ת.
    if( m_uiSaveFrameBufferID == 0 )
    {
        pRenderer->m_bReverseCullFace = !pRenderer->m_bReverseCullFace;
        CullState* pCState = pRenderer->GetCullState();
        pRenderer->SetCullState(pCState);
    }

    Image* pImage = m_apTargets[0]->GetImage();
    glViewport(0, 0, pImage->GetBound(0), pImage->GetBound(1));
}
//----------------------------------------------------------------------------
void OGLES2FrameBuffer::Disable()
{
    OGLES2Renderer* pRenderer = (OGLES2Renderer*)m_pRenderer;

    glBindFramebuffer(GL_FRAMEBUFFER, m_uiSaveFrameBufferID);

    pRenderer->OnViewportChange();
    pRenderer->OnFrustumChange();

    if( m_uiSaveFrameBufferID == 0 )
    {
        // ��������FBOʱ,�ָ�������view matrix��cull state.
        pRenderer->OnFrameChange();

        pRenderer->m_bReverseCullFace = !pRenderer->m_bReverseCullFace;
        CullState* pCState = pRenderer->GetCullState();
        pRenderer->SetCullState(pCState);
    }
    else
    {
        // ������ǰǶ�׵�FBOʱ,��Ҫ�����matrix�ָ���֮ǰ��FBO����Ҫ����ʽ.
        if( pRenderer->m_pCamera->IsPerspective() )
        {
            pRenderer->m_ProjectionMatrix[1][1] = 
                -pRenderer->m_ProjectionMatrix[1][1];
            pRenderer->m_ProjectionMatrix[2][1] = 
                -pRenderer->m_ProjectionMatrix[2][1];
        }
        else
        {
            pRenderer->m_ProjectionMatrix[1][1] = 
                -pRenderer->m_ProjectionMatrix[1][1];
            pRenderer->m_ProjectionMatrix[3][1] = 
                -pRenderer->m_ProjectionMatrix[3][1];
        }
    }
}
//----------------------------------------------------------------------------
void OGLES2FrameBuffer::CopyToTexture(int)
{
    // ��ʵ��.
}
//----------------------------------------------------------------------------
