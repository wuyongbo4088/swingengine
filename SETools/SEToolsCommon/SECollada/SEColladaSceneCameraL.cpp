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

#include "SEToolsCommonPCH.h"
#include "SEColladaScene.h"

using namespace Swing;

//----------------------------------------------------------------------------
Camera* ColladaScene::GetCamera(const char* acName)
{
    if( !acName )
    {
        return 0;
    }

    for( int i = 0; i < (int)m_Cameras.size(); i++ )
    {
        if( strcmp(m_Cameras[i]->GetName(), acName) == 0 )
        {
            return m_Cameras[i];
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
Camera* ColladaScene::LoadCamera(domCameraRef spDomCamera)
{
    xsID strCameraID = spDomCamera->getId();
    if( !strCameraID )
    {
        return 0;
    }

    Camera* pCamera = GetCamera((const char*)strCameraID);
    if( pCamera )
    {
        // This camera is already in our camera catalog.
        return pCamera;
    }

    ToolSystem::SE_DebugOutput("Add new camera %s", (const char*)strCameraID);

    domCamera* pDomCamera = (domCamera*)(domElement*)spDomCamera;
    if( pDomCamera )
    {	
        // Create a Swing Engine camera.
        pCamera = SE_NEW Camera;
        pCamera->SetName((const char*)strCameraID);

        // Get the optics.
        domCamera::domOptics* pDomCameraOptics = pDomCamera->getOptics();
        SE_ASSERT( pDomCameraOptics );
        
        // Get the optics common profile technique.
        domCamera::domOptics::domTechnique_common* pDomCameraCommonTechnique = 
            pDomCameraOptics->getTechnique_common();
        SE_ASSERT( pDomCameraCommonTechnique );

        // Get the perspective and orthographic common profiles(if they exist).
        DomCameraPerspective* pDomPerspective = 
            pDomCameraCommonTechnique->getPerspective();
        DomCameraOrthographic* pDomOrthographic = 
            pDomCameraCommonTechnique->getOrthographic();
        
        // Setup camera parameters, note we have to check if a parameter is 
        // there before doing getValue. Parameters not in the COLLADA data 
        // will be left at the Swing Engine camera defaults.
        float fFovx = Math<float>::ATan(0.5f) * Mathf::RAD_TO_DEG * 2.0f;
        float fFovy = fFovx;
        float fRMin = -0.5f;
        float fRMax = 0.5f;
        float fUMin = -0.5f;
        float fUMax = 0.5f;
        float fAspectRatio = 1.0f;
        float fZNear = 1.0f;
        float fZFar = 2.0f;
        if( pDomPerspective )
        {
            if( pDomPerspective->getAspect_ratio() )
            {
                fAspectRatio = 
                    (float)(pDomPerspective->getAspect_ratio()->getValue());
            }
            if( pDomPerspective->getXfov() )
            {
                fFovx = (float)(pDomPerspective->getXfov()->getValue());

                // Here we should adjust fovy base on fovx.
                float fTanHalfFovx = Math<float>::Tan(0.5f*fFovx);
                float fTanHalfFovy = fTanHalfFovx / fAspectRatio;
                fFovy = 2.0f * Math<float>::ATan(fTanHalfFovy);
            }
            if( pDomPerspective->getYfov() )
            {
                fFovy = (float)(pDomPerspective->getYfov()->getValue());
            }
            if( pDomPerspective->getZnear() )
            {
                fZNear = (float)(pDomPerspective->getZnear()->getValue());
            }
            if( pDomPerspective->getZfar() )
            {
                fZFar = (float)(pDomPerspective->getZfar()->getValue());
            }

            // Force ZNear to be >= 1.0 to avoid rendering problems.
            if( fZNear < 1.0f )
            {
                fZNear = 1.0f;
            }
            if( fZNear >= fZFar )
            {
                fZFar = fZNear + 1.0f;
            }

            pCamera->SetFrustum(fFovy, fAspectRatio, fZNear, fZFar);
        }
        else if( pDomOrthographic )
        {
            if( pDomOrthographic->getAspect_ratio() )
            {
                fAspectRatio = 
                    (float)(pDomOrthographic->getAspect_ratio()->getValue());
            }
            if( pDomOrthographic->getXmag() )
            {
                fRMax = (float)(pDomOrthographic->getXmag()->getValue());
                fRMin = -fRMax;
                fUMax = fRMax / fAspectRatio;
                fUMin = -fUMax;
            }
            if( pDomOrthographic->getYmag() )
            {
                fUMax = (float)(pDomOrthographic->getYmag()->getValue());
                fUMin = -fUMax;
                fRMax = fUMax * fAspectRatio;
                fRMin = -fRMax;
            }
            if( pDomOrthographic->getZnear() )
            {
                fZNear = (float)(pDomOrthographic->getZnear()->getValue());
            }
            if( pDomOrthographic->getZfar() )
            {
                fZFar = (float)(pDomOrthographic->getZfar()->getValue());
            }

            // Force ZNear to be >= 1.0 to avoid rendering problems.
            if( fZNear < 1.0f )
            {
                fZNear = 1.0f;
            }
            if( fZNear >= fZFar )
            {
                fZFar = fZNear + 1.0f;
            }

            pCamera->SetFrustum(fRMin, fRMax, fUMin, fUMax, fZNear, fZFar);
        }
        else
        {
            // Unknown camera type.
            SE_ASSERT( false );
        }

        m_Cameras.push_back(pCamera);
        return pCamera;
    }

    return 0;
}
//----------------------------------------------------------------------------
Camera* ColladaScene::LoadInstanceCamera(
    domInstance_cameraRef spDomInstanceCamera)
{
    xsAnyURI& rUrlType  = spDomInstanceCamera->getUrl();
    domElement* pDomElement = (domElement*) rUrlType.getElement();
    if( !pDomElement )
    {
        // This instance light is not found, skip to the next one.
        ToolSystem::SE_DebugOutput("Can't find this camera:%s", 
            rUrlType.getURI());

        return 0;
    }

    Camera* pCamera = LoadCamera((domCamera*)pDomElement);

    return pCamera;
}
//----------------------------------------------------------------------------