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

#include "SESceneEditorPCH.h"
#include "MainForm.h"

using namespace Swing;
using namespace Swing::Tools::SceneEditor;

[STAThreadAttribute]
int main(array<System::String^>^)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // ����Register.
    SE_Foundation_Register();
    SE_DX9Renderer_Register();

    // Swing Engine initialize.
    SESystem::SE_Initialize();
    std::string tempSEPath(SESystem::SE_PATH);
    SEMain::Initialize();

    // ���Ǽ�鵱ǰ����Ŀ¼.
    SESystem::SE_InsertDirectory(".");

    // scene graph�ļ���·��.
    std::string tempDir;
    tempDir = tempSEPath + std::string("/Data/seof");
    SESystem::SE_InsertDirectory(tempDir.c_str());

    // texture image�ļ���·��.
    tempDir = tempSEPath + std::string("/Data/seif");
    SESystem::SE_InsertDirectory(tempDir.c_str());

    // shader program�ļ���·��.
    tempDir = tempSEPath + std::string("/Data/sesp/Cg");
    SESystem::SE_InsertDirectory(tempDir.c_str());

    Application::Run(gcnew MainForm());

    // Swing Engine terminate.
    SEMain::Terminate();
    SESystem::SE_Terminate();

    return 0;
}
