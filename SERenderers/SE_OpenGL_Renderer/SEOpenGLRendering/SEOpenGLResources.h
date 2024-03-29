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

#ifndef Swing_OpenGLResources_H
#define Swing_OpenGLResources_H

#include "SEOpenGLRendererLIB.h"
#include "SEOpenGLRenderer.h"
#include "SEAttributes.h"
#include "SEBindable.h"
#include "SEOpenGLProgramInterface.h"

namespace Swing
{

class SE_RENDERER_API SEVProgramID : public SEResourceIdentifier
{
public:
    CGprogram ID;
};

class SE_RENDERER_API SEPProgramID : public SEResourceIdentifier
{
public:
    CGprogram ID;
};

class SE_RENDERER_API SETextureID : public SEResourceIdentifier
{
public:
    unsigned int ID;
    SETexture* TextureObject;
};

class SE_RENDERER_API SEVBufferID : public SEResourceIdentifier
{
public:
    SEAttributes IAttr;  // multipass时,用于shader输入属性匹配性检测.
    SEAttributes OAttr;
    unsigned int ID;
};

class SE_RENDERER_API SEIBufferID : public SEResourceIdentifier
{
public:
    unsigned int ID;
};

class SE_RENDERER_API SEProgramData
{
public:
    SEProgramData(void){ ID = 0; }

    CGprogram ID;

    // 该程序所需的程序接口.
    std::vector<SEOpenGLProgramInterfacePtr> Interfaces;
};

}

#endif
