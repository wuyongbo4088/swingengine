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

#ifndef Swing_PostProcess_H
#define Swing_PostProcess_H

#include "SEWindowApplication3.h"

using namespace Swing;

class PostProcess : public WindowApplication3
{
    SE_DECLARE_INITIALIZE;

public:
    PostProcess(void);

    virtual bool OnInitialize(void);
    virtual void OnTerminate(void);
    virtual void OnIdle(void);
    virtual bool OnKeyDown(unsigned char ucKey, int iX, int iY);

protected:
    void CreateScene(void);
    void CreateTeapot(void);
    void CreateLights(void);

    NodePtr m_spScene;
    TriMeshPtr m_spTeapot;
    WireframeStatePtr m_spWireframe;
    Culler m_Culler;

    CameraPtr m_spScreenCamera;
    ImagePtr m_spSceneImage;
    TriMeshPtr m_spScenePolygon1;
    TriMeshPtr m_spScenePolygon2;
    TriMeshPtr m_spScenePolygon3;
    Texture* m_pSceneTarget1;
    Texture* m_pSceneTarget2;
    Texture* m_pSceneTarget3;
    FrameBuffer* m_pFrameBuffer1;
    FrameBuffer* m_pFrameBuffer2;
    FrameBuffer* m_pFrameBuffer3;

    LightPtr m_aspLight[2];

    bool m_bPostProcess;
};

#endif
