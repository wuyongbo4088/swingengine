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

#include "SEFoundationPCH.h"
#include "SELightingEffect.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SELightingEffect, SEShaderEffect);
SE_IMPLEMENT_STREAM(SELightingEffect);
SE_IMPLEMENT_DEFAULT_STREAM(SELightingEffect, SEShaderEffect);
SE_IMPLEMENT_DEFAULT_NAME_ID(SELightingEffect, SEShaderEffect);

//SE_REGISTER_STREAM(SELightingEffect);

SELightingEffect::ConfigureLighting SELightingEffect::OnConfigureLighting = 0;

//----------------------------------------------------------------------------
SELightingEffect::SELightingEffect()
    :
    SEShaderEffect(1)
{
    // ���û��light,��Configure��������رո�effect.
}
//----------------------------------------------------------------------------
SELightingEffect::~SELightingEffect()
{
}
//----------------------------------------------------------------------------
void SELightingEffect::AttachLight(SELight* pLight)
{
    SE_ASSERT( pLight );

    // ����Ƿ��Ѿ����ڸ�light.
    for( int i = 0; i < (int)m_Lights.size(); i++ )
    {
        if( m_Lights[i] == pLight )
        {
            // �����򷵻�.
            return;
        }
    }

    // �����������.
    m_Lights.push_back(pLight);
}
//----------------------------------------------------------------------------
void SELightingEffect::DetachLight(SELight* pLight)
{
    std::vector<SELightPtr>::iterator pIter = m_Lights.begin();
    for( /**/; pIter != m_Lights.end(); pIter++ )
    {
        if( pLight == *pIter )
        {
            m_Lights.erase(pIter);

            return;
        }
    }
}
//----------------------------------------------------------------------------
void SELightingEffect::Configure(LightingMode eMode)
{
    m_eMode = eMode;

    SE_ASSERT( OnConfigureLighting );
    if( OnConfigureLighting )
    {
        OnConfigureLighting(this);
    }
}
//----------------------------------------------------------------------------