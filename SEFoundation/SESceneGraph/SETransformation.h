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

#ifndef Swing_Transformation_H
#define Swing_Transformation_H

#include "SEFoundationLIB.h"
#include "SEMatrix3.h"
#include "SEMatrix4.h"
#include "SEPlane3.h"

// ����任����Y = X*M + T����ʽ,����M��3*3����,T��3άƽ������.
// Mͨ��ΪR����SR�ĳ˻�,S���������ŵĶԽǾ���,R����ת����.
// Y = X*M + T����ʽ����֧�ֽ�ģ���ߵľ���任�ͷ�ͳһ���ű任.
// Y = X*M + T����任��һ����ʽΪX = (Y-T)*M^{-1}.���M = S*R,
// ����任��ת��ΪX = (Y-T)*R^T*S^{-1}.
//
// Transformation�;�����������ӷ�ʽһ��,Ҳ������������.
//
// ִ��Ч�ܷ���(����AMD Athlon64 3000+,1G,ATI X800).
// 1000000�γ˻�����,releaseģʽ,ִ��ʱ��ԼΪ260ms(ͳһ����SRT),
// 440ms(��ͳһ����SRT),DirectX��ξ���ԼΪ50ms.
// 10000�γ˻�����,releaseģʽ,ִ��ʱ�� < 0ms,DirectX��ξ��� < 0ms.
// ���Transformation�����������Ӧ�ò�,��Գ����ڵ�������10000Ϊ�������ľ���
// ����,��DirectX��ξ���û�����Բ���.
// ��������������߲�(��դ��,��ͼ����)�����������DirectX��ξ���.
// ����Transformation��װ����������ŵ�:
//     (1) ���ܹ�ֱ�ӻ�ȡSRT�����������,��任���õ�����.
//     (2) ��������,�ռ�任��API�޹ص�.
// ����Transformation��װ���������ȱ��:������������դ�����ٶȽ���.

namespace Swing
{

//----------------------------------------------------------------------------
// ����:�ռ�任��
// ˵��:��װSRT����任
// ����:Sun Che
// ʱ��:20070523
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Transformation
{
public:
    Transformation(void);
    ~Transformation(void);

    // ���任��Ϊ��λ����任.
    void Identity(void);
    // �����ű任��Ϊͳһ���ű任.
    void UnitScale(void);

    // ��ʾ�任��Ϣ���������ı任����.
    inline bool IsIdentity(void) const;
    inline bool IsSRMatrix(void) const;
    inline bool IsUniformScale(void) const;

    // (1) ����Set*����������m_bIsIdentityΪfalse.
    // (2) SetRotate����������m_bIsSRMatrixΪtrue.����ñ�־Ϊfalse,
    //     ��GetRotate���������.
    // (3) SetMatrix����������m_bIsSRMatrix,m_bIsUniformScaleΪfalse.
    // (4) SetScale����������m_bIsUniformScaleΪfalse.SetUniformScale��������
    //     m_bIsUniformScaleΪtrue.���m_bIsUniformScaleΪfalse,
    //     ��GetUniformScale���������.
    void SetRotate(const Matrix3f& rRotate);
    inline const Matrix3f& GetRotate(void) const;
    void SetMatrix(const Matrix3f& rMatrix);
    inline const Matrix3f& GetMatrix(void) const;
    void SetTranslate(const Vector3f& rTranslate);
    inline const Vector3f& GetTranslate(void) const;
    void SetScale(const Vector3f& rScale);
    inline const Vector3f& GetScale(void) const;
    void SetUniformScale(float fScale);
    inline float GetUniformScale(void) const;

    // ����M = S*R�����,�˺�������S�е����ֵ�ľ���ֵ.
    // ����һ�������M,���ؾ�����Ԫ�ؾ���ֵ֮�͵����ֵ.
    // ��BV��������ű任ʱ�õ��˺���,�÷���ֵ����BV��뾶.
    float GetNorm(void) const;

    // ���򶥵�任,����Y = X*M + T.
    void ApplyForward(const Vector3f& rInput, Vector3f& rOutput) const;
    // ���򶥵�����任,�任һ�鶥��.
    void ApplyForward(int iCount, const Vector3f* aInput, Vector3f* aOutput)
        const;

    // ���򶥵�任,����X = (Y-T)*M^{-1}.
    void ApplyInverse(const Vector3f& rInput, Vector3f& rOutput) const;
    // ���򶥵�任,�任һ�鶥��.
    void ApplyInverse(int iCount, const Vector3f* aInput, Vector3f* aOutput)
        const;

    // ���������任,����ֵ��һ���ռ�����,��˲���Ҫ����ʩ��T�任.
    // V1 = V0*M^{-1}.
    void InvertVector(const Vector3f& rInput, Vector3f& rOutput) const;

    // ��ƽ���������任,ƽ�淨�߱���Ϊ��λ����.
    void ApplyForward(const Plane3f& rInput, Plane3f& rOutput) const;

    // ����*this = rLhsTrans*rRhsTrans.
    void Product(const Transformation& rLhsTrans,
        const Transformation& rRhsTrans);

    // �赱ǰ�任ΪY = X*M + T,��ǰ�任����任ΪX = (Y-T)*M^{-1}.
    // �����ǰ�任ΪY = X*S*R + T,��ǰ�任����任ΪX = (Y-T)*R^T*S^{-1}.
    // Ҳ����M' = M^{-1} = R^T*S^{-1},T' = -T*M^{-1} = -T*R^T*S^{-1}.
    void GetInverse(Transformation& rInvTrans);

    // ����4x4Homogeneous����,��1x4���������ʹ��.
    void GetHomogeneous(Matrix4f& rHMatrix) const;

    static const Transformation IDENTITY;

    // stream����ʱ�õ�,����д����̵��ֽ���.
    enum
    {
        DISK_USED = sizeof(Matrix3f) + 2*sizeof(Vector3f) + 3
    };

private:
	friend class Stream;
    friend class Spatial;

    Matrix3f m_Matrix;
    Vector3f m_Translate;
    Vector3f m_Scale;

    bool m_bIsIdentity;
    bool m_bIsSRMatrix;
    bool m_bIsUniformScale;
};

#include "SETransformation.inl"

}

#endif