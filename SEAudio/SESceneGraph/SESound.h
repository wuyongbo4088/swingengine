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

#ifndef Swing_Sound_H
#define Swing_Sound_H

#include "SEAudioLIB.h"
#include "SESpatial.h"
#include "SESoundBuffer.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:������ͼ�����ڵ����
// ˵��:
// ����:Sun Che
// ʱ��:20090618
//----------------------------------------------------------------------------
class SE_AUDIO_API Sound : public Spatial, public AudioBindable
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

    // ע��:
    // Sound�ڵ����ڱ�ʾ3D�ռ��еķ�����Դ.
    // Sound�ڵ�洢�����Դ��ص�������Դ�������,
    // Sound�༰����������ֻ����Ϊ������ͼ����Ҷ�ӽڵ�.
    // �ڵ������任��������Դ�Ŀռ䶨λ��̬.
    // �û�������ȷ���ڵ�Ŀռ�任��������ͳһ���ű任.
    // �ڵ��ģ��/����ռ�BV��һ���뾶Ϊ0���ʵ�.
    // ������Դ����������,���ǲ�Ӧ��ʹ��view frustum����Sound�ڵ���п����޳�,
    // ��Ӧ�ø���������ڿռ���Listener����Ŀռ���̬,����,�Լ�ָ���Խ����޳�,
    // Ŀǰ����Ƶ����API����ɴ˹���.���Sound�ڵ��CullingӦ����ΪCULL_NEVER,
    // �Ӷ�ȷ���䲻������ͼ�ι��ߵ�view frustum�޳�.
    //
    // ��ʹ��PCM wave��Ϊ��Դ����ʱ,ֻ��WT_MONO*��ʽ֧��3D�ռ�������λ.
    // ������������ʽ���޷������û��������Ļ���Sound�ڵ�����Listener�����
    // �ռ�������λ.
    //
    // Ŀǰ���Ǽ�����ԴΪ��̬��Դ,��:
    // ��������Դ�˶������ٶȴ�С�������Լ������ղ���,�Ӷ����Զ�����ЧӦ.
    // ���ڶ�����ЧӦ:
    // The Doppler effect (or Doppler shift), named after Austrian physicist
    // Christian Doppler who proposed it in 1842, is the change in frequency
    // of a wave for an observer moving relative to the source of the waves.
    // It is commonly heard when a vehicle sounding a siren approaches, passes
    // and recedes from an observer. The received frequency is increased
    // (compared to the emitted frequency) during the approach, it is identical
    // at the instant of passing by, and it is decreased during the recession.
    // For waves that propagate in a medium, such as sound waves, the velocity
    // of the observer and of the source are relative to the medium in which the
    // waves are transmitted. The total Doppler effect may therefore result from
    // motion of the source, motion of the observer, or motion of the medium.
    // Each of these effects is analyzed separately. For waves which do not
    // require a medium, such as light or gravity in special relativity, only
    // the relative difference in velocity between the observer and the source
    // needs to be considered.
    // from: http://en.wikipedia.org/wiki/Doppler_effect

public:
    Sound(SoundBuffer* pSBuffer);
    virtual ~Sound(void);

    // ��Ա����.
    SoundBufferPtr SBuffer;
    BoundingVolumePtr ModelBound;

    // ��������.
    float Pitch;        // default: 1.0f
    float Gain;         // default: 1.0f
    float RollOffRate;  // default: 1.0f
    bool Looping;       // default: false

    // Picking support.
    // ������������ά������������������.
    class SE_AUDIO_API PickRecord : public Spatial::PickRecord
    {
    protected:
        PickRecord(Sound* pIObject, float fT);
    };

protected:
    Sound(void);

    // ����ռ����ݼ������Ƶ��������.
    virtual void UpdateWorldData(double dAppTime);
    virtual void UpdateWorldBound(void);

    // ��Ⱦ״̬����,ʵ�ֻ���ӿ�.
    // ����Sound��,����һ���պ���,
    // ��ΪSound�����ռ����ι��ߵ���Ⱦ״̬�Ǻ��������.
    virtual void UpdateState(std::vector<GlobalState*>* aGStack,
        std::vector<Light*>* pLStack);

    // ֧���޳�ϵͳ,�ռ�����Ⱦ����.
    virtual void GetUnculledSet(Culler& rCuller, bool bNoCull);
};

typedef SESmartPointer<Sound> SoundPtr;

}

#endif
