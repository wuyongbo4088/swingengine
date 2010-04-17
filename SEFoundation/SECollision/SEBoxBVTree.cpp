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
#include "SEBoxBVTree.h"
#include "SEContBox3.h"

using namespace Swing;

SE_IMPLEMENT_INITIALIZE(SEBoxBVTree);

//SE_REGISTER_INITIALIZE(SEBoxBVTree);

//----------------------------------------------------------------------------
void SEBoxBVTree::Initialize()
{
    ms_aoCreateModelBound[BoundingVolume::BV_BOX] =
        &SEBoxBVTree::CreateModelBound;

    ms_aoCreateWorldBound[BoundingVolume::BV_BOX] =
        &SEBoxBVTree::CreateWorldBound;
}
//----------------------------------------------------------------------------
SEBoxBVTree::SEBoxBVTree(const TriMesh* pMesh, int iMaxTrisPerLeaf,
    bool bStoreInteriorTris)
    :
    SEBoundingVolumeTree(BoundingVolume::BV_BOX, pMesh, iMaxTrisPerLeaf,
        bStoreInteriorTris)
{
}
//----------------------------------------------------------------------------
BoundingVolume* SEBoxBVTree::CreateModelBound(const TriMesh* pMesh, int i0,
    int i1, int* aiISplit, SELine3f& rLine)
{
    // 标记出在当前子网格中用过的那些顶点.
    int iVCount = pMesh->VBuffer->GetVertexCount();
    const int* aiIndex = pMesh->IBuffer->GetData();
    bool* abValid = SE_NEW bool[iVCount];
    memset(abValid, 0, iVCount*sizeof(bool));
    int i;
    for( i = i0; i <= i1; i++ )
    {
        int j = 3 * aiISplit[i];
        abValid[aiIndex[j++]] = true;
        abValid[aiIndex[j++]] = true;
        abValid[aiIndex[j++]] = true;
    }

    // 创建一个针对当前子网格的连续顶点数组.
    std::vector<SEVector3f> tempMeshVertices;
    for( i = 0; i < iVCount; i++ )
    {
        if( abValid[i] )
        {
            tempMeshVertices.push_back(pMesh->VBuffer->Position3(i));
        }
    }
    SE_DELETE[] abValid;

    BoxBV* pModelBound = SE_NEW BoxBV;
    pModelBound->Box() = ContOBBf((int)tempMeshVertices.size(),
        &tempMeshVertices.front());

    // 待检查.
    rLine.Origin = pModelBound->Box().Center;
    rLine.Direction = pModelBound->Box().Axis[2];

    return pModelBound;
}
//----------------------------------------------------------------------------
BoundingVolume* SEBoxBVTree::CreateWorldBound()
{
    return SE_NEW BoxBV;
}
//----------------------------------------------------------------------------
