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

#ifndef Swing_OpenALRenderer_H
#define Swing_OpenALRenderer_H

// ��ʵ��.
// ������ʱû��OpenAL�ķ�װ��.
// ���ʹ��ƽ̨SDK�ṩ��OpenAL headers.
#if defined(_WIN32)
#include <al.h>
#include <alc.h>
#elif defined(__APPLE__)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

// ����al.h��OPENAL�궨���AudioRenderer::OPENALö��ֵ�����ͻ��ɱ������.
#if defined(OPENAL)
#undef OPENAL
#define OPENALUNDEFINED
#endif

#include "SEOpenALRendererLIB.h"
#include "SEAudioRenderer.h"
#include "SEMain.h"
#include "SEWave.h"
#include "SEWaveCatalog.h"

//----------------------------------------------------------------------------
#ifdef _DEBUG
#define SE_AL_BEGIN_DEBUG_ALAPI alGetError()
#define SE_AL_END_DEBUG_ALAPI \
{ \
    ALenum tempError = alGetError(); \
    SE_ASSERT( tempError == AL_NO_ERROR ); \
}
#else
#define SE_AL_BEGIN_DEBUG_ALAPI
#define SE_AL_END_DEBUG_ALAPI
#endif
//----------------------------------------------------------------------------

namespace Swing
{

//----------------------------------------------------------------------------
// ����:OpenAL1.0 Renderer�����
// ˵��:
// ����:Sun Che
// ʱ��:20090623
//----------------------------------------------------------------------------
class SE_AUDIO_RENDERER_API OpenALRenderer : public AudioRenderer
{
    SE_DECLARE_INITIALIZE;
    SE_DECLARE_TERMINATE;

public:
    // RTTI.
    virtual int GetType(void) const { return AudioRenderer::OPENAL; }

    // �����.
    // ��OpenAL context������,�������๹�캯������InitializeState����.
    virtual ~OpenALRenderer(void);
    void InitializeState(void);

protected:
    OpenALRenderer(void);

    // listener����.
    virtual void OnFrameChange(void);
    virtual void OnMasterGainChange(void);

    // ������������(��������ռ���̬�����������������).
    virtual void SetSoundParams(AudioResourceIdentifier* pID);

    // ������Ⱦ����������Ⱦ�������.
    virtual void PlayElements(AudioResourceIdentifier* pID);

    // ������Ⱦ��������ֹͣ��Ⱦ�������.
    virtual void StopElements(AudioResourceIdentifier* pID);

    // ������Ⱦ����Դװ�����ͷ�.
    virtual void OnLoadSound(AudioResourceIdentifier*& rpID, Sound* pSound);
    virtual void OnReleaseSound(AudioResourceIdentifier* pID);
    virtual void OnLoadSBuffer(AudioResourceIdentifier*& rpID,
        SoundBuffer* pSBuffer);
    virtual void OnReleaseSBuffer(AudioResourceIdentifier* pID);

    // ������Ⱦ����Դ������ر�.
    virtual void OnEnableSound(AudioResourceIdentifier* pID);
    virtual void OnDisableSound(AudioResourceIdentifier* pID);
    virtual void OnEnableSBuffer(AudioResourceIdentifier* pID);
    virtual void OnDisableSBuffer(AudioResourceIdentifier* pID);
    virtual void OnAttachSBuffer(AudioResourceIdentifier* pSoundID,
        AudioResourceIdentifier* pSBufferID);

    // ������Ⱦ����Դ��������.
    virtual void OnUpdateSoundParams(AudioResourceIdentifier* pID);

    // helper function, change sound source's frame.
    void SetSoundFrame(unsigned int uiID, Sound* pSound);

    static WaveCatalog* ms_pWaveCatalog;

    // ��������������OpenAL��Ⱦ�����������ӳ���.
    static ALenum ms_aeWaveType[Wave::WT_COUNT];
};

}

#if defined(OPENALUNDEFINED)
#define OPENAL
#endif

#endif
