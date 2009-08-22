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

#ifndef Swing_Listener_H
#define Swing_Listener_H

#include "SEAudioLIB.h"
#include "SEObject.h"

// ���ǵ���������ϵΪ��������ϵ{Origin;X,Y,Z}:
// Ĭ��Origin = (0,0,0)
// Ĭ��X��    = (1,0,0)
// Ĭ��Y��    = (0,1,0)
// Ĭ��Z��    = (0,0,1)

// ����ʹ��:
// (1) ��������ϵ
// (2) ����������תtheta��,theta > 0ʱΪ˳ʱ����ת
// (3) 3ά�����������ʹ������ԭ��

namespace Swing
{

//----------------------------------------------------------------------------
// ����:listener����
// ˵��:
// ����:Sun Che
// ʱ��:20090618
//----------------------------------------------------------------------------
class SE_AUDIO_API Listener : public Object
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    Listener(void);
    virtual ~Listener(void);

    // listener{E;R,U,D}����������ϵ�±�ʾ:
    // Ĭ��location  E = (0,0,0)
    // Ĭ��right     R = (1,0,0)
    // Ĭ��up        U = (0,1,0)
    // Ĭ��direction D = (0,0,1)

    void SetFrame(const Vector3f& rLocation, const Vector3f& rRVector,
        const Vector3f& rUVector, const Vector3f& rDVector);
    void SetLocation(const Vector3f& rLocation);
    void SetAxes(const Vector3f& rRVector, const Vector3f& rUVector, 
        const Vector3f& rDVector);

    inline Vector3f GetLocation(void) const;
    inline Vector3f GetRVector(void) const;
    inline Vector3f GetUVector(void) const;
    inline Vector3f GetDVector(void) const;

    void SetMasterGain(float fMasterGain);
    inline float GetMasterGain(void) const;

protected:
    // listener E;R,U,D����������ϵ��ʾ.
    Vector3f m_Location, m_RVector, m_UVector, m_DVector;

    // master gain.
    float m_fMasterGain;  // default: 1.0f

    // ��ǰlistener������������Ⱦ��.
    // �ǿ�ʱ��ʾ��ǰlistener���ڼ���״̬,
    // listener����ͬʱ��Ӹ����������Ⱦ��,�������ڶ��������Ⱦ��֮�乲��,
    // ǰ����ȷ��ֻ��һ��������Ⱦ������ʹ�ø�listener,
    // ������Ⱦ�����������������Ա����.
    friend class AudioRenderer;
    AudioRenderer* m_pAudioRenderer;
};

typedef SmartPointer<Listener> ListenerPtr;

#include "SEListener.inl"

}

#endif
