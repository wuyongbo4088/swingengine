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

#ifndef Swing_DX10Resources_H
#define Swing_DX10Resources_H

#include "SEDX10RendererLIB.h"
#include "SEDX10Utility.h"
#include "SEAttributes.h"
#include "SEBindable.h"
#include "SEDX10ProgramInterface.h"

namespace Swing
{

class SE_RENDERER_API SEVProgramID : public SEResourceIdentifier
{
public:
    CGprogram ID;
};

class SE_RENDERER_API SEGProgramID : public SEResourceIdentifier
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
    ID3D10Resource* ID;
    SETexture* TextureObject;
};

class SE_RENDERER_API SEVBufferID : public SEResourceIdentifier
{
public:
    SEAttributes IAttr;  // multipassʱ,����shader��������ƥ���Լ��.
    SEAttributes OAttr;
    ID3D10Buffer* ID;
    UINT VertexSize;
    UINT Offset;
    ID3D10InputLayout* Layout;
};

class SE_RENDERER_API SEIBufferID : public SEResourceIdentifier
{
public:
    ID3D10Buffer* ID;
};

class SE_RENDERER_API SERStateBlockID : public SEResourceIdentifier
{
public:
    ID3D10BlendState* BlendState;
    ID3D10DepthStencilState* DepthStencilState;
    ID3D10RasterizerState* RasterizerState;
    SERenderStateBlock* RStateBlock;
};

class SE_RENDERER_API SEProgramData
{
public:
    SEProgramData(void){ ID = 0; }

    CGprogram ID;
    std::vector<SEDX10ProgramInterfacePtr> Interfaces;
};

}

#endif
