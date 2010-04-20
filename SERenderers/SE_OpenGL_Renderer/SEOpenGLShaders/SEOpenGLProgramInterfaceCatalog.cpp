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

#include "SEOpenGLRendererPCH.h"
#include "SEOpenGLProgramInterfaceCatalog.h"
#include "SEOpenGLProgramInterface.h"

using namespace Swing;

SEOpenGLProgramInterfaceCatalog* SEOpenGLProgramInterfaceCatalog::ms_pActive 
    = 0;

//----------------------------------------------------------------------------
SEOpenGLProgramInterfaceCatalog::SEOpenGLProgramInterfaceCatalog(
    const std::string& rName)
    :
    m_Name(rName),
    m_Entry(PROGRAM_MAP_SIZE)
{
    m_pRenderer = 0;
}
//----------------------------------------------------------------------------
SEOpenGLProgramInterfaceCatalog::~SEOpenGLProgramInterfaceCatalog()
{
}
//----------------------------------------------------------------------------
void SEOpenGLProgramInterfaceCatalog::SetRenderer(SEOpenGLRenderer* pRenderer)
{
    m_pRenderer = pRenderer;
}
//----------------------------------------------------------------------------
const std::string& SEOpenGLProgramInterfaceCatalog::GetName() const
{
    return m_Name;
}
//----------------------------------------------------------------------------
bool SEOpenGLProgramInterfaceCatalog::Insert(
    SEOpenGLProgramInterface* pProgramInterface)
{
    if( !pProgramInterface )
    {
        SE_ASSERT( false );

        return false;
    }

    std::string tempKey(pProgramInterface->GetName());

    // ��������ԴĿ¼�в���
    SEOpenGLProgramInterface** ppTempProgramInterface = 
        m_Entry.Find(tempKey);
    if( ppTempProgramInterface )
    {
        // �ó���ӿ��Ѿ�����
        return true;
    }

    // �ó���ӿڲ�����,�����
    m_Entry.Insert(tempKey, pProgramInterface);

    return true;
}
//----------------------------------------------------------------------------
bool SEOpenGLProgramInterfaceCatalog::Remove(
    SEOpenGLProgramInterface* pProgramInterface)
{
    if( !pProgramInterface )
    {
        SE_ASSERT( false );

        return false;
    }

    std::string tempKey(pProgramInterface->GetName());

    // ��������ԴĿ¼�в���
    SEOpenGLProgramInterface** ppTempProgramInterface = 
        m_Entry.Find(tempKey);
    if( !ppTempProgramInterface )
    {
        // �ó���ӿڲ�����
        return false;
    }

    // ����ӿڴ���,���Ƴ�
	m_Entry.Remove(tempKey);

    return true;
}
//----------------------------------------------------------------------------
SEOpenGLProgramInterface* SEOpenGLProgramInterfaceCatalog::Find(
    CGprogram hCgProgram, const std::string& rPInterfaceName)
{
    // ��������ԴĿ¼�в���
    SEOpenGLProgramInterface** ppTempProgramInterface = 
        m_Entry.Find(rPInterfaceName);
    if( ppTempProgramInterface )
    {
        // �ҵ��򷵻�
        return *ppTempProgramInterface;
    }

    // �ڴ����в���
    SE_ASSERT( m_pRenderer );
    SEOpenGLProgramInterface* pProgramInterface = 
        SEOpenGLProgramInterface::Load(hCgProgram, m_pRenderer->GetCgContext(), 
        rPInterfaceName);
    if( pProgramInterface )
    {
        // ����Դ����,���Ѿ���Load�󱻼�����ԴĿ¼,�����ٴε���Insert����
        return pProgramInterface;
    }

    return 0;
}
//----------------------------------------------------------------------------
bool SEOpenGLProgramInterfaceCatalog::PrintContents(const std::string& 
    rFileName) const
{
    const char* pDecorated = SESystem::SE_GetPath(rFileName.c_str(), 
        SESystem::SM_WRITE);

    if( pDecorated )
    {
        std::ofstream OStream(pDecorated);

        SE_ASSERT( OStream );

        std::string tempKey;
		SEOpenGLProgramInterface** ppTempProgramInterface = m_Entry.GetFirst(
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
void SEOpenGLProgramInterfaceCatalog::SetActive(
    SEOpenGLProgramInterfaceCatalog* pActive)
{
    ms_pActive = pActive;
}
//----------------------------------------------------------------------------
SEOpenGLProgramInterfaceCatalog* SEOpenGLProgramInterfaceCatalog::GetActive()
{
    return ms_pActive;
}
//----------------------------------------------------------------------------
