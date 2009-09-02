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

#ifndef Swing_Matrix3_H
#define Swing_Matrix3_H

// Matrices in Swing Engine are stored in row-major order.
// CAUTION: By default, DirectX uses row-major order, OpenGL uses column-major
// order. For example, given a matrix M in DirectX, the corresponding matrix
// in OpenGL is M^T.

// Matrix operation is concatenated from left to right. Say, given a matrix
// M and a vector V, V multiply M equals V*M, V is viewed as a row vector. 
// Some graphics APIs express the multiplication as M'*V', in which cases 
// V' equals V but is viewed as a column vector, and M' is the transpose of M.
// So a sequence of transformation should be expressed as V' = V*M0*M1...Mn,
// Others graphics APIs express the operation from right to left, in which 
// cases the sequence of transformation should be expressed as V' = Mn...M1*M0*V.
// CAUTION: DirectX uses the rule of from left to right, OpenGL uses the rule of
// from right to left.
// Say, in DirectX, given M1 and M2, the equivalent items in OpenGL are M1^T, 
// M2^T, ^T means transpose operation.
// M1*M2 is equivalent to (M1*M2)^T = M2^T*M1^T.

// Some natures of rotation matrix are referenced from this book:
// <<Quaternions and rotation sequences>> by Jack B. Kuipers.
// Orthogonal matrix is in the form of the matrix looks like: 
// A*A^T = I, A^T = A^(-1), and det^2(A) = det^2(A^T) = 1.
// iff(if and only if condition) of rotation matrix:
// Rotation matrix is orthogonal matrix, and it's determinant always equals +1.
// The inverse matrix of orthogonal matrix equals it's transpose matrix.
// In order to avoid the inverse operation, we could multiply a row vector on 
// the left side of the transpose matrix.
// But after a more deep looking, we can see that even the transpose itself
// could be avoided too. To achieve this goal, we simply apply a column vector
// which is equivalent to the original row vector on the right side of the 
// original rotation matrix.

// If we apply a row vector on the left side of a rotation matrix,
// If we define that when t > 0, the rotation about an axis is clockwise order,
// then the rotations about X,Y,Z axes are:
//   RX =    1       0       0
//           0     cos(t)  sin(t)
//           0    -sin(t)  cos(t)
//
//   RY =  cos(t)    0    -sin(t)
//           0       1       0
//         sin(t)    0     cos(t)
//
//   RZ =  cos(t)  sin(t)    0
//        -sin(t)  cos(t)    0
//           0       0       1
//
// If we apply a column vector on the right side of a rotation matrix,
// If we define that when t > 0, the rotation about an axis is counter-clockwise 
// order, then the rotations about X,Y,Z axes are:
//   RX' =   1       0       0
//           0     cos(t)  -sin(t)
//           0     sin(t) cos(t)
//
//   RY' = cos(t)    0     sin(t)
//           0       1       0
//        -sin(t)    0     cos(t)
//
//   RZ' = cos(t) -sin(t)    0
//         sin(t)  cos(t)    0
//           0       0       1
//
// Swing Engine uses these rules:
// (1) Left-hand based coordinate system.
// (2) Rotation about an arbitrary axis with an angle theta, the order is 
//     clockwise when theta > 0.
// (3) Direction of cross product of 3D vectors is based on Left-hand rule.

