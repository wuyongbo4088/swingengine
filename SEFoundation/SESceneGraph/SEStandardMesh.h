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

#ifndef Swing_StandardMesh_H
#define Swing_StandardMesh_H

#include "SEFoundationLIB.h"
#include "SETriMesh.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:Standard mesh��
// ˵��:	
// ����:Sun Che
// ʱ��:20081015
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SEStandardMesh
{
public:
    SEStandardMesh(const SEAttributes& rAttr, bool bInside = false,
        const SETransformation* pXFrm = 0);
    ~SEStandardMesh(void);

    void SetTransformation(const SETransformation& rXFrm);
    const SETransformation& GetTransformation(void) const;

    // ���ֱ�׼����mesh. 
    // ÿ��mesh��(0,0,0)Ϊ����,up-axisΪ(0,0,-1).����axesΪ(1,0,0)��(0,1,0).
    // Ӧ�ó�����Զ�mesh���б任.

    SETriMesh* Rectangle(int iXSamples, int iYSamples, float fXExtent,
        float fYExtent);

    SETriMesh* Disk(int iShellSamples, int iRadialSamples, float fRadius);

    SETriMesh* Box(float fXExtent, float fYExtent, float fZExtent);

    SETriMesh* Cylinder(int iAxisSamples, int iRadialSamples, float fRadius,
        float fHeight, bool bOpen);

    SETriMesh* Sphere(int iZSamples, int iRadialSamples, float fRadius);

    SETriMesh* Torus(int iCircleSamples, int iRadialSamples,
        float fOuterRadius, float fInnerRadius);

    SETriMesh* Cone(int iRadialSamples, float fRadius, float fHeight);

    // ����platonic������, �ڽ���һ����(0,0,0)Ϊ���ĵĵ�λ����.
    SETriMesh* Tetrahedron(void);  // ������
    SETriMesh* Hexahedron(void);   // ������
    SETriMesh* Octahedron(void);   // ������
    SETriMesh* Dodecahedron(void); // ʮ������
    SETriMesh* Icosahedron(void);  // ��ʮ����

private:
    void CreatePlatonicNormals(SEVertexBuffer* pVBuffer);
    void CreatePlatonicUVs(SEVertexBuffer* pVBuffer);
    void ReverseTriangleOrder(int iTCount, int* aiIndex);
    void TransformData(SEVertexBuffer* pVBuffer);

    SEAttributes m_Attr;
    SETransformation m_XFrm;
    bool m_bInside;
    SEVertexBuffer* m_pVBuffer;
    SEIndexBuffer* m_pIBuffer;
};

}

#endif
