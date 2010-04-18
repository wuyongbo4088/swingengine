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

#ifndef Swing_Light_H
#define Swing_Light_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEObject.h"
#include "SEColorRGB.h"
#include "SEVector3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ˵��:
// ����:Sun Che
// ʱ��:20080702
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SELight : public SEObject
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    enum LightType
    {
        LT_AMBIENT,
        LT_DIRECTIONAL,
        LT_POINT,
        LT_SPOT,
        LT_COUNT
    };

    SELight(LightType eType = LT_AMBIENT);
    virtual ~SELight(void);

    // ���SELight��û������,��ֻ�л�������ɫ��ǿ��,
    // �����ʹ��һ����׼����㼶��ϵ:
    //
    //   class SELight
    //       [ambient, intensity]
    //   class SEAmbientLight : public SELight
    //       [û�ж�������]
    //   class SEDirectionalLight : public SELight
    //       [direction, diffuse, specular]
    //   class SEPointLight : public SELight
    //       [position, diffuse, specular, attenuation]
    //   class SESpotLight : public SEPointLight
    //       [cone axis, cone angle, spot exponent]
    //
    // ��Ⱦ��ͨ������SELight����������light����.
    // ʹ�����ϱ�׼��㼶��ϵ,��Ⱦ������ͨ����̬����ת�����жϾ���ĵƹ�����,
    // ֮����ݾ���ƹ�����,ͨ��SERenderer::SetConstantLightFOOBARϵ�к���,
    // ����shader������,ϣ���ܹ��������ֵ�Ч�ʵ�����.
    //
    // ��һ��������,����SELight����public������洢���еƹ����������,
    // ʹ��һ���������Ļ���SELight���������ض���SELight����.
    // ���,��Ⱦ���ڲ�ʹ�ö�̬����ת��������¿��Է��ʵ����еƹ�����,
    // ����������ͨ�����Ա��������������������صĵƹ�����.
    // �����ҵ���,�������SEObject���Ա����Ȩ���޵�����(��������ָ��������������).
    //
    // ����,����ʹ������ȫ���ŵĲ���,
    // SELight����Ϊһ��ͨ����,�洢������������ĸ�������.

    LightType Type;     // default: LT_AMBIENT

    // The colors of the light.
    SEColorRGB Ambient;   // default: SEColorRGB(0,0,0)
    SEColorRGB Diffuse;   // default: SEColorRGB(0,0,0)
    SEColorRGB Specular;  // default: SEColorRGB(0,0,0)

    // Attenuation is typically specified as a modulator
    //   m = 1/(C + L*d + Q*d*d)
    // where C is the constant coefficient, L is the linear coefficient,
    // Q is the quadratic coefficient, and d is the distance from the light
    // position to the vertex position.  To allow for a linear adjustment of
    // intensity, my choice is to use instead
    //   m = I/(C + L*d + Q*d*d)
    // where I is an "intensity" factor.
    float Constant;     // default: 1
    float Linear;       // default: 0
    float Quadratic;    // default: 0
    float Intensity;    // default: 1

    // Parameters for spot lights.  The cone angle must be in radians and
    // should satisfy 0 < Angle <= pi.
    float Angle;        // default: pi
    float CosAngle;     // default: -1
    float SinAngle;     // default:  0
    float Exponent;     // default:  1

    // A helper function that lets you set Angle and have CosAngle and
    // SinAngle computed for you.
    void SetAngle(float fAngle);

    // ���ܱ�׼�ķ����;۹�ƶ�ֻ��Ҫһ����������,
    // ��Ϊ��֧��ʹ����������ϵ���µ���������,������SELight�����д洢���������ϵ.
    // �ƹ�����ϵ��ʾ���ǻ�����������ϵ.
    //   default position  P = (0,0,0)
    //   default right     R = (1,0,0)
    //   default up        U = (0,1,0)
    //   default direction D = (0,0,1)
    // {P:R,U,D}������������ϵ.
    SEVector3f Position, RVector, UVector, DVector;

    // ���÷�������D,ͬʱ�Զ����㲢����R,U.
    void SetDirection(const SEVector3f& rDirection, bool bUnitLength = true);

    // ��debugģʽʹ��,�ж��Ƿ�Ϊһ����Ч�����ֹ淶�����.
    bool IsValidFrame(void) const;

protected:
};

typedef SESmartPointer<SELight> SELightPtr;

}

#endif
