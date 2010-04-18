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

#ifndef Swing_PixelProgramCatalog_H
#define Swing_PixelProgramCatalog_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEObject.h"
#include "SEMainMCR.h"
#include "SEStringHashTable.h"
#include "SERenderer.h"
#include "SEInterfaceDescriptor.h"

namespace Swing
{

class SEPixelProgram;

//----------------------------------------------------------------------------
// ����:pixel shader������Դ������
// ˵��:ֻ������Դ,���ͷ�
// ����:Sun Che
// ʱ��:20080627
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SEPixelProgramCatalog
{
public:
    SEPixelProgramCatalog(const std::string& rName);
    ~SEPixelProgramCatalog(void);

    // ��ʼʱ���ǲ�֪����Ⱦ������,
    // ��Ӧ�ó���㴴����һ����Ⱦ����,���ܵ��ô˺���������catalog������Ⱦ��,
    // Ӧ�ó�������غ���ΪSEWindowApplication::SetRenderer
    void SetRenderer(SERenderer* pRenderer);

    const std::string& GetName(void) const;
    bool Insert(SEPixelProgram* pProgram);
    bool Remove(SEPixelProgram* pProgram);
    SEPixelProgram* Find(const std::string& rProgramName, 
        SEInterfaceDescriptor* pDescriptor = 0);
    bool PrintContents(const std::string& rFileName) const;
    int GetProfile(void) const;

    static void SetActive(SEPixelProgramCatalog* pActive);
    static SEPixelProgramCatalog* GetActive(void);

private:
    enum { PROGRAM_MAP_SIZE = 256 };
    std::string m_Name;
    SEStringHashTable<SEPixelProgram*> m_Entry;
    SEObjectPtr m_spDefaultPProgram;

    SERenderer* m_pRenderer;

    static const std::string ms_NullString;
    static const std::string ms_DefaultString;
    static SEPixelProgramCatalog* ms_pActive;
};

}

#endif
