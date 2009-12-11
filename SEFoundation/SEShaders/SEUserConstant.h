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

#ifndef Swing_UserConstant_H
#define Swing_UserConstant_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SESystem.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:�û����峣����
// ˵��:shader�����е�ȫ����ֵ��,
//      ��Ҫָ�û��Զ���ĸ�������
// ����:Sun Che
// ʱ��:20080623
//----------------------------------------------------------------------------
class SE_FOUNDATION_API UserConstant
{
public:
    UserConstant(const std::string& rName, void* pID, int iDataCount);
    UserConstant(const UserConstant& rUC);
    ~UserConstant(void);

    // ��Ա����,rendererʹ�����Ǹ�ͼ��API��������
    inline const std::string& GetName(void) const;
    inline void* GetID(void) const;
    inline int GetDataCount(void) const;
    inline float* GetData(void) const;

    // �û����Ը�����Ҫ,����ָ��user constant�����ݴ洢λ��.
    // Ĭ�������,����user constant����ָ��һ������ľ�̬������,
    // �Ӷ�ȷ����Ⱦ��������shader constantʱ��ָ�����ݰ�ȫ��.
    void SetDataSource(float* pData);

    bool Active; // default: true

    // ����û�ʵ�����Զ�������,��Ӧʵ���Զ������ݵ��ͷŹ���.
    typedef void (*ReleaseID)(void*);
    static ReleaseID OnReleaseID;

    // ����û�ʵ�����Զ�������,��Ӧʵ���Զ������ݵĿ�������.
    // �Ӷ����������캯����ȷ�����û��Զ�������.
    typedef void (*CopyID)(void*, void**);
    static CopyID OnCopyID;

private:
    std::string m_Name;
    void* m_pID;
    int m_iDataCount;
    float* m_pData;  // Ӧָ���û��ṩ��������

    static float ms_afDefaultData[16];
};

#include "SEUserConstant.inl"

}

#endif
