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

#include "SEDX9ApplicationPCH.h"
#include "SEApplication.h"
#include "SEDX9RendererRegister.h"
#include "SEAudioRegister.h"
#include "SEOpenALRendererRegister.h"

using namespace Swing;

SEApplication::EntryPoint SEApplication::Run = 0;
SECommand* SEApplication::TheCommand = 0;
SEApplication* SEApplication::TheApplication = 0;

//----------------------------------------------------------------------------
SEApplication::SEApplication()
{
    memset(m_acExtraData, 0, APP_EXTRA_DATA_COUNT*sizeof(char));
    m_bLaunchFileDialog = false;
}
//----------------------------------------------------------------------------
SEApplication::~SEApplication()
{
}
//----------------------------------------------------------------------------
void SEApplication::SetExtraData(int iIndex, int iSize, const void* pvData)
{
    if( 0 <= iIndex && iIndex < APP_EXTRA_DATA_COUNT - iSize )
    {
        SESystem::SE_Memcpy(m_acExtraData, APP_EXTRA_DATA_COUNT, pvData, 
            iSize);
    }
}
//----------------------------------------------------------------------------
void SEApplication::GetExtraData(int iIndex, int iSize, void* pvData) const
{
    if( 0 <= iIndex && iIndex < APP_EXTRA_DATA_COUNT - iSize )
    {
        SESystem::SE_Memcpy(pvData, APP_EXTRA_DATA_COUNT, m_acExtraData, 
            iSize);
    }
}
//----------------------------------------------------------------------------
bool SEApplication::LaunchFileDialog() const
{
    return m_bLaunchFileDialog;
}
//----------------------------------------------------------------------------
void SEApplication::TestStreaming(SESpatial* pScene, int iXPos, int iYPos,
    int iXSize, int iYSize, const char* acFilename)
{
    SEStream tempOStream;
    tempOStream.Insert(pScene);
    tempOStream.Save(acFilename);

    SEStream tempIStream;
    tempIStream.Load(acFilename);
    SESpatialPtr spScene = StaticCast<SESpatial>(tempIStream.GetObjectAt(0));
    spScene->SetName(acFilename);

    LaunchTreeControl(spScene, iXPos, iYPos, iXSize, iYSize);
}
//----------------------------------------------------------------------------
int main(int iArgCount, char* apcArgument[])
{
    // ����Register.
    SE_Foundation_Register();
    SE_Audio_Register();
    SE_DX9Renderer_Register();
    SE_OpenALRenderer_Register();

    // ��Ҫ�ֶ�����SE_PATH�����������,ָ��������Դ�ļ�������λ��.
    SESystem::SE_Initialize();
    SE_ASSERT( SESystem::SE_PATH[0] );
    std::string tempSEPath(SESystem::SE_PATH);

    SEMain::Initialize();

    int iExitCode = 0;
    if( SEApplication::Run )
    {
        // ���Ǽ�鵱ǰ����Ŀ¼.
        SESystem::SE_InsertDirectory(".");

        // scene graph�ļ���·��.
        std::string tempDir;
        tempDir = tempSEPath + std::string("/Data/seof");
        SESystem::SE_InsertDirectory(tempDir.c_str());

        // texture image�ļ���·��.
        tempDir = tempSEPath + std::string("/Data/seif");
        SESystem::SE_InsertDirectory(tempDir.c_str());

        // shader program�ļ���·��.
        tempDir = tempSEPath + std::string("/Data/sesp/Cg");
        SESystem::SE_InsertDirectory(tempDir.c_str());

        // wave�ļ���·��.
        tempDir = tempSEPath + std::string("/Data/sewf");
        SESystem::SE_InsertDirectory(tempDir.c_str());

        // ����image�ļ�·��.
        tempDir = tempSEPath + std::string("/Data/Im");
        SESystem::SE_InsertDirectory(tempDir.c_str());

        SEApplication::TheCommand = SE_NEW SECommand(iArgCount, apcArgument);
        iExitCode = SEApplication::Run(iArgCount, apcArgument);
        SE_DELETE SEApplication::TheCommand;
        SEApplication::TheCommand = 0;

        SESystem::SE_RemoveAllDirectories();
    }
    else
    {
        iExitCode = INT_MAX;
    }

    SEMain::Terminate();

    SE_DELETE SEApplication::TheApplication;
    SEApplication::TheApplication = 0;

    SESystem::SE_Terminate();

#ifdef SE_MEMORY_MANAGER
#ifdef _DEBUG
    Memory::GenerateReport("MemoryReportDebug.txt");
#else
    Memory::GenerateReport("MemoryReportRelease.txt");
#endif
#endif

    return iExitCode;
}
//----------------------------------------------------------------------------
