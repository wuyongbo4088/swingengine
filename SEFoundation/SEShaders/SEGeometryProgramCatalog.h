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

#ifndef Swing_GeometryProgramCatalog_H
#define Swing_GeometryProgramCatalog_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEObject.h"
#include "SEMainMCR.h"
#include "SEStringHashTable.h"
#include "SERenderer.h"
#include "SEInterfaceDescriptor.h"

namespace Swing
{

class GeometryProgram;

//----------------------------------------------------------------------------
// ����:geometry shader������Դ������
// ˵��:ֻ������Դ,���ͷ�
// ����:Sun Che
// ʱ��:20090523
//----------------------------------------------------------------------------
class SE_FOUNDATION_API GeometryProgramCatalog
{
public:
    GeometryProgramCatalog(const std::string& rName);
    ~GeometryProgramCatalog(void);

    // ��ʼʱ���ǲ�֪����Ⱦ������,
    // ��Ӧ�ó���㴴����һ����Ⱦ����,���ܵ��ô˺���������catalog������Ⱦ��,
    // Ӧ�ó�������غ���ΪWindowApplication::SetRenderer
    void SetRenderer(Renderer* pRenderer);

    const std::string& GetName(void) const;
    bool Insert(GeometryProgram* pProgram);
    bool Remove(GeometryProgram* pProgram);
    GeometryProgram* Find(const std::string& rProgramName,
        InterfaceDescriptor* pDescriptor = 0);
    bool PrintContents(const std::string& rFileName) const;
    int GetProfile(void) const;

    static void SetActive(GeometryProgramCatalog* pActive);
    static GeometryProgramCatalog* GetActive(void);

private:
    enum { PROGRAM_MAP_SIZE = 256 };
    std::string m_Name;
    StringHashTable<GeometryProgram*> m_Entry;
    ObjectPtr m_spDefaultGProgram;

    Renderer* m_pRenderer;

    static const std::string ms_NullString;
    static const std::string ms_DefaultString;
    static GeometryProgramCatalog* ms_pActive;
};

}

#endif
