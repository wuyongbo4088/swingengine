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
#include "SEMain.h"
#include "SEObject.h"
#include "SEImageCatalog.h"
#include "SEVertexProgramCatalog.h"
#include "SEGeometryProgramCatalog.h"
#include "SEPixelProgramCatalog.h"

using namespace Swing;

SEMain::InitializerArray* SEMain::ms_pInitializers = 0;
SEMain::TerminatorArray* SEMain::ms_pTerminators = 0;
int SEMain::ms_iStartObjects = 0;
int SEMain::ms_iFinalObjects = 0;
ImageCatalog* SEMain::ms_pImageCatalog = 0;
VertexProgramCatalog* SEMain::ms_pVertexProgramCatalog = 0;
GeometryProgramCatalog* SEMain::ms_pGeometryProgramCatalog = 0;
PixelProgramCatalog* SEMain::ms_pPixelProgramCatalog = 0;

//----------------------------------------------------------------------------
void SEMain::AddInitializer(Initializer FuncInitialize)
{
    if( !ms_pInitializers )
    {
        ms_pInitializers = SE_NEW InitializerArray;
    }

    ms_pInitializers->push_back(FuncInitialize);
}
//----------------------------------------------------------------------------
void SEMain::Initialize()
{
    bool bCountIsCorrect = true;

    // pre-main֮ǰ��������object
    if( SEObject::InUse )
    {
        bCountIsCorrect = false;
        SEObject::PrintInUse("AppLog.txt", 
            "Objects were created before pre-main initialization");
    }
    SE_ASSERT( bCountIsCorrect );

    ms_pImageCatalog = SE_NEW ImageCatalog("SEMain");
    ImageCatalog::SetActive(ms_pImageCatalog);

    ms_pVertexProgramCatalog = SE_NEW VertexProgramCatalog("SEMain");
    VertexProgramCatalog::SetActive(ms_pVertexProgramCatalog);

    ms_pGeometryProgramCatalog = SE_NEW GeometryProgramCatalog("SEMain");
    GeometryProgramCatalog::SetActive(ms_pGeometryProgramCatalog);

    ms_pPixelProgramCatalog = SE_NEW PixelProgramCatalog("SEMain");
    PixelProgramCatalog::SetActive(ms_pPixelProgramCatalog);

    // ���ø������͵�pre-main����
    if( ms_pInitializers )
    {
        for( int i = 0; i < (int)ms_pInitializers->size(); i++ )
        {
            (*ms_pInitializers)[i]();
        }
    }

    // ������Ҫȫ��pre-main����ָ��������
    SE_DELETE ms_pInitializers;
    ms_pInitializers = 0;

    // ��pre-main�󴴽���object��Ŀ
    if( SEObject::InUse )
    {
        ms_iStartObjects = SEObject::InUse->GetCount();
    }
    else
    {
        ms_iStartObjects = 0;
    }
}
//----------------------------------------------------------------------------
void SEMain::AddTerminator(Terminator FuncTerminate)
{
    if( !ms_pTerminators )
    {
        ms_pTerminators = SE_NEW TerminatorArray;
    }

    ms_pTerminators->push_back(FuncTerminate);
}
//----------------------------------------------------------------------------
void SEMain::Terminate()
{
    bool bCountIsCorrect = true;

    // ����Ӧ�ó��������ڼ䴴����object����Ӧ���Ѿ���delete
    if( SEObject::InUse )
    {
        ms_iFinalObjects = SEObject::InUse->GetCount();
    }
    else
    {
        ms_iFinalObjects = 0;
    }

    if( ms_iStartObjects != ms_iFinalObjects )
    {
        bCountIsCorrect = false;
        SEObject::PrintInUse("AppLog.txt", 
            "Not all objects were deleted before post-main termination");
    }

    // ���ø������͵�post-main����
    if( ms_pTerminators )
    {
        int iCount = (int)ms_pTerminators->size();
        for( int i = 0; i < iCount; i++ )
        {
            (*ms_pTerminators)[i]();
        }
    }

    // ������Ҫȫ��post-main����ָ��������
    SE_DELETE ms_pTerminators;
    ms_pTerminators = 0;

    if( PixelProgramCatalog::GetActive() == ms_pPixelProgramCatalog )
    {
        PixelProgramCatalog::SetActive(0);
    }
    SE_DELETE ms_pPixelProgramCatalog;

    if( GeometryProgramCatalog::GetActive() == ms_pGeometryProgramCatalog )
    {
        GeometryProgramCatalog::SetActive(0);
    }
    SE_DELETE ms_pGeometryProgramCatalog;

    if( VertexProgramCatalog::GetActive() == ms_pVertexProgramCatalog )
    {
        VertexProgramCatalog::SetActive(0);
    }
    SE_DELETE ms_pVertexProgramCatalog;

    if( ImageCatalog::GetActive() == ms_pImageCatalog )
    {
        ImageCatalog::SetActive(0);
    }
    SE_DELETE ms_pImageCatalog;

    if( bCountIsCorrect )
    {
        // ������pre-main�ڼ䴴����object��Ӧ����post-main�ڼ��ͷ�
        if( SEObject::InUse )
        {
            ms_iFinalObjects = SEObject::InUse->GetCount();
        }
        else
        {
            ms_iFinalObjects = 0;
        }

        if( ms_iFinalObjects != 0 )
        {
            bCountIsCorrect = false;
            SEObject::PrintInUse("AppLog.txt", 
                "Objects were deleted after post-main termination");
        }
    }

    SE_ASSERT( bCountIsCorrect );

    // object��й©���ϵͳ�Ѿ��������,
    // �ͷŸ�ϵͳ�Ӷ������ڴ�й©���ϵͳ�����ڴ�й©
    SE_DELETE SEObject::InUse;
    SEObject::InUse = 0;
}
//----------------------------------------------------------------------------
