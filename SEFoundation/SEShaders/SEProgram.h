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
#include "SEString.h"
#include "SERenderer.h"
#include "SEInterfaceDescriptor.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:shader������
// ˵��:
// ����:Sun Che
// ʱ��:20080623
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Program : public Object, public Bindable
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    // �����.
    virtual ~Program(void);

    enum ProgramType
    {
        PT_VERTEX,
        PT_PIXEL,
        PT_GEOMETRY,
        PT_COUNT
    };
    inline ProgramType GetProgramType(void) const;

    inline const Attributes& GetInputAttributes(void) const;
    inline const Attributes& GetOutputAttributes(void) const;

    // ����renderer constants.
    inline int GetRCCount(void) const;
    RendererConstant* GetRC(int i);
    RendererConstant* GetRC(RendererConstant::Type eType);

    // ����user constants.
    inline int GetUCCount(void) const;
    UserConstant* GetUC(int i);
    UserConstant* GetUC(const String& rName);

    // ����samplers.
    inline int GetSICount(void) const;
    SamplerInformation* GetSI(int i);
    SamplerInformation* GetSI(const String& rName);

    // �û�����ʵ�����������,���ڴ���һ��shader program�������������.
    typedef bool (*LoadProgram)(Renderer*, const String&, Program*, 
        ProgramType, InterfaceDescriptor*);

    // �û�����ʵ�����������,�����ͷ�shader program�Զ�������.
    typedef void (*ReleaseUserData)(void*);

    // �û�����Ϊ��Щ��̬����ָ�븳ֵ,ʹ���ܹ�����û���ϣ�����Զ�����Ϊ.
    static LoadProgram OnLoadProgram;
    static ReleaseUserData OnReleaseUserData;

    // �û����붨�岢�������Զ�������.
    // ��Ҫ���ڴ���program��Ծ���ƽ̨API��handle�������������.
    void* UserData;

protected:
    static bool Load(Renderer* pRenderer, const String& rProgramName,
        Program* pProgram, ProgramType eType, 
        InterfaceDescriptor* pInterfaceDesc);

    Program(void);

    ProgramType m_eProgramType;

    // shader���������������ݸ�ʽ.
    Attributes m_InputAttributes;
    Attributes m_OutputAttributes;

    // �ó����������Ⱦ������.
    std::vector<RendererConstant> m_RendererConstants;

    // �ó���������û����峣��,
    // �û����峣����Ӧ�ó�����������.
    std::vector<UserConstant> m_UserConstants;

    // �ó�������Ĳ�������Ԫ��Ϣ.
    std::vector<SamplerInformation> m_SamplerInformation;

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

typedef SmartPointer<Program> ProgramPtr;

#include "SEProgram.inl"

}

#endif
