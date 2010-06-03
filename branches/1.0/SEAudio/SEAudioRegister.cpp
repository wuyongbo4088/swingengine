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
#include "SEAudioRegister.h"

// Audio Rendering
#include "SEWave.h"
#include "SEListener.h"
// Scene Graph
#include "SESoundBuffer.h"
#include "SESound.h"

using namespace Swing;

//----------------------------------------------------------------------------
void Swing::SE_Audio_Register()
{
    // Audio Rendering
    SE_REGISTER_STREAM(SEWave);
    SE_REGISTER_STREAM(SEListener);
    SE_INVOKE_STREAM(SEWave);
    SE_INVOKE_STREAM(SEListener);

    // Scene Graph
    SE_REGISTER_STREAM(SESoundBuffer);
    SE_REGISTER_STREAM(SESound);
    SE_INVOKE_STREAM(SESoundBuffer);
    SE_INVOKE_STREAM(SESound);
}
//----------------------------------------------------------------------------