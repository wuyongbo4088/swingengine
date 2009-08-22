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

#ifndef Swing_Bindable_H
#define Swing_Bindable_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SESystem.h"
#include "SEAttributes.h"
#include "SERenderer.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:��ԴID��
// ˵��:
// ����:Sun Che
// ʱ��:20080320
//----------------------------------------------------------------------------
class SE_FOUNDATION_API ResourceIdentifier
{
public:
    // �����,ע���������������麯��,
    // ��˱�������,����������Ϊ��δ�����,
    // ���ڴ���û���κ����ݳ�Ա,Ҳû���麯��,
    // �Ӷ��������麯����ָ��Ĵ���,
    // �⽫����������VBufferIdentifier���ȴ洢�Լ��ĳ�Ա����,
    // ���Ұ�ȫ�Ľ���������ʾ������ת������:
    //
    //   class VBufferIdentifier : public ResourceIdentifier
    //   {
    //   public:  Attributes IAttributes;
    //   }
    //   VBufferIdentifier* pID = <some identifier>;
    //   Attributes& rIAttributes = *(Attributes*)pID;

    ~ResourceIdentifier(void){}

protected:
    ResourceIdentifier(void){}
};

//----------------------------------------------------------------------------
// ����:��Դ����Ϣ��
// ˵��:������Ҫ�����Դ����Դ,��VB,IB,shader����,texture�Ļ���.
// ����:Sun Che
// ʱ��:20080320
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Bindable
{
public:
    Bindable(void);
    ~Bindable(void);

    // ����Դ��VRAM����Ψһ��ʾʱʹ��(����vertex buffers֮���������Դ)
    ResourceIdentifier* GetIdentifier(Renderer* pUser) const;

    // ����Դ��VRAM���ж����ʾʱʹ��(vertex buffers)
    int GetInfoCount(void) const;
    ResourceIdentifier* GetIdentifier(int i, Renderer* pUser) const;

    void Release(void);

private:
    friend class Renderer;

    void OnLoad(Renderer* pUser, Renderer::ReleaseFunction oRelease,
        ResourceIdentifier* pID);

    void OnRelease(Renderer* pUser, ResourceIdentifier* pID);

    struct Info
    {
        // ��Դ���󶨵�renderer
        Renderer* User;

        // �ͷ���Դʱ�����renderer�ͷź���
        Renderer::ReleaseFunction Release;

        // ��Դ�ڸ�renderer�ϵ�ID
        ResourceIdentifier* ID;
    };

    // ����ͬʱ�󶨸����renderer,��Ҫ���ڰ�һ��renderer�Ķ����ͬʵ��
    std::vector<Info> m_InfoArray;
};

}

#endif
