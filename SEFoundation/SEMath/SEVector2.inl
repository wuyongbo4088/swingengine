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

//----------------------------------------------------------------------------
// 单精度2维向量类
//----------------------------------------------------------------------------
inline int SEVector2f::CompareData(const SEVector2f& rVec) const
{
    return memcmp(m_fData, rVec.m_fData, 2*sizeof(float));
}
//----------------------------------------------------------------------------
inline float SEVector2f::GetLength() const
{
    return SEMath<float>::Sqrt(
                               m_fData[0]*m_fData[0] +
                               m_fData[1]*m_fData[1]);
}
//----------------------------------------------------------------------------
inline float SEVector2f::GetSquaredLength() const
{
    return
          m_fData[0]*m_fData[0] +
          m_fData[1]*m_fData[1];
}
//----------------------------------------------------------------------------
inline float SEVector2f::Dot(const SEVector2f& rRhsVec) const
{
    return
          m_fData[0]*rRhsVec.m_fData[0] +
          m_fData[1]*rRhsVec.m_fData[1];
}
//----------------------------------------------------------------------------
inline float SEVector2f::Normalize()
{
    float fLength = GetLength();

    if( fLength > SEMath<float>::ZERO_TOLERANCE )
    {
        float fInvLength = 1.0f / fLength;
        m_fData[0] *= fInvLength;
        m_fData[1] *= fInvLength;
    }
    else
    {
        m_fData[0] = 0.0f;
        m_fData[1] = 0.0f;
    }

    return fLength;
}
//----------------------------------------------------------------------------
inline void SEVector2f::GetPerp(SEVector2f& rDesVec) const
{
	rDesVec.m_fData[0] = m_fData[1];
	rDesVec.m_fData[1] = -m_fData[0];
}
//----------------------------------------------------------------------------
inline float SEVector2f::GetDotPerp(const SEVector2f& rVec) const
{
    return m_fData[0]*rVec.m_fData[1] - m_fData[1]*rVec.m_fData[0];
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 双精度2维向量类
//----------------------------------------------------------------------------
inline int SEVector2d::CompareData(const SEVector2d& rVec) const
{
    return memcmp(m_dData, rVec.m_dData, 2*sizeof(double));
}
//----------------------------------------------------------------------------
inline double SEVector2d::GetLength() const
{
    return SEMath<double>::Sqrt(m_dData[0]*m_dData[0] + m_dData[1]*m_dData[1]);
}
//----------------------------------------------------------------------------
inline double SEVector2d::GetSquaredLength() const
{
    return
          m_dData[0]*m_dData[0] +
          m_dData[1]*m_dData[1];
}
//----------------------------------------------------------------------------
inline double SEVector2d::Dot(const SEVector2d& rRhsVec) const
{
    return
          m_dData[0]*rRhsVec.m_dData[0] +
          m_dData[1]*rRhsVec.m_dData[1];
}
//----------------------------------------------------------------------------
inline double SEVector2d::Normalize()
{
    double dLength = GetLength();

    if( dLength > SEMath<double>::ZERO_TOLERANCE )
    {
        double dInvLength = 1.0 / dLength;
        m_dData[0] *= dInvLength;
        m_dData[1] *= dInvLength;
    }
    else
    {
        m_dData[0] = 0.0;
        m_dData[1] = 0.0;
    }

    return dLength;
}
//----------------------------------------------------------------------------
inline void SEVector2d::GetPerp(SEVector2d& rDesVec) const
{
	rDesVec.m_dData[0] = m_dData[1];
	rDesVec.m_dData[1] = -m_dData[0];
}
//----------------------------------------------------------------------------
inline double SEVector2d::GetDotPerp(const SEVector2d& rVec) const
{
    return m_dData[0]*rVec.m_dData[1] - m_dData[1]*rVec.m_dData[0];
}
//----------------------------------------------------------------------------