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

#ifndef Swing_Effect_H
#define Swing_Effect_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEObject.h"

namespace Swing
{

class Spatial;
class Geometry;
class Renderer;
class UnculledObject;

//----------------------------------------------------------------------------
// ����:effect����
// ˵��:
// ����:Sun Che
// ʱ��:20080702
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Effect : public Object
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    // �����
    virtual ~Effect(void);

    // ������������Ӷ��ɻ��effect��������Զ�����Ⱦ��Ϊ.
    // Ĭ���������������ȾpVisibleSet�е�geometry����.
    virtual void Draw(Renderer* pRenderer, Spatial* pGlobalObject,
        int iMin, int iMax, UnculledObject* pVisibleSet);

    // ������������Ӷ����Ը���effect������������Զ�������.
    virtual void OnUpdateData(void* pUserData);

    // ������Щ�����Ӷ��ɻ��effect��������Զ�����Դװ��.
    // Ĭ�������ʲô������.
    // ��Щ������Renderer::LoadResources��Renderer::ReleaseResources����,
    // ����geometry��effect����.
    virtual void LoadResources(Renderer* pRenderer, Geometry* pGeometry);
    virtual void ReleaseResources(Renderer* pRenderer, Geometry* pGeometry);

protected:
    Effect(void);
};

typedef SmartPointer<Effect> EffectPtr;

}

#endif