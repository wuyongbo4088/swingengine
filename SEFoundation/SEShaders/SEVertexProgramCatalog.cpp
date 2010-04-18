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
#include "SEVertexProgramCatalog.h"
#include "SEVertexProgram.h"

using namespace Swing;

const std::string SEVertexProgramCatalog::ms_NullString("");
const std::string SEVertexProgramCatalog::ms_DefaultString(
    "Default.v_Default");
SEVertexProgramCatalog* SEVertexProgramCatalog::ms_pActive = 0;

//----------------------------------------------------------------------------
SEVertexProgramCatalog::SEVertexProgramCatalog(const std::string& rName)
    :
    m_Name(rName),
    m_Entry(PROGRAM_MAP_SIZE)
{
    m_pRenderer = 0;
    m_spDefaultVProgram = 0;
}
//----------------------------------------------------------------------------
SEVertexProgramCatalog::~SEVertexProgramCatalog()
{
}
//----------------------------------------------------------------------------
void SEVertexProgramCatalog::SetRenderer(SERenderer* pRenderer)
{
    m_pRenderer = pRenderer;

    if( m_pRenderer )
    {
        m_spDefaultVProgram = SEVertexProgram::Load(m_pRenderer, 
            ms_DefaultString, ms_DefaultString, 0);

        SE_ASSERT( m_spDefaultVProgram );
    }
    else
    {
        m_spDefaultVProgram = 0;
    }
}
//----------------------------------------------------------------------------
const std::string& SEVertexProgramCatalog::GetName() const
{
    return m_Name;
}
//----------------------------------------------------------------------------
bool SEVertexProgramCatalog::Insert(SEVertexProgram* pProgram)
{
    if( !pProgram )
    {
        SE_ASSERT( false );

        return false;
    }

    std::string StrProgramName(pProgram->GetName());
    if( StrProgramName == ms_NullString
    ||  StrProgramName == ms_DefaultString
    ||  pProgram == m_spDefaultVProgram )
    {
        return false;
    }

    // ��������ԴĿ¼�в���
    SEVertexProgram** ppTempProgram = m_Entry.Find(StrProgramName);
    if( ppTempProgram )
    {
        // �ó����Ѿ�����
        return true;
    }

    // �ó��򲻴���,�����
    m_Entry.Insert(StrProgramName, pProgram);

    return true;
}
//----------------------------------------------------------------------------
bool SEVertexProgramCatalog::Remove(SEVertexProgram* pProgram)
{
    if( !pProgram )
    {
        SE_ASSERT( false );

        return false;
    }

    std::string StrProgramName(pProgram->GetName());
    if( StrProgramName == ms_NullString
    ||  StrProgramName == ms_DefaultString
    ||  pProgram == m_spDefaultVProgram )
    {
        return false;
    }

    // ��������ԴĿ¼�в���
    SEVertexProgram** ppTempProgram = m_Entry.Find(StrProgramName);
    if( !ppTempProgram )
    {
        // �ó��򲻴���
        return false;
    }

    // �������,���Ƴ�
    m_Entry.Remove(StrProgramName);

    return true;
}
//----------------------------------------------------------------------------
SEVertexProgram* SEVertexProgramCatalog::Find(const std::string& rProgramName,
    SEInterfaceDescriptor* pDescriptor)
{
    if( rProgramName == ms_NullString
    ||  rProgramName == ms_DefaultString )
    {
        return StaticCast<SEVertexProgram>(m_spDefaultVProgram);
    }

    // ��������ԴĿ¼�в���
    std::string tempKey = rProgramName;
    if( pDescriptor )
    {
        std::string tempPostFix;
        pDescriptor->GetDescription(tempPostFix);
        tempKey += tempPostFix;
    }
    SEVertexProgram** ppTempProgram = m_Entry.Find(tempKey);
    if( ppTempProgram )
    {
        // �ҵ��򷵻�
        return *ppTempProgram;
    }

    // �ڴ����в���
    SEVertexProgram* pProgram = SEVertexProgram::Load(m_pRenderer, 
        rProgramName, tempKey, pDescriptor);
    if( pProgram )
    {
        // ����Դ����,���Ѿ���Load�󱻼�����ԴĿ¼,�����ٴε���Insert����
        return pProgram;
    }

    // ���򲻴���,��ʹ��Ĭ�ϳ���
    return StaticCast<SEVertexProgram>(m_spDefaultVProgram);
}
//----------------------------------------------------------------------------
bool SEVertexProgramCatalog::PrintContents(const std::string& rFileName) const
{
    const char* pDecorated = SESystem::SE_GetPath(rFileName.c_str(), 
        SESystem::SM_WRITE);

    if( pDecorated )
    {
        std::ofstream OStream(pDecorated);

        SE_ASSERT( OStream );

		std::string StrProgramName;
		SEVertexProgram** ppTempProgram = m_Entry.GetFirst(&StrProgramName);
        while( ppTempProgram )
        {
            OStream << StrProgramName.c_str() << std::endl;
            OStream << std::endl;
            ppTempProgram = m_Entry.GetNext(&StrProgramName);
        }
        OStream.close();

        return true;
    }

    return false;
}
//----------------------------------------------------------------------------
int SEVertexProgramCatalog::GetProfile() const
{
    return m_pRenderer ? m_pRenderer->GetMaxVShaderProfile() : -1;
}
//----------------------------------------------------------------------------
void SEVertexProgramCatalog::SetActive(SEVertexProgramCatalog* pActive)
{
    ms_pActive = pActive;
}
//----------------------------------------------------------------------------
SEVertexProgramCatalog* SEVertexProgramCatalog::GetActive()
{
    return ms_pActive;
}
//----------------------------------------------------------------------------
