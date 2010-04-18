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

#include "SEFoundationPCH.h"
#include "SEShadowMapEffect.h"
#include "SEBitHacks.h"
#include "SEGeometry.h"
#include "SEUnculledSet.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SEShadowMapEffect, SEEffect);
SE_IMPLEMENT_STREAM(SEShadowMapEffect);

//SE_REGISTER_STREAM(SEShadowMapEffect);

//----------------------------------------------------------------------------
SEShadowMapEffect::SEShadowMapEffect(SECamera* pProjector,
    const std::string& rProjectionImage, SEImage::FormatMode eDepthFormat, 
    int iDepthWidth, int iDepthHeight, float fDepthBias)
    :
    m_spProjector(pProjector)
{
    // 20090305����.
    //SE_ASSERT(IsPowerOfTwo((unsigned int)iDepthWidth)
    //    && IsPowerOfTwo((unsigned int)iDepthHeight));

    // ����depth effect.
    SEVertexShader* pVShader = 
        SE_NEW SEVertexShader("ProjectedShadow.v_ProjectedDepth");
    SEPixelShader* pPShader = 
       SE_NEW SEPixelShader("ProjectedShadow.p_ProjectedDepth");
    m_spDepthEffect = SE_NEW SEShaderEffect(1);
    m_spDepthEffect->SetVShader(0, pVShader);
    m_spDepthEffect->SetPShader(0, pPShader);
    m_pDepthBuffer = 0;  // �ӳٴ���,ֱ����һ��Draw����ʱ.

    // Ϊdepth texture����һ��image.
    int iCount = 4*iDepthWidth*iDepthHeight;
    unsigned char* aucData = SE_NEW unsigned char[iCount];
    m_spDepthImage = SE_NEW SEImage(eDepthFormat, iDepthWidth,
        iDepthHeight, aucData, "DepthImage");

    // ����shadow map effect.
    pVShader = SE_NEW SEVertexShader("ProjectedShadow.v_ShadowMap");
    pPShader = SE_NEW SEPixelShader("ProjectedShadow.p_ShadowMap");
    pPShader->SetTextureCount(2);
    pPShader->SetImageName(0, rProjectionImage);
    pPShader->SetImageName(1, "DepthImage");
    SETexture* pProjectedTexture = pPShader->GetTexture(0);
    pProjectedTexture->SetFilterType(SETexture::LINEAR);
    m_spShadowEffect = SE_NEW SEShaderEffect(1);
    m_spShadowEffect->SetVShader(0, pVShader);
    m_spShadowEffect->SetPShader(0, pPShader);
    SEAlphaState* pAState = m_spShadowEffect->GetBlending(0);
    pAState->SrcBlend = SEAlphaState::SBF_DST_COLOR;
    pAState->DstBlend = SEAlphaState::DBF_ZERO;

    m_afDepthBias[0] = fDepthBias;
}
//----------------------------------------------------------------------------
SEShadowMapEffect::SEShadowMapEffect()
{
    m_pDepthBuffer = 0;
    m_pDepthTexture = 0;
}
//----------------------------------------------------------------------------
SEShadowMapEffect::~SEShadowMapEffect()
{
    SE_DELETE m_pDepthBuffer;
}
//----------------------------------------------------------------------------
void SEShadowMapEffect::Draw(SERenderer* pRenderer, SESpatial*, int iMin, int
    iMax, SEUnculledObject* pVisibleSet)
{
    // ������Ҫ�õ��������Ⱦ��ʵ��,����depth buffer�������ӳٴ���.
    // ��Ϊshadow map effect���ܻ�Ӵ���ͨ��streamװ��,����б�Ҫ�ӳٴ���.
    if( !m_pDepthBuffer )
    {
        m_pDepthTexture = m_spShadowEffect->GetPShader(0)->GetTexture(1);
        m_pDepthTexture->SetFilterType(SETexture::LINEAR);
        m_pDepthTexture->SetOffscreenTexture(true);
        pRenderer->LoadResources(m_spShadowEffect);

        SEPixelProgram* pProgram = m_spShadowEffect->GetPProgram(0);
        pProgram->GetUC("DepthBias")->SetDataSource(m_afDepthBias);

        SETexture** apTargets = SE_NEW SETexture*[1];
        apTargets[0] = m_pDepthTexture;
        m_pDepthBuffer = SEFrameBuffer::Create(pRenderer->GetFormatType(),
            pRenderer->GetDepthType(), pRenderer->GetStencilType(),
            pRenderer->GetBufferingType(), pRenderer->GetMultisamplingType(),
            pRenderer, 1, apTargets);
        SE_ASSERT(m_pDepthBuffer);
    }

    // ��projector��Ϊcamera��Ⱦ�����ͼ.
    // projector camera�����ñ�������framebuffer��Enable����,
    // ��Ϊ��Enable�����н���ʹ��projector����ر任����.
    SECamera* pSaveCamera = pRenderer->GetCamera();
    pRenderer->SetCamera(m_spProjector);
    m_pDepthBuffer->Enable();
    pRenderer->ClearBuffers();

    // ��һ����ͼ���depth bias����ķ���.
    // SECullState* pCState = pRenderer->GetCullState();
    // pCState->CullFace = SECullState::CT_FRONT;
    // pRenderer->SetCullState(pCState);

    int i;
    for( i = iMin; i <= iMax; i++ )
    {
        if( pVisibleSet[i].IsRenderable() )
        {
            SEGeometry* pGeometry = (SEGeometry*)pVisibleSet[i].SEObject;
            pGeometry->AttachEffect(m_spDepthEffect);
            pGeometry->SetStartEffect(pGeometry->GetEffectCount() - 1);
            pRenderer->Draw(pGeometry);
            pGeometry->SetStartEffect(0);
            pGeometry->DetachEffect(m_spDepthEffect);
        }
    }

    // ��һ����ͼ���depth bias����ķ���.
    // pCState->CullFace = SECullState::CT_BACK;
    // pRenderer->SetCullState(pCState);

//#if 0
//    // BEGIN TEST
//    m_pDepthBuffer->CopyToTexture(true);
//    SEImage* pDTImage = m_pDepthTexture->GetImage();
//    ImageRGB82D tempImage(pDTImage->GetBound(0), pDTImage->GetBound(1));
//    unsigned char* pucData = pDTImage->GetData();
//    for( int i = 0; i < pDTImage->GetCount(); i++ )
//    {
//        unsigned char ucR = *pucData++;
//        unsigned char ucG = *pucData++;
//        unsigned char ucB = *pucData++;
//        pucData++;
//        tempImage[i] = GetColor24(ucR, ucG, ucB);
//    }
//    tempImage.Save("dimage.im");
//    // END TEST
//#endif

    // ���ͼ��Ⱦ���.�����ڵ���Disable����ǰ,�ָ�֮ǰ��camera.
    // �Ӷ�ʹ֮ǰ��camera����ر任������Disable�����б�ʹ��.
    pRenderer->SetCamera(pSaveCamera);
    m_pDepthBuffer->Disable();

    // ������Ⱦshadow map effect�������ڵ����п���Ⱦ����, 
    // shadow effect��ͬʱʹ����projection image��shadow.
    // ͨ��projection image����ֱ�۵Ĺ۲쵽projector��Ϊcameraʱ����Ⱦ����.
    pRenderer->SetProjector(m_spProjector);
    pRenderer->ClearBuffers();
    for( i = iMin; i <= iMax; i++ )
    {
        if( pVisibleSet[i].IsRenderable() )
        {
            SEGeometry* pGeometry = (SEGeometry*)pVisibleSet[i].SEObject;
            pGeometry->AttachEffect(m_spShadowEffect);
            pRenderer->Draw(pGeometry);
            pGeometry->DetachEffect(m_spShadowEffect);
        }
    }
    pRenderer->SetProjector(0);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// name and unique id
//----------------------------------------------------------------------------
SEObject* SEShadowMapEffect::GetObjectByName(const std::string& rName)
{
    SEObject* pFound = SEEffect::GetObjectByName(rName);
    if( pFound )
    {
        return pFound;
    }

    if( m_spProjector )
    {
        pFound = m_spProjector->GetObjectByName(rName);
        if( pFound )
        {
            return pFound;
        }
    }

    if( m_spDepthEffect )
    {
        pFound = m_spDepthEffect->GetObjectByName(rName);
        if( pFound )
        {
            return pFound;
        }
    }

    if( m_spDepthImage )
    {
        pFound = m_spDepthImage->GetObjectByName(rName);
        if( pFound )
        {
            return pFound;
        }
    }

    if( m_spShadowEffect )
    {
        pFound = m_spShadowEffect->GetObjectByName(rName);
        if( pFound )
        {
            return pFound;
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
void SEShadowMapEffect::GetAllObjectsByName(const std::string& rName,
    std::vector<SEObject*>& rObjects)
{
    SEEffect::GetAllObjectsByName(rName, rObjects);

    if( m_spProjector )
    {
        m_spProjector->GetAllObjectsByName(rName, rObjects);
    }

    if( m_spDepthEffect )
    {
        m_spDepthEffect->GetAllObjectsByName(rName, rObjects);
    }

    if( m_spDepthImage )
    {
        m_spDepthImage->GetAllObjectsByName(rName, rObjects);
    }

    if( m_spShadowEffect )
    {
        m_spShadowEffect->GetAllObjectsByName(rName, rObjects);
    }
}
//----------------------------------------------------------------------------
SEObject* SEShadowMapEffect::GetObjectByID(unsigned int uiID)
{
    SEObject* pFound = SEEffect::GetObjectByID(uiID);
    if( pFound )
    {
        return pFound;
    }

    if( m_spProjector )
    {
        pFound = m_spProjector->GetObjectByID(uiID);
        if( pFound )
        {
            return pFound;
        }
    }

    if( m_spDepthEffect )
    {
        pFound = m_spDepthEffect->GetObjectByID(uiID);
        if( pFound )
        {
            return pFound;
        }
    }

    if( m_spDepthImage )
    {
        pFound = m_spDepthImage->GetObjectByID(uiID);
        if( pFound )
        {
            return pFound;
        }
    }

    if( m_spShadowEffect )
    {
        pFound = m_spShadowEffect->GetObjectByID(uiID);
        if( pFound )
        {
            return pFound;
        }
    }

    return 0;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// streaming
//----------------------------------------------------------------------------
void SEShadowMapEffect::Load(SEStream& rStream, SEStream::SELink* pLink)
{
    SE_BEGIN_DEBUG_STREAM_LOAD;

    SEEffect::Load(rStream, pLink);

    // native data
    rStream.Read(m_afDepthBias[0]);

    // link data
    SEObject* pObject;
    rStream.Read(pObject);  // m_spProjector
    pLink->Add(pObject);
    rStream.Read(pObject);  // m_spDepthEffect
    pLink->Add(pObject);
    rStream.Read(pObject);  // m_spDepthImage
    pLink->Add(pObject);
    rStream.Read(pObject);  // m_spShadowEffect
    pLink->Add(pObject);

    SE_END_DEBUG_STREAM_LOAD(SEShadowMapEffect);
}
//----------------------------------------------------------------------------
void SEShadowMapEffect::SELink(SEStream& rStream, SEStream::SELink* pLink)
{
    SEEffect::SELink(rStream, pLink);

    SEObject* pLinkID = pLink->GetLinkID();
    m_spProjector = (SECamera*)rStream.GetFromMap(pLinkID);
    pLinkID = pLink->GetLinkID();
    m_spDepthEffect = (SEShaderEffect*)rStream.GetFromMap(pLinkID);
    pLinkID = pLink->GetLinkID();
    m_spDepthImage = (SEImage*)rStream.GetFromMap(pLinkID);
    pLinkID = pLink->GetLinkID();
    m_spShadowEffect = (SEShaderEffect*)rStream.GetFromMap(pLinkID);
}
//----------------------------------------------------------------------------
bool SEShadowMapEffect::Register(SEStream& rStream) const
{
    if( !SEEffect::Register(rStream) )
    {
        return false;
    }

    if( m_spProjector )
    {
        m_spProjector->Register(rStream);
    }

    if( m_spDepthEffect )
    {
        m_spDepthEffect->Register(rStream);
    }

    if( m_spDepthImage )
    {
        m_spDepthImage->Register(rStream);
    }

    if( m_spShadowEffect )
    {
        m_spShadowEffect->Register(rStream);
    }

    return true;
}
//----------------------------------------------------------------------------
void SEShadowMapEffect::Save(SEStream& rStream) const
{
    SE_BEGIN_DEBUG_STREAM_SAVE;

    SEEffect::Save(rStream);

    // native data
    rStream.Write(m_afDepthBias[0]);

    // link data
    rStream.Write(m_spProjector);
    rStream.Write(m_spDepthEffect);
    rStream.Write(m_spDepthImage);
    rStream.Write(m_spShadowEffect);

    SE_END_DEBUG_STREAM_SAVE(SEShadowMapEffect);
}
//----------------------------------------------------------------------------
int SEShadowMapEffect::GetDiskUsed(const SEStreamVersion& rVersion) const
{
    return SEEffect::GetDiskUsed(rVersion) +
        sizeof(m_afDepthBias[0]) +
        sizeof(m_spProjector) +
        sizeof(m_spDepthEffect) +
        sizeof(m_spDepthImage) +
        sizeof(m_spShadowEffect);
}
//----------------------------------------------------------------------------
SEStringTree* SEShadowMapEffect::SaveStrings(const char*)
{
    SEStringTree* pTree = SE_NEW SEStringTree;

    // strings
    pTree->Append(Format(&TYPE, GetName().c_str()));
    pTree->Append(Format("depth bias =", m_afDepthBias[0]));

    // children
    pTree->Append(SEEffect::SaveStrings());

    if( m_spProjector )
    {
        pTree->Append(m_spProjector->SaveStrings());
    }

    if( m_spDepthEffect )
    {
        pTree->Append(m_spDepthEffect->SaveStrings());
    }

    if( m_spDepthImage )
    {
        pTree->Append(m_spDepthImage->SaveStrings());
    }

    if( m_spShadowEffect )
    {
        pTree->Append(m_spShadowEffect->SaveStrings());
    }

    return pTree;
}
//----------------------------------------------------------------------------
