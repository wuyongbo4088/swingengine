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

#include "SEAudioPCH.h"
#include "SEWaveCatalog.h"
#include "SEWave.h"

using namespace Swing;

const String WaveCatalog::ms_NullString("");
const String WaveCatalog::ms_DefaultString("Default");
WaveCatalog* WaveCatalog::ms_pActive = 0;

//----------------------------------------------------------------------------
WaveCatalog::WaveCatalog(const String& rName)
    :
    m_Name(rName),
    m_Entry(WAVE_MAP_SIZE)
{
    // ��ʵ��.
    // ����һ��Ĭ��wave,�����޷��ҵ�ָ����Դʱ�����û�
    m_spDefaultWave = 0;
}
//----------------------------------------------------------------------------
WaveCatalog::~WaveCatalog()
{
}
//----------------------------------------------------------------------------
const String& WaveCatalog::GetName() const
{
    return m_Name;
}
//----------------------------------------------------------------------------
bool WaveCatalog::Insert(Wave* pWave)
{
    if( !pWave )
    {
        SE_ASSERT( false );

        return false;
    }

    String tempWaveName(pWave->GetName());
    if( tempWaveName == ms_NullString
    ||  tempWaveName == ms_DefaultString
    ||  pWave == m_spDefaultWave )
    {
        return false;
    }

    // ��������ԴĿ¼�в���
    Wave** ppTempWave = m_Entry.Find(tempWaveName);
    if( ppTempWave )
    {
        // ��wave�Ѿ�����
        return true;
    }

    // ��wave������,�����
    m_Entry.Insert(tempWaveName, pWave);

    return true;
}
//----------------------------------------------------------------------------
bool WaveCatalog::Remove(Wave* pWave)
{
    if( !pWave )
    {
        SE_ASSERT( false );

        return false;
    }

    String tempWaveName(pWave->GetName());
    if( tempWaveName == ms_NullString
    ||  tempWaveName == ms_DefaultString
    ||  pWave == m_spDefaultWave )
    {
        return false;
    }

    // ��������ԴĿ¼�в���
    Wave** ppTempWave = m_Entry.Find(tempWaveName);
    if( !ppTempWave )
    {
        // ��wave������
        return false;
    }

    // wave����,���Ƴ�
    m_Entry.Remove(tempWaveName);

    return true;
}
//----------------------------------------------------------------------------
Wave* WaveCatalog::Find(const String& rWaveName)
{
    if( rWaveName == ms_NullString 
    ||  rWaveName == ms_DefaultString )
    {
        return StaticCast<Wave>(m_spDefaultWave);
    }

    // ��������ԴĿ¼�в���
    Wave** ppTempWave = m_Entry.Find(rWaveName);
    if( ppTempWave )
    {
        // �ҵ��򷵻�
        return *ppTempWave;
    }

    // �ڴ����в���
    Wave* pWave = Wave::Load(rWaveName);
    if( pWave )
    {
        // ����Դ����,���Ѿ���Load�󱻼�����ԴĿ¼,�����ٴε���Insert����
        return pWave;
    }

    // wave������,��ʹ��Ĭ��wave
    return StaticCast<Wave>(m_spDefaultWave);
}
//----------------------------------------------------------------------------
bool WaveCatalog::PrintContents(const String& rFileName) const
{
    const char* pDecorated = System::SE_GetPath(rFileName, System::SM_WRITE);

    if( pDecorated )
    {
        std::ofstream OStream(pDecorated);

        SE_ASSERT( OStream );

        String tempWaveName;
        Wave** ppTempWave = m_Entry.GetFirst(&tempWaveName);
        while( ppTempWave )
        {
            Wave* pWave = *ppTempWave;
            OStream << (const char*)tempWaveName << ":" << std::endl;
            OStream << "    format = " << (const char*)(pWave->GetFormatName())
                << std::endl;
            OStream << "    frequency = " << pWave->GetFrequency()
                << std::endl;
            OStream << std::endl;
            ppTempWave = m_Entry.GetNext(&tempWaveName);
        }
        OStream.close();

        return true;
    }

    return false;
}
//----------------------------------------------------------------------------
void WaveCatalog::SetActive(WaveCatalog* pActive)
{
    ms_pActive = pActive;
}
//----------------------------------------------------------------------------
WaveCatalog* WaveCatalog::GetActive()
{
    return ms_pActive;
}
//----------------------------------------------------------------------------