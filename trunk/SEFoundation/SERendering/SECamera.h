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

#ifndef Swing_Camera_H
#define Swing_Camera_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEObject.h"
#include "SERay3.h"

// ���ǵ���������ϵΪ��������ϵ{Origin;X,Y,Z}:
// Ĭ��Origin = (0,0,0)
// Ĭ��X��    = (1,0,0)
// Ĭ��Y��    = (0,1,0)
// Ĭ��Z��    = (0,0,1)

// ����ʹ��:
// (1) ��������ϵ
// (2) ����������תtheta��,theta > 0ʱΪ˳ʱ����ת
// (3) 3ά�����������ʹ������ԭ��

namespace Swing
{

//----------------------------------------------------------------------------
// ����:���������
// ˵��:
// ����:Sun Che
// ʱ��:20080707
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Camera : public Object
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    Camera(void);
    virtual ~Camera(void);

    // �����{E;R,U,D}����������ϵ�±�ʾ:
    // Ĭ��location  E = (0,0,0)
    // Ĭ��right     R = (1,0,0)
    // Ĭ��up        U = (0,1,0)
    // Ĭ��direction D = (0,0,1)

    void SetFrame(const Vector3f& rLocation, const Vector3f& rRVector,
        const Vector3f& rUVector, const Vector3f& rDVector);
    void SetLocation(const Vector3f& rLocation);
    void SetAxes(const Vector3f& rRVector, const Vector3f& rUVector, 
        const Vector3f& rDVector);

    inline Vector3f GetLocation(void) const;
    inline Vector3f GetRVector(void) const;
    inline Vector3f GetUVector(void) const;
    inline Vector3f GetDVector(void) const;

    // view frustum����ö��.
    enum
    {
        VF_DMIN     = 0,  // near
        VF_DMAX     = 1,  // far
        VF_UMIN     = 2,  // bottom
        VF_UMAX     = 3,  // top
        VF_RMIN     = 4,  // left
        VF_RMAX     = 5,  // right
        VF_COUNT = 6
    };

    // ����view frustum����.
    void SetFrustum(float fRMin, float fRMax, float fUMin, float fUMax,
        float fDMin, float fDMax);

    void SetFrustum(const float* pFrustum);

    // �Գ�view frustum(umin = -umax, rmin = -rmax),
    // ����ʹ��U�᷽��FOV(field of view),aspect ratio,Dmin,Dmax��ʾ.
    void SetFrustum(float fUpFovDegrees, float fAspectRatio, float fDMin,
        float fDMax);

    // ��ȡview frustum����.
    void GetFrustum(float& rRMin, float& rRMax, float& rUMin,
        float& rUMax, float& rDMin, float& rDMax) const;

    // ��ȡview frustum����.
    inline const float* GetFrustum(void) const;

    // ��ȡview frustum����,
    // ����Ƿ�Ϊ�Գ�view frustum,
    // ����ֵΪtrueʱ,��ȡֵ�źϷ�.
    bool GetFrustum(float& rUpFovDegrees, float& rAspectRatio,
        float& rDMin, float& rDMax) const;

    // ��ȡview frustum����.
    inline float GetDMin(void) const;
    inline float GetDMax(void) const;
    inline float GetUMin(void) const;
    inline float GetUMax(void) const;
    inline float GetRMin(void) const;
    inline float GetRMax(void) const;

    // ����orthogonal camera��perspective camera,
    // Ĭ��Ϊtrue��ʾperspective,
    // TO DO.  Stream this member
    bool Perspective;

    // viewport (����Ϊ[0,1]^2).
    void SetViewport(float fLeft, float fRight, float fTop, float fBottom);
    void GetViewport(float& rLeft, float& rRight, float& rTop,
        float& rBottom);

    // depth range (����Ϊ[0,1]).
    void SetDepthRange(float fNear, float fFar);
    void GetDepthRange(float& rNear, float& rFar);

    // Mouse picking support.  The (x,y) input point is in left-handed screen
    // coordinates (what you usually read from the windowing system).  The
    // function returns 'true' if and only if the input point is located in
    // the current viewport.  When 'true', the origin and direction values
    // are valid and are in world coordinates.  The direction vector is unit
    // length.
    bool GetPickRay(int iX, int iY, int iWidth, int iHeight, Ray3f& rRay)
        const;

protected:
    // �����E;R,U,D����������ϵ��ʾ.
    Vector3f m_Location, m_RVector, m_UVector, m_DVector;

    // ��Ͷ�����(near, far, bottom, top, left, right).
    float m_Frustum[VF_COUNT];

    // �ӿڲ���,
    // ��ʱʹ��OpenGL���,�Ժ��ΪDirectX���.
    float m_fPortL, m_fPortR, m_fPortT, m_fPortB;

    // ��ȷ�Χ����.
    float m_fPortN, m_fPortF;

    // ��ǰ�������������Ⱦ��.
    // �ǿ�ʱ��ʾ��ǰ��������ڼ���״̬,
    // ���������ͬʱ���Ӹ������Ⱦ��,�������ڶ����Ⱦ��֮�乲��,
    // ǰ����ȷ��ֻ��һ����Ⱦ������ʹ�ø������,
    // ��Ⱦ�����������������Ա����.
    friend class Renderer;
    Renderer* m_pRenderer;
};

typedef SmartPointer<Camera> CameraPtr;

#include "SECamera.inl"

}

#endif