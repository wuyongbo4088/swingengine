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

#ifndef Swing_BasicMesh_H
#define Swing_BasicMesh_H

#include "SEFoundationLIB.h"
#include "SEVector3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:BasicMesh��
// ˵��:
// ����:Sun Che
// ʱ��:20081123
//----------------------------------------------------------------------------
class SE_FOUNDATION_API BasicMesh
{
public:
    // ����Ķ�������iVCount.
    // ����������aPoint����,�����ݵ����ݿ�����������������(����points+attributes).
    // ����������iTCount.������������int������ʾ,��aiIndex����.
    // ������������ɾ�����������.

    BasicMesh(int iVCount, const void* aPoint, int iTCount,
        const int* aiIndex);

    virtual ~BasicMesh(void);

    // �������ݱ���Ϊ�Ϸ�mesh����.
    // Ҳ����˵������edgeֻ�ܱ��������������µ�������������.
    // �ڹ���������,���һ��edge���������ϵ������ι���,��IsValid����false.
    // ���򷵻�true.
    inline bool IsValid(void) const;

    class SE_FOUNDATION_API Vertex
    {
    public:
        Vertex(void);
        ~Vertex(void);

        enum { MV_CHUNK = 8 };

        void InsertEdge(int iV, int iE);
        void InsertTriangle(int iT);

        // �ڽӶ���/��,��Ȼ���.
        int VCount;
        int* V;
        int* E;
        // �ڽ�������.
        int TCount;
        int* T;
    };

    class SE_FOUNDATION_API Edge
    {
    public:
        Edge(void);

        int V[2];
        int T[2];
    };

    class SE_FOUNDATION_API Triangle
    {
    public:
        Triangle(void);

        int V[3];
        int E[3];
        int T[3];
    };

    inline int GetVCount(void) const;
    inline int GetECount(void) const;
    inline int GetTCount(void) const;
    inline const void* GetPoints(void) const;
    inline const int* GetIndices(void) const;
    inline const Vertex* GetVertices(void) const;
    inline const Edge* GetEdges(void) const;
    inline const Triangle* GetTriangles(void) const;

protected:
    int m_iVCount, m_iECount, m_iTCount;
    const void* m_aPoint;
    const int* m_aiIndex;
    Vertex* m_aVertex;
    Edge* m_aEdge;
    Triangle* m_aTriangle;
    bool m_bIsValid;
};

#include "SEBasicMesh.inl"

}

#endif
