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

#include "SEMax8SceneBuilder.h"
#include "decomp.h"

//----------------------------------------------------------------------------
void Max8SceneBuilder::BuildAmbientLight()
{
    // ��ȡMaxȫ�ֻ�����������Ϣ,��������Ӧ��Swing Engine�ƹ�.
    Color tempColor = m_pMax->GetAmbient(0, FOREVER);
    Swing::Light* pSELight = new Swing::Light(Swing::Light::LT_AMBIENT);
    pSELight->Ambient = Swing::ColorRGB(tempColor.r, tempColor.g, tempColor.b);
    m_spSEScene->AttachLight(pSELight);
}
//----------------------------------------------------------------------------
void Max8SceneBuilder::BuildLight(INode* pMaxNode, Swing::Spatial* pSENode)
{
    // ����һ��Max�㼶�е�light����Swing Engine��light.
    // ֧��Max��omnidirectional,directional,free spot������light.
    // ע��Max�в��ð��ܵ�lightӰ���node����light,
    // ��Swing Engine�в��ð�Ӱ��node��light����node,node����һ��list of lights.
    // ����ʹ�õķ�����,��Swing Engine lights����Swing Engine node,
    // ��node����Max������light node�ĸ��ڵ�.
    //
    // pMaxNode:
    //     ����light��Max node.
    // pSENode:
    //     Ҫ������Max light state��Swing Engine node.

    // ��Max��ȡlight����.
    ObjectState tempObjectState = pMaxNode->EvalWorldState(m_iTimeStart);
    GenLight* pGenLight = (GenLight*)tempObjectState.obj;
    LightState tempMaxLightState;
    Interval tempValid = FOREVER;
    pGenLight->EvalLightState(m_iTimeStart, tempValid, &tempMaxLightState);

    Swing::Light* pSELight = NULL;

    switch( pGenLight->Type() ) 
    {
    case OMNI_LIGHT:
        pSELight = BuildPointLight(pMaxNode);

        break;
    case TSPOT_LIGHT:
    case FSPOT_LIGHT:
        pSELight = BuildSpotLight(pMaxNode, tempMaxLightState);

        break;
    case TDIR_LIGHT:
    case DIR_LIGHT:
        pSELight = BuildDirectionalLight(pMaxNode);

        break;
    default: // ��֧�ֵ�light����
        assert( false );

        return;
    }
    
    pSELight->Intensity = tempMaxLightState.intens;
    
    // Max lights����ֻ����һ����ɫ.
    // ��Swing Engine lights��specular,diffuse���������ɫ.
    if( tempMaxLightState.affectDiffuse )
    {
        pSELight->Diffuse = Swing::ColorRGB(tempMaxLightState.color.r,
            tempMaxLightState.color.g, tempMaxLightState.color.b);
    }
    if( tempMaxLightState.affectSpecular )
    {
        pSELight->Specular = Swing::ColorRGB(tempMaxLightState.color.r,
            tempMaxLightState.color.g, tempMaxLightState.color.b);
    }
    
    // ����attenuation����.
    // Max�Ĺ���˥����ʽ��Swing Engine�ļ�,
    // ����ΪMax�Ĺ���˥����ʽֻ����constant,linear,quadratic֮һ��������,
    // �����������߹�ͬ��������.
    if( pGenLight->GetDecayType() != 0 )
    {
        Interval tempValid;
        float fDecayRadius = pGenLight->GetDecayRadius(m_iTimeStart, tempValid);
        
        switch( pGenLight->GetDecayType() )
        {
        case 0:
            pSELight->Constant = ATTENUATION;
            pSELight->Linear = 0.0f;
            pSELight->Quadratic = 0.0f;

            break;
        case 1:
            pSELight->Constant = 0.0f;
            pSELight->Linear = ATTENUATION / fDecayRadius;
            pSELight->Quadratic = 0.0f;

            break;
        case 2:
            pSELight->Constant = 0.0f;
            pSELight->Linear = 0.0f;
            pSELight->Quadratic = ATTENUATION / (fDecayRadius*fDecayRadius);

            break;
        } 
    }
    else if( pGenLight->GetUseAtten() )
    {
        float fFarAtten = tempMaxLightState.attenEnd;             
        pSELight->Constant = 0.0f;
        pSELight->Linear = ATTENUATION / fFarAtten;
        pSELight->Quadratic = 0.0f;
    }

    pSENode->AttachLight(pSELight);
}
//----------------------------------------------------------------------------
Swing::Vector3f Max8SceneBuilder::GetLightLocation(INode* pNode)
{
    // ����node��local�任.
    Matrix3 tempLocal = pNode->GetNodeTM(m_iTimeStart) *
        Inverse(pNode->GetParentTM(m_iTimeStart));

    // ��local�任�ֽ������任����.
    AffineParts tempAffParts;
    decomp_affine(tempLocal, &tempAffParts);

    // ��ȡƽ�Ʊ任����.
    return Swing::Vector3f(tempAffParts.t.x, tempAffParts.t.y, tempAffParts.t.z);
}
//----------------------------------------------------------------------------
Swing::Light* Max8SceneBuilder::BuildPointLight(INode* pNode)
{
    Swing::Light* pSELight = new Swing::Light(Swing::Light::LT_POINT);

    // ����light��λ��.
    pSELight->Position = GetLightLocation(pNode);
    
    return pSELight;
}
//----------------------------------------------------------------------------
Swing::Light* Max8SceneBuilder::BuildSpotLight(INode* pNode, LightState& rLightState)
{
    Swing::Light* pSELight = new Swing::Light(Swing::Light::LT_SPOT);

    // ����light��λ�úͷ�������.
    pSELight->Position = GetLightLocation(pNode);
    pSELight->SetDirection(Swing::Vector3f::UNIT_Z);

    // ����light��angle.
    pSELight->Angle = HALFPI * rLightState.fallsize / 180.0f;

    // ����light��exponent.
    pSELight->Exponent = 128.0f * ((rLightState.fallsize -
        rLightState.hotsize) / rLightState.fallsize);
    
    return pSELight;
}
//----------------------------------------------------------------------------
Swing::Light* Max8SceneBuilder::BuildDirectionalLight(INode* pNode)
{
    Swing::Light* pSELight = new Swing::Light(Swing::Light::LT_DIRECTIONAL);

    // ����light�ķ�������.
    pSELight->SetDirection(Swing::Vector3f::UNIT_Z);

    return pSELight;
}
//----------------------------------------------------------------------------



