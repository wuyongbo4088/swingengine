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

#ifndef Swing_ParticleController_H
#define Swing_ParticleController_H

#include "SEFoundationLIB.h"
#include "SEController.h"
#include "SEParticles.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:Particle controller����
// ˵��:
// ����:Sun Che
// ʱ��:20090602
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SEParticleController : public SEController
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    virtual ~SEParticleController(void);

    // system motion,��local����ϵ��.�����ٶ�����ӦΪ��λ����.
    float SystemLinearSpeed;
    float SystemAngularSpeed;
    SEVector3f SystemLinearAxis;
    SEVector3f SystemAngularAxis;

    // point motion,��system��ģ�Ϳռ���.�����ٶ�����ӦΪ��λ����.
    // ���ڰ���Щ���ӵ���һ��rigid body(����)��ʹ�õ��û�,
    // ����ѡ��system��ԭ����Ϊ��Щ���ӵ���������,ѡ��system����������Ϊ
    // inertia tensor(��������)��principal directions(������).
    inline float* PointLinearSpeed(void);
    inline SEVector3f* PointLinearAxis(void);

    // system size change����.
    float SystemSizeChange;
    inline float* PointSizeChange(void);

    // ��������,�����������,�Ӷ�ʵ���Զ��������Ϊ.
    virtual bool Update(double dAppTime);

protected:
    SEParticleController(void);

    // �����ӳٷ���point motion arrays.
    void Reallocate(int iVertexCount);
    virtual void SetObject(SEObject* pObject);

    // ����motion����,�����������.
    virtual void UpdateSystemMotion(float fCtrlTime);
    virtual void UpdatePointMotion(float fCtrlTime);

    // point motion(��system��ģ�Ϳռ���).
    int m_iLCount;
    float* m_afPointLinearSpeed;
    SEVector3f* m_aPointLinearAxis;

    // point size change����.
    float* m_afPointSizeChange;
};

typedef SESmartPointer<SEParticleController> SEParticleControllerPtr;

#include "SEParticleController.inl"

}

#endif
