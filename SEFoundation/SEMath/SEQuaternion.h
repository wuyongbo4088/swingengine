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

#ifndef Swing_Quaternion_H
#define Swing_Quaternion_H

// Quaternion������q = w + x*i + y*j + z*k�ĳ�����,
// ����4D�����ռ�����(w,x,y,z),��ģ��һ��Ϊ1,
// �鲿:i^2 = j^2 = k^2 = -1,i*j = k,j*k = i,k*i = j,j*i = -k,k*j = -i,i*k = -j

#include "SEFoundationLIB.h"
#include "SEMatrix3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:������4Ԫ����
// ˵��:
// ����:Sun Che
// ʱ��:20070627
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Quaternionf
{
public:
    union
    {
        struct
		{
            float W;
            float X;
            float Y;
            float Z;
		};
        float m_fData[4];
    };

public:
    Quaternionf(void);
    Quaternionf(float fW, float fX, float fY, float fZ);
    Quaternionf(const Quaternionf& rQ);

    // ����ת������quaternion.
    Quaternionf(const Matrix3f& rRotMat);

    // ��������Ƕȹ���quaternion.
    Quaternionf(const Vector3f& rAxis, float fAngle);

    operator const float* (void) const;
    operator float* (void);
    float operator [] (int i) const;
    float& operator [] (int i);

    Quaternionf& operator = (const Quaternionf& rQ);

    bool operator == (const Quaternionf& rQ) const;
    bool operator != (const Quaternionf& rQ) const;
    bool operator <  (const Quaternionf& rQ) const;
    bool operator <= (const Quaternionf& rQ) const;
    bool operator >  (const Quaternionf& rQ) const;
    bool operator >= (const Quaternionf& rQ) const;

    Quaternionf operator + (const Quaternionf& rRhsQ) const;
    Quaternionf operator - (const Quaternionf& rRhsQ) const;
    Quaternionf operator * (const Quaternionf& rRhsQ) const;
    Quaternionf operator * (float fScalar) const;
    Quaternionf operator / (float fScalar) const;
    Quaternionf operator - (void) const;

    Quaternionf& operator += (const Quaternionf& rRhsQ);
    Quaternionf& operator -= (const Quaternionf& rRhsQ);
    Quaternionf& operator *= (float fScalar);
    Quaternionf& operator /= (float fScalar);

    SE_FOUNDATION_API friend Quaternionf operator * (float fLhsScalar, 
        const Quaternionf& rRhsQ);

    // ����Ϊ��ת����.
    inline Quaternionf& FromRotationMatrix(const Matrix3f& rRotMat);
    inline void ToRotationMatrix(Matrix3f& rRotMat) const;
    Quaternionf& FromRotationMatrix(const Vector3f aRot[3],
        bool bIsRow = true);
    void ToRotationMatrix(Vector3f aRot[3], bool bIsRow = true) const;

    // ��ת������ǵ�λ����.
    inline Quaternionf& FromAxisAngle(const Vector3f& rAxis, float fAngle);
    inline void ToAxisAngle(Vector3f& rAxis, float& rfAngle) const;

    // ��ȡ����.
    inline float GetLength(void) const;
    // ��ȡ����ƽ��.
    inline float GetSquaredLength(void) const;
    // ��ȡ���.
    inline float Dot(const Quaternionf& rQ) const;
    // �淶��.
    inline float Normalize(void);

    // Ӧ���ڷ�0 quaternion.
    inline void GetInverse(Quaternionf& rDesQ) const;
    // ��ȡ����quaternion.
    inline void GetConjugate(Quaternionf& rDesQ) const;

    // ��תһ������.
    Vector3f Rotate(const Vector3f& rSrcVec) const;

    // �������Բ�ֵ.
    Quaternionf& Slerp(float fT, const Quaternionf& rP,
        const Quaternionf& rQ);
    // ���Բ�ֵ.
    Quaternionf& Lerp(float fT, const Quaternionf& rP,
        const Quaternionf& rQ);

    static const Quaternionf IDENTITY;
    static const Quaternionf ZERO;

private:
    inline int CompareData(const Quaternionf& rQ) const;

    // ����FromRotationMatrix.
    static int m_iNext[3];
};

