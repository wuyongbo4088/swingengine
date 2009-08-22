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
#include "SEImageCatalog.h"
#include "SEImage.h"

using namespace Swing;

const String ImageCatalog::ms_NullString("");
const String ImageCatalog::ms_DefaultString("Default");
ImageCatalog* ImageCatalog::ms_pActive = 0;

//----------------------------------------------------------------------------
ImageCatalog::ImageCatalog(const String& rName)
    :
    m_Name(rName),
    m_Entry(IMAGE_MAP_SIZE)
{
    // 由于有些图形管线不支持1D image,
    // 因此创建一个默认2x2 image,用于无法找到指定资源时提醒用户.
    unsigned char* pData = SE_NEW unsigned char[16];
    pData[0] = 0;
    pData[1] = 255;
    pData[2] = 255;
    pData[3] = 255;
    pData[4] = 255;
    pData[5] = 0;
    pData[6] = 255;
    pData[7] = 255;
    pData[8] = 255;
    pData[9] = 255;
    pData[10] = 0;
    pData[11] = 255;
    pData[12] = 255;
    pData[13] = 255;
    pData[14] = 255;
    pData[15] = 255;

    m_spDefaultImage = SE_NEW Image(Image::IT_RGBA8888, 2, 2, pData,
        ms_DefaultString);
}
//----------------------------------------------------------------------------
ImageCatalog::~ImageCatalog()
{
}
//----------------------------------------------------------------------------
const String& ImageCatalog::GetName() const
{
    return m_Name;
}
//----------------------------------------------------------------------------
bool ImageCatalog::Insert(Image* pImage)
{
    if( !pImage )
    {
        SE_ASSERT( false );

        return false;
    }

    String StrImageName(pImage->GetName());
    if( StrImageName == ms_NullString
    ||  StrImageName == ms_DefaultString
    ||  pImage == m_spDefaultImage )
    {
        return false;
    }

    // 首先在资源目录中查找
    Image** ppTempImage = m_Entry.Find(StrImageName);
    if( ppTempImage )
    {
        // 该image已经存在
        return true;
    }

    // 该image不存在,则插入
    m_Entry.Insert(StrImageName, pImage);

    return true;
}
//----------------------------------------------------------------------------
bool ImageCatalog::Remove(Image* pImage)
{
    if( !pImage )
    {
        SE_ASSERT( false );

        return false;
    }

    String StrImageName(pImage->GetName());
    if( StrImageName == ms_NullString
    ||  StrImageName == ms_DefaultString
    ||  pImage == m_spDefaultImage )
    {
        return false;
    }

    // 首先在资源目录中查找
    Image** ppTempImage = m_Entry.Find(StrImageName);
    if( !ppTempImage )
    {
        // 该image不存在
        return false;
    }

    // image存在,则移除
    m_Entry.Remove(StrImageName);

    return true;
}
//----------------------------------------------------------------------------
Image* ImageCatalog::Find(const String& rImageName)
{
    if( rImageName == ms_NullString 
    ||  rImageName == ms_DefaultString )
    {
        return StaticCast<Image>(m_spDefaultImage);
    }

    // 首先在资源目录中查找
    Image** ppTempImage = m_Entry.Find(rImageName);
    if( ppTempImage )
    {
        // 找到则返回
        return *ppTempImage;
    }

    // 在磁盘中查找
    Image* pImage = Image::Load(rImageName);
    if( pImage )
    {
        // 该资源存在,且已经在Load后被加入资源目录,不用再次调用Insert函数
        return pImage;
    }

    // image不存在,则使用默认image
    return StaticCast<Image>(m_spDefaultImage);
}
//----------------------------------------------------------------------------
bool ImageCatalog::PrintContents(const String& rFileName) const
{
    const char* pDecorated = System::SE_GetPath(rFileName, System::SM_WRITE);

    if( pDecorated )
    {
        std::ofstream OStream(pDecorated);

        SE_ASSERT( OStream );

        String StrImageName;
		Image** ppTempImage = m_Entry.GetFirst(&StrImageName);
        while( ppTempImage )
        {
            Image* pImage = *ppTempImage;
            OStream << (const char*)StrImageName << ":" << std::endl;
            OStream << "    dimension = " << pImage->GetDimension()
                << std::endl;
            for( int i = 0; i < pImage->GetDimension(); i++ )
            {
                OStream << "    bound(" << i << ") = " << pImage->GetBound(i)
                    << std::endl;
            }
            OStream << "    format = " << (const char*)(pImage->GetFormatName()) << std::endl;
            OStream << std::endl;
			ppTempImage = m_Entry.GetNext(&StrImageName);
        }
        OStream.close();

        return true;
    }

    return false;
}
//----------------------------------------------------------------------------
void ImageCatalog::SetActive(ImageCatalog* pActive)
{
    ms_pActive = pActive;
}
//----------------------------------------------------------------------------
ImageCatalog* ImageCatalog::GetActive()
{
    return ms_pActive;
}
//----------------------------------------------------------------------------
