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

#include "SEDX10RendererPCH.h"
#include "SEDX10Renderer.h"
#include "SEDX10Resources.h"
#include "SEShaderEffect.h"

using namespace Swing;

const std::string DX10Renderer::ms_PositionStr("POSITION");
const std::string DX10Renderer::ms_NormalStr("NORMAL");
const std::string DX10Renderer::ms_ColorStr("COLOR");
const std::string DX10Renderer::ms_TexCoordStr("TEXCOORD");

//----------------------------------------------------------------------------
// ��Դװ�����Ƴ�(��Ҫ����Դ�).
//----------------------------------------------------------------------------
void DX10Renderer::OnLoadVProgram(ResourceIdentifier*& rpID, 
    VertexProgram* pVProgram)
{
    VProgramID* pResource = SE_NEW VProgramID;
    pResource->ID = pVProgram->GetProgram();
    rpID = pResource;

    ms_hResult = cgD3D10LoadProgram(pResource->ID, 0);
    SE_ASSERT( SUCCEEDED(ms_hResult) );
}
//----------------------------------------------------------------------------
void DX10Renderer::OnReleaseVProgram(ResourceIdentifier* pID)
{
    VProgramID* pResource = (VProgramID*)pID;
    cgD3D10UnloadProgram(pResource->ID);
    SE_DELETE pResource;
}
//----------------------------------------------------------------------------
void DX10Renderer::OnLoadGProgram(ResourceIdentifier*& rpID,
    GeometryProgram* pGProgram)
{
    GProgramID* pResource = SE_NEW GProgramID;
    pResource->ID = pGProgram->GetProgram();
    rpID = pResource;

    ms_hResult = cgD3D10LoadProgram(pResource->ID, 0);
    SE_ASSERT( SUCCEEDED(ms_hResult) );
}
//----------------------------------------------------------------------------
void DX10Renderer::OnReleaseGProgram(ResourceIdentifier* pID)
{
    GProgramID* pResource = (GProgramID*)pID;
    cgD3D10UnloadProgram(pResource->ID);
    SE_DELETE pResource;
}
//----------------------------------------------------------------------------
void DX10Renderer::OnLoadPProgram(ResourceIdentifier*& rpID, 
    PixelProgram* pPProgram)
{
    PProgramID* pResource = SE_NEW PProgramID;
    pResource->ID = pPProgram->GetProgram();
    rpID = pResource;

    ms_hResult = cgD3D10LoadProgram(pResource->ID, 0);
    SE_ASSERT( SUCCEEDED(ms_hResult) );
}
//----------------------------------------------------------------------------
void DX10Renderer::OnReleasePProgram(ResourceIdentifier* pID)
{
    PProgramID* pResource = (PProgramID*)pID;
    cgD3D10UnloadProgram(pResource->ID);
    SE_DELETE pResource;
}
//----------------------------------------------------------------------------
void DX10Renderer::OnLoadTexture(ResourceIdentifier*& rpID, Texture* pTexture)
{
    // ��һ��������������Դ.
    // Ϊʹ�ø��������豸����������Ԫ.
    TextureID* pResource = SE_NEW TextureID;
    pResource->TextureObject = pTexture;
    rpID = pResource;

    SamplerInformation* pSI = pTexture->GetSamplerInformation();
    SamplerInformation::Type eSType = pSI->GetType();
    bool bDepth = pTexture->IsDepthTexture();
    bool bOffscreen = pTexture->IsOffscreenTexture();

    // ��image���ݴ�ϵͳ�ڴ�װ�����Դ�..
    const Image* pImage = pTexture->GetImage();
    SE_ASSERT( pImage );

    // DXGI_FORMAT_R8G8B8A8_UNORM����ABGR(����ֽڵ�����ֽ�)˳�򴢴���ɫ,
    // ���ǰ���RGBA˳��.��˱��뷭ת�ֽڴ洢˳��.
    int iCount, iByteCount = 0;
    unsigned char* aucSrc = pImage->GetData();
    unsigned char* aucRSrc = 0;
    int i, iSrcBase = 0, iRSrcBase = 0;
    DXGI_FORMAT eDXGIFMT = DXGI_FORMAT_UNKNOWN;
    if( pImage->GetFormat() == Image::IT_RGB888 )
    {
        iCount = pImage->GetCount();
        iByteCount = 4*iCount;
        aucRSrc = SE_NEW unsigned char[iByteCount];
        for( i = 0; i < iCount; i++, iSrcBase += 3, iRSrcBase += 4 )
        {
            aucRSrc[iRSrcBase    ] = 255;
            aucRSrc[iRSrcBase + 1] = aucSrc[iSrcBase + 2];
            aucRSrc[iRSrcBase + 2] = aucSrc[iSrcBase + 1];
            aucRSrc[iRSrcBase + 3] = aucSrc[iSrcBase    ];
        }
        eDXGIFMT = DXGI_FORMAT_R8G8B8A8_UNORM;
    }
    else if( pImage->GetFormat() == Image::IT_RGBA8888 )
    {
        iCount = pImage->GetCount();
        iByteCount = 4*iCount;
        aucRSrc = SE_NEW unsigned char[iByteCount];
        for( i = 0; i < iCount; i++, iSrcBase += 4 )
        {
            aucRSrc[iSrcBase    ] = aucSrc[iSrcBase + 3];
            aucRSrc[iSrcBase + 1] = aucSrc[iSrcBase + 2];
            aucRSrc[iSrcBase + 2] = aucSrc[iSrcBase + 1];
            aucRSrc[iSrcBase + 3] = aucSrc[iSrcBase    ];
        }
        eDXGIFMT = DXGI_FORMAT_R8G8B8A8_UNORM;
    }
    else if( pImage->GetFormat() == Image::IT_L8 )
    {
        iCount = pImage->GetCount();
        iByteCount = iCount;
        aucRSrc = SE_NEW unsigned char[iByteCount];
        for( i = 0; i < iCount; i++ )
        {
            aucRSrc[i] = aucSrc[i];
        }
        eDXGIFMT = DXGI_FORMAT_R8_UNORM;
    }
    else if( pImage->GetFormat() == Image::IT_L16 )
    {
        iCount = pImage->GetCount();
        iByteCount = 2*iCount;
        aucRSrc = SE_NEW unsigned char[iByteCount];
        for( i = 0; i < iByteCount; i++ )
        {
            aucRSrc[i] = aucSrc[i];
        }
        eDXGIFMT = DXGI_FORMAT_R16_UNORM;
    }
    else if( pImage->GetFormat() == Image::IT_R32 )
    {
        iCount = pImage->GetCount();
        iByteCount = 4*iCount;
        aucRSrc = SE_NEW unsigned char[iByteCount];
        for( i = 0; i < iByteCount; i++ )
        {
            aucRSrc[i] = aucSrc[i];
        }
        eDXGIFMT = DXGI_FORMAT_R32_FLOAT;
    }
    else if( pImage->GetFormat() == Image::IT_CUBE_RGB888 )
    {
        iCount = 6*pImage->GetCount();
        // 6�������������洢
        iByteCount = 4*iCount;
        aucRSrc = SE_NEW unsigned char[iByteCount];
        for( i = 0; i < iCount; i++, iSrcBase += 3, iRSrcBase += 4 )
        {
            aucRSrc[iRSrcBase    ] = 255;
            aucRSrc[iRSrcBase + 1] = aucSrc[iSrcBase + 2];
            aucRSrc[iRSrcBase + 2] = aucSrc[iSrcBase + 1];
            aucRSrc[iRSrcBase + 3] = aucSrc[iSrcBase    ];
        }
        // ÿ������ֽ���,�Ժ���
        iByteCount = 4*pImage->GetCount();
    }
    else if( pImage->GetFormat() == Image::IT_CUBE_RGBA8888
    || pImage->GetFormat() == Image::IT_RGB32
    || pImage->GetFormat() == Image::IT_RGBA32)
    {
        // ��ʵ��.
        // ֧����Щ��ʽ.
        SE_ASSERT( false );
    }

    D3D10_BIND_FLAG eBindFlag;
    D3D10_USAGE eUsage;
    if( bOffscreen )
    {
        eBindFlag = (D3D10_BIND_FLAG)(D3D10_BIND_SHADER_RESOURCE | 
            D3D10_BIND_RENDER_TARGET);
        eUsage = D3D10_USAGE_DEFAULT;
    }
    else if( bDepth )
    {
        eBindFlag = (D3D10_BIND_FLAG)(D3D10_BIND_SHADER_RESOURCE | 
            D3D10_BIND_DEPTH_STENCIL);
        eUsage = D3D10_USAGE_DEFAULT;
    }
    else
    {
        if( pImage->GetDimension() < 3 )
        {
            eBindFlag = D3D10_BIND_SHADER_RESOURCE;
        }
        else
        {
            eBindFlag = (D3D10_BIND_FLAG)0;
        }
        eUsage = D3D10_USAGE_DEFAULT;
    }

    D3DX10_IMAGE_LOAD_INFO tempLoadInfo;
    tempLoadInfo.FirstMipLevel  = 0;
    // a full mipmap chain will be created.
    tempLoadInfo.MipLevels = D3DX10_DEFAULT;
    tempLoadInfo.Usage = eUsage;
    tempLoadInfo.BindFlags = eBindFlag;
    tempLoadInfo.CpuAccessFlags = 0;
    tempLoadInfo.MiscFlags = 0;
    tempLoadInfo.Format = eDXGIFMT;
    tempLoadInfo.Filter = D3DX10_FILTER_NONE;
    tempLoadInfo.MipFilter = D3DX10_FILTER_NONE;
    tempLoadInfo.pSrcInfo = 0;

    switch( eSType )
    {
    case SamplerInformation::SAMPLER_1D:
    {
        tempLoadInfo.Width = pImage->GetBound(0);
        tempLoadInfo.Height = 1;
        tempLoadInfo.Depth = 0;

        ID3D10Resource* pDX10Texture;

        ms_hResult = D3DX10CreateTextureFromMemory(m_pDX10Device, aucRSrc,
            iByteCount, &tempLoadInfo, 0, &pDX10Texture, 0);
        SE_ASSERT( SUCCEEDED(ms_hResult) );

        pResource->ID = pDX10Texture;
        break;
    }
    case SamplerInformation::SAMPLER_2D:
    case SamplerInformation::SAMPLER_PROJ:
    {
        tempLoadInfo.Width = pImage->GetBound(0);
        tempLoadInfo.Height = pImage->GetBound(1);
        tempLoadInfo.Depth = 0;

        ID3D10Resource* pDX10Texture;

        ms_hResult = D3DX10CreateTextureFromMemory(m_pDX10Device, aucRSrc,
            iByteCount, &tempLoadInfo, 0, &pDX10Texture, 0);
        SE_ASSERT( SUCCEEDED(ms_hResult) );

        pResource->ID = pDX10Texture;
        break;
    }
    case SamplerInformation::SAMPLER_CUBE:
    {
        // ��ʵ��.
        SE_ASSERT( false );

        break;
    }
    case SamplerInformation::SAMPLER_3D:
    {
        // ��ʵ��.
        SE_ASSERT( false );

        break;
    }
    default:
        SE_ASSERT( false );

        break;
    }

    SE_DELETE[] aucRSrc;
}
//----------------------------------------------------------------------------
void DX10Renderer::OnReleaseTexture(ResourceIdentifier* pID)
{
    TextureID* pResource = (TextureID*)pID;
    pResource->ID->Release();
    SE_DELETE pResource;
}
//----------------------------------------------------------------------------
void DX10Renderer::OnLoadVBuffer(ResourceIdentifier*& rpID,
    const Attributes& rIAttr, const Attributes& rOAttr,
    VertexBuffer* pVBuffer, VertexProgram* pVProgram)
{
    VBufferID* pResource = SE_NEW VBufferID;
    rpID = pResource;
    pResource->IAttr = rIAttr;
    pResource->OAttr = rOAttr;

    // ���ڴ���ID3D10InputLayout�ӿ�.
    std::vector<D3D10_INPUT_ELEMENT_DESC*> tempDescs;

    // vertex buffer����߱�vertex position.
    D3D10_INPUT_ELEMENT_DESC* pTempDesc = SE_NEW D3D10_INPUT_ELEMENT_DESC;
    pTempDesc->SemanticName = ms_PositionStr.c_str();
    pTempDesc->SemanticIndex = 0;
    pTempDesc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
    pTempDesc->InputSlot = 0;
    pTempDesc->AlignedByteOffset = 0;
    pTempDesc->InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
    pTempDesc->InstanceDataStepRate = 0;
    tempDescs.push_back(pTempDesc);
    int iVertexSize = 3*sizeof(float);

    if( rIAttr.HasNormal() )
    {
        pTempDesc = SE_NEW D3D10_INPUT_ELEMENT_DESC;
        pTempDesc->SemanticName = ms_NormalStr.c_str();
        pTempDesc->SemanticIndex = 0;
        pTempDesc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
        pTempDesc->InputSlot = 0;
        pTempDesc->AlignedByteOffset = iVertexSize;
        pTempDesc->InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
        pTempDesc->InstanceDataStepRate = 0;
        tempDescs.push_back(pTempDesc);
        iVertexSize += 3*sizeof(float);
    }

    if( rIAttr.HasColor(0) )
    {
        pTempDesc = SE_NEW D3D10_INPUT_ELEMENT_DESC;
        pTempDesc->SemanticName = ms_ColorStr.c_str();
        pTempDesc->SemanticIndex = 0;
        pTempDesc->InputSlot = 0;
        pTempDesc->AlignedByteOffset = iVertexSize;
        pTempDesc->InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
        pTempDesc->InstanceDataStepRate = 0;

        switch( rIAttr.GetColorChannels(0) )
        {
        case 1:
            pTempDesc->Format = DXGI_FORMAT_R32_FLOAT;
            iVertexSize += sizeof(float);
            break;
        case 2:
            pTempDesc->Format = DXGI_FORMAT_R32G32_FLOAT;
            iVertexSize += 2*sizeof(float);
            break;
        case 3:
            pTempDesc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
            iVertexSize += 3*sizeof(float);
            break;
        case 4:
            pTempDesc->Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            iVertexSize += 4*sizeof(float);
            break;
        }

        tempDescs.push_back(pTempDesc);
    }

    if( rIAttr.HasColor(1) )
    {
        pTempDesc = SE_NEW D3D10_INPUT_ELEMENT_DESC;
        pTempDesc->SemanticName = ms_ColorStr.c_str();
        pTempDesc->SemanticIndex = 1;
        pTempDesc->InputSlot = 0;
        pTempDesc->AlignedByteOffset = iVertexSize;
        pTempDesc->InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
        pTempDesc->InstanceDataStepRate = 0;

        switch( rIAttr.GetColorChannels(1) )
        {
        case 1:
            pTempDesc->Format = DXGI_FORMAT_R32_FLOAT;
            iVertexSize += sizeof(float);
            break;
        case 2:
            pTempDesc->Format = DXGI_FORMAT_R32G32_FLOAT;
            iVertexSize += 2*sizeof(float);
            break;
        case 3:
            pTempDesc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
            iVertexSize += 3*sizeof(float);
            break;
        case 4:
            pTempDesc->Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            iVertexSize += 4*sizeof(float);
            break;
        }

        tempDescs.push_back(pTempDesc);
    }

    int iUnit;
    for( iUnit = 0; iUnit < rIAttr.GetMaxTCoords(); iUnit++ )
    {
        if( rIAttr.HasTCoord(iUnit) )
        {
            pTempDesc = SE_NEW D3D10_INPUT_ELEMENT_DESC;
            pTempDesc->SemanticName = ms_TexCoordStr.c_str();
            pTempDesc->SemanticIndex = iUnit;
            pTempDesc->InputSlot = 0;
            pTempDesc->AlignedByteOffset = iVertexSize;
            pTempDesc->InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
            pTempDesc->InstanceDataStepRate = 0;

            switch( rIAttr.GetTCoordChannels(iUnit) )
            {
            case 1:
                pTempDesc->Format = DXGI_FORMAT_R32_FLOAT;
                iVertexSize += sizeof(float);
                break;
            case 2:
                pTempDesc->Format = DXGI_FORMAT_R32G32_FLOAT;
                iVertexSize += 2*sizeof(float);
                break;
            case 3:
                pTempDesc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
                iVertexSize += 3*sizeof(float);
                break;
            case 4:
                pTempDesc->Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
                iVertexSize += 4*sizeof(float);
                break;
            }

            tempDescs.push_back(pTempDesc);
        }
    }

    // ����input layout.
    int iElementCount = (int)tempDescs.size();
    D3D10_INPUT_ELEMENT_DESC* pIEDescs = 
        SE_NEW D3D10_INPUT_ELEMENT_DESC[iElementCount];
    for( int i = 0; i < iElementCount; i++ )
    {
        pIEDescs[i] = *tempDescs[i];
    }

    ID3D10Blob* pVShaderBuffer = cgD3D10GetCompiledProgram(
        pVProgram->GetProgram());
    SE_ASSERT( pVShaderBuffer );

    ID3D10InputLayout* pDX10Layout;
    ms_hResult = m_pDX10Device->CreateInputLayout(pIEDescs, iElementCount, 
        pVShaderBuffer->GetBufferPointer(), pVShaderBuffer->GetBufferSize(),
        &pDX10Layout);
    SE_ASSERT( SUCCEEDED(ms_hResult) );

    SE_DELETE[] pIEDescs;
    for( int i = 0; i < iElementCount; i++ )
    {
        SE_DELETE tempDescs[i];
    }

    int iChannels;
    float* afCompatible = 0;  // ��BuildCompatibleArray������������
    pVBuffer->BuildCompatibleArray(rIAttr, false, iChannels, afCompatible);

    // ����vertex buffer.
    unsigned int uiVBSize = (unsigned int)(iChannels*sizeof(float));
    D3D10_BUFFER_DESC tempVBDesc;
    tempVBDesc.Usage = D3D10_USAGE_DEFAULT;
    tempVBDesc.ByteWidth = uiVBSize;
    tempVBDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    tempVBDesc.CPUAccessFlags = 0;
    tempVBDesc.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA tempInitData;
    tempInitData.pSysMem = afCompatible;

    ID3D10Buffer* pDX10VBuffer;
    ms_hResult = m_pDX10Device->CreateBuffer(&tempVBDesc, &tempInitData, 
        &pDX10VBuffer);
    SE_ASSERT( SUCCEEDED(ms_hResult) );

    SE_DELETE[] afCompatible;

    // ��������Ϣ������.
    pResource->ID = pDX10VBuffer;
    pResource->VertexSize = iVertexSize;
    pResource->Offset = 0;
    pResource->Layout = pDX10Layout;
}
//----------------------------------------------------------------------------
void DX10Renderer::OnReleaseVBuffer(ResourceIdentifier* pID)
{
    VBufferID* pResource = (VBufferID*)pID;
    pResource->ID->Release();
    pResource->Layout->Release();
    SE_DELETE pResource;
}
//----------------------------------------------------------------------------
void DX10Renderer::OnLoadIBuffer(ResourceIdentifier*& rpID, IndexBuffer* pIBuffer)
{
    IBufferID* pResource = SE_NEW IBufferID;
    rpID = pResource;

    int iICount = pIBuffer->GetIndexCount();
    int* aiIndex = pIBuffer->GetData();
    unsigned int uiIBSize = (unsigned int)(iICount*sizeof(int));

    // ����index buffer.
    D3D10_BUFFER_DESC tempIBDesc;
    tempIBDesc.Usage = D3D10_USAGE_DEFAULT;
    tempIBDesc.ByteWidth = uiIBSize;
    tempIBDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
    tempIBDesc.CPUAccessFlags = 0;
    tempIBDesc.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA tempInitData;
    tempInitData.pSysMem = aiIndex;

    ID3D10Buffer* pDX10IBuffer;
    ms_hResult = m_pDX10Device->CreateBuffer(&tempIBDesc, &tempInitData,
        &pDX10IBuffer);
    SE_ASSERT( SUCCEEDED(ms_hResult) );

    // ��������Ϣ������.
    pResource->ID = pDX10IBuffer;
}
//----------------------------------------------------------------------------
void DX10Renderer::OnReleaseIBuffer(ResourceIdentifier* pID)
{
    IBufferID* pResource = (IBufferID*)pID;
    pResource->ID->Release();
    SE_DELETE pResource;
}
//----------------------------------------------------------------------------
void DX10Renderer::OnLoadRenderStateBlock(ResourceIdentifier*& rpID,
    RenderStateBlock* pRStateBlock)
{
    RStateBlockID* pResource = SE_NEW RStateBlockID;
    pResource->RStateBlock = pRStateBlock;
    rpID = pResource;

    // ����ID3D10BlendState����.
    ID3D10BlendState* pDX10BState = 0;
    GenerateBlendState(pRStateBlock, pDX10BState);

    // ����ID3D10DepthStencilState����.
    ID3D10DepthStencilState* pDX10DSState = 0;
    GenerateDepthStencilState(pRStateBlock, pDX10DSState);

    // ����ID3D10RasterizerState����.
    ID3D10RasterizerState* pDX10RState = 0;
    GenerateRasterizerState(pRStateBlock, pDX10RState);

    // ��������Ϣ������.
    pResource->BlendState = pDX10BState;
    pResource->DepthStencilState = pDX10DSState;
    pResource->RasterizerState = pDX10RState;
}
//----------------------------------------------------------------------------
void DX10Renderer::OnReleaseRenderStateBlock(ResourceIdentifier* pID)
{
    RStateBlockID* pResource = (RStateBlockID*)pID;
    SE_DX10_SAFE_RELEASE(pResource->BlendState);
    SE_DX10_SAFE_RELEASE(pResource->DepthStencilState);
    SE_DX10_SAFE_RELEASE(pResource->RasterizerState);
    SE_DELETE pResource;
}
//----------------------------------------------------------------------------