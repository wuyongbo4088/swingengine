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
//
//----------------------------------------------------------------------------
// AngelCode Tool Box Library
// Copyright (c) 2007-2008 Andreas Jonsson
//
// This software is provided 'as-is', without any express or implied 
// warranty. In no event will the authors be held liable for any 
// damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any 
// purpose, including commercial applications, and to alter it and 
// redistribute it freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you 
//    must not claim that you wrote the original software. If you use
//    this software in a product, an acknowledgment in the product 
//    documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and 
//    must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source 
//    distribution.
//
// Andreas Jonsson
// andreas@angelcode.com
//----------------------------------------------------------------------------

#include "SEGuiPCH.h"
#include "SEBMFont.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SEBMFont, SEObject);
SE_IMPLEMENT_DEFAULT_STREAM(SEBMFont, SEObject);
SE_IMPLEMENT_DEFAULT_NAME_ID(SEBMFont, SEObject);

//SE_REGISTER_STREAM(SEBMFont);

//----------------------------------------------------------------------------
SEBMFont::SEBMFont(unsigned char* pRawData, int iDataSize)
{
    Initialize(pRawData, iDataSize);
}
//----------------------------------------------------------------------------
SEBMFont::SEBMFont()
{
    m_pData = 0;
    m_iDataSize = 0;
    m_pInfoBlock = 0;
    m_pCommonBlock = 0;
    m_pCharInfo = 0;
    m_iCharInfoCount = 0;
    m_pKerningPair = 0;
    m_ikerningPairCount = 0;
    m_iTexturePageCount = 0;
}
//----------------------------------------------------------------------------
SEBMFont::~SEBMFont()
{
    SE_DELETE[] m_pData;
}
//----------------------------------------------------------------------------
SEBMFont* SEBMFont::Load(const char*)
{
    return 0;
}
//----------------------------------------------------------------------------
void SEBMFont::Initialize(unsigned char* pRawData, int iDataSize)
{
    SE_ASSERT( pRawData && iDataSize > 0 );

    m_pData = pRawData;
    m_iDataSize = iDataSize;
    unsigned char* pData = m_pData;

    if( pData[0] == 'B' && pData[1] == 'M' && pData[2] == 'F' && 
        pData[3] == 3 )
    {
        // Skip "BMP2".
        pData += 4;

        while( pData != (m_pData + iDataSize) )
        {
            // Get block type.
            unsigned char ucBlockType = *pData++;
            int iBlockLength = *(int*)pData;
            pData += 4;

            // Process each block.
            switch( ucBlockType )
            {
            case 1:
                m_pInfoBlock = (SEBMFontInfoBlock*)(pData);
                break;

            case 2:
                m_pCommonBlock = (SEBMFontCommonBlock*)(pData);
                break;

            case 3:
                SE_ASSERT( m_pCommonBlock );

                if( m_pCommonBlock )
                {
                    m_iTexturePageCount = m_pCommonBlock->Pages;

                    char* acTextureName = (char*)pData;
                    for( int i = 0; i < m_pCommonBlock->Pages; i++ )
                    {
                        m_aTextureNames[i] = acTextureName;
                        acTextureName += strlen(acTextureName) + 1;
                    }
                }

                // Manual update pointers as this block does not move data 
                // about.
                pData += iBlockLength;
                iBlockLength = 0;
                break;

            case 4:
                m_iCharInfoCount = iBlockLength / sizeof(SEBMFontCharInfo);
                m_pCharInfo = (SEBMFontCharInfo*)(pData);
                break;

            case 5:
                m_ikerningPairCount = iBlockLength / 
                    sizeof(SEBMFontKerningPair);
                m_pKerningPair = (SEBMFontKerningPair*)(pData);
                break;

            default:
                SE_ASSERT( false );
                break;
            }

            pData += iBlockLength;
        }
    }
}
//----------------------------------------------------------------------------