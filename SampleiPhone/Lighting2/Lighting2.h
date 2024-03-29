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

#ifndef Swing_Lighting2_H
#define Swing_Lighting2_H

#include "SEWindowApplication3.h"

using namespace Swing;

class Lighting2 : public SEWindowApplication3
{
    SE_DECLARE_INITIALIZE;

public:
    Lighting2(void);

    virtual bool OnInitialize(void);
    virtual void OnTerminate(void);
    virtual void OnIdle(void);

protected:
    void CreateScene(void);
    void CreateLights(void);
    void CreateModels(void);

    SENodePtr m_spScene;
    SENodePtr m_spModelRoot;
    SEWireframeStatePtr m_spWireframe;
    SECuller m_Culler;

    float m_fLight0Height;
    SEColorRGB m_Light0Color;
    SELightPtr m_spLight0;
    SELightNodePtr m_spLight0Node;

    float m_fLight1Height;
    SEColorRGB m_Light1Color;
    SELightPtr m_spLight1;
    SELightNodePtr m_spLight1Node;
};

#endif
