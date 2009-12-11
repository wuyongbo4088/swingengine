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

#ifndef Swing_OpenGLProgramInterfaceCatalog_H
#define Swing_OpenGLProgramInterfaceCatalog_H

#include "SEOpenGLRendererLIB.h"
#include "SEOpenGLRenderer.h"
#include "SEStringHashTable.h"

namespace Swing
{

class OpenGLProgramInterface;

//----------------------------------------------------------------------------
// ����:OpenGL shader program interface��Դ������
// ˵��:
// ����:Sun Che
// ʱ��:20090417
//----------------------------------------------------------------------------
class SE_RENDERER_API OpenGLProgramInterfaceCatalog
{
public:
    OpenGLProgramInterfaceCatalog(const std::string& rName);
    ~OpenGLProgramInterfaceCatalog(void);

    // ��ʼʱ���ǲ�֪����Ⱦ������,
    // ��Ӧ�ó���㴴����һ����Ⱦ����,���ܵ��ô˺���������catalog������Ⱦ��,
    // Ӧ�ó�������غ���ΪWindowApplication::SetRenderer
    void SetRenderer(OpenGLRenderer* pRenderer);

    const std::string& GetName(void) const;
    bool Insert(OpenGLProgramInterface* pProgramInterface);
    bool Remove(OpenGLProgramInterface* pProgramInterface);
    OpenGLProgramInterface* Find(CGprogram hCgProgram, 
        const std::string& rPInterfaceName);
    bool PrintContents(const std::string& rFileName) const;

    static void SetActive(OpenGLProgramInterfaceCatalog* pActive);
    static OpenGLProgramInterfaceCatalog* GetActive(void);

private:
    enum { PROGRAM_MAP_SIZE = 256 };
    std::string m_Name;
    StringHashTable<OpenGLProgramInterface*> m_Entry;

    OpenGLRenderer* m_pRenderer;

    static OpenGLProgramInterfaceCatalog* ms_pActive;
};

}

#endif
