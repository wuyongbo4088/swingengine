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

#include "Billboard.h"

SE_WINDOW_APPLICATION(Billboard);

SE_REGISTER_INITIALIZE(Billboard);

//----------------------------------------------------------------------------
Billboard::Billboard()
    :
    SEWindowApplication3("Billboard", 0, 0, 640, 480, 
        SEColorRGBA(0.5f, 0.5f, 0.5f, 1.0f))
{
}
//----------------------------------------------------------------------------
bool Billboard::OnInitialize()
{
    if( !SEWindowApplication3::OnInitialize() )
    {
        return false;
    }

    m_spCamera->SetFrustum(-0.55f, 0.55f, -0.4125f, 0.4125f, 1.0f, 100.0f);
    SEVector3f tempCLoc(0.0f, 0.0f, -5.0f);
    SEVector3f tempCDir(0.0f, 0.0f, 1.0f);
    SEVector3f tempCUp(0.0f, 1.0f, 0.0f);
    SEVector3f tempCRight = tempCUp.Cross(tempCDir);
    m_spCamera->SetFrame(tempCLoc, tempCRight, tempCUp, tempCDir);

    CreateScene();

    m_spScene->UpdateGS();
    m_spScene->UpdateRS();

    m_Culler.SetCamera(m_spCamera);
    m_Culler.ComputeUnculledSet(m_spScene);

    InitializeCameraMotion(0.01f, 0.001f);
    InitializeObjectMotion(m_spScene);
    return true;
}
//----------------------------------------------------------------------------
void Billboard::OnTerminate()
{
    m_spScene = 0;
    m_spBillboard0 = 0;
    m_spWireframe = 0;
    SEWindowApplication3::OnTerminate();
}
//----------------------------------------------------------------------------
void Billboard::OnIdle()
{
    MeasureTime();

    if( MoveCamera() )
    {
        m_spBillboard0->UpdateGS();
        m_Culler.ComputeUnculledSet(m_spScene);
    }

    if( MoveObject() )
    {
        m_spScene->UpdateGS();
        m_Culler.ComputeUnculledSet(m_spScene);
    }

    m_pRenderer->ClearBuffers();
    if( m_pRenderer->BeginScene() )
    {
        m_pRenderer->DrawScene(m_Culler.GetVisibleSet());
        DrawFrameRate(8, 20, SEColorRGBA::SE_RGBA_WHITE);
        m_pRenderer->EndScene();
    }
    m_pRenderer->DisplayBackBuffer();

    UpdateFrameCount();
}
//----------------------------------------------------------------------------
bool Billboard::OnKeyDown(unsigned char ucKey, int iX, int iY)
{
    if( SEWindowApplication3::OnKeyDown(ucKey, iX, iY) )
    {
        return true;
    }

    switch( ucKey )
    {
    case 'w':
    case 'W':
		m_spWireframe->Enabled = !m_spWireframe->Enabled;

#if defined(SE_USING_DX10)
        // DX10 render state objects must be re-created based on our new states.
        m_spScene->UpdateRS();
#endif

        return true;
    case 's':
    case 'S':
        TestStreaming(m_spScene, 128, 128, 640, 480, "Billboard.seof");
        return true;
    }

    return false;
}
//----------------------------------------------------------------------------
void Billboard::CreateScene()
{
    m_spScene = SE_NEW SENode;
    m_spWireframe = SE_NEW SEWireframeState;
    m_spScene->AttachGlobalState(m_spWireframe);

    m_spBillboard0 = SE_NEW SEBillboardNode(m_spCamera);
    m_spScene->AttachChild(m_spBillboard0);

    SEAttributes tempAttr;
    tempAttr.SetPositionChannels(3);
    tempAttr.SetTCoordChannels(0, 2);
    SEStandardMesh tempSM(tempAttr);

    SETriMesh* pMesh = tempSM.Rectangle(2, 2, 1.0f, 1.0f);
    m_spBillboard0->AttachChild(pMesh);
    SEShaderEffect* pEffect = SE_NEW SETextureEffect("kate");
    pMesh->AttachEffect(pEffect);

    // floor.
    pMesh = tempSM.Rectangle(4, 4, 5.0f, 5.0f);
    SEMatrix3f mat3fRot(SEVector3f::UNIT_X, SEMathf::PI/2.0f);
    pMesh->Local.SetRotate(mat3fRot);
    pMesh->Local.SetTranslate(SEVector3f(0.0f, -1.0f, 0.0f));
    m_spScene->AttachChild(pMesh);
    pEffect = SE_NEW SETextureEffect("wood512");
    pMesh->AttachEffect(pEffect);
}
//----------------------------------------------------------------------------
