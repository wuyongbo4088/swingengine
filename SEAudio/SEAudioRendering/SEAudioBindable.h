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

#ifndef Swing_AudioBindable_H
#define Swing_AudioBindable_H

#include "SEAudioLIB.h"
#include "SEAudioRenderer.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ˵��:
// ����:Sun Che
// ʱ��:20090619
//----------------------------------------------------------------------------
class SE_AUDIO_API SEAudioResourceIdentifier
{
public:
    // �����,ע���������������麯��,
    // ��˱�������,����������Ϊ��δ�����,
    // ���ڴ���û���κ����ݳ�Ա,Ҳû���麯��,
    // �Ӷ��������麯����ָ��Ĵ���,
    // �⽫�������������ȴ洢�Լ��ĳ�Ա����,
    // ���Ұ�ȫ�Ľ���������ʾ������ת������:
    //
    //   class SubClassResourceIdentifier : public SEAudioResourceIdentifier
    //   {
    //   public:  DataType SubClassMember;
    //   }
    //   SubClassResourceIdentifier* pID = <some identifier>;
    //   Type& rSubClassMember = *(DataType*)pID;

    ~SEAudioResourceIdentifier(void){}

protected:
    SEAudioResourceIdentifier(void){}
};

//----------------------------------------------------------------------------
// ����:��Ƶ��Դ����Ϣ��
// ˵��:
// ����:Sun Che
// ʱ��:20090619
//----------------------------------------------------------------------------
class SE_AUDIO_API SEAudioBindable
{
public:
    SEAudioBindable(void);
    ~SEAudioBindable(void);

    // ����Դ����Ƶ�豸����Ψһ��ʾʱʹ��.
    SEAudioResourceIdentifier* GetIdentifier(SEAudioRenderer* pUser) const;

    // ����Դ����Ƶ�豸���ж����ʾʱʹ��.
    int GetInfoCount(void) const;
    SEAudioResourceIdentifier* GetIdentifier(int i, SEAudioRenderer* pUser) 
        const;

    void Release(void);

    // ������Ƶ��Դ��������ϵ�µĿռ���̬�Լ����������Ƶ�����Ķ�̬����.
    // ��SESound������UpdateWorldDate�����е���.
    void UpdateParams(void);

private:
    friend class SEAudioRenderer;

    void OnLoad(SEAudioRenderer* pUser, SEAudioRenderer::ReleaseFunction 
        oRelease, SEAudioRenderer::UpdateParamsFunction oUpdateParams,
        SEAudioResourceIdentifier* pID);

    void OnRelease(SEAudioRenderer* pUser, SEAudioResourceIdentifier* pID);

    struct Info
    {
        // ��Դ���󶨵�audio renderer.
        SEAudioRenderer* User;

        // �ͷ���Դʱ�����audio renderer�ͷź���.
        SEAudioRenderer::ReleaseFunction Release;
        // ������Դ����ʱ�����audio renderer���º���.
        SEAudioRenderer::UpdateParamsFunction UpdateParams;

        // ��Դ�ڸ�audio renderer�ϵ�ID.
        SEAudioResourceIdentifier* ID;
    };

    // ����ͬʱ�󶨸����audio renderer,
    // ��Ҫ���ڰ�һ��audio renderer�Ķ����ͬʵ��.
    std::vector<Info> m_InfoArray;
};

}

#endif
