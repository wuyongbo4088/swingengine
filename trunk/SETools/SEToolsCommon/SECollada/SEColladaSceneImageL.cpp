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

#include "SEToolsCommonPCH.h"
#include "SEColladaScene.h"

using namespace Swing;

//----------------------------------------------------------------------------
Image* ColladaScene::GetImage(const char* acName)
{
    if( !acName )
    {
        return 0;
    }

    for( int i = 0; i < (int)m_Images.size(); i++ )
    {
        if( strcmp(m_Images[i]->GetName(), acName) == 0 )
        {
            return m_Images[i];
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
bool ColladaScene::LoadImageLibrary(domLibrary_imagesRef spLib)
{
    ToolSystem::SE_DebugOutput("ColladaScene::Loading Image Library" );

    int iImageCount = (int)spLib->getImage_array().getCount();
    for( int i = 0; i < iImageCount; i++ )
    {
        LoadImage(spLib->getImage_array()[i]);
    }

    return true;
}
//----------------------------------------------------------------------------
Image* ColladaScene::LoadImage(domImageRef spDomImage)
{
    if( !spDomImage )
    {
        return 0;
    }

    xsID strImageID = spDomImage->getId();
    if( !strImageID )
    {
        return 0;
    }

    Image* pImage = GetImage((const char*)strImageID);
    if( pImage )
    {
        // This image is already in our image catalog.
        return pImage;
    }

    //ToolSystem::SE_DebugOutput("Add new image %s", strImageID);

    domImage* pDomImage = (domImage*)spDomImage;
    if( pDomImage )
    {
        xsAnyURI tempImageFileURI = pDomImage->getInit_from()->getValue();
        std::string strImageFileName;
        strImageFileName = cdom::uriToFilePath(tempImageFileURI.str());

        // Load the actual image by image file name.
        pImage = m_pImageConverter->CreateImageFromFile(
            strImageFileName.c_str());

        if( pImage )
        {
            pImage->SetName((const char*)strImageID);
            m_Images.push_back(pImage);

            return pImage;
        }
    }

    return 0; 	
}
//----------------------------------------------------------------------------