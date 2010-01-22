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

// 我们的世界坐标系为左手坐标系{Origin;X,Y,Z}:
// 默认Origin = (0,0,0)
// 默认X轴    = (1,0,0)
// 默认Y轴    = (0,1,0)
// 默认Z轴    = (0,0,1)

// 我们使用:
// (1) 左手坐标系
// (2) 绕任意轴旋转theta度,theta > 0时为顺时针旋转
// (3) 3维向量叉积方向使用左手原则

namespace Swing
{

//----------------------------------------------------------------------------
// 名称:摄像机基类
// 说明:
// 作者:Sun Che
// 时间:20080707
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Camera : public Object
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    Camera(void);
    virtual ~Camera(void);

    // 摄像机{E;R,U,D}的世界坐标系下表示:
    // 默认location  E = (0,0,0)
    // 默认right     R = (1,0,0)
    // 默认up        U = (0,1,0)
    // 默认direction D = (0,0,1)

    void SetFrame(const Vector3f& rLocation, const Vector3f& rRVector,
        const Vector3f& rUVector, const Vector3f& rDVector);
    void SetLocation(const Vector3f& rLocation);
    void SetAxes(const Vector3f& rRVector, const Vector3f& rUVector, 
        const Vector3f& rDVector);

    inline Vector3f GetLocation(void) const;
    inline Vector3f GetRVector(void) const;
    inline Vector3f GetUVector(void) const;
    inline Vector3f GetDVector(void) const;

    // view frustum参数枚举.
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

    // 设置view frustum参数.
    void SetFrustum(float fRMin, float fRMax, float fUMin, float fUMax,
        float fDMin, float fDMax);

    void SetFrustum(const float* pFrustum);

    // 对称view frustum(umin = -umax, rmin = -rmax),
    // 可以使用U轴方向FOV(field of view),aspect ratio,Dmin,Dmax表示.
    void SetFrustum(float fUpFovDegrees, float fAspectRatio, float fDMin,
        float fDMax);

    // 获取view frustum参数.
    void GetFrustum(float& rRMin, float& rRMax, float& rUMin,
        float& rUMax, float& rDMin, float& rDMax) const;

    // 获取view frustum参数.
    inline const float* GetFrustum(void) const;

    // 获取view frustum参数,
    // 检测是否为对称view frustum,
    // 返回值为true时,获取值才合法.
    bool GetFrustum(float& rUpFovDegrees, float& rAspectRatio,
        float& rDMin, float& rDMax) const;

    // 获取view frustum参数.
    inline float GetDMin(void) const;
    inline float GetDMax(void) const;
    inline float GetUMin(void) const;
    inline float GetUMax(void) const;
    inline float GetRMin(void) const;
    inline float GetRMax(void) const;

    // Perspective/orthogonal projection access.
    void SetPerspective(bool bPerspective);
    inline bool IsPerspective(void) const;

    // Viewport access(区间为[0,1]^2).
    void SetViewport(float fLeft, float fRight, float fTop, float fBottom);
    void GetViewport(float& rLeft, float& rRight, float& rTop,
        float& rBottom);

    // Depth range access(区间为[0,1]).
    void SetDepthRange(float fNear, float fFar);
    void GetDepthRange(float& rNear, float& rFar);

    // Mouse picking support. The (x,y) input point is in left-handed screen
    // coordinates (what you usually read from the windowing system). The
    // function returns 'true' if and only if the input point is located in
    // the current viewport. When 'true', the origin and direction values are
    // valid and are in world coordinates. The direction vector is unit length.
    bool GetPickRay(int iX, int iY, int iWidth, int iHeight, Ray3f& rRay)
        const;

    // Virtual track ball rotation support. (x0,y0) and (x1,y1) input points
    // are in a right-handed coordinates(the origin is at the center of window
    // screen, which is (width/2,height/2). Let r = width/height, then any 
    // point (x,y) on the screen is in the range x:[-r,+r] y:[-1,+1]). The
    // function returns 'true' if and only if the two points are different.
    // When 'true', the rotation matrix is valid and is in world coordinates.
    bool GetTrackBallRotate(float fX0, float fY0, float fX1, float fY1, 
        Matrix3f& rMat) const;

protected:
    // 摄像机E;R,U,D的世界坐标系表示.
    Vector3f m_Location, m_RVector, m_UVector, m_DVector;

    // 截投体参数(near, far, bottom, top, left, right).
    float m_Frustum[VF_COUNT];

    // 默认为true表示perspective camera.
    // TO DO.  Stream this member
    bool m_bPerspective;

    // 视口参数,
    // 暂时使用OpenGL风格,以后改为DirectX风格.
    float m_fPortL, m_fPortR, m_fPortT, m_fPortB;

    // 深度范围参数.
    float m_fPortN, m_fPortF;

    // 当前摄像机所属的渲染器.
    // 非空时表示当前摄像机处于激活状态,
    // 摄像机不能同时添加给多个渲染器,但可以在多个渲染器之间共享,
    // 前提是确保只有一个渲染器正在使用该摄像机,
    // 渲染器有责任设置这个成员变量.
    friend class Renderer;
    Renderer* m_pRenderer;
};

typedef SmartPointer<Camera> CameraPtr;

#include "SECamera.inl"

}

#endif
