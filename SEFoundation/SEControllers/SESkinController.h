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

#ifndef Swing_SkinController_H
#define Swing_SkinController_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEController.h"
#include "SEVector3.h"

namespace Swing
{

class SENode;

//----------------------------------------------------------------------------
// ����:��Ƥ��������
// ˵��:��CPU������Ƥ�������.
// ����:Sun Che
// ʱ��:20080826
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SESkinController : public SEController
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    // SESkinController����ɾ�����캯���������Щ����.
    // weight��offset����������Ѿ�ͨ��SESystem.h�е�ģ�庯������������.
    SESkinController(int iVertexCount, int iBoneCount, SENode** apBones,
        float** aafWeight, SEVector3f** aaOffset);
    virtual ~SESkinController(void);

    inline int GetVertexCount(void) const;
    inline int GetBoneCount(void) const;
    inline SENode* GetBone(int iBone) const;
    inline float& Weight(int iVertex, int iBone);
    inline SEVector3f& Offset(int iVertex, int iBone);

    virtual bool Update(double dAppTime);

protected:
    SESkinController(void);

    int m_iVertexCount;    // vc
    int m_iBoneCount;      // bc
    SENode** m_apBones;      // bones[bc]
    float** m_aafWeight;   // weight[vc][bc]
    SEVector3f** m_aaOffset; // offset[vc][bc]
};

typedef SESmartPointer<SESkinController> SESkinControllerPtr;
#include "SESkinController.inl"

}

#endif
