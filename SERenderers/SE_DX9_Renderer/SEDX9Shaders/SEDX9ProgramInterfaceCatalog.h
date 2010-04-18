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

#ifndef Swing_DX9ProgramInterfaceCatalog_H
#define Swing_DX9ProgramInterfaceCatalog_H

#include "SEDX9RendererLIB.h"
#include "SEDX9Utility.h"
#include "SEStringHashTable.h"

namespace Swing
{

class SEDX9ProgramInterface;

//----------------------------------------------------------------------------
// ˵��:
// ����:Sun Che
// ʱ��:20090417
//----------------------------------------------------------------------------
class SE_RENDERER_API SEDX9ProgramInterfaceCatalog
{
public:
    SEDX9ProgramInterfaceCatalog(const std::string& rName);
    ~SEDX9ProgramInterfaceCatalog(void);

    // ��ʼʱ���ǲ�֪����Ⱦ������,
    // ��Ӧ�ó���㴴����һ����Ⱦ����,���ܵ��ô˺���������catalog������Ⱦ��,
    // Ӧ�ó�������غ���ΪWindowApplication::SetRenderer
    void SetRenderer(SEDX9Renderer* pRenderer);

    const std::string& GetName(void) const;
    bool Insert(SEDX9ProgramInterface* pProgramInterface);
    bool Remove(SEDX9ProgramInterface* pProgramInterface);
    SEDX9ProgramInterface* Find(CGprogram hCgProgram, 
        const std::string& rPInterfaceName);
    bool PrintContents(const std::string& rFileName) const;

    static void SetActive(SEDX9ProgramInterfaceCatalog* pActive);
    static SEDX9ProgramInterfaceCatalog* GetActive(void);

private:
    enum { PROGRAM_MAP_SIZE = 256 };
    std::string m_Name;
    SEStringHashTable<SEDX9ProgramInterface*> m_Entry;

    SEDX9Renderer* m_pRenderer;

    static SEDX9ProgramInterfaceCatalog* ms_pActive;
};

}

#endif
