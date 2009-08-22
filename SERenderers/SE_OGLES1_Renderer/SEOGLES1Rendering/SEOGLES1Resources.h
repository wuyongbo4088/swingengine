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

#ifndef Swing_OGLES1Resources_H
#define Swing_OGLES1Resources_H

#include "SEOGLES1RendererLIB.h"
#include "SEOGLES1Renderer.h"
#include "SEAttributes.h"
#include "SEBindable.h"

namespace Swing
{

class SE_RENDERER_API TextureID : public ResourceIdentifier
{
public:
    unsigned int ID;
    Texture* TextureObject;
};

class SE_RENDERER_API VBufferID : public ResourceIdentifier
{
public:
    // û��shader����,����Ҫ��֤IAttr��OAttr��ƥ������.
    Attributes IAttr;
    Attributes OAttr;
    unsigned int ID;
};

class SE_RENDERER_API IBufferID : public ResourceIdentifier
{
public:
    unsigned int ID;
};

class SE_RENDERER_API SamplerInformationID
{
public:
    unsigned int TextureUnit;
};
	
}

#endif
