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

#include "SEFoundationPCH.h"
#include "SEConvexHull2.h"
#include "SEMapper2.h"
#include "SEQuery2Filtered.h"
#include "SEQuery2Int64.h"
#include "SEQuery2TInteger.h"
#include "SEQuery2TRational.h"

using namespace Swing;

//----------------------------------------------------------------------------
ConvexHull2f::ConvexHull2f(int iVertexCount,  Vector2f* aVertex,  float fEpsilon,  
    bool bOwner,  Query::Type eQueryType)
    :
    ConvexHullf(iVertexCount,  fEpsilon,  bOwner,  eQueryType), 
    m_LineOrigin(Vector2f::ZERO), 
    m_LineDirection(Vector2f::ZERO)
{
    SE_ASSERT(aVertex);

    m_aVertex = aVertex;
    m_aSVertex = 0;
    m_pQuery = 0;

    Mapper2f tempMapper(m_iVertexCount,  m_aVertex,  m_fEpsilon);
    if( tempMapper.GetDimension() == 0 )
    {
        // The values of m_iDimension,  m_aiIndex,  and m_aiAdjacent were
        // already initialized by the ConvexHullf base class.
        return;
    }

    if( tempMapper.GetDimension() == 1 )
    {
        // The set is (nearly) collinear.  The caller is responsible for
        // creating a ConvexHull1f object.
        m_iDimension = 1;
        m_LineOrigin = tempMapper.GetOrigin();
        m_LineDirection = tempMapper.GetDirection(0);
        return;
    }

    m_iDimension = 2;

    int i0 = tempMapper.GetExtremeIndex(0);
    int i1 = tempMapper.GetExtremeIndex(1);
    int i2 = tempMapper.GetExtremeIndex(2);

    m_aSVertex = SE_NEW Vector2f[m_iVertexCount];
    int i;

    if( eQueryType != Query::QT_RATIONAL && eQueryType != Query::QT_FILTERED )
    {
        // Transform the vertices to the square [0, 1]^2.
        Vector2f vec2fMin = tempMapper.GetMin();
        float fScale = 1.0f / tempMapper.GetMaxRange();
        for( i = 0; i < m_iVertexCount; i++ )
        {
            m_aSVertex[i] = (m_aVertex[i] - vec2fMin)*fScale;
        }

        float fExpand;
        if( eQueryType == Query::QT_INT64 )
        {
            // Scale the vertices to the square [0, 2^{20}]^2 to allow use of
            // 64-bit integers.
            fExpand = (float)(1 << 20);
            m_pQuery = SE_NEW Query2Int64f(m_iVertexCount,  m_aSVertex);
        }
        else if( eQueryType == Query::QT_INTEGER )
        {
            // Scale the vertices to the square [0, 2^{24}]^2 to allow use of
            // TInteger.
            fExpand = (float)(1 << 24);
            m_pQuery = SE_NEW Query2TIntegerf(m_iVertexCount,  m_aSVertex);
        }
        else  // eQueryType == Query::QT_REAL
        {
            // No scaling for floating point.
            fExpand = 1.0f;
            m_pQuery = SE_NEW Query2f(m_iVertexCount,  m_aSVertex);
        }

        for( i = 0; i < m_iVertexCount; i++ )
        {
            m_aSVertex[i] *= fExpand;
        }
    }
    else
    {
        // No transformation needed for exact rational arithmetic or filtered
        // predicates.
        size_t uiSize = m_iVertexCount * sizeof(Vector2f);
        System::SE_Memcpy(m_aSVertex,  uiSize,  m_aVertex,  uiSize);

        if( eQueryType == Query::QT_RATIONAL )
        {
            m_pQuery = SE_NEW Query2TRationalf(m_iVertexCount,  m_aSVertex);
        }
        else // eQueryType == Query::QT_FILTERED
        {
            m_pQuery = SE_NEW Query2Filteredf(m_iVertexCount,  m_aSVertex,  
                m_fEpsilon);
        }
    }

    HullEdge2f* pE0;
    HullEdge2f* pE1;
    HullEdge2f* pE2;

    if( tempMapper.GetExtremeCCW() )
    {
        pE0 = SE_NEW HullEdge2f(i0,  i1);
        pE1 = SE_NEW HullEdge2f(i1,  i2);
        pE2 = SE_NEW HullEdge2f(i2,  i0);
    }
    else
    {
        pE0 = SE_NEW HullEdge2f(i0,  i2);
        pE1 = SE_NEW HullEdge2f(i2,  i1);
        pE2 = SE_NEW HullEdge2f(i1,  i0);
    }

    pE0->Insert(pE2,  pE1);
    pE1->Insert(pE0,  pE2);
    pE2->Insert(pE1,  pE0);

    HullEdge2f* pHull = pE0;
    for( i = 0; i < m_iVertexCount; i++ )
    {
        if( !Update(pHull,  i) )
        {
            pHull->DeleteAll();

            return;
        }
    }

    pHull->GetIndices(m_iSimplexCount,  m_aiIndex);
    pHull->DeleteAll();
}
//----------------------------------------------------------------------------
ConvexHull2f::~ConvexHull2f()
{
    if( m_bOwner )
    {
        SE_DELETE[] m_aVertex;
    }
    SE_DELETE[] m_aSVertex;
    SE_DELETE m_pQuery;
}
//----------------------------------------------------------------------------
const Vector2f& ConvexHull2f::GetLineOrigin() const
{
    return m_LineOrigin;
}
//----------------------------------------------------------------------------
const Vector2f& ConvexHull2f::GetLineDirection() const
{
    return m_LineDirection;
}
//----------------------------------------------------------------------------
ConvexHull1f* ConvexHull2f::GetConvexHull1() const
{
    SE_ASSERT( m_iDimension == 1 );

    if( m_iDimension != 1 )
    {
        return 0;
    }

    float* afProjection = SE_NEW float[m_iVertexCount];
    for( int i = 0; i < m_iVertexCount; i++ )
    {
        Vector2f vec2fDiff = m_aVertex[i] - m_LineOrigin;
        afProjection[i] = m_LineDirection.Dot(vec2fDiff);
    }

    return SE_NEW ConvexHull1f(m_iVertexCount,  afProjection, 
        m_fEpsilon,  true,  m_eQueryType);
}
//----------------------------------------------------------------------------
bool ConvexHull2f::Update(HullEdge2f*& rpHull,  int i)
{
    // Locate an edge visible to the input point (if possible).
    HullEdge2f* pVisible = 0;
    HullEdge2f* pCurrent = rpHull;
    do
    {
        if( pCurrent->GetSign(i,  m_pQuery) > 0 )
        {
            pVisible = pCurrent;
            break;
        }

        pCurrent = pCurrent->A[1];
    }
    while( pCurrent != rpHull );

    if( !pVisible )
    {
        // The point is inside the current hull; nothing to do.
        return true;
    }

    // Remove the visible edges.
    HullEdge2f* pAdj0 = pVisible->A[0];
    SE_ASSERT( pAdj0 );

    if( !pAdj0 )
    {
        return false;
    }

    HullEdge2f* pAdj1 = pVisible->A[1];
    SE_ASSERT( pAdj1 );

    if( !pAdj1 )
    {
        return false;
    }

    pVisible->DeleteSelf();

    while( pAdj0->GetSign(i,  m_pQuery) > 0 )
    {
        rpHull = pAdj0;
        pAdj0 = pAdj0->A[0];
        SE_ASSERT( pAdj0 );

        if( !pAdj0 )
        {
            return false;
        }

        pAdj0->A[1]->DeleteSelf();
    }

    while( pAdj1->GetSign(i,  m_pQuery) > 0 )
    {
        rpHull = pAdj1;
        pAdj1 = pAdj1->A[1];
        SE_ASSERT( pAdj1 );

        if( !pAdj1 )
        {
            return false;
        }

        pAdj1->A[0]->DeleteSelf();
    }

    // Insert the new edges formed by the input point and the end points of
    // the polyline of invisible edges.
    HullEdge2f* pEdge0 = SE_NEW HullEdge2f(pAdj0->V[1],  i);
    HullEdge2f* pEdge1 = SE_NEW HullEdge2f(i,  pAdj1->V[0]);
    pEdge0->Insert(pAdj0,  pEdge1);
    pEdge1->Insert(pEdge0,  pAdj1);
    rpHull = pEdge0;

    return true;
}
//----------------------------------------------------------------------------
ConvexHull2f::ConvexHull2f(const char* acFilename)
    :
    ConvexHullf(0,  0.0f,  false,  Query::QT_REAL)
{
    m_aVertex = 0;
    m_aSVertex = 0;
    m_pQuery = 0;
    bool bLoaded = Load(acFilename);
    SE_ASSERT( bLoaded );
    (void)bLoaded;  // avoid warning in Release build
}
//----------------------------------------------------------------------------
bool ConvexHull2f::Load(const char* acFilename)
{
    FILE* pIFile = System::SE_Fopen(acFilename,  "rb");
    if( !pIFile )
    {
        return false;
    }

    ConvexHullf::Load(pIFile);

    SE_DELETE m_pQuery;
    SE_DELETE[] m_aSVertex;
    if( m_bOwner )
    {
        SE_DELETE[] m_aVertex;
    }

    m_bOwner = true;
    m_aVertex = SE_NEW Vector2f[m_iVertexCount];
    m_aSVertex = SE_NEW Vector2f[m_iVertexCount];

    size_t uiSize = sizeof(float);
    int iVC = 2*m_iVertexCount;
    if( uiSize == 4 )
    {
        System::SE_Read4le(pIFile,  iVC,  m_aVertex);
        System::SE_Read4le(pIFile,  iVC,  m_aSVertex);
        System::SE_Read4le(pIFile,  2,  (float*)m_LineOrigin);
        System::SE_Read4le(pIFile,  2,  (float*)m_LineDirection);
    }
    else // iSize == 8
    {
        System::SE_Read8le(pIFile,  iVC,  m_aVertex);
        System::SE_Read8le(pIFile,  iVC,  m_aSVertex);
        System::SE_Read8le(pIFile,  2,  (float*)m_LineOrigin);
        System::SE_Read8le(pIFile,  2,  (float*)m_LineDirection);
    }

    System::SE_Fclose(pIFile);

    switch( m_eQueryType )
    {
    case Query::QT_INT64:
        m_pQuery = SE_NEW Query2Int64f(m_iVertexCount,  m_aSVertex);

        break;
    case Query::QT_INTEGER:
        m_pQuery = SE_NEW Query2TIntegerf(m_iVertexCount,  m_aSVertex);

        break;
    case Query::QT_RATIONAL:
        m_pQuery = SE_NEW Query2TRationalf(m_iVertexCount,  m_aSVertex);

        break;
    case Query::QT_REAL:
        m_pQuery = SE_NEW Query2f(m_iVertexCount,  m_aSVertex);

        break;
    case Query::QT_FILTERED:
        m_pQuery = SE_NEW Query2Filteredf(m_iVertexCount,  m_aSVertex,  
            m_fEpsilon);

        break;
    }

    return true;
}
//----------------------------------------------------------------------------
bool ConvexHull2f::Save(const char* acFilename) const
{
    FILE* pOFile = System::SE_Fopen(acFilename,  "wb");
    if( !pOFile )
    {
        return false;
    }

    ConvexHullf::Save(pOFile);

    size_t uiSize = sizeof(float);
    int iVC = 2 * m_iVertexCount;
    if( uiSize == 4 )
    {
        System::SE_Write4le(pOFile,  iVC,  m_aVertex);
        System::SE_Write4le(pOFile,  iVC,  m_aSVertex);
        System::SE_Write4le(pOFile,  2,  (const float*)m_LineOrigin);
        System::SE_Write4le(pOFile,  2,  (const float*)m_LineDirection);
    }
    else // iSize == 8
    {
        System::SE_Write8le(pOFile,  iVC,  m_aVertex);
        System::SE_Write8le(pOFile,  iVC,  m_aSVertex);
        System::SE_Write8le(pOFile,  2,  (const float*)m_LineOrigin);
        System::SE_Write8le(pOFile,  2,  (const float*)m_LineDirection);
    }

    System::SE_Fclose(pOFile);

    return true;
}
//----------------------------------------------------------------------------