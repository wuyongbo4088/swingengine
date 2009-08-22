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

// 矩阵存储方式采用行优先方式.
// 注意DirectX采用行优先方式,OpenGL采用列优先方式.
// 例如,DirectX中给定一个矩阵M,OpenGL中与之对应的矩阵是M^T

// 矩阵运算采用向右连接的方式.例如,给定一个矩阵M和一个向量V,向量乘以矩阵是
// V*M.V被看作行向量.有些图形API采用M*V的形式,此时V被看作列向量,此时的M实际
// 上是V*M情况时的M的转置矩阵.一系列连续的矩阵变换可表示为V' = V*M0*M1...Mn,
// 有些图形API采用向左连接的方式,此时V' = Mn...M1*M0*V.
// 注意DirectX采用向右连接的方式,OpenGL采用向左连接的方式.
// 例如,DirectX中给定矩阵M1,M2,OpenGL中与它们对应的是M1^T,M2^T,
// M1*M2对应(M1*M2)^T = M2^T*M1^T.

// 旋转矩阵特性,参考<<Quaternions and rotation sequences by Jack B. Kuipers>>.
// 正交矩阵是形如: A*A^T = I的矩阵,即A^T = A^(-1),且det^2(A) = det^2(A^T) = 1.
// 旋转矩阵的充要条件(iff):
// 旋转矩阵是正交矩阵,并且其行列式的值必为+1.
// 正交矩阵的逆矩阵是其转置矩阵.为避免求逆,可用行向量左乘其转置矩阵达到逆变换
// 的目的.
// 实际上,转置也可以避免,只要用等价的列向量右乘该矩阵,就达到了不求逆不转置进行
// 逆变换的目的.

// 如果使用行向量左乘进行变换,
// 如果规定t > 0时为沿纸面(观察方向的反方向)逆时针旋转,
// 则XY平面旋转矩阵为:
//   R =  cos(t)  sin(t)
//       -sin(t)  cos(t)

#include "SEFoundationLIB.h"
#include "SEVector2.h"

namespace Swing
{

//----------------------------------------------------------------------------
// 名称:单精度2阶方阵类
// 说明:
// 作者:Sun Che
// 时间:20070512
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
	// 默认用行向量构建矩阵
    Matrix2f(const Vector2f& rVecU, const Vector2f& rVecV, bool bIsRow = true);
    // 创建对角矩阵
    Matrix2f(float fM11, float fM22);
	// 创建旋转矩阵,弧度制,fAngle > 0为逆时针旋转
    Matrix2f(float fAngle);
    // 用列向量U和行向量V的张量积构建矩阵
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

    // 置为0矩阵
    inline void Zero(void);
    // 置为单位矩阵
    inline void Identity(void);
    // 置为对角矩阵
    inline void Diagonal(float fM11, float fM22);
    // 列向量与行向量的张量积构建矩阵
    inline void TensorProduct(const Vector2f& rVecU, const Vector2f& rVecV);
    // 将自己转置
    void Transpose(void);
    // 获取转置矩阵
    inline void GetTranspose(Matrix2f& rDesMat) const;
    // 将自己求逆
    void Inverse(void);
    // 获取逆矩阵
    inline void GetInverse(Matrix2f& rDesMat) const;
    // 获取伴随矩阵
    inline void GetAdjoint(Matrix2f& rDesMat) const;
    // 获取行列式
    inline float GetDeterminant(void) const;
    // this^T * M
    inline void GetTransposeTimes(const Matrix2f& rRhsMat, 
        Matrix2f& rDesMat) const;
    // this * M^T
    inline void GetTimesTranspose(const Matrix2f& rRhsMat, 
        Matrix2f& rDesMat) const;

    // 构建旋转矩阵
    inline void FromAngle(float fAngle);
    // 必须为旋转矩阵,解析出旋转角度
    inline void ToAngle(float& rfAngle) const;

    static const Matrix2f ZERO;
    static const Matrix2f IDENTITY;

private:
    inline int CompareData(const Matrix2f& rMat) const;
};

//----------------------------------------------------------------------------
// 名称:双精度2阶方阵类
// 说明:
// 作者:Sun Che
// 时间:20090516
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
	// 默认用行向量构建矩阵
    Matrix2d(const Vector2d& rVecU, const Vector2d& rVecV, bool bIsRow = true);
    // 创建对角矩阵
    Matrix2d(double dM11, double dM22);
	// 创建旋转矩阵,弧度制,dAngle > 0为逆时针旋转
    Matrix2d(double dAngle);
    // 用列向量U和行向量V的张量积构建矩阵
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

    // 置为0矩阵
    inline void Zero(void);
    // 置为单位矩阵
    inline void Identity(void);
    // 置为对角矩阵
    inline void Diagonal(double dM11, double dM22);
    // 列向量与行向量的张量积构建矩阵
    inline void TensorProduct(const Vector2d& rVecU, const Vector2d& rVecV);
    // 将自己转置
    void Transpose(void);
    // 获取转置矩阵
    inline void GetTranspose(Matrix2d& rDesMat) const;
    // 将自己求逆
    void Inverse(void);
    // 获取逆矩阵
    inline void GetInverse(Matrix2d& rDesMat) const;
    // 获取伴随矩阵
    inline void GetAdjoint(Matrix2d& rDesMat) const;
    // 获取行列式
    inline double GetDeterminant(void) const;
    // this^T * M
    inline void GetTransposeTimes(const Matrix2d& rRhsMat, 
        Matrix2d& rDesMat) const;
    // this * M^T
    inline void GetTimesTranspose(const Matrix2d& rRhsMat, 
        Matrix2d& rDesMat) const;

    // 构建旋转矩阵
    inline void FromAngle(double dAngle);
    // 必须为旋转矩阵,解析出旋转角度
    inline void ToAngle(double& rdAngle) const;

    static const Matrix2d ZERO;
    static const Matrix2d IDENTITY;

private:
    inline int CompareData(const Matrix2d& rMat) const;
};

#include "SEMatrix2.inl"

}

#endif