//----------------------------------------------------------------------------
// ����:˫����4Ԫ����
// ˵��:
// ����:Sun Che
// ʱ��:20090725
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Quaterniond
{
public:
    union
    {
        struct
		{
            double W;
            double X;
            double Y;
            double Z;
		};
        double m_dData[4];
    };

public:
    Quaterniond(void);
    Quaterniond(double dW, double dX, double dY, double dZ);
    Quaterniond(const Quaterniond& rQ);

    // ����ת������quaternion.
    Quaterniond(const Matrix3d& rRotMat);

    // ��������Ƕȹ���quaternion.
    Quaterniond(const Vector3d& rAxis, double dAngle);

    operator const double* (void) const;
    operator double* (void);
    double operator [] (int i) const;
    double& operator [] (int i);

    Quaterniond& operator = (const Quaterniond& rQ);

    bool operator == (const Quaterniond& rQ) const;
    bool operator != (const Quaterniond& rQ) const;
    bool operator <  (const Quaterniond& rQ) const;
    bool operator <= (const Quaterniond& rQ) const;
    bool operator >  (const Quaterniond& rQ) const;
    bool operator >= (const Quaterniond& rQ) const;

    Quaterniond operator + (const Quaterniond& rRhsQ) const;
    Quaterniond operator - (const Quaterniond& rRhsQ) const;
    Quaterniond operator * (const Quaterniond& rRhsQ) const;
    Quaterniond operator * (double dScalar) const;
    Quaterniond operator / (double dScalar) const;
    Quaterniond operator - (void) const;

    Quaterniond& operator += (const Quaterniond& rRhsQ);
    Quaterniond& operator -= (const Quaterniond& rRhsQ);
    Quaterniond& operator *= (double dScalar);
    Quaterniond& operator /= (double dScalar);

    SE_FOUNDATION_API friend Quaterniond operator * (double dLhsScalar, 
        const Quaterniond& rRhsQ);

    // ����Ϊ��ת����.
    inline Quaterniond& FromRotationMatrix(const Matrix3d& rRotMat);
    inline void ToRotationMatrix(Matrix3d& rRotMat) const;
    Quaterniond& FromRotationMatrix(const Vector3d aRot[3],
        bool bIsRow = true);
    void ToRotationMatrix(Vector3d aRot[3], bool bIsRow = true) const;

    // ��ת������ǵ�λ����.
    inline Quaterniond& FromAxisAngle(const Vector3d& rAxis, double dAngle);
    inline void ToAxisAngle(Vector3d& rAxis, double& rdAngle) const;

    // ��ȡ����.
    inline double GetLength(void) const;
    // ��ȡ����ƽ��.
    inline double GetSquaredLength(void) const;
    // ��ȡ���.
    inline double Dot(const Quaterniond& rQ) const;
    // �淶��.
    inline double Normalize(void);

    // Ӧ���ڷ�0 quaternion.
    inline void GetInverse(Quaterniond& rDesQ) const;
    // ��ȡ����quaternion.
    inline void GetConjugate(Quaterniond& rDesQ) const;

    // ��תһ������.
    Vector3d Rotate(const Vector3d& rSrcVec) const;

    // �������Բ�ֵ.
    Quaterniond& Slerp(double dT, const Quaterniond& rP,
        const Quaterniond& rQ);
    // ���Բ�ֵ.
    Quaterniond& Lerp(double dT, const Quaterniond& rP,
        const Quaterniond& rQ);

    static const Quaterniond IDENTITY;
    static const Quaterniond ZERO;

private:
    inline int CompareData(const Quaterniond& rQ) const;

    // ����FromRotationMatrix.
    static int m_iNext[3];
};

#include "SEQuaternion.inl"

}

#endif
