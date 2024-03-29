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

#include "TerrainLod.h"

SE_WINDOW_APPLICATION(TerrainLod);

SE_REGISTER_INITIALIZE(TerrainLod);

//----------------------------------------------------------------------------
TerrainLod::TerrainLod()
    :
    SEWindowApplication3("TerrainLod", 0, 0, 640, 480, 
        SEColorRGBA(0.5f, 0.5f, 0.5f, 1.0f)),
        m_FogColor(SEColorRGB(0.5686f, 0.7255f, 0.8353f)),
        m_fUVBias(0.0f),
        m_fFogDensity(0.0005f),
        m_fVerticalDistance(10.0f)
{
    SESystem::SE_InsertDirectory("../../Data/Terrain/Height32");
    SESystem::SE_InsertDirectory("../../Data/Terrain/Image32/");
    SESystem::SE_InsertDirectory("../../Data/Terrain");

    m_bLod = true;
}
//----------------------------------------------------------------------------
bool TerrainLod::OnInitialize()
{
    if( !SEWindowApplication3::OnInitialize() )
    {
        return false;
    }

    m_spCamera->SetFrustum(-0.55f, 0.55f, -0.4125f, 0.4125f, 1.0f, 5000.0f);
    SEVector3f tempCLoc(2048.0f, m_fVerticalDistance+50, 2048.0f);
    SEVector3f tempCDir(1.0f, 0.0f, 1.0f);
    SEVector3f tempCUp(0.0f, 1.0f, .0f);
    SEVector3f tempCRight = tempCUp.Cross(tempCDir);
    m_spCamera->SetFrame(tempCLoc, tempCRight, tempCUp, tempCDir);

    CreateScene();

    if( m_spRoamTerrain )
    {
        m_spRoamTerrain->Simplify(m_Culler);
    }

    // initial update of objects
    m_spScene->UpdateGS();
    m_spScene->UpdateRS();

    // initial culling of scene
    m_Culler.SetCamera(m_spCamera);
    m_Culler.ComputeUnculledSet(m_spScene);

    InitializeCameraMotion(0.5f, 0.01f);
    InitializeObjectMotion(m_spScene);
    return true;
}
//----------------------------------------------------------------------------
void TerrainLod::OnTerminate()
{
    m_spScene = 0;
    m_spRoamTerrain = 0;
    m_spWireframe = 0;
    SEWindowApplication3::OnTerminate();
}
//----------------------------------------------------------------------------
void TerrainLod::OnIdle()
{
    //m_spRoamTerrain->Simplify(m_Culler);

    MeasureTime();

    if( MoveCamera() )
    {
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
bool TerrainLod::OnKeyDown(unsigned char ucKey, int iX, int iY)
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
        TestStreaming(m_spScene, 128, 128, 640, 480, "TerrainLod.seof");
        return true;

    case 'l':
        if( m_spRoamTerrain )
        {
            m_bLod = !m_bLod;
            m_spRoamTerrain->SetLod(m_bLod);
        }
        return true;
    }

    return false;
}
//----------------------------------------------------------------------------
void TerrainLod::CreateScene()
{
    m_spScene = SE_NEW SENode;
    m_spWireframe = SE_NEW SEWireframeState;
    m_spScene->AttachGlobalState(m_spWireframe);

    // Create ROAM terrain.
    SEAttributes tempAttr;
    tempAttr.SetPositionChannels(3);
    tempAttr.SetNormalChannels(3);
    tempAttr.SetTCoordChannels(0, 2);  // terrain base texture
    tempAttr.SetTCoordChannels(1, 2);  // detail texture
    SEColorRGBA tempBorderColor(m_FogColor[0], m_FogColor[1], m_FogColor[2],
        1.0f);
    m_spRoamTerrain = SE_NEW SERoamTerrain("height", "image", tempAttr, 
        m_spCamera, m_fUVBias, &tempBorderColor);
    m_spScene->AttachChild(m_spRoamTerrain);

    int iRows = m_spRoamTerrain->GetRowCount();
    int iCols = m_spRoamTerrain->GetColCount();
    for( int iRow = 0; iRow < iRows; iRow++ )
    {
        for( int iCol = 0; iCol < iCols; iCol++ )
        {
            SERoamTerrainPage* pPage = m_spRoamTerrain->GetPage(iRow, iCol);

            // The default texturing uses a single base texture. Change this
            // to use a base texture modulated by a detail texture and the
            // result blended with fog.
            SEMultitextureEffect* pEffect = DynamicCast<SEMultitextureEffect>(
                pPage->GetEffect(0));
            std::string tempBaseName = pEffect->GetImageName(0);
            pPage->DetachEffect(pEffect);

            SETerrainEffect* pTEffect = SE_NEW SETerrainEffect(
                tempBaseName.c_str(), "Detail", m_FogColor, m_fFogDensity);
            pPage->AttachEffect(pTEffect);
        }
    }
}
//----------------------------------------------------------------------------
