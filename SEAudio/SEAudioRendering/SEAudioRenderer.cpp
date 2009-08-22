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

#include "SEAudioPCH.h"
#include "SEAudioRenderer.h"
#include "SEListener.h"
#include "SESound.h"
#include "SENode.h"

using namespace Swing;

//----------------------------------------------------------------------------
AudioRenderer::AudioRenderer()
{
    // ������Ӧ�ü�Ⲣ�����豸����.
    m_pListener = 0;
    m_pSound = 0;
}
//----------------------------------------------------------------------------
AudioRenderer::~AudioRenderer()
{
    SetListener(0);
}
//----------------------------------------------------------------------------
void AudioRenderer::SetListener(Listener* pListener)
{
    if( m_pListener )
    {
        m_pListener->m_pAudioRenderer = 0;
    }

    if( pListener )
    {
        pListener->m_pAudioRenderer = this;
    }

    m_pListener = pListener;

    if( m_pListener )
    {
        OnFrameChange();
    }
}
//----------------------------------------------------------------------------
void AudioRenderer::OnPrePlaySound()
{
    // �������ฺ��ʵ��.
}
//----------------------------------------------------------------------------
void AudioRenderer::OnPostPlaySound()
{
    // �������ฺ��ʵ��.
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// װ�����ͷ���Դ
//----------------------------------------------------------------------------
void AudioRenderer::LoadAllResources(Spatial* pScene)
{
    Sound* pSound = DynamicCast<Sound>(pScene);
    if( pSound )
    {
        LoadResources(pSound);
    }

    Node* pNode = DynamicCast<Node>(pScene);
    if( pNode )
    {
        for( int i = 0; i < pNode->GetCount(); i++ )
        {
            Spatial* pChild = pNode->GetChild(i);
            if( pChild )
            {
                LoadAllResources(pChild);
            }
        }
    }
}
//----------------------------------------------------------------------------
void AudioRenderer::ReleaseAllResources(Spatial* pScene)
{
    Sound* pSound = DynamicCast<Sound>(pScene);
    if( pSound )
    {
        ReleaseResources(pSound);
    }

    Node* pNode = DynamicCast<Node>(pScene);
    if( pNode )
    {
        for( int i = 0; i < pNode->GetCount(); i++ )
        {
            Spatial* pChild = pNode->GetChild(i);
            if( pChild )
            {
                ReleaseAllResources(pChild);
            }
        }
    }
}
//----------------------------------------------------------------------------
void AudioRenderer::LoadResources(Sound* pSound)
{
    SE_ASSERT( pSound );

    // ��soundװ����Ƶ�豸.
    LoadSound(pSound);
}
//----------------------------------------------------------------------------
void AudioRenderer::ReleaseResources(Sound* pSound)
{
    SE_ASSERT( pSound );

    // ����Ƶ�豸�ͷ�sound.
    ReleaseSound(pSound);
}
//----------------------------------------------------------------------------
void AudioRenderer::LoadSound(Sound* pSound)
{
    if( !pSound )
    {
        return;
    }

    AudioResourceIdentifier* pSoundID = pSound->GetIdentifier(this);
    if( !pSoundID )
    {
        OnLoadSound(pSoundID, pSound);
        pSound->OnLoad(this, &AudioRenderer::ReleaseSound,
            &AudioRenderer::UpdateSoundParams, pSoundID);

        if( pSound->SBuffer )
        {
            LoadSBuffer(pSound->SBuffer);
            AudioResourceIdentifier* pSBufferID = 
                pSound->SBuffer->GetIdentifier(this);

            OnAttachSBuffer(pSoundID, pSBufferID);
        }
    }
}
//----------------------------------------------------------------------------
void AudioRenderer::ReleaseSound(AudioBindable* pSound)
{
    if( !pSound )
    {
        return;
    }

    AudioResourceIdentifier* pID = pSound->GetIdentifier(this);
    if( pID )
    {
        OnReleaseSound(pID);
        pSound->OnRelease(this, pID);
    }
}
//----------------------------------------------------------------------------
void AudioRenderer::LoadSBuffer(SoundBuffer* pSBuffer)
{
    if( !pSBuffer )
    {
        return;
    }

    AudioResourceIdentifier* pID = pSBuffer->GetIdentifier(this);
    if( !pID )
    {
        OnLoadSBuffer(pID, pSBuffer);
        pSBuffer->OnLoad(this, &AudioRenderer::ReleaseSBuffer, 0, pID);
    }
}
//----------------------------------------------------------------------------
void AudioRenderer::ReleaseSBuffer(AudioBindable* pSBuffer)
{
    if( !pSBuffer )
    {
        return;
    }

    AudioResourceIdentifier* pID = pSBuffer->GetIdentifier(this);
    if( pID )
    {
        OnReleaseSBuffer(pID);
        pSBuffer->OnRelease(this, pID);
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ��Դ��������
//----------------------------------------------------------------------------
void AudioRenderer::UpdateSoundParams(AudioBindable* pSound)
{
    if( !pSound )
    {
        return;
    }

    AudioResourceIdentifier* pID = pSound->GetIdentifier(this);
    if( pID )
    {
        OnUpdateSoundParams(pID);
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ��Դ������ر�
//----------------------------------------------------------------------------
AudioResourceIdentifier* AudioRenderer::EnableSound()
{
    // ȷ��sound����sound bufferװ������Ƶ�豸.
    LoadSound(m_pSound);
    AudioResourceIdentifier* pID = m_pSound->GetIdentifier(this);
    SE_ASSERT( pID );

    OnEnableSound(pID);
    return pID;
}
//----------------------------------------------------------------------------
void AudioRenderer::DisableSound()
{
    AudioResourceIdentifier* pID = m_pSound->GetIdentifier(this);
    SE_ASSERT( pID );

    OnDisableSound(pID);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ������Ⱦ���
//----------------------------------------------------------------------------
void AudioRenderer::PlayScene(UnculledSet& rAudibleSet)
{
    const int iAudibleCount = rAudibleSet.GetCount();
    UnculledObject* pAudibleSet = rAudibleSet.GetUnculled();
    for( int i = 0; i < iAudibleCount; i++ )
    {
        if( pAudibleSet[i].IsRenderable() )
        {
            Play((Sound*)pAudibleSet[i].Object);
        }
    }
}
//----------------------------------------------------------------------------
void AudioRenderer::Play(Sound* pSound)
{
    SE_ASSERT( pSound );

    // ���õ�ǰ����Ⱦ����,����������ʹ��.
    m_pSound = pSound;

    // per-sound pre-play,������Ⱦ���������Զ�����Ϊ���.
    OnPrePlaySound();

    // ������ǰ����Ⱦ�����sound,��Ϊ��װ��sound buffer.
    AudioResourceIdentifier* pID = EnableSound();

    // ������������.
    SetSoundParams(pID);

    // ��Ⱦ.
    PlayElements(pID);

    // �رյ�ǰ����Ⱦ�����sound.
    DisableSound();

    // per-sound post-play,������Ⱦ���������Զ�����Ϊ���.
    OnPostPlaySound();

    // ��Ⱦ���,������õ�ǰ����Ⱦ����.
    m_pSound = 0;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ����ֹͣ��Ⱦ���
//----------------------------------------------------------------------------
void AudioRenderer::StopScene(UnculledSet& rAudibleSet)
{
    const int iAudibleCount = rAudibleSet.GetCount();
    UnculledObject* pAudibleSet = rAudibleSet.GetUnculled();
    for( int i = 0; i < iAudibleCount; i++ )
    {
        if( pAudibleSet[i].IsRenderable() )
        {
            Stop((Sound*)pAudibleSet[i].Object);
        }
    }
}
//----------------------------------------------------------------------------
void AudioRenderer::Stop(Sound* pSound)
{
    SE_ASSERT( pSound );

    AudioResourceIdentifier* pID = pSound->GetIdentifier(this);
    if( pID )
    {
        // ֹͣ��Ⱦ.
        StopElements(pID);
    }
}
//----------------------------------------------------------------------------