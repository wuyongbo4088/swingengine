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

#pragma once

using namespace System;

namespace Swing{ namespace Tools{ namespace ManagedFramework{

//----------------------------------------------------------------------------
// Description:
// Author:Sun Che
// Date:20091224
//----------------------------------------------------------------------------
public ref class ManagedColorRGBA sealed
{
public:
    ManagedColorRGBA(void);
    ManagedColorRGBA(float fR, float fG, float fB, float fA);

    // Properties.
    property float R
    {
        float get(void);
        void set(float fR);
    }
    property float G
    {
        float get(void);
        void set(float fG);
    }
    property float B
    {
        float get(void);
        void set(float fB);
    }
    property float A
    {
        float get(void);
        void set(float fA);
    }

internal:
    [CLSCompliant(false)]
    ManagedColorRGBA(const SEColorRGBA& rColor);

    [CLSCompliant(false)]
    void ToColorRGBA(SEColorRGBA& rColor);

    [CLSCompliant(false)]
    void FromColorRGBA(const SEColorRGBA& rColor);

private:
    float m_fR;
    float m_fG;
    float m_fB;
    float m_fA;
};

}}}