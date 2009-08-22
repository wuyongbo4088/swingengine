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

#ifndef Swing_Matrix2_H
#define Swing_Matrix2_H

// ����洢��ʽ���������ȷ�ʽ.
// ע��DirectX���������ȷ�ʽ,OpenGL���������ȷ�ʽ.
// ����,DirectX�и���һ������M,OpenGL����֮��Ӧ�ľ�����M^T

// ������������������ӵķ�ʽ.����,����һ������M��һ������V,�������Ծ�����
// V*M.V������������.��Щͼ��API����M*V����ʽ,��ʱV������������,��ʱ��Mʵ��
// ����V*M���ʱ��M��ת�þ���.һϵ�������ľ���任�ɱ�ʾΪV' = V*M0*M1...Mn,
// ��Щͼ��API�����������ӵķ�ʽ,��ʱV' = Mn...M1*M0*V.
// ע��DirectX�����������ӵķ�ʽ,OpenGL�����������ӵķ�ʽ.
// ����,DirectX�и�������M1,M2,OpenGL�������Ƕ�Ӧ����M1^T,M2^T,
// M1*M2��Ӧ(M1*M2)^T = M2^T*M1^T.

// ��ת��������,�ο�<<Quaternions and rotation sequences by Jack B. Kuipers>>.
// ��������������: A*A^T = I�ľ���,��A^T = A^(-1),��det^2(A) = det^2(A^T) = 1.
// ��ת����ĳ�Ҫ����(iff):
// ��ת��������������,����������ʽ��ֵ��Ϊ+1.
// ������������������ת�þ���.Ϊ��������,���������������ת�þ���ﵽ��任
// ��Ŀ��.
// ʵ����,ת��Ҳ���Ա���,ֻҪ�õȼ۵��������ҳ˸þ���,�ʹﵽ�˲����治ת�ý���
// ��任��Ŀ��.

// ���ʹ����������˽��б任,
// ����涨t > 0ʱΪ��ֽ��(�۲췽��ķ�����)��ʱ����ת,
// ��XYƽ����ת����Ϊ:
//   R =  cos(t)  sin(t)
//       -sin(t)  cos(t)

#include "SEFoundationLIB.h"
#include "SEVector2.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:������2�׷�����
// ˵��:
// ����:Sun Che
// ʱ��:20070512
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Matrix2f
{
public:
    union
    {
        struct
        {
            float M11, M12;
            float M21, M22;
        };
        float m_fData[2][2];
    };

public:
    Matrix2f(void);
    Matrix2f(const Matrix2f& rMat);
    Matrix2f(float fM11, float fM12, float fM21, float fM22);
	// Ĭ������������������
    Matrix2f(const Vector2f& rVecU, const Vector2f& rVecV, bool bIsRow = true);
    // �����ԽǾ���
    Matrix2f(float fM11, float fM22);
	// ������ת����,������,fAngle > 0Ϊ��ʱ����ת
    Matrix2f(float fAngle);
    // ��������U��������V����������������
    Matrix2f(const Vector2f& rVecU, const Vector2f& rVecV);

    operator const float* (void) const;
    operator float* (void);
    const float* operator [] (int iCurRow) const;
    float* operator [] (int iCurRow);
    float operator () (int iCurRow, int iCurCol) const;
    float& operator () (int iCurRow, int iCurCol);

    inline void SetRow(int iDesRow, const Vector2f& rSrcVec);
    inline void GetRow(int iSrcRow, Vector2f& rDesVec) const;
    inline void SetColumn(int iDesCol, const Vector2f& rSrcVec);
    inline void GetColumn(int SrciCol, Vector2f& rDesVec) const;

    Matrix2f& operator = (const Matrix2f& rMat);

    bool operator == (const Matrix2f& rMat) const;
    bool operator != (const Matrix2f& rMat) const;
    bool operator < (const Matrix2f& rMat) const;
    bool operator <= (const Matrix2f& rMat) const;
    bool operator > (const Matrix2f& rMat) const;
    bool operator >= (const Matrix2f& rMat) const;

    Matrix2f operator + (const Matrix2f& rMat) const;
    Matrix2f operator - (const Matrix2f& rMat) const;
    Matrix2f operator * (const Matrix2f& rMat) const;
    Matrix2f operator * (float fScalar) const;
    Matrix2f operator / (float fScalar) const;
    Matrix2f operator - (void) const;

    Matrix2f& operator += (const Matrix2f& rMat);
    Matrix2f& operator -= (const Matrix2f& rMat);
    Matrix2f& operator *= (float fScalar);
    Matrix2f& operator /= (float fScalar);

    Vector2f operator * (const Vector2f& rVec) const;

    SE_FOUNDATION_API friend Matrix2f operator * (float fLhsScalar, 
        const Matrix2f& rRhsMat);
    SE_FOUNDATION_API friend Vector2f operator * (const Vector2f& rLhsVec, 
        const Matrix2f& rRhsMat);

    // ��Ϊ0����
    inline void Zero(void);
    // ��Ϊ��λ����
    inline void Identity(void);
    // ��Ϊ�ԽǾ���
    inline void Diagonal(float fM11, float fM22);
    // ������������������������������
    inline void TensorProduct(const Vector2f& rVecU, const Vector2f& rVecV);
    // ���Լ�ת��
    void Transpose(void);
    // ��ȡת�þ���
    inline void GetTranspose(Matrix2f& rDesMat) const;
    // ���Լ�����
    void Inverse(void);
    // ��ȡ�����
    inline void GetInverse(Matrix2f& rDesMat) const;
    // ��ȡ�������
    inline void GetAdjoint(Matrix2f& rDesMat) const;
    // ��ȡ����ʽ
    inline float GetDeterminant(void) const;
    // this^T * M
    inline void GetTransposeTimes(const Matrix2f& rRhsMat, 
        Matrix2f& rDesMat) const;
    // this * M^T
    inline void GetTimesTranspose(const Matrix2f& rRhsMat, 
        Matrix2f& rDesMat) const;

    // ������ת����
    inline void FromAngle(float fAngle);
    // ����Ϊ��ת����,��������ת�Ƕ�
    inline void ToAngle(float& rfAngle) const;

    static const Matrix2f ZERO;
    static const Matrix2f IDENTITY;

private:
    inline int CompareData(const Matrix2f& rMat) const;
};

