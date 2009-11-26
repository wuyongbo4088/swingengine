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

#ifndef Swing_SkinDefaultEffect_H
#define Swing_SkinDefaultEffect_H

#include "SEFoundationLIB.h"
#include "SEShaderEffect.h"

namespace Swing
{

class Node;
class Transformation;

//----------------------------------------------------------------------------
// ����:skin default effect��
// ˵��:��GPU������Ƥ�������.���Ĭ��������ɫ.
// ����:Sun Che
// ʱ��:20080904
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SkinDefaultEffect : public ShaderEffect
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    SkinDefaultEffect(int iBoneCount, Node** apBones, Transformation* aOffset);
    virtual ~SkinDefaultEffect(void);

protected:
    // streaming
    SkinDefaultEffect(void);

    virtual void OnLoadPrograms(int iPass, Program* pVProgram,
        Program* pPProgram, Program* pGProgram);
    virtual void OnPreApplyEffect(Renderer* pRenderer, bool bPrimaryEffect);

    int m_iBoneCount;          // bc
    Node** m_apBones;          // bones[bc]
    Transformation* m_aOffset; // offset[bc]

    // skin matrix�������
    enum { SM_COUNT = 32 };
    // ��Ը�profile,skin matrixʵ�ʿ�������.
    enum 
    { 
        VS_2_0_COUNT = 32,
        VS_3_0_COUNT = 32,
        ARBVP1_COUNT = 22,
        VP40_COUNT = 32,
        OES2VP1_COUNT = 24,
        SFTVP1_COUNT = 22
    };
    static Matrix4f ms_aSkinMatrix[SM_COUNT];
    static bool ms_bSkinMatrixUCInitialized;
};

typedef SmartPointer<SkinDefaultEffect> SkinDefaultEffectPtr;

}

#endif
