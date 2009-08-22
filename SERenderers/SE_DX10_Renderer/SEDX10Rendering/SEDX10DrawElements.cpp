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

#include "SEDX10RendererPCH.h"
#include "SEDX10Renderer.h"
#include "SEPolyline.h"

using namespace Swing;

D3D10_PRIMITIVE_TOPOLOGY DX10Renderer::ms_aeObjectType[Geometry::GT_MAX_COUNT] =
{
    D3D10_PRIMITIVE_TOPOLOGY_POINTLIST,      // GT_POLYPOINT
    D3D10_PRIMITIVE_TOPOLOGY_LINELIST,       // GT_POLYLINE_SEGMENTS
    D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP,      // GT_POLYLINE_OPEN
    D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP,      // GT_POLYLINE_CLOSED
    D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST    // GT_TRIMESH
};

//----------------------------------------------------------------------------
void DX10Renderer::DrawElements()
{
    IndexBuffer* pIBuffer = m_pGeometry->IBuffer;
    SE_ASSERT( pIBuffer );

    D3D10_PRIMITIVE_TOPOLOGY eType = ms_aeObjectType[m_pGeometry->Type];
    m_pDX10Device->IASetPrimitiveTopology(eType);
    m_pDX10Device->DrawIndexed(pIBuffer->GetIndexCount(), 0, 0);
}
//----------------------------------------------------------------------------
