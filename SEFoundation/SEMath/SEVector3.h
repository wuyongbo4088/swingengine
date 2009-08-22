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

#ifndef Swing_Vector3_H
#define Swing_Vector3_H

#include "SEFoundationLIB.h"
#include "SEMath.h"

// �������ʹ������ԭ��

namespace Swing
{

//----------------------------------------------------------------------------
// ����:������3ά������
// ˵��:
// ����:Sun Che
// ʱ��:20070512
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Vector3f
{
public:
    union
    {
        struct
        {
            float X;
            float Y;
            float Z;
        };
        float m_fData[3];
    };

public:
    Vector3f(void);
    Vector3f(float fX, float fY, float fZ);
    Vector3f(const Vector3f& rVec);

    // ת��Ϊ����ָ��ռ�
    operator const float* (void) const;
    operator float* (void);
    // ȡ��iԪ����ʽ
    float operator [] (int i) const;
    float& operator [] (int i);

    Vector3f& operator = (const Vector3f& rVec);

    bool operator == (const Vector3f& rVec) const;
    bool operator != (const Vector3f& rVec) const;
    bool operator < (const Vector3f& rVec) const;
    bool operator <= (const Vector3f& rVec) const;
    bool operator > (const Vector3f& rVec) const;
    bool operator >= (const Vector3f& rVec) const;

    Vector3f operator + (const Vector3f& rRhsVec) const;
    Vector3f operator - (const Vector3f& rRhsVec) const;
    Vector3f operator * (float fScalar) const;
    Vector3f operator / (float fScalar) const;
    Vector3f operator - (void) const;

    Vector3f& operator += (const Vector3f& rRhsVec);
    Vector3f& operator += (float fScalar);
    Vector3f& operator -= (const Vector3f& rRhsVec);
    Vector3f& operator -= (float fScalar);
    Vector3f& operator *= (float fScalar);
    Vector3f& operator /= (float fScalar);

    SE_FOUNDATION_API friend Vector3f operator * (float fLhsScalar, 
        const Vector3f& rRhsVec);

    // ����������
    inline float GetLength(void) const;
    // ����������ƽ��
    inline float GetSquaredLength(void) const;
    // ����
	inline float Dot(const Vector3f& rRhsVec) const;
    // �淶��
	inline float Normalize(void);

    // ע��������ʹ������ԭ��,������DirectX����������ϵ,
    // OpenGL�в��ʹ������ԭ��
    inline Vector3f Cross(const Vector3f& rRhsVec) const;
    inline Vector3f UnitCross(const Vector3f& rRhsVec) const;

    // �������޹ص�U,V,W��������Gram-Schmidt�淶������
    static void Orthonormalize(Vector3f& rVecU, Vector3f& rVecV, 
        Vector3f& rVecW);
    static void Orthonormalize(Vector3f* pVec);

    // ����W����һ��淶������,W����Ϊ��0����,
    // U,V,W��������������ϵ
    static void GetOrthonormalBasis(Vector3f& rVecU, Vector3f& rVecV, 
        Vector3f& rVecW, bool bUnitLengthW);

    // ������������ɶ���,������ռ�ֲ���Сֵ�����ֵ
	static void ComputeExtremes(int iCount, const Vector3f* pPoints, 
        Vector3f& rMin, Vector3f& rMax);

    static const Vector3f ZERO;
    static const Vector3f UNIT_X;
    static const Vector3f UNIT_Y;
    static const Vector3f UNIT_Z;

private:
    inline int CompareData(const Vector3f& rVec) const;
};

//----------------------------------------------------------------------------
// ����:˫����3ά������
// ˵��:
// ����:Sun Che
// ʱ��:20090514
//----------------------------------------------------------------------------
class Vector3d
{
public:
    union
    {
        struct
        {
            double X;
            double Y;
            double Z;
        };
        double m_dData[3];
    };

public:
    Vector3d(void);
    Vector3d(double fX, double fY, double fZ);
    Vector3d(const Vector3d& rVec);

    // ת��Ϊ����ָ��ռ�
    operator const double* (void) const;
    operator double* (void);
    // ȡ��iԪ����ʽ
    double operator [] (int i) const;
    double& operator [] (int i);

    Vector3d& operator = (const Vector3d& rVec);

    bool operator == (const Vector3d& rVec) const;
    bool operator != (const Vector3d& rVec) const;
    bool operator < (const Vector3d& rVec) const;
    bool operator <= (const Vector3d& rVec) const;
    bool operator > (const Vector3d& rVec) const;
    bool operator >= (const Vector3d& rVec) const;

    Vector3d operator + (const Vector3d& rRhsVec) const;
    Vector3d operator - (const Vector3d& rRhsVec) const;
    Vector3d operator * (double dScalar) const;
    Vector3d operator / (double dScalar) const;
    Vector3d operator - (void) const;

    Vector3d& operator += (const Vector3d& rRhsVec);
    Vector3d& operator += (double dScalar);
    Vector3d& operator -= (const Vector3d& rRhsVec);
    Vector3d& operator -= (double dScalar);
    Vector3d& operator *= (double dScalar);
    Vector3d& operator /= (double dScalar);

    SE_FOUNDATION_API friend Vector3d operator * (double fLhsScalar, 
        const Vector3d& rRhsVec);

    // ����������
    inline double GetLength(void) const;
    // ����������ƽ��
    inline double GetSquaredLength(void) const;
    // ����
	inline double Dot(const Vector3d& rRhsVec) const;
    // �淶��
	inline double Normalize(void);

    // ע��������ʹ������ԭ��,������DirectX����������ϵ,
    // OpenGL�в��ʹ������ԭ��
    inline Vector3d Cross(const Vector3d& rRhsVec) const;
    inline Vector3d UnitCross(const Vector3d& rRhsVec) const;

    // �������޹ص�U,V,W��������Gram-Schmidt�淶������
    static void Orthonormalize(Vector3d& rVecU, Vector3d& rVecV, 
        Vector3d& rVecW);
    static void Orthonormalize(Vector3d* pVec);

    // ����W����һ��淶������,W����Ϊ��0����,
    // U,V,W��������������ϵ
    static void GetOrthonormalBasis(Vector3d& rVecU, Vector3d& rVecV, 
        Vector3d& rVecW, bool bUnitLengthW);

    // ������������ɶ���,������ռ�ֲ���Сֵ�����ֵ
	static void ComputeExtremes(int iCount, const Vector3d* pPoints, 
        Vector3d& rMin, Vector3d& rMax);

    SE_FOUNDATION_API static const Vector3d ZERO;
    SE_FOUNDATION_API static const Vector3d UNIT_X;
    SE_FOUNDATION_API static const Vector3d UNIT_Y;
    SE_FOUNDATION_API static const Vector3d UNIT_Z;

private:
    inline int CompareData(const Vector3d& rVec) const;
};

#include "SEVector3.inl"

}

#endif