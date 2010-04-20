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

#ifndef Swing_OGLES2Program_H
#define Swing_OGLES2Program_H

#include "SEOGLES2RendererLIB.h"
#include "SEOGLES2Renderer.h"
#include "SEProgram.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ˵��:����һ��������,����ʵ��SEProgram��Ĵ�����,������ʵ����.
//    ��Щ��������Ҫ���ڰ������ഴ��,����ESSL����.
// ����:Sun Che
// ʱ��:20090705
//----------------------------------------------------------------------------
class SE_RENDERER_API SEOGLES2Program : public SEProgram
{
    SE_DECLARE_INITIALIZE;

public:
    // ���ڴ���һ��shader program�������������.
    static bool OnLoadProgram(SERenderer* pRenderer, 
        const std::string& rProgramName, SEProgram* pProgram, 
        SEProgram::ProgramType eType, SEInterfaceDescriptor* pInterfaceDesc);

    static void ParseLinkedProgram(unsigned int uiProgram, SEProgram* 
        pVProgram, SEProgram* pPProgram);

    // �����ͷ�shader program�Զ�������.
    static void OnReleaseUserData(void* pUserData);

    // ���ڴ���lighting effect����shader����.
    static void OnConfigureLighting(SELightingEffect* pLEffect);

private:
    SEOGLES2Program(void);

    // Parsing shader����ʱʹ��.
    static const std::string ms_PositionStr;
    static const std::string ms_NormalStr;
    static const std::string ms_ColorStr;
    static const std::string ms_Color0Str;
    static const std::string ms_Color1Str;
    static const std::string ms_TexCoordStr;
};

}

#endif
