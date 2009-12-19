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

#include "SESceneEditorFrameworkPCH.h"
#include "SEManagedFramework.h"

using namespace Swing;
using namespace Swing::Tools::SceneEditor::Framework;

//---------------------------------------------------------------------------
ManagedFramework::ManagedFramework()
{
    // 运行Register.
    SE_Foundation_Register();
    SE_DX9Renderer_Register();

    // Swing Engine initialize.
    System::SE_Initialize();
    assert( Swing::System::SE_PATH[0] );
    std::string tempSEPath(System::SE_PATH);
    Main::Initialize();

    // 添加application所需资源目录.
    System::SE_InsertDirectory(".");
    std::string tempDir;
    tempDir = tempSEPath + std::string("/Data/sesp/Cg");
    System::SE_InsertDirectory(tempDir.c_str());
}
//---------------------------------------------------------------------------
ManagedFramework::~ManagedFramework()
{
    // Swing Engine terminate.
    Main::Terminate();
    System::SE_Terminate();
}
//---------------------------------------------------------------------------
void ManagedFramework::Initialize()
{
	if( !ms_thThis )
	{
		ms_thThis = gcnew ManagedFramework;
	}
}
//---------------------------------------------------------------------------
void ManagedFramework::Terminate()
{
	if( ms_thThis )
	{
		delete ms_thThis;
		ms_thThis = nullptr;
	}
}
//---------------------------------------------------------------------------
