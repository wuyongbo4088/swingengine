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

#ifndef Swing_DX10Program_H
#define Swing_DX10Program_H

#include "SEDX10RendererLIB.h"
#include "SEDX10Utility.h"
#include "SEProgram.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:This is a delegation of SEProgram class, the delegation class 
//     impliments delegate functions for it's base class, then the functions 
//     will be used to create/link/compile Cg programs.The delegation class 
//     can not be instantiated. 
// Author:Sun Che
// Date:20091202
//----------------------------------------------------------------------------
class SE_RENDERER_API SEDX10Program : public SEProgram
{
    SE_DECLARE_INITIALIZE;

public:
    // This function creates a shader program and initialize it.
    static bool OnLoadProgram(SERenderer* pRenderer, const std::string& 
        rProgramName, SEProgram* pProgram, SEProgram::ProgramType eType, 
        SEInterfaceDescriptor* pInterfaceDesc);

    // Helper functions used by OnLoadProgram().
    static bool RecursParams(CGparameter hCgParam, SEProgram* pProgram);
    static bool ParseParam(CGparameter hCgParam, SEProgram* pProgram);

    // This function releases a shader program's user data.
    static void OnReleaseUserData(void* pUserData);

    // This function creates a specific shader program used by SELightingEffect.
    static void OnConfigureLighting(SELightingEffect* pLEffect);

private:
    // Can not be instantiated. 
    SEDX10Program(void);
};

}

#endif
