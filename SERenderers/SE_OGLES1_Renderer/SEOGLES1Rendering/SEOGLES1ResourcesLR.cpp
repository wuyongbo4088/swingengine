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

GLenum OGLES1Renderer::ms_aeTextureMipmap[Texture::MAX_FILTER_TYPES] =
{
    GL_NEAREST,
    GL_LINEAR,
    GL_NEAREST_MIPMAP_NEAREST,
    GL_NEAREST_MIPMAP_LINEAR,
    GL_LINEAR_MIPMAP_NEAREST,
    GL_LINEAR_MIPMAP_LINEAR
};

// ע��:
// OpenGL ES1ֻ֧������������texture wrap mode,����������֧�ֵ�ģʽ,
// ��ص�������������ӳ���Ϊ0���û������α���ʹ����Щ������������.

GLenum OGLES1Renderer::ms_aeWrapMode[Texture::MAX_WRAP_TYPES] =
{
    0,
    GL_REPEAT,
    0,
    0,
    GL_CLAMP_TO_EDGE
};

// ע��:
// OpenGL ES1ֻ֧������������texture pixel format,����������֧�ֵ�ģʽ,
// ��ص�������������ӳ���Ϊ0���û������α���ʹ����Щ������������.

GLenum OGLES1Renderer::ms_aeImageComponents[Image::IT_COUNT] =
{
    GL_RGB,                    // Image::IT_RGB888
    GL_RGBA,                   // Image::IT_RGBA8888

    // ��ʵ��.��δ����.
    GL_DEPTH_COMPONENT16_OES,  // Image::IT_DEPTH16
    GL_DEPTH_COMPONENT24_OES,  // Image::IT_DEPTH24
    0,                         // Image::IT_DEPTH32
    GL_RGB,                    // Image::IT_CUBE_RGB888
    GL_RGBA,                   // Image::IT_CUBE_RGBA8888

    0,                         // Image::IT_RGB32
    0,                         // Image::IT_RGBA32

    // ��ʵ��.��δ����.
    GL_LUMINANCE,              // Image::IT_L8
    GL_LUMINANCE,              // Image::IT_L16

    0,                         // Image::IT_R32
    0,                         // Image::IT_RGB16
    0                          // Image::IT_RGBA16
};

GLenum OGLES1Renderer::ms_aeImageFormats[Image::IT_COUNT] =
{
    GL_RGB,                    // Image::IT_RGB888
    GL_RGBA,                   // Image::IT_RGBA8888

    // ��ʵ��.��δ����.
    GL_DEPTH_COMPONENT16_OES,  // Image::IT_DEPTH16
    GL_DEPTH_COMPONENT24_OES,  // Image::IT_DEPTH24
    0,                         // Image::IT_DEPTH32
    GL_RGB,                    // Image::IT_CUBE_RGB888
    GL_RGBA,                   // Image::IT_CUBE_RGBA8888

    0,                         // Image::IT_RGB32
    0,                         // Image::IT_RGBA32

    // ��ʵ��.��δ����.
    GL_LUMINANCE,              // Image::IT_L8
    GL_LUMINANCE,              // Image::IT_L16

    0,                         // Image::IT_R32
    0,                         // Image::IT_RGB16
    0                          // Image::IT_RGBA16
};

GLenum OGLES1Renderer::ms_aeImageTypes[Image::IT_COUNT] =
{
    GL_UNSIGNED_BYTE,    // Image::IT_RGB888
    GL_UNSIGNED_BYTE,    // Image::IT_RGBA8888

    // ��ʵ��.��δ����.
    GL_FLOAT,            // Image::IT_DEPTH16
    GL_FLOAT,            // Image::IT_DEPTH24
    0,                   // Image::IT_DEPTH32
    GL_UNSIGNED_BYTE,    // Image::IT_CUBE_RGB888
    GL_UNSIGNED_BYTE,    // Image::IT_CUBE_RGBA8888

    0,                   // Image::IT_RGB32
    0,                   // Image::IT_RGBA32

    // ��ʵ��.��δ����.
    GL_UNSIGNED_BYTE,    // Image::IT_L8
    GL_UNSIGNED_BYTE,    // Image::IT_L16

    0,                   // Image::IT_R32
    0,                   // Image::IT_RGB16
    0                    // Image::IT_RGBA16
};