#include "SEFoundationLIB.h"
#include "SEVector3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Name: single precision floating-point 3x3 matrix class
// Description:
// Author: Sun Che
// Date: 20070512
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Matrix3f
{
public:
    inline float M11(void) const;
    inline float& M11(void);
    inline float M12(void) const;
    inline float& M12(void);
    inline float M13(void) const;
    inline float& M13(void);
    inline float M21(void) const;
    inline float& M21(void);
    inline float M22(void) const;
    inline float& M22(void);
    inline float M23(void) const;
    inline float& M23(void);
    inline float M31(void) const;
    inline float& M31(void);
    inline float M32(void) const;
    inline float& M32(void);
    inline float M33(void) const;
    inline float& M33(void);
private:
    //float m_fData[3][4];
    float m_fData[3][3];

public:
    Matrix3f(void);
    Matrix3f(const Matrix3f& rMat);
    Matrix3f(float fM11, float fM12, float fM13,
             float fM21, float fM22, float fM23,
             float fM31, float fM32, float fM33);
    // By defalut, construct a row-major order matrix.
    Matrix3f(const Vector3f& rVecU, const Vector3f& rVecV, 
        const Vector3f& rVecW, bool bIsRow = true);
    // Construct a diagonal matrix.
    Matrix3f(float fM11, float fM22, float fM33);
    // ��������������ת����,������,fAngle > 0Ϊ˳ʱ����ת,��ת�����Ϊ��λ����
    Matrix3f(const Vector3f& rAxisVec, float fAngle);
    // ��������U��������V����������������
    Matrix3f(const Vector3f& rVecU, const Vector3f& rVecV);

    operator const float* (void) const;
    operator float* (void);
    const float* operator [] (int iCurRow) const;
    float* operator [] (int iCurRow);
    float operator () (int iCurRow, int iCurCol) const;
    float& operator () (int iCurRow, int iCurCol);

    inline void SetRow(int iDesRow, const Vector3f& rSrcVec);
    inline void GetRow(int iSrcRow, Vector3f& rDesVec) const;
    inline void SetColumn(int iDesCol, const Vector3f& rSrcVec);
    inline void GetColumn(int iSrcCol, Vector3f& rDesVec) const;

    Matrix3f& operator = (const Matrix3f& rMat);

    bool operator == (const Matrix3f& rMat) const;
    bool operator != (const Matrix3f& rMat) const;
    bool operator < (const Matrix3f& rMat) const;
    bool operator <= (const Matrix3f& rMat) const;
    bool operator > (const Matrix3f& rMat) const;
    bool operator >= (const Matrix3f& rMat) const;

    Matrix3f operator + (const Matrix3f& rRhsMat) const;
    Matrix3f operator - (const Matrix3f& rRhsMat) const;
    Matrix3f operator * (const Matrix3f& rRhsMat) const;
    Matrix3f operator * (float fScalar) const;
    Matrix3f operator / (float fScalar) const;
    Matrix3f operator - (void) const;

    Matrix3f& operator += (const Matrix3f& rRhsMat);
    Matrix3f& operator -= (const Matrix3f& rRhsMat);
    Matrix3f& operator *= (float fScalar);
    Matrix3f& operator /= (float fScalar);

    // ����������
    Vector3f operator * (const Vector3f& rRhsVec) const;

    SE_FOUNDATION_API friend Matrix3f operator * (float fLhsScalar, 
        const Matrix3f& rRhsMat);
    SE_FOUNDATION_API friend Vector3f operator * (const Vector3f& rLhsVec, 
        const Matrix3f& rRhsMat);

    // ��Ϊ0����
    inline void Zero(void);
    // ��Ϊ��λ����
    inline void Identity(void);
    // ��Ϊ�ԽǾ���
    inline void Diagonal(float fM11, float fM22, float fM33);
    // ������������������������������
    inline void TensorProduct(const Vector3f& rVecU, const Vector3f& rVecV);
    // ���Լ�ת��
    void Transpose(void);
    // ��ȡת�þ���
    inline void GetTranspose(Matrix3f& rDesMat) const;
    // ���Լ�����
    void Inverse(void);
    // ��ȡ�����
    inline void GetInverse(Matrix3f& rDesMat) const;
    // ��ȡ�������
    inline void GetAdjoint(Matrix3f& rDesMat) const;
    // ��ȡ����ʽ
    inline float GetDeterminant(void) const;
    // ��ȡ��ǰ������ԶԽǾ���,�ԽǾ���ֻ�贫��Խ�������
    inline void GetTimesDiagonal(const Vector3f& rDiag, 
        Matrix3f& rDesMat) const;
    // ��ȡ�ԽǾ�����Ե�ǰ����,�ԽǾ���ֻ�贫��Խ�������
    inline void GetDiagonalTimes(const Vector3f& rDiag, 
        Matrix3f& rDesMat) const;
    // this^T * M
    inline void GetTransposeTimes(const Matrix3f& rRhsMat, 
        Matrix3f& rDesMat) const;
    // this * M^T
    inline void GetTimesTranspose(const Matrix3f& rRhsMat, 
        Matrix3f& rDesMat) const;

    // ������������ת����,��ת����������Ϊ��λ����
    inline void FromAxisAngle(const Vector3f& rAxisVec, float fAngle);
    // ����Ϊ��ת����,��������ת�����ת�Ƕ�
    inline void ToAxisAngle(Vector3f& rAxisVec, float& rfAngle) const;
    // ����Ϊ��ת����,Gram-Schmidt�淶������ʹ�������Ϊ�淶�����.
    void Orthonormalize(void);

    Matrix3f& FromEulerAnglesXYZ(float fYAngle, float fPAngle, 
        float fRAngle);
    Matrix3f& FromEulerAnglesXZY(float fYAngle, float fPAngle, 
        float fRAngle);
    Matrix3f& FromEulerAnglesYXZ(float fYAngle, float fPAngle, 
        float fRAngle);
    Matrix3f& FromEulerAnglesYZX(float fYAngle, float fPAngle, 
        float fRAngle);
    Matrix3f& FromEulerAnglesZXY(float fYAngle, float fPAngle, 
        float fRAngle);
    Matrix3f& FromEulerAnglesZYX(float fYAngle, float fPAngle, 
        float fRAngle);

    bool ToEulerAnglesXYZ(float& rfYAngle, float& rfPAngle, 
        float& rfRAngle) const;
    bool ToEulerAnglesXZY(float& rfYAngle, float& rfPAngle, 
        float& rfRAngle) const;
    bool ToEulerAnglesYXZ(float& rfYAngle, float& rfPAngle, 
        float& rfRAngle) const;
    bool ToEulerAnglesYZX(float& rfYAngle, float& rfPAngle, 
        float& rfRAngle) const;
    bool ToEulerAnglesZXY(float& rfYAngle, float& rfPAngle, 
        float& rfRAngle) const;
    bool ToEulerAnglesZYX(float& rfYAngle, float& rfPAngle, 
        float& rfRAngle) const;

    static const Matrix3f ZERO;
    static const Matrix3f IDENTITY;

private:
    inline int CompareData(const Matrix3f& rMat) const;
};

