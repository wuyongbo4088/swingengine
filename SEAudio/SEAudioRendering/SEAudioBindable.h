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
// ����:audio��ԴID��
// ˵��:
// ����:Sun Che
// ʱ��:20090619
//----------------------------------------------------------------------------
class SE_AUDIO_API AudioResourceIdentifier
{
public:
    // �����,ע���������������麯��,
    // ��˱�������,����������Ϊ��δ�����,
    // ���ڴ���û���κ����ݳ�Ա,Ҳû���麯��,
    // �Ӷ��������麯����ָ��Ĵ���,
    // �⽫�������������ȴ洢�Լ��ĳ�Ա����,
    // ���Ұ�ȫ�Ľ���������ʾ������ת������:
    //
    //   class SubClassResourceIdentifier : public AudioResourceIdentifier
    //   {
    //   public:  DataType SubClassMember;
    //   }
    //   SubClassResourceIdentifier* pID = <some identifier>;
    //   Type& rSubClassMember = *(DataType*)pID;

    ~AudioResourceIdentifier(void){}

protected:
    AudioResourceIdentifier(void){}
};

//----------------------------------------------------------------------------
// ����:��Ƶ��Դ����Ϣ��
// ˵��:
// ����:Sun Che
// ʱ��:20090619
//----------------------------------------------------------------------------
class SE_AUDIO_API AudioBindable
{
public:
    AudioBindable(void);
    ~AudioBindable(void);

    // ����Դ����Ƶ�豸����Ψһ��ʾʱʹ��.
    AudioResourceIdentifier* GetIdentifier(AudioRenderer* pUser) const;

    // ����Դ����Ƶ�豸���ж����ʾʱʹ��.
    int GetInfoCount(void) const;
    AudioResourceIdentifier* GetIdentifier(int i, AudioRenderer* pUser) const;

    void Release(void);

    // ������Ƶ��Դ��������ϵ�µĿռ���̬�Լ����������Ƶ�����Ķ�̬����.
    // ��Sound������UpdateWorldDate�����е���.
    void UpdateParams(void);

private:
    friend class AudioRenderer;

    void OnLoad(AudioRenderer* pUser, AudioRenderer::ReleaseFunction oRelease,
        AudioRenderer::UpdateParamsFunction oUpdateParams,
        AudioResourceIdentifier* pID);

    void OnRelease(AudioRenderer* pUser, AudioResourceIdentifier* pID);

    struct Info
    {
        // ��Դ���󶨵�audio renderer.
        AudioRenderer* User;

        // �ͷ���Դʱ�����audio renderer�ͷź���.
        AudioRenderer::ReleaseFunction Release;
        // ������Դ����ʱ�����audio renderer���º���.
        AudioRenderer::UpdateParamsFunction UpdateParams;

        // ��Դ�ڸ�audio renderer�ϵ�ID.
        AudioResourceIdentifier* ID;
    };

    // ����ͬʱ�󶨸����audio renderer,
    // ��Ҫ���ڰ�һ��audio renderer�Ķ����ͬʵ��.
    std::vector<Info> m_InfoArray;
};

}

#endif
