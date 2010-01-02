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

#pragma once

#include "SEINativeObject.h"
#include "SEINativeSpatial.h"
#include "SEINativeGeometry.h"
#include "SEINativeEffect.h"

namespace Swing{ namespace Tools{ namespace ManagedFramework{

//----------------------------------------------------------------------------
// Name:Managed triangle mesh class
// Description:
// Author:Sun Che
// Date:20091231
//----------------------------------------------------------------------------
public ref class ManagedTriMesh sealed : INativeObject, INativeSpatial, 
    INativeGeometry
{
public:
    ManagedTriMesh(void);
    ~ManagedTriMesh(void);

    // Implement INativeSpatial interface.
    virtual void UpdateGS(double dAppTime);
    //
    virtual void UpdateRS(void);
    //
    virtual void AttachEffect(INativeEffect^ thEffect);
    virtual void DetachEffect(INativeEffect^ thEffect);
    virtual void DetachAllEffects(void);
    //
    virtual int GetGlobalStateCount(void);
    virtual INativeGlobalState^ GetGlobalState(int i);
    virtual INativeGlobalState^ GetGlobalState(
        INativeGlobalState::StateType eType);
    virtual void AttachGlobalState(INativeGlobalState^ thState);
    virtual void DetachGlobalState(INativeGlobalState::StateType eType);
    virtual void DetachAllGlobalStates(void);

    // Implement INativeObject interface.
    virtual int GetNativeReferences(void);

internal:
    [CLSCompliant(false)]
    ManagedTriMesh(TriMesh* pTriMesh);

    // Implement INativeSpatial interface.
    [CLSCompliant(false)]
    virtual Spatial* GetNativeSpatial(void) = INativeSpatial::GetNativeSpatial;

    // Implement INativeGeometry interface.
    [CLSCompliant(false)]
    virtual Geometry* GetNativeGeometry(void) = 
        INativeGeometry::GetNativeGeometry;

private:
    TriMeshPtr* m_pspTriMesh;
};

}}}