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
// Description:
// Author:Sun Che
// Date:20070627
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SEQuaternionf
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
    SEQuaternionf(void);
    SEQuaternionf(float fW, float fX, float fY, float fZ);
    SEQuaternionf(const SEQuaternionf& rQ);

    // ����ת������quaternion.
    SEQuaternionf(const SEMatrix3f& rRotMat);

    // ��������Ƕȹ���quaternion.
    SEQuaternionf(const SEVector3f& rAxis, float fAngle);

    operator const float* (void) const;
    operator float* (void);
    float operator [] (int i) const;
    float& operator [] (int i);

    SEQuaternionf& operator = (const SEQuaternionf& rQ);

    bool operator == (const SEQuaternionf& rQ) const;
    bool operator != (const SEQuaternionf& rQ) const;
    bool operator <  (const SEQuaternionf& rQ) const;
    bool operator <= (const SEQuaternionf& rQ) const;
    bool operator >  (const SEQuaternionf& rQ) const;
    bool operator >= (const SEQuaternionf& rQ) const;

    SEQuaternionf operator + (const SEQuaternionf& rRhsQ) const;
    SEQuaternionf operator - (const SEQuaternionf& rRhsQ) const;
    SEQuaternionf operator * (const SEQuaternionf& rRhsQ) const;
    SEQuaternionf operator * (float fScalar) const;
    SEQuaternionf operator / (float fScalar) const;
    SEQuaternionf operator - (void) const;

    SEQuaternionf& operator += (const SEQuaternionf& rRhsQ);
    SEQuaternionf& operator -= (const SEQuaternionf& rRhsQ);
    SEQuaternionf& operator *= (float fScalar);
    SEQuaternionf& operator /= (float fScalar);

    SE_FOUNDATION_API friend SEQuaternionf operator * (float fLhsScalar, 
        const SEQuaternionf& rRhsQ);

    // ����Ϊ��ת����.
    inline SEQuaternionf& FromRotationMatrix(const SEMatrix3f& rRotMat);
    inline void ToRotationMatrix(SEMatrix3f& rRotMat) const;
    SEQuaternionf& FromRotationMatrix(const SEVector3f aRot[3],
        bool bIsRow = true);
    void ToRotationMatrix(SEVector3f aRot[3], bool bIsRow = true) const;

    // ��ת������ǵ�λ����.
    inline SEQuaternionf& FromAxisAngle(const SEVector3f& rAxis, float fAngle);
    inline void ToAxisAngle(SEVector3f& rAxis, float& rfAngle) const;

    // ��ȡ����.
    inline float GetLength(void) const;
    // ��ȡ����ƽ��.
    inline float GetSquaredLength(void) const;
    // ��ȡ���.
    inline float Dot(const SEQuaternionf& rQ) const;
    // �淶��.
    inline float Normalize(void);

    // Ӧ���ڷ�0 quaternion.
    inline void GetInverse(SEQuaternionf& rDesQ) const;
    // ��ȡ����quaternion.
    inline void GetConjugate(SEQuaternionf& rDesQ) const;

    // ��תһ������.
    SEVector3f Rotate(const SEVector3f& rSrcVec) const;

    // �������Բ�ֵ.
    SEQuaternionf& Slerp(float fT, const SEQuaternionf& rP,
        const SEQuaternionf& rQ);
    // ���Բ�ֵ.
    SEQuaternionf& Lerp(float fT, const SEQuaternionf& rP,
        const SEQuaternionf& rQ);

    static const SEQuaternionf IDENTITY;
    static const SEQuaternionf ZERO;

private:
    inline int CompareData(const SEQuaternionf& rQ) const;

    // ����FromRotationMatrix.
    static int m_iNext[3];
};

//----------------------------------------------------------------------------
// Description:
// Author:Sun Che
// Date:20090725
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SEQuaterniond
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
    SEQuaterniond(void);
    SEQuaterniond(double dW, double dX, double dY, double dZ);
    SEQuaterniond(const SEQuaterniond& rQ);

    // ����ת������quaternion.
    SEQuaterniond(const SEMatrix3d& rRotMat);

    // ��������Ƕȹ���quaternion.
    SEQuaterniond(const SEVector3d& rAxis, double dAngle);

    operator const double* (void) const;
    operator double* (void);
    double operator [] (int i) const;
    double& operator [] (int i);

    SEQuaterniond& operator = (const SEQuaterniond& rQ);

    bool operator == (const SEQuaterniond& rQ) const;
    bool operator != (const SEQuaterniond& rQ) const;
    bool operator <  (const SEQuaterniond& rQ) const;
    bool operator <= (const SEQuaterniond& rQ) const;
    bool operator >  (const SEQuaterniond& rQ) const;
    bool operator >= (const SEQuaterniond& rQ) const;

    SEQuaterniond operator + (const SEQuaterniond& rRhsQ) const;
    SEQuaterniond operator - (const SEQuaterniond& rRhsQ) const;
    SEQuaterniond operator * (const SEQuaterniond& rRhsQ) const;
    SEQuaterniond operator * (double dScalar) const;
    SEQuaterniond operator / (double dScalar) const;
    SEQuaterniond operator - (void) const;

    SEQuaterniond& operator += (const SEQuaterniond& rRhsQ);
    SEQuaterniond& operator -= (const SEQuaterniond& rRhsQ);
    SEQuaterniond& operator *= (double dScalar);
    SEQuaterniond& operator /= (double dScalar);

    SE_FOUNDATION_API friend SEQuaterniond operator * (double dLhsScalar, 
        const SEQuaterniond& rRhsQ);

    // ����Ϊ��ת����.
    inline SEQuaterniond& FromRotationMatrix(const SEMatrix3d& rRotMat);
    inline void ToRotationMatrix(SEMatrix3d& rRotMat) const;
    SEQuaterniond& FromRotationMatrix(const SEVector3d aRot[3],
        bool bIsRow = true);
    void ToRotationMatrix(SEVector3d aRot[3], bool bIsRow = true) const;

    // ��ת������ǵ�λ����.
    inline SEQuaterniond& FromAxisAngle(const SEVector3d& rAxis, double 
        dAngle);
    inline void ToAxisAngle(SEVector3d& rAxis, double& rdAngle) const;

    // ��ȡ����.
    inline double GetLength(void) const;
    // ��ȡ����ƽ��.
    inline double GetSquaredLength(void) const;
    // ��ȡ���.
    inline double Dot(const SEQuaterniond& rQ) const;
    // �淶��.
    inline double Normalize(void);

    // Ӧ���ڷ�0 quaternion.
    inline void GetInverse(SEQuaterniond& rDesQ) const;
    // ��ȡ����quaternion.
    inline void GetConjugate(SEQuaterniond& rDesQ) const;

    // ��תһ������.
    SEVector3d Rotate(const SEVector3d& rSrcVec) const;

    // �������Բ�ֵ.
    SEQuaterniond& Slerp(double dT, const SEQuaterniond& rP,
        const SEQuaterniond& rQ);
    // ���Բ�ֵ.
    SEQuaterniond& Lerp(double dT, const SEQuaterniond& rP,
        const SEQuaterniond& rQ);

    static const SEQuaterniond IDENTITY;
    static const SEQuaterniond ZERO;

private:
    inline int CompareData(const SEQuaterniond& rQ) const;

    // ����FromRotationMatrix.
    static int m_iNext[3];
};

#include "SEQuaternion.inl"

}

#endif
