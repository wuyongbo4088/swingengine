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
SEImageCatalog* SEMain::ms_pImageCatalog = 0;
SEVertexProgramCatalog* SEMain::ms_pVertexProgramCatalog = 0;
SEGeometryProgramCatalog* SEMain::ms_pGeometryProgramCatalog = 0;
SEPixelProgramCatalog* SEMain::ms_pPixelProgramCatalog = 0;

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

    // pre-main之前不允许创建object
    if( SEObject::InUse )
    {
        bCountIsCorrect = false;
        SEObject::PrintInUse("AppLog.txt", 
            "Objects were created before pre-main initialization");
    }
    SE_ASSERT( bCountIsCorrect );

    ms_pImageCatalog = SE_NEW SEImageCatalog("SEMain");
    SEImageCatalog::SetActive(ms_pImageCatalog);

    ms_pVertexProgramCatalog = SE_NEW SEVertexProgramCatalog("SEMain");
    SEVertexProgramCatalog::SetActive(ms_pVertexProgramCatalog);

    ms_pGeometryProgramCatalog = SE_NEW SEGeometryProgramCatalog("SEMain");
    SEGeometryProgramCatalog::SetActive(ms_pGeometryProgramCatalog);

    ms_pPixelProgramCatalog = SE_NEW SEPixelProgramCatalog("SEMain");
    SEPixelProgramCatalog::SetActive(ms_pPixelProgramCatalog);

    // 调用各个类型的pre-main函数
    if( ms_pInitializers )
    {
        for( int i = 0; i < (int)ms_pInitializers->size(); i++ )
        {
            (*ms_pInitializers)[i]();
        }
    }

    // 不再需要全局pre-main函数指针数组了
    SE_DELETE ms_pInitializers;
    ms_pInitializers = 0;

    // 在pre-main后创建的object数目
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

    // 所有应用程序运行期间创建的object现在应该已经被delete
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

    // 调用各个类型的post-main函数
    if( ms_pTerminators )
    {
        int iCount = (int)ms_pTerminators->size();
        for( int i = 0; i < iCount; i++ )
        {
            (*ms_pTerminators)[i]();
        }
    }

    // 不再需要全局post-main函数指针数组了
    SE_DELETE ms_pTerminators;
    ms_pTerminators = 0;

    if( SEPixelProgramCatalog::GetActive() == ms_pPixelProgramCatalog )
    {
        SEPixelProgramCatalog::SetActive(0);
    }
    SE_DELETE ms_pPixelProgramCatalog;

    if( SEGeometryProgramCatalog::GetActive() == ms_pGeometryProgramCatalog )
    {
        SEGeometryProgramCatalog::SetActive(0);
    }
    SE_DELETE ms_pGeometryProgramCatalog;

    if( SEVertexProgramCatalog::GetActive() == ms_pVertexProgramCatalog )
    {
        SEVertexProgramCatalog::SetActive(0);
    }
    SE_DELETE ms_pVertexProgramCatalog;

    if( SEImageCatalog::GetActive() == ms_pImageCatalog )
    {
        SEImageCatalog::SetActive(0);
    }
    SE_DELETE ms_pImageCatalog;

    if( bCountIsCorrect )
    {
        // 所有在pre-main期间创建的object都应该在post-main期间释放
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

    // object级泄漏检测系统已经完成任务,
    // 释放该系统从而避免内存泄漏检测系统发现内存泄漏
    SE_DELETE SEObject::InUse;
    SEObject::InUse = 0;
}
//----------------------------------------------------------------------------
