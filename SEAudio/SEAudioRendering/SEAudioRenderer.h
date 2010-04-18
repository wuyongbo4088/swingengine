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

#ifndef Swing_AudioRenderer_H
#define Swing_AudioRenderer_H

#include "SEAudioLIB.h"
#include "SEMatrix4.h"

namespace Swing
{

class SEAudioBindable;
class SEAudioResourceIdentifier;
class SEListener;
class SESound;
class SESoundBuffer;
class SESpatial;
class SEUnculledObject;
class SEUnculledSet;

//----------------------------------------------------------------------------
// ˵��:
// ����:Sun Che
// ʱ��:20090619
//----------------------------------------------------------------------------
class SE_AUDIO_API SEAudioRenderer
{
public:
    // �����.
    // ������Ⱦ������API.������������API��Ҫʵ�����.
    virtual ~SEAudioRenderer(void);

    // RTTI��Ϣ.
    enum
    {
        OPENAL,
        DIRECTSOUND,
        MAX_AUDIORENDERER_TYPES
    };
    virtual int GetType(void) const = 0; // ��������Ⱦ��������ʵ��

    // listener����.
    void SetListener(SEListener* pListener);
    inline SEListener* GetListener(void) const;

    // ������Ⱦ���.
    virtual void PlayScene(SEUnculledSet& rAudibleSet);
    virtual void Play(SESound* pSound);

    // ����ֹͣ��Ⱦ���.
    virtual void StopScene(SEUnculledSet& rAudibleSet);
    virtual void Stop(SESound* pSound);

    // ��ʵ��.
    // ������Ⱦ���������Ƽ��.

    // ������Ⱦ������Դ�ͷź����ĺ���ָ������.
    typedef void (SEAudioRenderer::*ReleaseFunction)(SEAudioBindable*);
    // ������Ⱦ������Դ�������»ص������ĺ���ָ������.
    typedef void (SEAudioRenderer::*UpdateParamsFunction)(SEAudioBindable*);

    // ������Ⱦ����Դװ�����ͷ�.
    void LoadAllResources(SESpatial* pScene);
    void ReleaseAllResources(SESpatial* pScene);
    void LoadResources(SESound* pSound);
    void ReleaseResources(SESound* pSound);
    void LoadSound(SESound* pSound);
    void ReleaseSound(SEAudioBindable* pSound);
    void LoadSBuffer(SESoundBuffer* pSBuffer);
    void ReleaseSBuffer(SEAudioBindable* pSBuffer);

    // ������Ⱦ����Դ��������.
    void UpdateSoundParams(SEAudioBindable* pSound);

protected:
    // �����
    SEAudioRenderer(void);

    // ֧��listener���ʺ���ظ��²���.
    // ��listener������ݸı�ʱ,������Щ�ص�����.
    friend class SEListener;
    virtual void OnFrameChange(void) = 0;       // {E:R,U,D}�ı�
    virtual void OnMasterGainChange(void) = 0;  // master gain�ı�.

    // per-sound pre/post-play�������.
    virtual void OnPrePlaySound(void);
    virtual void OnPostPlaySound(void);

    // ������������(��������ռ���̬�����������������).
    virtual void SetSoundParams(SEAudioResourceIdentifier* pID) = 0;

    // ������Ⱦ����������Ⱦ�������.
    virtual void PlayElements(SEAudioResourceIdentifier* pID) = 0;

    // ������Ⱦ��������ֹͣ��Ⱦ�������.
    virtual void StopElements(SEAudioResourceIdentifier* pID) = 0;

    // ������Ⱦ����Դװ�����ͷ�.
    // ��Ҫ������ƵAPI����ʵ��.
    virtual void OnLoadSound(SEAudioResourceIdentifier*& rpID,
        SESound* pSound) = 0;
    virtual void OnReleaseSound(SEAudioResourceIdentifier* pID) = 0;
    virtual void OnLoadSBuffer(SEAudioResourceIdentifier*& rpID,
        SESoundBuffer* pSBuffer) = 0;
    virtual void OnReleaseSBuffer(SEAudioResourceIdentifier* pID) = 0;

    // ������Ⱦ����Դ������ر����.
    SEAudioResourceIdentifier* EnableSound(void);
    void DisableSound(void);

    // ������Ⱦ����Դ������ر�.
    // ��Ҫ������ƵAPI����ʵ��.
    virtual void OnEnableSound(SEAudioResourceIdentifier* pID) = 0;
    virtual void OnDisableSound(SEAudioResourceIdentifier* pID) = 0;
    virtual void OnEnableSBuffer(SEAudioResourceIdentifier* pID) = 0;
    virtual void OnDisableSBuffer(SEAudioResourceIdentifier* pID) = 0;
    virtual void OnAttachSBuffer(SEAudioResourceIdentifier* pSoundID,
        SEAudioResourceIdentifier* pSBufferID) = 0;

    // ������Ⱦ����Դ��������.
    // ��Ҫ������ƵAPI����ʵ��.
    virtual void OnUpdateSoundParams(SEAudioResourceIdentifier* pID) = 0;

// ���ݳ�Ա:
protected:
    // ��ʵ��.
    // ��Դ����,������Ⱦ�������ฺ������.

    // ��ǰlistener.
    SEListener* m_pListener;

    // ��ǰ������Ⱦ����������.
    SESound* m_pSound;
};

#include "SEAudioRenderer.inl"

}

#endif