//----------------------------------------------------------------------------
// ����:˫����2�׷�����
// ˵��:
// ����:Sun Che
// ʱ��:20090516
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Matrix2d
{
public:
    union
    {
        struct
        {
            double M11, M12;
            double M21, M22;
        };
        double m_dData[2][2];
    };

public:
    Matrix2d(void);
    Matrix2d(const Matrix2d& rMat);
    Matrix2d(double dM11, double dM12, double dM21, double dM22);
	// Ĭ������������������
    Matrix2d(const Vector2d& rVecU, const Vector2d& rVecV, bool bIsRow = true);
    // �����ԽǾ���
    Matrix2d(double dM11, double dM22);
	// ������ת����,������,dAngle > 0Ϊ��ʱ����ת
    Matrix2d(double dAngle);
    // ��������U��������V����������������
    Matrix2d(const Vector2d& rVecU, const Vector2d& rVecV);

    operator const double* (void) const;
    operator double* (void);
    const double* operator [] (int iCurRow) const;
    double* operator [] (int iCurRow);
    double operator () (int iCurRow, int iCurCol) const;
    double& operator () (int iCurRow, int iCurCol);

    inline void SetRow(int iDesRow, const Vector2d& rSrcVec);
    inline void GetRow(int iSrcRow, Vector2d& rDesVec) const;
    inline void SetColumn(int iDesCol, const Vector2d& rSrcVec);
    inline void GetColumn(int SrciCol, Vector2d& rDesVec) const;

    Matrix2d& operator = (const Matrix2d& rMat);

    bool operator == (const Matrix2d& rMat) const;
    bool operator != (const Matrix2d& rMat) const;
    bool operator < (const Matrix2d& rMat) const;
    bool operator <= (const Matrix2d& rMat) const;
    bool operator > (const Matrix2d& rMat) const;
    bool operator >= (const Matrix2d& rMat) const;

    Matrix2d operator + (const Matrix2d& rMat) const;
    Matrix2d operator - (const Matrix2d& rMat) const;
    Matrix2d operator * (const Matrix2d& rMat) const;
    Matrix2d operator * (double dScalar) const;
    Matrix2d operator / (double dScalar) const;
    Matrix2d operator - (void) const;

    Matrix2d& operator += (const Matrix2d& rMat);
    Matrix2d& operator -= (const Matrix2d& rMat);
    Matrix2d& operator *= (double dScalar);
    Matrix2d& operator /= (double dScalar);

    Vector2d operator * (const Vector2d& rVec) const;

    SE_FOUNDATION_API friend Matrix2d operator * (double dLhsScalar, 
        const Matrix2d& rRhsMat);
    SE_FOUNDATION_API friend Vector2d operator * (const Vector2d& rLhsVec, 
        const Matrix2d& rRhsMat);

    // ��Ϊ0����
    inline void Zero(void);
    // ��Ϊ��λ����
    inline void Identity(void);
    // ��Ϊ�ԽǾ���
    inline void Diagonal(double dM11, double dM22);
    // ������������������������������
    inline void TensorProduct(const Vector2d& rVecU, const Vector2d& rVecV);
    // ���Լ�ת��
    void Transpose(void);
    // ��ȡת�þ���
    inline void GetTranspose(Matrix2d& rDesMat) const;
    // ���Լ�����
    void Inverse(void);
    // ��ȡ�����
    inline void GetInverse(Matrix2d& rDesMat) const;
    // ��ȡ�������
    inline void GetAdjoint(Matrix2d& rDesMat) const;
    // ��ȡ����ʽ
    inline double GetDeterminant(void) const;
    // this^T * M
    inline void GetTransposeTimes(const Matrix2d& rRhsMat, 
        Matrix2d& rDesMat) const;
    // this * M^T
    inline void GetTimesTranspose(const Matrix2d& rRhsMat, 
        Matrix2d& rDesMat) const;

    // ������ת����
    inline void FromAngle(double dAngle);
    // ����Ϊ��ת����,��������ת�Ƕ�
    inline void ToAngle(double& rdAngle) const;

    static const Matrix2d ZERO;
    static const Matrix2d IDENTITY;

private:
    inline int CompareData(const Matrix2d& rMat) const;
};

#include "SEMatrix2.inl"

}

#endif



