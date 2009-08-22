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

class AudioBindable;
class AudioResourceIdentifier;
class Listener;
class Sound;
class SoundBuffer;
class Spatial;
class UnculledObject;
class UnculledSet;

//----------------------------------------------------------------------------
// ����:������Ⱦ������
// ˵��:
// ����:Sun Che
// ʱ��:20090619
//----------------------------------------------------------------------------
class SE_AUDIO_API AudioRenderer
{
public:
    // �����.
    // ������Ⱦ������API.������������API��Ҫʵ�����.
    virtual ~AudioRenderer(void);

    // RTTI��Ϣ.
    enum
    {
        OPENAL,
        DIRECTSOUND,
        MAX_AUDIORENDERER_TYPES
    };
    virtual int GetType(void) const = 0; // ��������Ⱦ��������ʵ��

    // listener����.
    void SetListener(Listener* pListener);
    inline Listener* GetListener(void) const;

    // ������Ⱦ���.
    virtual void PlayScene(UnculledSet& rAudibleSet);
    virtual void Play(Sound* pSound);

    // ����ֹͣ��Ⱦ���.
    virtual void StopScene(UnculledSet& rAudibleSet);
    virtual void Stop(Sound* pSound);

    // ��ʵ��.
    // ������Ⱦ���������Ƽ��.

    // ������Ⱦ������Դ�ͷź����ĺ���ָ������.
    typedef void (AudioRenderer::*ReleaseFunction)(AudioBindable*);
    // ������Ⱦ������Դ�������»ص������ĺ���ָ������.
    typedef void (AudioRenderer::*UpdateParamsFunction)(AudioBindable*);

    // ������Ⱦ����Դװ�����ͷ�.
    void LoadAllResources(Spatial* pScene);
    void ReleaseAllResources(Spatial* pScene);
    void LoadResources(Sound* pSound);
    void ReleaseResources(Sound* pSound);
    void LoadSound(Sound* pSound);
    void ReleaseSound(AudioBindable* pSound);
    void LoadSBuffer(SoundBuffer* pSBuffer);
    void ReleaseSBuffer(AudioBindable* pSBuffer);

    // ������Ⱦ����Դ��������.
    void UpdateSoundParams(AudioBindable* pSound);

protected:
    // �����
    AudioRenderer(void);

    // ֧��listener���ʺ���ظ��²���.
    // ��listener������ݸı�ʱ,������Щ�ص�����.
    friend class Listener;
    virtual void OnFrameChange(void) = 0;       // {E:R,U,D}�ı�
    virtual void OnMasterGainChange(void) = 0;  // master gain�ı�.

    // per-sound pre/post-play�������.
    virtual void OnPrePlaySound(void);
    virtual void OnPostPlaySound(void);

    // ������������(��������ռ���̬�����������������).
    virtual void SetSoundParams(AudioResourceIdentifier* pID) = 0;

    // ������Ⱦ����������Ⱦ�������.
    virtual void PlayElements(AudioResourceIdentifier* pID) = 0;

    // ������Ⱦ��������ֹͣ��Ⱦ�������.
    virtual void StopElements(AudioResourceIdentifier* pID) = 0;

    // ������Ⱦ����Դװ�����ͷ�.
    // ��Ҫ������ƵAPI����ʵ��.
    virtual void OnLoadSound(AudioResourceIdentifier*& rpID,
        Sound* pSound) = 0;
    virtual void OnReleaseSound(AudioResourceIdentifier* pID) = 0;
    virtual void OnLoadSBuffer(AudioResourceIdentifier*& rpID,
        SoundBuffer* pSBuffer) = 0;
    virtual void OnReleaseSBuffer(AudioResourceIdentifier* pID) = 0;

    // ������Ⱦ����Դ������ر����.
    AudioResourceIdentifier* EnableSound(void);
    void DisableSound(void);

    // ������Ⱦ����Դ������ر�.
    // ��Ҫ������ƵAPI����ʵ��.
    virtual void OnEnableSound(AudioResourceIdentifier* pID) = 0;
    virtual void OnDisableSound(AudioResourceIdentifier* pID) = 0;
    virtual void OnEnableSBuffer(AudioResourceIdentifier* pID) = 0;
    virtual void OnDisableSBuffer(AudioResourceIdentifier* pID) = 0;
    virtual void OnAttachSBuffer(AudioResourceIdentifier* pSoundID,
        AudioResourceIdentifier* pSBufferID) = 0;

    // ������Ⱦ����Դ��������.
    // ��Ҫ������ƵAPI����ʵ��.
    virtual void OnUpdateSoundParams(AudioResourceIdentifier* pID) = 0;

// ���ݳ�Ա:
protected:
    // ��ʵ��.
    // ��Դ����,������Ⱦ�������ฺ������.

    // ��ǰlistener.
    Listener* m_pListener;

    // ��ǰ������Ⱦ����������.
    Sound* m_pSound;
};

#include "SEAudioRenderer.inl"

}

#endif
