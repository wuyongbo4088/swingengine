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

#ifndef Swing_Program_H
#define Swing_Program_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEObject.h"
#include "SEAttributes.h"
#include "SEBindable.h"
#include "SERendererConstant.h"
#include "SEUserConstant.h"
#include "SESamplerInformation.h"
#include "SERenderer.h"
#include "SEInterfaceDescriptor.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Author:Sun Che
// Date:20080623
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SEProgram : public SEObject, public SEBindable
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    // �����.
    virtual ~SEProgram(void);

    enum ProgramType
    {
        PT_VERTEX,
        PT_PIXEL,
        PT_GEOMETRY,
        PT_COUNT
    };
    inline ProgramType GetProgramType(void) const;

    inline const SEAttributes& GetInputAttributes(void) const;
    inline const SEAttributes& GetOutputAttributes(void) const;

    // ����renderer constants.
    inline int GetRCCount(void) const;
    SERendererConstant* GetRC(int i);
    SERendererConstant* GetRC(SERendererConstant::Type eType);

    // ����user constants.
    inline int GetUCCount(void) const;
    SEUserConstant* GetUC(int i);
    SEUserConstant* GetUC(const std::string& rName);

    // ����samplers.
    inline int GetSICount(void) const;
    SESamplerInformation* GetSI(int i);
    SESamplerInformation* GetSI(const std::string& rName);

    // �û�����ʵ�����������,���ڴ���һ��shader program�������������.
    typedef bool (*LoadProgram)(SERenderer*, const std::string&, SEProgram*, 
        ProgramType, SEInterfaceDescriptor*);

    // �û�����ʵ�����������,�����ͷ�shader program�Զ�������.
    typedef void (*ReleaseUserData)(void*);

    // �û�����Ϊ��Щ��̬����ָ�븳ֵ,ʹ���ܹ�����û���ϣ�����Զ�����Ϊ.
    static LoadProgram OnLoadProgram;
    static ReleaseUserData OnReleaseUserData;

    // �û����붨�岢�������Զ�������.
    // ��Ҫ���ڴ���program��Ծ���ƽ̨API��handle�������������.
    void* UserData;

protected:
    static bool Load(SERenderer* pRenderer, const std::string& rProgramName,
        SEProgram* pProgram, ProgramType eType, 
        SEInterfaceDescriptor* pInterfaceDesc);

    SEProgram(void);

    ProgramType m_eProgramType;

    // shader���������������ݸ�ʽ.
    SEAttributes m_InputAttributes;
    SEAttributes m_OutputAttributes;

    // �ó����������Ⱦ������.
    std::vector<SERendererConstant> m_RendererConstants;

    // �ó���������û����峣��,
    // �û����峣����Ӧ�ó�����������.
    std::vector<SEUserConstant> m_UserConstants;

    // �ó�������Ĳ�������Ԫ��Ϣ.
    std::vector<SESamplerInformation> m_SamplerInformation;

    // parsing shader����ʱʹ��.
    static const std::string ms_PositionStr;
    static const std::string ms_Position0Str;
    static const std::string ms_NormalStr;
    static const std::string ms_Normal0Str;
    static const std::string ms_ColorStr;
    static const std::string ms_Color0Str;
    static const std::string ms_Color1Str;
    static const std::string ms_Color2Str;
    static const std::string ms_Color3Str;
    static const std::string ms_TexCoordStr;
};

typedef SESmartPointer<SEProgram> SEProgramPtr;

#include "SEProgram.inl"

}

#endif
