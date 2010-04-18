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

#ifndef Swing_SamplerInformation_H
#define Swing_SamplerInformation_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SESystem.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:��������Ϣ��
// ˵��:shader�����еĲ�����
// ����:Sun Che
// ʱ��:20080624
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SESamplerInformation
{
public:
    enum Type
    {
        SAMPLER_1D,       // dimension 1
        SAMPLER_2D,       // dimension 2
        SAMPLER_3D,       // dimension 3
        SAMPLER_CUBE,     // dimension 2 (a set of 2D images)
        SAMPLER_PROJ,     // dimension 2
        MAX_SAMPLER_TYPES
    };

    // ����������,����������,��Ӧ����Ԫ����Դhandle��������shader�����ļ���,
    // ������ά���ɲ�����������֪,
    // ����������Ԫ����Դhandle��װ��shader����ʱ������,
    // filterģʽ��wrapģʽ�����������ȷ�������������������
    //
    // �������µ�����fx�ļ����ļ�ϵͳ��,filterģʽ��wrapģʽ�����ļ�ָ��,
    // ���Ҫ���µ�ģ����֧�ֶ�ȡģʽ���ṩ���������Ĺ���
    SESamplerInformation(const std::string& rName, Type eType, void* pID);
    SESamplerInformation(const SESamplerInformation& rSI);
    ~SESamplerInformation(void);

    inline const std::string& GetName(void) const;
    inline Type GetType(void) const;
    inline void* GetID(void) const;
    inline int GetDimension(void) const;

    // ����û�ʵ�����Զ�������,��Ӧʵ���Զ������ݵ��ͷŹ���.
    typedef void (*ReleaseID)(void*);
    static ReleaseID OnReleaseID;

    // ����û�ʵ�����Զ�������,��Ӧʵ���Զ������ݵĿ�������.
    // �Ӷ����������캯����ȷ�����û��Զ�������.
    typedef void (*CopyID)(void*, void**);
    static CopyID OnCopyID;

private:
    std::string m_Name;
    Type m_eType;
    int m_iDimension;
    void* m_pID;
};

#include "SESamplerInformation.inl"

}

#endif
