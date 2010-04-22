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

#include "SEDX10ApplicationPCH.h"
#include "SEWindowApplication.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEWindowApplication::SEWindowApplication(const char* acWindowTitle,
    int iXPosition, int iYPosition, int iWidth, int iHeight,
    const SEColorRGBA& rBackgroundColor)
    :
    m_acWindowTitle(acWindowTitle),
    m_iXPosition(iXPosition),
    m_iYPosition(iYPosition),
    m_iWidth(iWidth),
    m_iHeight(iHeight),
    m_BackgroundColor(rBackgroundColor)
{
    m_iWindowID = 0;
    m_pRenderer = 0;

    // Ĭ��frame buffer����.
    // ��Щ�������Ա������๹�캯������.
    // ��ǰӲ������ʹ��24bit depth buffer,8bit stencil buffer,���鲻Ҫ�޸�.
    // ��޸ĵ���multisamplingֵ.
    m_eFormat = SEFrameBuffer::FT_FORMAT_RGBA;
    m_eDepth = SEFrameBuffer::DT_DEPTH_24;
    m_eStencil = SEFrameBuffer::ST_STENCIL_8;
    m_eBuffering = SEFrameBuffer::BT_BUFFERED_DOUBLE;
    m_eMultisampling = SEFrameBuffer::MT_SAMPLING_NONE;
}
//----------------------------------------------------------------------------
SEWindowApplication::~SEWindowApplication()
{
}
//----------------------------------------------------------------------------
int SEWindowApplication::Run(int iArgCount, char** apcArgument)
{
    SEWindowApplication* pTheApp = (SEWindowApplication*)TheApplication;

    return pTheApp->Main(iArgCount, apcArgument);
}
//----------------------------------------------------------------------------
bool SEWindowApplication::OnInitialize()
{
    m_pRenderer->SetClearColor(m_BackgroundColor);

    // ֪ͨcatalogs,��Ⱦ���Ѿ�����.
    SE_ASSERT( SEVertexProgramCatalog::GetActive() );
    SEVertexProgramCatalog::GetActive()->SetRenderer(m_pRenderer);

    SE_ASSERT( SEPixelProgramCatalog::GetActive() );
    SEPixelProgramCatalog::GetActive()->SetRenderer(m_pRenderer);

    SE_ASSERT( SEDX10ProgramInterfaceCatalog::GetActive() );
    SEDX10ProgramInterfaceCatalog::GetActive()->SetRenderer(
        (SEDX10Renderer*)m_pRenderer);

    return true;
}
//----------------------------------------------------------------------------
void SEWindowApplication::OnTerminate()
{
    // ֪ͨcatalogs,��Ⱦ���������ͷ�,
    // ��˿���ɾ��Ĭ��shader������.
    SE_ASSERT( SEVertexProgramCatalog::GetActive() );
    SEVertexProgramCatalog::GetActive()->SetRenderer(0);

    SE_ASSERT( SEPixelProgramCatalog::GetActive() );
    SEPixelProgramCatalog::GetActive()->SetRenderer(0);

    SE_ASSERT( SEDX10ProgramInterfaceCatalog::GetActive() );
    SEDX10ProgramInterfaceCatalog::GetActive()->SetRenderer(0);    

    SE_DELETE m_pRenderer;
    m_pRenderer = 0;
}
//----------------------------------------------------------------------------
void SEWindowApplication::OnMove(int iX, int iY)
{
    m_iXPosition = iX;
    m_iYPosition = iY;
}
//----------------------------------------------------------------------------
void SEWindowApplication::OnResize(int iWidth, int iHeight)
{
    if( iWidth > 0 && iHeight > 0 )
    {
        if( m_pRenderer )
        {
            m_pRenderer->Resize(iWidth, iHeight);
        }

        m_iWidth = iWidth;
        m_iHeight = iHeight;
    }
}
//----------------------------------------------------------------------------
