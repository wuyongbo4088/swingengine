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

#ifndef Swing_Vector2_H
#define Swing_Vector2_H

#include "SEFoundationLIB.h"
#include "SEMath.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:������2ά������
// ˵��:
// ����:Sun Che
// ʱ��:20070512
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Vector2f
{
public:
    union
    {
        struct
        {
            float X;
            float Y;
        };
        float m_fData[2];
    };

public:
    Vector2f(void);
    Vector2f(float fX, float fY);
    Vector2f(const Vector2f& rVec);

    // ת��Ϊ����ָ��ռ�
    operator const float* (void) const;
    operator float* (void);
    // ȡ��iԪ����ʽ
    float operator [] (int i) const;
    float& operator [] (int i);

    Vector2f& operator = (const Vector2f& rVec);

    bool operator == (const Vector2f& rVec) const;
    bool operator != (const Vector2f& rVec) const;
    bool operator < (const Vector2f& rVec) const;
    bool operator <= (const Vector2f& rVec) const;
    bool operator > (const Vector2f& rVec) const;
    bool operator >= (const Vector2f& rVec) const;

    Vector2f operator + (const Vector2f& rRhsVec) const;
    Vector2f operator - (const Vector2f& rRhsVec) const;
    Vector2f operator * (float fScalar) const;
    Vector2f operator / (float fScalar) const;
    Vector2f operator - (void) const;

    Vector2f& operator += (const Vector2f& rRhsVec);
    Vector2f& operator += (float fScalar);
    Vector2f& operator -= (const Vector2f& rRhsVec);
    Vector2f& operator -= (float fScalar);
    Vector2f& operator *= (float fScalar);
    Vector2f& operator /= (float fScalar);

    SE_FOUNDATION_API friend Vector2f operator * (float fLhsScalar,
        const Vector2f& rRhsVec);

    // ����������
    inline float GetLength(void) const;
    // ����������ƽ��
    inline float GetSquaredLength(void) const;
    // ����
    inline float Dot(const Vector2f& rVec) const;
    // �淶��
	inline float Normalize(void);

    // ��ȡ��ֱ�ڵ�ǰ����������(y,-x)
    inline void GetPerp(Vector2f& rDesVec) const;

    // ��ȡ((x,y),(V.x,V.y)) = x*V.y - y*V.x
    inline float GetDotPerp(const Vector2f& rVec) const;

	// ��2�������޹ص���������Gram-Schmidt�淶������.
    static void Orthonormalize(Vector2f& rVecU, Vector2f& rVecV);

    // ����V����һ��淶������,V����Ϊ��0����
    static void GetOrthonormalBasis(Vector2f& rVecU, Vector2f& rVecV,
        bool bUnitLengthV);

    static const Vector2f ZERO;
    static const Vector2f UNIT_X;
    static const Vector2f UNIT_Y;

private:
    inline int CompareData(const Vector2f& rVec) const;
};

//----------------------------------------------------------------------------
// ����:˫����2ά������
// ˵��:
// ����:Sun Che
// ʱ��:20090513
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Vector2d
{
public:
    union
    {
        struct
        {
            double X;
            double Y;
        };
        double m_dData[2];
    };

public:
    Vector2d(void);
    Vector2d(double dX, double dY);
    Vector2d(const Vector2d& rVec);

    // ת��Ϊ����ָ��ռ�
    operator const double* (void) const;
    operator double* (void);
    // ȡ��iԪ����ʽ
    double operator [] (int i) const;
    double& operator [] (int i);

    Vector2d& operator = (const Vector2d& rVec);

    bool operator == (const Vector2d& rVec) const;
    bool operator != (const Vector2d& rVec) const;
    bool operator < (const Vector2d& rVec) const;
    bool operator <= (const Vector2d& rVec) const;
    bool operator > (const Vector2d& rVec) const;
    bool operator >= (const Vector2d& rVec) const;

    Vector2d operator + (const Vector2d& rRhsVec) const;
    Vector2d operator - (const Vector2d& rRhsVec) const;
    Vector2d operator * (double dScalar) const;
    Vector2d operator / (double dScalar) const;
    Vector2d operator - (void) const;

    Vector2d& operator += (const Vector2d& rRhsVec);
    Vector2d& operator += (double dScalar);
    Vector2d& operator -= (const Vector2d& rRhsVec);
    Vector2d& operator -= (double dScalar);
    Vector2d& operator *= (double dScalar);
    Vector2d& operator /= (double dScalar);

    SE_FOUNDATION_API friend Vector2d operator * (double dLhsScalar, 
        const Vector2d& rRhsVec);

    // ����������
    inline double GetLength(void) const;
    // ����������ƽ��
    inline double GetSquaredLength(void) const;
    // ����
    inline double Dot(const Vector2d& rVec) const;
    // �淶��
	inline double Normalize(void);

    // ��ȡ��ֱ�ڵ�ǰ����������(y,-x)
    inline void GetPerp(Vector2d& rDesVec) const;

    // ��ȡ((x,y),(V.x,V.y)) = x*V.y - y*V.x
    inline double GetDotPerp(const Vector2d& rVec) const;

	// ��2�������޹ص���������Gram-Schmidt�淶������.
    static void Orthonormalize(Vector2d& rVecU, Vector2d& rVecV);

    // ����V����һ��淶������,V����Ϊ��0����
    static void GetOrthonormalBasis(Vector2d& rVecU, Vector2d& rVecV,
        bool bUnitLengthV);

    static const Vector2d ZERO;
    static const Vector2d UNIT_X;
    static const Vector2d UNIT_Y;

private:
    inline int CompareData(const Vector2d& rVec) const;
};

#include "SEVector2.inl"

}

#endif