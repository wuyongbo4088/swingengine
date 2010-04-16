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

#ifndef Swing_DefaultMRT3Effect_H
#define Swing_DefaultMRT3Effect_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEShaderEffect.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:default multiple render targets 3 effect��
// ˵��:ʹ�õ�pixel shader���3��color4��color0,color1,color2.
//    ʾ��MRT����.
// ����:Sun Che
// ʱ��:20090309
//----------------------------------------------------------------------------
class SE_FOUNDATION_API DefaultMRT3Effect : public ShaderEffect
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    DefaultMRT3Effect(const std::string& rBaseName);
    virtual ~DefaultMRT3Effect(void);

protected:
    // streaming
    DefaultMRT3Effect(void);
};

typedef SESmartPointer<DefaultMRT3Effect> DefaultMRT3EffectPtr;

}

#endif
