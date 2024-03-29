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

#include "ShadowMaps.h"

SE_WINDOW_APPLICATION(ShadowMaps);
SE_REGISTER_INITIALIZE(ShadowMaps);

char* ShadowMaps::ms_aacCaptions[3] =
{
    "mouse moves scene and light together (press 'm' to change)", 
    "mouse moves scene (press 'm' to change)", 
    "mouse moves light (press 'm' to change)"
};

//----------------------------------------------------------------------------
ShadowMaps::ShadowMaps()
    :
    SEWindowApplication3("ShadowMaps", 0, 0, 800, 600, 
        SEColorRGBA(0.0f, 0.25f, 0.75f, 1.0f))
{
    m_acCaption = 0;
    m_pFrameBuffer = 0;
}
//----------------------------------------------------------------------------
bool ShadowMaps::OnInitialize()
{
    if( !SEWindowApplication3::OnInitialize() )
    {
        return false;
    }

    // set up camera
    m_spCamera->SetFrustum(60.0f, 4.0f/3.0f, 1.0f, 100.0f);
    SEVector3f tempCLoc(0.0f, 0.0f, -10.0f);
    SEVector3f tempCDir(0.0f, 0.0f, 1.0f);
    SEVector3f tempCUp(0.0f, 1.0f, 0.0f);
    SEVector3f tempCRight = tempCUp.Cross(tempCDir);
    m_spCamera->SetFrame(tempCLoc, tempCRight, tempCUp, tempCDir);

    CreateScene();

    // initial update of objects
    m_spScene->UpdateGS();
    m_spScene->UpdateRS();

    // initial culling of scene
    m_Culler.SetCamera(m_spCamera);
    m_Culler.ComputeUnculledSet(m_spScene);

    InitializeCameraMotion(0.01f, 0.002f);
    InitializeObjectMotion(m_spScene);
    m_acCaption = ms_aacCaptions[0];

    //m_pRenderer->ToggleFullscreen();

    return true;
}
//----------------------------------------------------------------------------
void ShadowMaps::OnTerminate()
{
    SEFrameBuffer::Destroy(m_pFrameBuffer);

    m_spScene = 0;
    m_spShadowMapNode = 0;
    m_spFlashlight = 0;
    m_spGround = 0;
    m_spTorus = 0;
    m_spGlobe = 0;
    m_spLightSphere = 0;
    m_spWireframe = 0;
    m_spSMEffect = 0;
    m_spPMEffect = 0;
    m_spLight = 0;
    m_spScreenCamera = 0;
    m_spScenePolygon1 = 0;
    m_spSceneImage = 0;

    SEWindowApplication3::OnTerminate();
}
//----------------------------------------------------------------------------
void ShadowMaps::OnIdle()
{
    MeasureTime();

    //m_spFlashlight->World.GetRotate().GetRow(2, m_spPMEffect->LightDirection); 

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
        // 把场景渲染到一个render target.
        m_pFrameBuffer->Enable();
        m_pRenderer->SetClearColor(SEColorRGBA(0.5f, 0.5f, 0.5f, 1.0f));
        m_pRenderer->ClearBuffers();
        m_pRenderer->DrawScene(m_Culler.GetVisibleSet());
        m_pFrameBuffer->Disable();

        // 把场景渲染到backbuffer.
        //m_pRenderer->SetBackgroundColor(SEColorRGBA(0.0f, 0.0f, 1.0f, 1.0f));
        //m_pRenderer->ClearBuffers();
        //m_pRenderer->DrawScene(m_Culler.GetVisibleSet());

        m_pRenderer->SetCamera(m_spScreenCamera);
        m_pRenderer->Draw(m_spScenePolygon1);

        m_pRenderer->SetCamera(m_spCamera);
        //m_pRenderer->Draw(8, 16, SEColorRGBA::SE_RGBA_WHITE, m_acCaption);
        DrawFrameRate(8, 20, SEColorRGBA::SE_RGBA_WHITE);

        m_pRenderer->EndScene();
    }
    m_pRenderer->DisplayBackBuffer();

    UpdateFrameCount();
}
//----------------------------------------------------------------------------
bool ShadowMaps::OnKeyDown(unsigned char ucKey, int iX, int iY)
{
    if( SEWindowApplication3::OnKeyDown(ucKey, iX, iY) )
    {
        return true;
    }

    const float fTrnDelta = 0.1f;
    SEVector3f vec3fTrn;

    switch( ucKey )
    {
    // Toggle wireframe for the entire scene.
    case 'w':
    case 'W':
        m_spWireframe->Enabled = !m_spWireframe->Enabled;
        return true;

    // Move the spotlight within its plane of constant z.
    case 'x':
        vec3fTrn = m_spFlashlight->Local.GetTranslate();
        vec3fTrn.X -= fTrnDelta;
        m_spFlashlight->Local.SetTranslate(vec3fTrn);
        m_spFlashlight->UpdateGS();
        return true;
    case 'X':
        vec3fTrn = m_spFlashlight->Local.GetTranslate();
        vec3fTrn.X += fTrnDelta;
        m_spFlashlight->Local.SetTranslate(vec3fTrn);
        m_spFlashlight->UpdateGS();
        return true;
    case 'y':
        vec3fTrn = m_spFlashlight->Local.GetTranslate();
        vec3fTrn.Y -= fTrnDelta;
        m_spFlashlight->Local.SetTranslate(vec3fTrn);
        m_spFlashlight->UpdateGS();
        return true;
    case 'Y':
        vec3fTrn = m_spFlashlight->Local.GetTranslate();
        vec3fTrn.Y += fTrnDelta;
        m_spFlashlight->Local.SetTranslate(vec3fTrn);
        m_spFlashlight->UpdateGS();
        return true;
    case 'z':
        vec3fTrn = m_spFlashlight->Local.GetTranslate();
        vec3fTrn.Z -= fTrnDelta;
        m_spFlashlight->Local.SetTranslate(vec3fTrn);
        m_spFlashlight->UpdateGS();
        return true;
    case 'Z':
        vec3fTrn = m_spFlashlight->Local.GetTranslate();
        vec3fTrn.Z += fTrnDelta;
        m_spFlashlight->Local.SetTranslate(vec3fTrn);
        m_spFlashlight->UpdateGS();
        return true;

    // Change the object affected by the virtual trackball.
    case 'm':
    case 'M':
        if( m_spMotionObject == m_spScene )
        {
            m_spMotionObject = m_spShadowMapNode;
            m_acCaption = ms_aacCaptions[1];
        }
        else if( m_spMotionObject == m_spShadowMapNode )
        {
            m_spMotionObject = m_spFlashlight;
            m_acCaption = ms_aacCaptions[2];
        }
        else if( m_spMotionObject == m_spFlashlight )
        {
            m_spMotionObject = m_spScene;
            m_acCaption = ms_aacCaptions[0];
        }
        return true;

    // Adjust the polygon offset scale.
    case '+':
    case '=':
        m_spSMEffect->SetDepthBias(m_spSMEffect->GetDepthBias()+0.001f);
        return true;

    case '-':
    case '_':
        m_spSMEffect->SetDepthBias(m_spSMEffect->GetDepthBias()-0.001f);
        return true;

    case 's':
    case 'S':
        TestStreaming(m_spScene, 128, 128, 640, 480, "ShadowMaps.seof");
        return true;
    }

    return false;
}
//----------------------------------------------------------------------------
void ShadowMaps::CreateScene()
{
    // screen camera把[0, 1]^3区间上的(x, y, z)映射到[-1, 1]^2 x [0, 1]区间上
    // 的(x', y, 'z').
    m_spScreenCamera = SE_NEW SECamera;
    m_spScreenCamera->SetPerspective(false);
    m_spScreenCamera->SetFrustum(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
    m_spScreenCamera->SetFrame(SEVector3f::ZERO, SEVector3f::UNIT_X, 
        SEVector3f::UNIT_Y, SEVector3f::UNIT_Z);

    // The scene graph is
    //
    // scene -+---> shadowmapnode -+---> ground
    //        |                    |
    //        |                    +---> torus
    //        |                    |
    //        |                    +---> globe
    //        |
    //        +---> flashlight -+---> lightsphere (visualize light source)
    //                          |
    //                          +---> spotlight
    //                          |
    //                          +---> projector camera
    //

    m_spScene = SE_NEW SENode;
    m_spWireframe = SE_NEW SEWireframeState;
    m_spScene->AttachGlobalState(m_spWireframe);
    m_spShadowMapNode = SE_NEW SENode;
    m_spScene->AttachChild(m_spShadowMapNode);

    CreateFlashlight();
    CreateGround();
    CreateTorus();
    CreateGlobe();

    // 创建一个使用RGBA渲染目标纹理的屏幕矩形..
    SEAttributes tempAttrScenePoly;
    tempAttrScenePoly.SetPositionChannels(3);
    tempAttrScenePoly.SetTCoordChannels(0, 2);

    float fExtend = 1.0f;
    SEVertexBuffer* pVBufferScenePoly = SE_NEW SEVertexBuffer(tempAttrScenePoly, 4);
    pVBufferScenePoly->Position3(0) = SEVector3f(0.0f, 0.0f, 0.0f);
    pVBufferScenePoly->Position3(1) = SEVector3f(fExtend, 0.0f, 0.0f);
    pVBufferScenePoly->Position3(2) = SEVector3f(fExtend, fExtend, 0.0f);
    pVBufferScenePoly->Position3(3) = SEVector3f(0.0f, fExtend, 0.0f);
    pVBufferScenePoly->TCoord2(0,0) = SEVector2f(0.0f, 1.0f);
    pVBufferScenePoly->TCoord2(0,1) = SEVector2f(1.0f, 1.0f);
    pVBufferScenePoly->TCoord2(0,2) = SEVector2f(1.0f, 0.0f);
    pVBufferScenePoly->TCoord2(0,3) = SEVector2f(0.0f, 0.0f);
    SEIndexBuffer* pIBufferScenePoly = SE_NEW SEIndexBuffer(6);
    int* pIBufferDataScenePoly = pIBufferScenePoly->GetData();
    pIBufferDataScenePoly[0] = 0;  
    pIBufferDataScenePoly[1] = 3;  
    pIBufferDataScenePoly[2] = 1;
    pIBufferDataScenePoly[3] = 1;  
    pIBufferDataScenePoly[4] = 3;  
    pIBufferDataScenePoly[5] = 2;
    m_spScenePolygon1 = SE_NEW SETriMesh(pVBufferScenePoly, pIBufferScenePoly);

    int iWidth = m_iWidth, iHeight = m_iHeight;
    unsigned char* aucData = SE_NEW unsigned char[4*iWidth*iHeight];
    unsigned char* pucData = aucData;
    int i;
    for( i = 0; i < iWidth*iHeight; i++ )
    {
        *pucData++ = 0xFF;
        *pucData++ = 0x00;
        *pucData++ = 0x00;
        *pucData++ = 0xFF;
    }
    m_spSceneImage = SE_NEW SEImage(SEImage::IT_RGBA8888, iWidth, iHeight, 
        aucData, "SceneImage");

    SETextureEffect* pEffectScenePoly = SE_NEW SETextureEffect("SceneImage");
    m_pSceneTarget1 = pEffectScenePoly->GetPTexture(0, 0);
    m_pSceneTarget1->SetOffscreenTexture(true);
    m_spScenePolygon1->AttachEffect(pEffectScenePoly);
    m_spScenePolygon1->UpdateGS();
    m_spScenePolygon1->UpdateRS();
    m_pRenderer->LoadResources(m_spScenePolygon1);

    // 创建绑定到纹理的RGBA frame buffer.
    SETexture** apTargets = SE_NEW SETexture*[1];
    apTargets[0] = m_pSceneTarget1;
    m_pFrameBuffer = SEFrameBuffer::Create(m_eFormat, m_eDepth, m_eStencil,
        m_eBuffering, m_eMultisampling, m_pRenderer, 1, apTargets);
    SE_ASSERT( m_pFrameBuffer );
}
//----------------------------------------------------------------------------
void ShadowMaps::CreateGround()
{
    SEAttributes tempAttr;
    tempAttr.SetPositionChannels(3);
    tempAttr.SetTCoordChannels(0, 2);
    m_spGround = SEStandardMesh(tempAttr).Rectangle(2, 2, 8.0f, 8.0f);
    m_spShadowMapNode->AttachChild(m_spGround);

    m_spGround->GenerateNormals();
    m_spGround->GenerateTangents(0, 1, 2);
    SEParallaxMapL1Effect* pEffect = 
        SE_NEW SEParallaxMapL1Effect("rock", "rocknormal", "rockheight");
    pEffect->LightDirection = m_spLight->DVector;
    pEffect->LightDirection.Normalize();
    m_spPMEffect = pEffect;
    m_spGround->AttachEffect(pEffect);

    SEMaterialState* pMState = SE_NEW SEMaterialState;
    pMState->Emissive = SEColorRGB::SE_RGB_BLACK;
    pMState->Ambient = SEColorRGB(0.2295f, 0.08825f, 0.0275f);
    pMState->Diffuse = SEColorRGB(0.5508f, 0.2118f, 0.066f);
    pMState->Specular = SEColorRGB(0.580594f, 0.223257f, 0.0695701f);
    pMState->Alpha = 1.0f;
    pMState->Shininess = 51.2f;
    m_spGround->AttachGlobalState(pMState);
    m_spGround->AttachLight(m_spLight);
}
//----------------------------------------------------------------------------
void ShadowMaps::CreateTorus()
{
    SEAttributes tempAttr;
    tempAttr.SetPositionChannels(3);
    tempAttr.SetNormalChannels(3);
    m_spTorus = SEStandardMesh(tempAttr).Torus(64, 64, 1.0f, 0.2f);
    m_spShadowMapNode->AttachChild(m_spTorus);
    m_spTorus->Local.SetTranslate(SEVector3f(0.0f, 0.0f, -1.0f));

    SEMaterialState* pMState = SE_NEW SEMaterialState;
    pMState->Emissive = SEColorRGB::SE_RGB_BLACK;
    pMState->Ambient = SEColorRGB(0.2295f, 0.08825f, 0.0275f);
    pMState->Diffuse = SEColorRGB(0.5508f, 0.2118f, 0.066f);
    pMState->Specular = SEColorRGB(0.580594f, 0.223257f, 0.0695701f);
    pMState->Alpha = 1.0f;
    pMState->Shininess = 51.2f;
    m_spTorus->AttachGlobalState(pMState);
    m_spTorus->AttachLight(m_spLight);
}
//----------------------------------------------------------------------------
void ShadowMaps::CreateGlobe()
{
    SEAttributes tempAttr;
    tempAttr.SetPositionChannels(3);
    tempAttr.SetNormalChannels(3);
    m_spGlobe = SEStandardMesh(tempAttr).Sphere(64, 64, 0.5f);
    m_spShadowMapNode->AttachChild(m_spGlobe);
    m_spGlobe->Local.SetTranslate(SEVector3f(0.0f, 0.0f, -2.0f));
    m_spGlobe->Local.SetUniformScale(0.5f);

    SEMaterialState* pMState = SE_NEW SEMaterialState;
    pMState->Emissive = SEColorRGB::SE_RGB_BLACK;
    pMState->Ambient = SEColorRGB(0.24725f, 0.4245f, 0.0645f);
    pMState->Diffuse = SEColorRGB(0.34615f, 0.4143f, 0.0903f);
    pMState->Specular = SEColorRGB(0.797357f, 0.523991f, 0.208006f);
    pMState->Alpha = 1.0f;
    pMState->Shininess = 83.2f;
    m_spGlobe->AttachGlobalState(pMState);
    m_spGlobe->AttachLight(m_spLight);
}
//----------------------------------------------------------------------------
void ShadowMaps::CreateFlashlight()
{
    m_spFlashlight = SE_NEW SENode;
    m_spFlashlight->Local.SetTranslate(SEVector3f(0.0f, 0.0f, -4.0f));
    m_spScene->AttachChild(m_spFlashlight);

    // Create a sphere to represent the light source.  The sphere has a
    // gradient of yellow color, bright at the bottom (facing the objects to
    // be lit) and dark at the top.
    SEAttributes tempAttr;
    tempAttr.SetPositionChannels(3);
    tempAttr.SetColorChannels(0, 3);
    float fRadius = 0.1f;
    m_spLightSphere = SEStandardMesh(tempAttr).Sphere(32, 32, fRadius);
    m_spFlashlight->AttachChild(m_spLightSphere);
    SEVertexBuffer* pVBuffer = m_spLightSphere->VBuffer;
    int iVCount = pVBuffer->GetVertexCount();
    SEColorRGB tempYellow(1.0f, 1.0f, 0.0f);
    for( int i = 0; i < iVCount; i++ )
    {
        float fAmpl = 0.5f*(fRadius + pVBuffer->Position3(i).Z)/fRadius;
        pVBuffer->Color3(0, i) = fAmpl*tempYellow;
    }
    m_spLightSphere->AttachEffect(SE_NEW SEVertexColor3Effect);

    // Create the spotlight.
    m_spLight = SE_NEW SELight(SELight::LT_SPOT);
    m_spLight->Ambient = SEColorRGB::SE_RGB_WHITE;
    m_spLight->Diffuse = SEColorRGB::SE_RGB_WHITE;
    m_spLight->Specular = SEColorRGB::SE_RGB_WHITE;
    m_spLight->Exponent = 1.0f;
    m_spLight->SetAngle(SEMathf::HALF_PI);
    m_spFlashlight->AttachChild(SE_NEW SELightNode(m_spLight));

    // Create a camera to project the texture and shadows.  The default
    // coordinate frame is the same as the light's, so no need to set these.
    // The frustum values were chosen by empirical means.  That is, I modified
    // them until the projection looked right.
    SECamera* pProjector = SE_NEW SECamera;
    pProjector->SetFrustum(120.0f, 1.0f, 1.0f, 5.0f);
    m_spFlashlight->AttachChild(SE_NEW SECameraNode(pProjector));

    // The depth bias parameter was chosen by empirical means; that is, I
    // modified it until the scene looked right.
    float fDepthBias = 0.02f;

    m_spSMEffect = SE_NEW SEShadowMapEffect(pProjector, "kate", 
        SEImage::IT_RGBA8888, 512, 512, fDepthBias);

    m_spShadowMapNode->AttachEffect(m_spSMEffect);
}
//----------------------------------------------------------------------------
