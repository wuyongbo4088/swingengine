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

#ifndef Swing_OGLES1FixedEffect_H
#define Swing_OGLES1FixedEffect_H

#include "SEOGLES1RendererLIB.h"
#include "SEEffect.h"
#include "SETexture.h"
#include "SELight.h"

namespace Swing
{

class SERenderer;

//----------------------------------------------------------------------------
// ˵��:
// ����:Sun Che
// ʱ��:20090610
//----------------------------------------------------------------------------
class SE_RENDERER_API SEOGLES1FixedEffect : public SEEffect
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    SEOGLES1FixedEffect(int iTextureCount, int iLightCount);
    virtual ~SEOGLES1FixedEffect(void);

    // texture����.
    int GetTextureCount(void) const;
    SETexture* GetTexture(int i);
    const SETexture* GetTexture(int i) const;
    void SetTexture(int i, SETexture* pTexture);
    const std::string& GetImageName(int i) const;
    void SetImageName(int i, const std::string& rName);

    // light����.
    int GetLightCount(void) const;
    SELight* GetLight(int i);
    const SELight* GetLight(int i) const;
    void SetLight(int i, SELight* pLight);

    // ��Щ������SERenderer::LoadResources��SERenderer::ReleaseResources����.
    // Ҳ������Ӧ�ó��������û��ֶ����ó�ʼ��������Դ.
    virtual void LoadResources(SERenderer* pRenderer, SEGeometry* pGeometry);
    virtual void ReleaseResources(SERenderer* pRenderer, SEGeometry* 
        pGeometry);

protected:
    // streaming support.
    SEOGLES1FixedEffect(void);

    // textures.
    std::vector<std::string> m_ImageNames;
    std::vector<SETexturePtr> m_Textures;

    // lights.
    std::vector<SELightPtr> m_Lights;
};

typedef SESmartPointer<SEOGLES1FixedEffect> SEOGLES1FixedEffectPtr;

}

#endif
