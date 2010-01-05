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

#include "SEManagedFrameworkPCH.h"
#include "SEManagedObjectFactory.h"
#include "SEManagedWireframeState.h"
#include "SEManagedMaterialState.h"
#include "SEManagedAlphaState.h"
#include "SEManagedPolygonOffsetState.h"
#include "SEManagedCullState.h"

using namespace Swing;
using namespace Swing::Tools::ManagedFramework;

//---------------------------------------------------------------------------
INativeGlobalState^ ManagedObjectFactory::CreateGlobalState(
    GlobalState* pState)
{
    if( !pState )
    {
        return nullptr;
    }

    GlobalState::StateType eType = pState->GetStateType();
    switch( eType )
    {
    case GlobalState::WIREFRAME:
        return gcnew ManagedWireframeState((WireframeState*)pState);
        break;

    case GlobalState::MATERIAL:
        return gcnew ManagedMaterialState((MaterialState*)pState);
        break;

    case GlobalState::ALPHA:
        return gcnew ManagedAlphaState((AlphaState*)pState);
        break;

    case GlobalState::POLYGONOFFSET:
        return gcnew ManagedPolygonOffsetState((PolygonOffsetState*)pState);
        break;

    case GlobalState::CULL:
        return gcnew ManagedCullState((CullState*)pState);
        break;

    default:
        return nullptr;
        break;
    }
}
//---------------------------------------------------------------------------