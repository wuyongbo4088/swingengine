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

#ifndef Swing_KeyFrameController_H
#define Swing_KeyFrameController_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEController.h"
#include "SEFloatArray.h"
#include "SEQuaternionArray.h"
#include "SEVector3Array.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:�ؼ����������
// ˵��:
// ����:Sun Che
// ʱ��:20080825
//----------------------------------------------------------------------------
class SE_FOUNDATION_API KeyframeController : public Controller
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    KeyframeController(void);
    virtual ~KeyframeController(void);

    FloatArrayPtr TranslationTimes;
    Vector3fArrayPtr TranslationData;

    FloatArrayPtr RotationTimes;
    QuaternionfArrayPtr RotationData;

    FloatArrayPtr ScaleTimes;
    FloatArrayPtr ScaleData;

    virtual bool Update(double dAppTime);

protected:
    static void GetKeyInfo(float fCtrlTime, int iCount, float* afTime,
        int& riLastIndex, float& rfNormTime, int& ri0, int& ri1);

    Vector3f GetTranslate(float fNormTime, int i0, int i1);
    Matrix3f GetRotate(float fNormTime, int i0, int i1);
    float GetScale(float fNormTime, int i0, int i1);

    // �ݴ��ϴβ�ֵʱʹ�õĹؼ����,
    // ����һ��ʱ��������˵,�⽫ȷ�����ҹؼ���Ե�ʱ�临�Ӷ�ΪO(1).
    int m_iTLastIndex;
    int m_iRLastIndex;
    int m_iSLastIndex;
};

typedef SmartPointer<KeyframeController> KeyframeControllerPtr;

}

#endif