GLenum OGLES1Renderer::ms_aeSamplerTypes[
    SamplerInformation::MAX_SAMPLER_TYPES] =
{
    0,                    // SamplerInformation::SAMPLER_1D
    GL_TEXTURE_2D,        // SamplerInformation::SAMPLER_2D
    0,                    // SamplerInformation::SAMPLER_3D
    0,                    // SamplerInformation::SAMPLER_CUBE
    GL_TEXTURE_2D,        // SamplerInformation::SAMPLER_PROJ
};

GLenum OGLES1Renderer::ms_aeDepthCompare[Texture::DC_COUNT] =
{
    GL_NEVER,
    GL_LESS,
    GL_EQUAL,
    GL_LEQUAL,
    GL_GREATER,
    GL_NOTEQUAL,
    GL_GEQUAL,
    GL_ALWAYS
};

//----------------------------------------------------------------------------
void OGLES1Renderer::OnLoadVProgram(ResourceIdentifier*&, VertexProgram*)
{
    // ��֧�ֵĻ��๦��.
}
//----------------------------------------------------------------------------
void OGLES1Renderer::OnReleaseVProgram(ResourceIdentifier*)
{
    // ��֧�ֵĻ��๦��.
}
//----------------------------------------------------------------------------
void OGLES1Renderer::OnLoadPProgram(ResourceIdentifier*&, PixelProgram*)
{
    // ��֧�ֵĻ��๦��.
}
//----------------------------------------------------------------------------
void OGLES1Renderer::OnReleasePProgram(ResourceIdentifier*)
{
    // ��֧�ֵĻ��๦��.
}
//----------------------------------------------------------------------------
void OGLES1Renderer::OnLoadTexture(ResourceIdentifier*& rpID,
    Texture* pTexture)
{
    TextureID* pResource = SE_NEW TextureID;
    pResource->TextureObject = pTexture;
    rpID = pResource;

    // Get the texture image and its information.
    const Image* pImage = pTexture->GetImage();
    SE_ASSERT( pImage );
    int iDimension = pImage->GetDimension();
    SE_ASSERT( iDimension == 2 );
    unsigned char* aucData = pImage->GetData();
    int iComponent = ms_aeImageComponents[pImage->GetFormat()];
    int eFormat = ms_aeImageFormats[pImage->GetFormat()];
    int eIType = ms_aeImageTypes[pImage->GetFormat()];

    bool bIsRegularImage = !pImage->IsCubeImage();
    int eTarget = 0;
    if( bIsRegularImage )
    {
        eTarget = ms_aeSamplerTypes[iDimension-1];
    }
    else
    {
        // ��ʵ��.
        SE_ASSERT( false );
    }

    // Generate the name and binding information.
    glGenTextures((GLsizei)1, &pResource->ID);
    glBindTexture(eTarget, pResource->ID);

    // Set the filter mode.
    Texture::FilterType eFType = pTexture->GetFilterType();
    if( eFType == Texture::NEAREST )
    {
        glTexParameteri(eTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    else
    {
        glTexParameteri(eTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    // Set the mipmap mode.
    glTexParameteri(eTarget, GL_TEXTURE_MIN_FILTER,
					ms_aeTextureMipmap[eFType]);

    // Copy the image data from system memory to video memory.
    bool bNoMip =
        (eFType == Texture::NEAREST || eFType == Texture::LINEAR);

    // OpenGL ES2 only support 2D texture and cube texture,
    // (unless there is an 3D EXT type).
    switch( iDimension )
    {
    case 2:
        if( bIsRegularImage )
        {
            glTexImage2D(eTarget, 0, iComponent, pImage->GetBound(0),
                pImage->GetBound(1), 0, eFormat, eIType, aucData);

            if( !bNoMip )
            {
                glGenerateMipmapOES(eTarget);
			}
        }
        else
        {
		    // ��ʵ��.
            // OpenGL ES1�Ƿ�֧��cube map?
            SE_ASSERT( false );
        }

        glTexParameteri(eTarget, GL_TEXTURE_WRAP_S,
            ms_aeWrapMode[pTexture->GetWrapType(0)]);
        glTexParameteri(eTarget, GL_TEXTURE_WRAP_T,
            ms_aeWrapMode[pTexture->GetWrapType(1)]);
        break;

    default:
        SE_ASSERT( false );
        break;
    }	
}
//----------------------------------------------------------------------------
void OGLES1Renderer::OnReleaseTexture(ResourceIdentifier* pID)
{
    TextureID* pResource = (TextureID*)pID;
    glDeleteTextures((GLsizei)1, (GLuint*)&pResource->ID);
    SE_DELETE pResource;
}
//----------------------------------------------------------------------------
void OGLES1Renderer::OnLoadVBuffer(ResourceIdentifier*& rpID,
    const Attributes& rIAttr, const Attributes& rOAttr,
    VertexBuffer* pVBuffer, VertexProgram*)
{
    VBufferID* pResource = SE_NEW VBufferID;
    rpID = pResource;
    pResource->IAttr = rIAttr;
    pResource->OAttr = rOAttr;

    int iChannels;
    float* afCompatible = 0;  // ��BuildCompatibleArray������������
    pVBuffer->BuildCompatibleArray(rIAttr, false, iChannels, afCompatible);

    // ����buffer id����vertex buffer.
    glGenBuffers(1, &pResource->ID);
    glBindBuffer(GL_ARRAY_BUFFER, pResource->ID);

    // ��vertex buffer���ݴ�ϵͳ�ڴ濽���Դ�.
    glBufferData(GL_ARRAY_BUFFER, iChannels*sizeof(float), afCompatible,
        GL_STATIC_DRAW);

    SE_DELETE[] afCompatible;
}
//----------------------------------------------------------------------------
void OGLES1Renderer::OnReleaseVBuffer(ResourceIdentifier* pID)
{
    VBufferID* pResource = (VBufferID*)pID;
    glDeleteBuffers(1, &pResource->ID);
    SE_DELETE pResource;
}
//----------------------------------------------------------------------------
void OGLES1Renderer::OnLoadIBuffer(ResourceIdentifier*& rpID,
    IndexBuffer* pIBuffer)
{
    IBufferID* pResource = SE_NEW IBufferID;
    rpID = pResource;

    // ����buffer id����index buffer.
    glGenBuffers(1, &pResource->ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pResource->ID);

    // ע��:	
    // OpenGL ES1ֻ֧��16λ����������,�û�������ȷ����ʹ�õ������32λ����������,
    // ��16λ��������������,Ҳ������������ķ�Χֻ��������[0,65535]��.
    // �����ʹ��ǰ����,���ǲ���ȷ����������ݴ��������ǰ�ȫ��.	
    int iICount = pIBuffer->GetIndexCount();
    size_t uiSize = iICount*sizeof(unsigned short);
    unsigned short* ausDstData = SE_NEW unsigned short[iICount];
    unsigned short* pDstData = ausDstData;
    int* pSrcData = pIBuffer->GetData();
    for( int i = 0; i < iICount; i++ )
    {
        *pDstData++ = (unsigned short)*pSrcData++;
    }

    // ��index buffer���ݴ�ϵͳ�ڴ濽���Դ�.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, uiSize, ausDstData, GL_STATIC_DRAW);

    SE_DELETE[] ausDstData;
}
//----------------------------------------------------------------------------
void OGLES1Renderer::OnReleaseIBuffer(ResourceIdentifier* pID)
{
    IBufferID* pResource = (IBufferID*)pID;
    glDeleteBuffers(1, &pResource->ID);
    SE_DELETE pResource;
}
//----------------------------------------------------------------------------
