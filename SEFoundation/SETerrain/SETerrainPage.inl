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
inline int SETerrainPage::GetSize() const
{
    return m_iSize;
}
//----------------------------------------------------------------------------
inline const unsigned short* SETerrainPage::GetHeights() const
{
    return m_ausHeight;
}
//----------------------------------------------------------------------------
inline const SEVector2f& SETerrainPage::GetOrigin() const
{
    return m_Origin;
}
//----------------------------------------------------------------------------
inline float SETerrainPage::GetMinElevation() const
{
    return m_fMinElevation;
}
//----------------------------------------------------------------------------
inline float SETerrainPage::GetMaxElevation() const
{
    return m_fMaxElevation;
}
//----------------------------------------------------------------------------
inline float SETerrainPage::GetSpacing() const
{
    return m_fSpacing;
}
//----------------------------------------------------------------------------
inline float SETerrainPage::GetX(int iX) const
{
    return m_Origin.X + m_fSpacing*float(iX);
}
//----------------------------------------------------------------------------
inline float SETerrainPage::GetZ(int iZ) const
{
    return m_Origin.Y + m_fSpacing*float(iZ);
}
//----------------------------------------------------------------------------
inline float SETerrainPage::GetHeight(int iIndex) const
{
    return m_fMinElevation + m_fMultiplier*float(m_ausHeight[iIndex]);
}
//----------------------------------------------------------------------------
inline float& SETerrainPage::UVBias()
{
    return m_fUVBias;
}
//----------------------------------------------------------------------------
inline float SETerrainPage::GetTextureCoordinate(int iIndex) const
{
    return m_fTextureSpacing * float(iIndex);
}
//----------------------------------------------------------------------------
