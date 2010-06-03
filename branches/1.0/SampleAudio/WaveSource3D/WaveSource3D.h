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

#ifndef Swing_WaveSource3D_H
#define Swing_WaveSource3D_H

#include "SEWindowApplication3.h"

using namespace Swing;

class WaveSource3D : public SEWindowApplication3
{
    SE_DECLARE_INITIALIZE;

public:
    WaveSource3D(void);

    virtual bool OnInitialize(void);
    virtual void OnTerminate(void);
    virtual void OnIdle(void);
    virtual bool OnKeyDown(unsigned char ucKey, int iX, int iY);
    virtual bool OnMouseClick(int iButton, int iState, int iX, int iY,
        unsigned int uiModifiers);

protected:
    void CreateScene(void);

    // arrange for camera to be fixed distance above the nearest object.
    void AdjustVerticalDistance(void);
    virtual void MoveForward(void);
    virtual void MoveBackward(void);
    float m_fVerticalDistance;

    enum{ SOUND_COUNT = 5 };

    SENodePtr m_spScene;
    SENodePtr m_aspSoundNode[SOUND_COUNT];
    SESoundPtr m_aspSound[SOUND_COUNT];
    bool m_bSoundSwitch[SOUND_COUNT];
    SELightPtr m_spLight;
    SETriMeshPtr m_spGround;
    SEWireframeStatePtr m_spWireframe;
    SECuller m_Culler;

    SESoundBufferPtr m_spSBufferGunfire;
    SESoundBufferPtr m_spSBufferAfflictionAlert;
    SESoundBufferPtr m_spSBufferFootsteps;
    SESoundBufferPtr m_spSBufferGunhit;
    SESoundBufferPtr m_spSBufferBlip;
    SESoundBufferPtr m_spSBufferKillZone2;

    // ����ʰȡ��������ز���.
    bool AllowMotion(float fSign);
    int m_iNumRays;
    float m_fHalfAngle;
    float* m_afCos;
    float* m_afSin;
    float* m_afTolerance;
};

#endif