//----------------------------------------------------------------------------
// Name: double precision floating-point 3x3 matrix class
// Description:
// Author: Sun Che
// Date: 20090517
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Matrix3d
{
public:
    inline double M11(void) const;
    inline double& M11(void);
    inline double M12(void) const;
    inline double& M12(void);
    inline double M13(void) const;
    inline double& M13(void);
    inline double M21(void) const;
    inline double& M21(void);
    inline double M22(void) const;
    inline double& M22(void);
    inline double M23(void) const;
    inline double& M23(void);
    inline double M31(void) const;
    inline double& M31(void);
    inline double M32(void) const;
    inline double& M32(void);
    inline double M33(void) const;
    inline double& M33(void);
private:
    double m_dData[3][3];

public:
    Matrix3d(void);
    Matrix3d(const Matrix3d& rMat);
    Matrix3d(double dM11, double dM12, double dM13,
            double dM21, double dM22, double dM23,
            double dM31, double dM32, double dM33);
    // Ĭ������������������
    Matrix3d(const Vector3d& rVecU, const Vector3d& rVecV, 
        const Vector3d& rVecW, bool bIsRow = true);
    // �����ԽǾ���
    Matrix3d(double dM11, double dM22, double dM33);
    // ��������������ת����,������,fAngle > 0Ϊ˳ʱ����ת,��ת�����Ϊ��λ����
    Matrix3d(const Vector3d& rAxisVec, double dAngle);
    // ��������U��������V����������������
    Matrix3d(const Vector3d& rVecU, const Vector3d& rVecV);

    operator const double* (void) const;
    operator double* (void);
    const double* operator [] (int iCurRow) const;
    double* operator [] (int iCurRow);
    double operator () (int iCurRow, int iCurCol) const;
    double& operator () (int iCurRow, int iCurCol);

    inline void SetRow(int iDesRow, const Vector3d& rSrcVec);
    inline void GetRow(int iSrcRow, Vector3d& rDesVec) const;
    inline void SetColumn(int iDesCol, const Vector3d& rSrcVec);
    inline void GetColumn(int iSrcCol, Vector3d& rDesVec) const;

    Matrix3d& operator = (const Matrix3d& rMat);

    bool operator == (const Matrix3d& rMat) const;
    bool operator != (const Matrix3d& rMat) const;
    bool operator < (const Matrix3d& rMat) const;
    bool operator <= (const Matrix3d& rMat) const;
    bool operator > (const Matrix3d& rMat) const;
    bool operator >= (const Matrix3d& rMat) const;

    Matrix3d operator + (const Matrix3d& rRhsMat) const;
    Matrix3d operator - (const Matrix3d& rRhsMat) const;
    Matrix3d operator * (const Matrix3d& rRhsMat) const;
    Matrix3d operator * (double dScalar) const;
    Matrix3d operator / (double dScalar) const;
    Matrix3d operator - (void) const;

    Matrix3d& operator += (const Matrix3d& rRhsMat);
    Matrix3d& operator -= (const Matrix3d& rRhsMat);
    Matrix3d& operator *= (double dScalar);
    Matrix3d& operator /= (double dScalar);

    // ����������
    Vector3d operator * (const Vector3d& rRhsVec) const;

    SE_FOUNDATION_API friend Matrix3d operator * (double dLhsScalar, 
        const Matrix3d& rRhsMat);
    SE_FOUNDATION_API friend Vector3d operator * (const Vector3d& rLhsVec, 
        const Matrix3d& rRhsMat);

    // ��Ϊ0����
    inline void Zero(void);
    // ��Ϊ��λ����
    inline void Identity(void);
    // ��Ϊ�ԽǾ���
    inline void Diagonal(double dM11, double dM22, double dM33);
    // ������������������������������
    inline void TensorProduct(const Vector3d& rVecU, const Vector3d& rVecV);
    // ���Լ�ת��
    void Transpose(void);
    // ��ȡת�þ���
    inline void GetTranspose(Matrix3d& rDesMat) const;
    // ���Լ�����
    void Inverse(void);
    // ��ȡ�����
    inline void GetInverse(Matrix3d& rDesMat) const;
    // ��ȡ�������
    inline void GetAdjoint(Matrix3d& rDesMat) const;
    // ��ȡ����ʽ
    inline double GetDeterminant(void) const;
    // ��ȡ��ǰ������ԶԽǾ���,�ԽǾ���ֻ�贫��Խ�������
    inline void GetTimesDiagonal(const Vector3d& rDiag, 
        Matrix3d& rDesMat) const;
    // ��ȡ�ԽǾ�����Ե�ǰ����,�ԽǾ���ֻ�贫��Խ�������
    inline void GetDiagonalTimes(const Vector3d& rDiag, 
        Matrix3d& rDesMat) const;
    // this^T * M
    inline void GetTransposeTimes(const Matrix3d& rRhsMat, 
        Matrix3d& rDesMat) const;
    // this * M^T
    inline void GetTimesTranspose(const Matrix3d& rRhsMat, 
        Matrix3d& rDesMat) const;

    // ������������ת����,��ת����������Ϊ��λ����
    inline void FromAxisAngle(const Vector3d& rAxisVec, double dAngle);
    // ����Ϊ��ת����,��������ת�����ת�Ƕ�
    inline void ToAxisAngle(Vector3d& rAxisVec, double& rdAngle) const;
    // ����Ϊ��ת����,Gram-Schmidt�淶������ʹ�������Ϊ�淶�����.
    void Orthonormalize(void);

    Matrix3d& FromEulerAnglesXYZ(double dYAngle, double dPAngle, 
        double dRAngle);
    Matrix3d& FromEulerAnglesXZY(double dYAngle, double dPAngle, 
        double dRAngle);
    Matrix3d& FromEulerAnglesYXZ(double dYAngle, double dPAngle, 
        double dRAngle);
    Matrix3d& FromEulerAnglesYZX(double dYAngle, double dPAngle, 
        double dRAngle);
    Matrix3d& FromEulerAnglesZXY(double dYAngle, double dPAngle, 
        double dRAngle);
    Matrix3d& FromEulerAnglesZYX(double dYAngle, double dPAngle, 
        double dRAngle);

    bool ToEulerAnglesXYZ(double& rdYAngle, double& rdPAngle, 
        double& rdRAngle) const;
    bool ToEulerAnglesXZY(double& rdYAngle, double& rdPAngle, 
        double& rdRAngle) const;
    bool ToEulerAnglesYXZ(double& rdYAngle, double& rdPAngle, 
        double& rdRAngle) const;
    bool ToEulerAnglesYZX(double& rdYAngle, double& rdPAngle, 
        double& rdRAngle) const;
    bool ToEulerAnglesZXY(double& rdYAngle, double& rdPAngle, 
        double& rdRAngle) const;
    bool ToEulerAnglesZYX(double& rdYAngle, double& rdPAngle, 
        double& rdRAngle) const;

    static const Matrix3d ZERO;
    static const Matrix3d IDENTITY;

private:
    inline int CompareData(const Matrix3d& rMat) const;
};

typedef SE_ALIGN16 Matrix3f Matrix3fA16;
typedef SE_ALIGN16 Matrix3d Matrix3dA16;

#include "SEMatrix3.inl"

}

#endif



