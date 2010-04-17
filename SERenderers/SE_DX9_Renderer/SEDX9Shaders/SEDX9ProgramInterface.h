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

#ifndef Swing_DX9ProgramInterface_H
#define Swing_DX9ProgramInterface_H

#include "SEDX9RendererLIB.h"
#include "SEDX9Utility.h"
#include "SEObject.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:DirectX9 shader program interface��
// ˵��:
// ����:Sun Che
// ʱ��:20090417
//----------------------------------------------------------------------------
class SE_RENDERER_API DX9ProgramInterface : public SEObject
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    // ����ͨ��Load������һ��ʵ��.
    static DX9ProgramInterface* Load(CGprogram hCgProgram, CGcontext hCgContext, 
        const std::string& rPInterfaceName);
    ~DX9ProgramInterface(void);

    inline CGtype GetProgramInterfaceType(void) const;
    inline CGparameter GetParam(void) const;
    inline CGprogram GetProgram(void) const;

protected:
    DX9ProgramInterface(void);

    CGtype m_eCgType;
    CGparameter m_hCgParam;
    CGprogram m_hCgProgram;
};

typedef SESmartPointer<DX9ProgramInterface> DX9ProgramInterfacePtr;

#include "SEDX9ProgramInterface.inl"

}

#endif
