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

#ifndef Swing_Particles_H
#define Swing_Particles_H

#include "SEFoundationLIB.h"
#include "SETriMesh.h"
#include "SEFloatArray.h"

namespace Swing
{

class Camera;
class Culler;

// particle's billboard quad.
//
//  1 --------- 2
//    | \     |
//    |   \   |
//    |     \ |
//  0 --------- 3

//----------------------------------------------------------------------------
// ����:particles��
// ˵��:���ڶ�̬ˢ��VB��particle system.
// ����:Sun Che
// ʱ��:20090602
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Particles : public TriMesh
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    Particles(const Attributes& rAttr, Vector3fArray* pLocations,
        FloatArray* pSizes);
    virtual ~Particles(void);

    // ���ݳ�Ա.
    Vector3fArrayPtr Locations;
    FloatArrayPtr Sizes;
    float SizeAdjust;

    void SetActiveCount(int iActiveCount);
    inline int GetActiveCount(void) const;

protected:
    Particles(void);

    // �������Ӷ��ǳ���camera�Ĺ����.
    void GenerateParticles(const Camera* pCamera);

    // culling.
    virtual void GetUnculledSet(Culler& rCuller, bool bNoCull);

    // �����û�ָ������ʵ���������������������draw.
    int m_iActiveCount;
};

typedef SmartPointer<Particles> ParticlesPtr;

#include "SEParticles.inl"

}

#endif
