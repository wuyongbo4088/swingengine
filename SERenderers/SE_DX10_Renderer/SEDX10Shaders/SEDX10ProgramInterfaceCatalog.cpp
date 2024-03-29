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
#include "SEDX10ProgramInterfaceCatalog.h"
#include "SEDX10ProgramInterface.h"

using namespace Swing;

SEDX10ProgramInterfaceCatalog* SEDX10ProgramInterfaceCatalog::ms_pActive = 0;

//----------------------------------------------------------------------------
SEDX10ProgramInterfaceCatalog::SEDX10ProgramInterfaceCatalog(
    const std::string& rName)
    :
    m_Name(rName),
    m_Entry(PROGRAM_MAP_SIZE)
{
    m_pRenderer = 0;
}
//----------------------------------------------------------------------------
SEDX10ProgramInterfaceCatalog::~SEDX10ProgramInterfaceCatalog()
{
}
//----------------------------------------------------------------------------
void SEDX10ProgramInterfaceCatalog::SetRenderer(SEDX10Renderer* pRenderer)
{
    m_pRenderer = pRenderer;
}
//----------------------------------------------------------------------------
const std::string& SEDX10ProgramInterfaceCatalog::GetName() const
{
    return m_Name;
}
//----------------------------------------------------------------------------
bool SEDX10ProgramInterfaceCatalog::Insert(
    SEDX10ProgramInterface* pProgramInterface)
{
    if( !pProgramInterface )
    {
        SE_ASSERT( false );

        return false;
    }

    std::string tempKey(pProgramInterface->GetName());

    // 首先在资源目录中查找
    SEDX10ProgramInterface** ppTempProgramInterface = 
        m_Entry.Find(tempKey);
    if( ppTempProgramInterface )
    {
        // 该程序接口已经存在
        return true;
    }

    // 该程序接口不存在,则插入
    m_Entry.Insert(tempKey, pProgramInterface);

    return true;
}
//----------------------------------------------------------------------------
bool SEDX10ProgramInterfaceCatalog::Remove(
    SEDX10ProgramInterface* pProgramInterface)
{
    if( !pProgramInterface )
    {
        SE_ASSERT( false );

        return false;
    }

    std::string tempKey(pProgramInterface->GetName());

    // 首先在资源目录中查找
    SEDX10ProgramInterface** ppTempProgramInterface = 
        m_Entry.Find(tempKey);
    if( !ppTempProgramInterface )
    {
        // 该程序接口不存在
        return false;
    }

    // 程序接口存在,则移除
	m_Entry.Remove(tempKey);

    return true;
}
//----------------------------------------------------------------------------
SEDX10ProgramInterface* SEDX10ProgramInterfaceCatalog::Find(
    CGprogram hCgProgram, const std::string& rPInterfaceName)
{
    // 首先在资源目录中查找
    SEDX10ProgramInterface** ppTempProgramInterface = 
        m_Entry.Find(rPInterfaceName);
    if( ppTempProgramInterface )
    {
        // 找到则返回
        return *ppTempProgramInterface;
    }

    // 在磁盘中查找
    SEDX10ProgramInterface* pProgramInterface = SEDX10ProgramInterface::Load(
        hCgProgram, m_pRenderer->GetCgContext(), rPInterfaceName);
    if( pProgramInterface )
    {
        // 该资源存在,且已经在Load后被加入资源目录,不用再次调用Insert函数
        return pProgramInterface;
    }

    return 0;
}
//----------------------------------------------------------------------------
bool SEDX10ProgramInterfaceCatalog::PrintContents(const std::string& 
    rFileName) const
{
    const char* pDecorated = SESystem::SE_GetPath(rFileName.c_str(), 
        SESystem::SM_WRITE);

    if( pDecorated )
    {
        std::ofstream OStream(pDecorated);

        SE_ASSERT( OStream );

        std::string tempKey;
		SEDX10ProgramInterface** ppTempProgramInterface = m_Entry.GetFirst(
            &tempKey);
        while( ppTempProgramInterface )
        {
            OStream << tempKey.c_str() << std::endl;
            OStream << std::endl;
            ppTempProgramInterface = m_Entry.GetNext(&tempKey);
        }
        OStream.close();

        return true;
    }

    return false;
}
//----------------------------------------------------------------------------
void SEDX10ProgramInterfaceCatalog::SetActive(
    SEDX10ProgramInterfaceCatalog* pActive)
{
    ms_pActive = pActive;
}
//----------------------------------------------------------------------------
SEDX10ProgramInterfaceCatalog* SEDX10ProgramInterfaceCatalog::GetActive()
{
    return ms_pActive;
}
//----------------------------------------------------------------------------
