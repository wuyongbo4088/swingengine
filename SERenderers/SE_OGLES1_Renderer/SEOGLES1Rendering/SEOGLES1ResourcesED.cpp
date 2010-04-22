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
#include "SEOGLES1Resources.h"

using namespace Swing;

//----------------------------------------------------------------------------
// ��Դ������ر�.
//----------------------------------------------------------------------------
void SEOGLES1Renderer::SetVProgramRC(SERendererConstant*)
{
    // ��֧�ֵĻ��๦��.
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::SetVProgramUC(SEUserConstant*)
{
    // ��֧�ֵĻ��๦��.
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::SetGProgramRC(SERendererConstant*)
{
    // ��֧�ֵĻ��๦��.
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::SetGProgramUC(SEUserConstant*)
{
    // ��֧�ֵĻ��๦��.
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::SetPProgramRC(SERendererConstant*)
{
    // ��֧�ֵĻ��๦��.
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::SetPProgramUC(SEUserConstant*)
{
    // ��֧�ֵĻ��๦��.
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::UpdateVProgramConstants(SEVertexProgram*)
{
    // ��֧�ֵĻ��๦��.
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::UpdateGProgramConstants(SEGeometryProgram*)
{
    // ��֧�ֵĻ��๦��.
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::UpdatePProgramConstants(SEPixelProgram*)
{
    // ��֧�ֵĻ��๦��.
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::OnEnableVProgram(SEResourceIdentifier*)
{
    // ��֧�ֵĻ��๦��.
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::OnDisableVProgram(SEResourceIdentifier*)
{
    // ��֧�ֵĻ��๦��.
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::OnEnablePProgram(SEResourceIdentifier*)
{
    // ��֧�ֵĻ��๦��.
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::OnDisablePProgram(SEResourceIdentifier*)
{
    // ��֧�ֵĻ��๦��.
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::OnEnableTexture(SEResourceIdentifier* pID)
{
    SETextureID* pResource = (SETextureID*)pID;

    // ��ʵ��.
    // Ŀǰֻ֧��2D texture.
    SESamplerInformation::Type eSType = SESamplerInformation::SAMPLER_2D;
    int iTextureUnit = m_iCurrentSampler;
    int eTarget = ms_aeSamplerTypes[eSType];

    glClientActiveTexture(GL_TEXTURE0 + iTextureUnit);
    glActiveTexture(GL_TEXTURE0 + iTextureUnit);
    glBindTexture(eTarget, pResource->ID);
    glEnable(eTarget);
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::OnDisableTexture(SEResourceIdentifier* pID)
{
    // ��ʵ��.
    // Ŀǰֻ֧��2D texture.
    SESamplerInformation::Type eSType = SESamplerInformation::SAMPLER_2D;
    int eTarget = ms_aeSamplerTypes[eSType];
    glBindTexture(eTarget, 0);
    glDisable(eTarget);
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::OnEnableVBuffer(SEResourceIdentifier* pID, 
    SEVertexProgram*)
{
    // Bind��ǰvertex buffer.
    SEVBufferID* pResource = (SEVBufferID*)pID;
    glBindBuffer(GL_ARRAY_BUFFER, pResource->ID);

    const SEAttributes& rRAttr = pResource->IAttr;
    GLsizei iSize = (GLsizei)(sizeof(float)*rRAttr.GetChannelCount());
    const float* afData = 0;

    if( rRAttr.HasPosition() )
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(rRAttr.GetPositionChannels(), GL_FLOAT, iSize,
            afData + rRAttr.GetPositionOffset());
    }

    if( rRAttr.HasNormal() )
    {
        // OpenGL�������û�ָ��normal vector�ķ�������.
        // ���,rRAttr.GetNChannels�������صķ�������Ӧ��Ϊ3.
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, iSize, afData + rRAttr.GetNormalOffset());
    }

    // OpenGL ES1ֻ֧��һ�鶥����ɫ,��˲���������color channel.
    if( rRAttr.HasColor(0) )
    {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(rRAttr.GetColorChannels(0), GL_FLOAT, iSize,
            afData + rRAttr.GetColorOffset(0));
    }

    for( int iUnit = 0; iUnit < rRAttr.GetMaxTCoords(); iUnit++ )
    {
        if( rRAttr.HasTCoord(iUnit) )
        {
            glClientActiveTexture(GL_TEXTURE0 + iUnit);
            glActiveTexture(GL_TEXTURE0 + iUnit);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY); 
            glTexCoordPointer(rRAttr.GetTCoordChannels(iUnit), GL_FLOAT, iSize,
                afData + rRAttr.GetTCoordOffset(iUnit));
        }
    }
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::OnDisableVBuffer(SEResourceIdentifier* pID, 
    SEVertexProgram*)
{
    SEVBufferID* pResource = (SEVBufferID*)pID;
    const SEAttributes& rRAttr = pResource->IAttr;

    // Unbind��ǰvertex buffer.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if( rRAttr.HasPosition() )
    {
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    if( rRAttr.HasNormal() )
    {
        glDisableClientState(GL_NORMAL_ARRAY);
    }

    if( rRAttr.HasColor(0) )
    {
        glDisableClientState(GL_COLOR_ARRAY);
    }

    for( int iUnit = 0; iUnit < rRAttr.GetMaxTCoords(); iUnit++ )
    {
        if( rRAttr.HasTCoord(iUnit) )
        {
            glClientActiveTexture(GL_TEXTURE0 + iUnit);
            glActiveTexture(GL_TEXTURE0 + iUnit);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    }
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::OnEnableIBuffer(SEResourceIdentifier* pID)
{
    // Bind��ǰindex buffer.
    SEIBufferID* pResource = (SEIBufferID*)pID;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pResource->ID);
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::OnDisableIBuffer(SEResourceIdentifier*)
{
    // Unbind��ǰindex buffer.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::EnableLightingFixed()
{
    glEnable(GL_LIGHTING);

    // ��ʵ��.
    // �Ƿ��б�ҪƵ������������Ⱦ״̬?�⽫��������.

    // Ҫ�����ڱ任������ʼ�ձ��ֵ�λ����.
    glEnable(GL_NORMALIZE);
    // ƽ����ɫ.
    glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixf(m_ViewMatrix);
}
//----------------------------------------------------------------------------
void SEOGLES1Renderer::DisableLightingFixed()
{
    glDisable(GL_LIGHTING);
}
//----------------------------------------------------------------------------
