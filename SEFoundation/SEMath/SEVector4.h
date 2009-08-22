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

#ifndef Swing_Vector4_H
#define Swing_Vector4_H

#include "SEFoundationLIB.h"
#include "SEMath.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:������4ά������
// ˵��:
// ����:Sun Che
// ʱ��:20070512
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Vector4f
{
public:
    union
    {
        struct
        {
            float X;
            float Y;
            float Z;
            float W;
        };
        float m_fData[4];
    };

public:
    Vector4f(void);
    Vector4f(float fX, float fY, float fZ, float fW);
    Vector4f(const Vector4f& rVec);

	// ת��Ϊ����ָ��ռ�
    operator const float* (void) const;
    operator float* (void);
	// ȡ��iԪ����ʽ
    float operator [] (int i) const;
    float& operator [] (int i);

    Vector4f& operator = (const Vector4f& rVec);

    bool operator == (const Vector4f& rVec) const;
    bool operator != (const Vector4f& rVec) const;
    bool operator <  (const Vector4f& rVec) const;
    bool operator <= (const Vector4f& rVec) const;
    bool operator >  (const Vector4f& rVec) const;
    bool operator >= (const Vector4f& rVec) const;

    Vector4f operator + (const Vector4f& rRhsVec) const;
    Vector4f operator - (const Vector4f& rRhsVec) const;
    Vector4f operator * (float fScalar) const;
    Vector4f operator / (float fScalar) const;
    Vector4f operator - (void) const;

    Vector4f& operator += (const Vector4f& rRhsVec);
    Vector4f& operator += (float fScalar);
    Vector4f& operator -= (const Vector4f& rRhsVec);
    Vector4f& operator -= (float fScalar);
    Vector4f& operator *= (float fScalar);
    Vector4f& operator /= (float fScalar);

    SE_FOUNDATION_API friend Vector4f operator * (float fLhsScalar, 
        const Vector4f& rRhsVec);
	
    // ����������
    inline float GetLength(void) const;
    // ����������ƽ��
    inline float GetSquaredLength(void) const;
    // ����
    inline float Dot(const Vector4f& rRhsVec) const;
	// �淶��
	inline float Normalize(void);

    static const Vector4f ZERO;
    static const Vector4f UNIT_X;
    static const Vector4f UNIT_Y;
    static const Vector4f UNIT_Z;
    static const Vector4f UNIT_W;

private:
    inline int CompareData(const Vector4f& rVec) const;
};

//----------------------------------------------------------------------------
// ����:˫����4ά������
// ˵��:
// ����:Sun Che
// ʱ��:20090515
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Vector4d
{
public:
    union
    {
        struct
        {
            double X;
            double Y;
            double Z;
            double W;
        };
        double m_dData[4];
    };

public:
    Vector4d(void);
    Vector4d(double dX, double dY, double dZ, double dW);
    Vector4d(const Vector4d& rVec);

	// ת��Ϊ����ָ��ռ�
    operator const double* (void) const;
    operator double* (void);
	// ȡ��iԪ����ʽ
    double operator [] (int i) const;
    double& operator [] (int i);

    Vector4d& operator = (const Vector4d& rVec);

    bool operator == (const Vector4d& rVec) const;
    bool operator != (const Vector4d& rVec) const;
    bool operator <  (const Vector4d& rVec) const;
    bool operator <= (const Vector4d& rVec) const;
    bool operator >  (const Vector4d& rVec) const;
    bool operator >= (const Vector4d& rVec) const;

    Vector4d operator + (const Vector4d& rRhsVec) const;
    Vector4d operator - (const Vector4d& rRhsVec) const;
    Vector4d operator * (double dScalar) const;
    Vector4d operator / (double dScalar) const;
    Vector4d operator - (void) const;

    Vector4d& operator += (const Vector4d& rRhsVec);
    Vector4d& operator += (double dScalar);
    Vector4d& operator -= (const Vector4d& rRhsVec);
    Vector4d& operator -= (double dScalar);
    Vector4d& operator *= (double dScalar);
    Vector4d& operator /= (double dScalar);

    SE_FOUNDATION_API friend Vector4d operator * (double dLhsScalar, 
        const Vector4d& rRhsVec);
	
    // ����������
    inline double GetLength(void) const;
    // ����������ƽ��
    inline double GetSquaredLength(void) const;
    // ����
    inline double Dot(const Vector4d& rRhsVec) const;
	// �淶��
	inline double Normalize(void);

    static const Vector4d ZERO;
    static const Vector4d UNIT_X;
    static const Vector4d UNIT_Y;
    static const Vector4d UNIT_Z;
    static const Vector4d UNIT_W;

private:
    inline int CompareData(const Vector4d& rVec) const;
};

typedef SE_ALIGN16 Vector4f Vector4fA16;
typedef SE_ALIGN16 Vector4d Vector4dA16;

#include "SEVector4.inl"

}

#endif