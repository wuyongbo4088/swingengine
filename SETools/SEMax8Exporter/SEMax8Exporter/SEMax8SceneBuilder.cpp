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

const float Max8SceneBuilder::ATTENUATION = 1e+03f;
const float Max8SceneBuilder::MIN_DIFFERENCE = 1e-03f;

//----------------------------------------------------------------------------
Max8SceneBuilder::Max8SceneBuilder(const TCHAR* acFilename, BOOL bExportSelected, 
    Max8ExportSettings* pSettings, ExpInterface* pExport, Interface* pMax, 
    INode* pRootNode)
{
    // acFilename:
    //     ���泡����ͼ�ļ�ʱ����ļ���.
    // bExportSelected:
    //     Ϊtrueʱ�����û�ѡ����һ��������ͼ�ӽڵ����ڵ���,
    //     Ϊfalseʱ������������������ͼ.
    // pSettings:
    //     �û�ѡ��ĵ���������ѡ������ָ�����赼��������.
    // pExport:
    //     Max�������Ľӿ�ָ��.
    // pMax:
    //     Max�Ľӿ�ָ��.
    // pRootNode:
    //     ����������ͼ�ĸ��ڵ�(���û�ѡ��ĳ����������ڵ�).

    m_acFilename = new TCHAR[_tcslen(acFilename) + 1];
    _tcscpy(m_acFilename, acFilename);
    m_bExportSelected = bExportSelected;
    m_pSettings = pSettings;
    m_pExport = pExport;
    m_pMax = pMax;
    m_pRootNode = pRootNode;  
    m_iTicksPerFrame = GetTicksPerFrame();

    if( m_pSettings->IncludeCurrentFrame )
    {
        // �û�ѡ��ֻ������ǰ��.
        // ��ȡ��ǰ�巢��ʱ��.
        m_iTimeStart = 1600;//m_pMax->GetAnimRange().Start();
        m_iTimeEnd = m_iTimeStart;
    }
    else
    {
        // ��ȡ��ʼ����������tickʱ��.
        m_iTimeStart = m_pMax->GetAnimRange().Start();
        m_iTimeEnd = m_pMax->GetAnimRange().End();
        if( m_pSettings->UseLocalTime )
        {
            m_iTimeOffset = m_iTimeStart;
        }
        else
        {
            m_iTimeOffset = 0;
        }
    }

    // ����Max������ͼ�㼶������.
    // ��һ��,�������в��ʼ�������.
    // �ڶ���,����node�㼶��ϵ.
    // ������,��������modifier.

    // ����Swing Engine�ĳ�����ͼ������ڵ�.
    // ����ʹ��һ��depth buffer state.
    // ���Ӧ�ó�����ʹ��,��Ӧ�ó����ֶ��Ƴ���depth buffer state.
    m_spSEScene = new Swing::Node;
	m_spSEScene->AttachGlobalState(new Swing::ZBufferState);

    // ����Max����,�������в���.
    ConvertMaterials(m_pRootNode);

    if( !m_bExportSelected )
    {
        // ������������.

        // ����ȫ�ֻ�����.
        if( m_pSettings->IncludeLights )
        {
            BuildAmbientLight();
        }

        // ��������Swing Engine����.
        for( int i = 0; i < m_pRootNode->NumberOfChildren(); i++ )
        {
            Traverse(m_pRootNode->GetChildNode(i), m_spSEScene);
        }
    }
    else
    {
        // ����ѡ�в���Swing Engine�ӳ���.
        Traverse(m_pRootNode, m_spSEScene);
    }

    if( m_pSettings->IncludeModifiers )
    {
        m_spSEScene->UpdateGS((double)(TicksToSec(0.0f)));
        ApplyModifiers();
    }

    // ����Swing Engine�����������ļ�.
    Swing::Stream tempStream;

    if( m_bExportSelected )
    {
        tempStream.Insert(m_spSEScene->GetChild(0));
    }
    else
    {
        tempStream.Insert(m_spSEScene);
    }

    tempStream.Save(m_acFilename); 
}
//----------------------------------------------------------------------------
Max8SceneBuilder::~Max8SceneBuilder()
{
    delete[] m_acFilename;
    m_spSEScene = NULL;
    vector<ModifierInfo*>::iterator pIter;
    for( pIter = m_ModList.begin(); pIter < m_ModList.end(); pIter++ )
    {
        delete *pIter;
    }
}
//----------------------------------------------------------------------------
bool Max8SceneBuilder::Traverse(INode* pMaxNode, Swing::Spatial* pSENode)
{
    // �ݹ����Max������,������ض�����Swing Engine������ͼ.
    // pMaxNode:
    //    Max�������еĵ�ǰ�ڵ�.
    // pSENode:
    //     ����������Swing Engine������ͼ�Ľڵ�ĸ��ڵ�.
    // ����trueʱ�����ݹ齫������ȥ,������ֹ�ݹ�.

    Swing::Spatial* pSEChild = NULL;

    // ��ȡ��ʼʱ��ʱ��Max�ڵ������״̬.
    ObjectState tempObjectState = pMaxNode->EvalWorldState(m_iTimeStart);
    bool bSupported = true;

    // �ж�����Ҫ�������ʲô���͵Ķ���,��������Ӧ����.
    // ���һ���ڵ������֮��صĶ���(����һ��mesh����),
    // �����û�ѡ�񲻵����������͵Ķ���,
    // ������ͨ������һ���߱���ö���ͬ���任���������������ռ�㼶��ϵ.
    // ������ǿ��Ը�����Ҫ,ֻ�������������Ϣ����������������Ϣ.
    if( tempObjectState.obj )
    {
        switch( tempObjectState.obj->SuperClassID() )
        {
        case GEOMOBJECT_CLASS_ID:
            pSEChild = BuildGeometry(pMaxNode, pSENode);

            break;
        case CAMERA_CLASS_ID:
            // ��ʵ��.

            break;
        case LIGHT_CLASS_ID:
            if( m_pSettings->IncludeLights && !pMaxNode->IsHidden() )
            {
                BuildLight(pMaxNode, pSENode);
            }

            break;
        case HELPER_CLASS_ID:
            // "helper"�ڵ�û����֮��صļ���������(����dummy��bone�ڵ�).
            // ��˴���һ���߱���ͬ�任����ڵ㲢����Swing Engine������ͼ.
            pSEChild = BuildSpatial(pMaxNode, pSENode);

            break;
        default:
            assert( false );
            bSupported = false;

            return false;
        }
    }

    // Ϊ�ڵ���Ӷ�����Ϣ.
    if( pSEChild )
    {   
        if( m_pSettings->IncludeKeyFrames )
        {
            BuildKeyFrameController(pMaxNode, pSEChild);
        }
        else if( m_pSettings->IncludeAllFrames )
        {
            BuildFrameController(pMaxNode, pSEChild);
        }
    }

    // �ռ�Max�����е��޸�����Ϣ.
    // �ڳ���������ɺ�,����Ӧ���ڳ����ڵ���޸�����������Swing Engine����.
    if( m_pSettings->IncludeModifiers )
    {
        ModifierInfo* pModInfo = new ModifierInfo;
        FindModifiers(pMaxNode, pModInfo->Modifiers);
        if( pModInfo->Modifiers.empty() )
        {
            delete pModInfo;
        }
        else
        {
            pModInfo->Node = pMaxNode;
            m_ModList.push_back(pModInfo);
        }
    }

    int iNumChildren = pMaxNode->NumberOfChildren();
    if( iNumChildren == 0 )
    {
        return true;
    }

    for( int i = 0; i < iNumChildren; i++ )
    {
        if( !Traverse(pMaxNode->GetChildNode(i), pSEChild) 
        ||   m_pMax->GetCancel() )
        {
            return false;
        }
    }

    return true;
}
//----------------------------------------------------------------------------
Swing::Spatial* Max8SceneBuilder::BuildGeometry(INode* pMaxNode,
    Swing::Spatial* pSENode)
{
    // ����������һ����������󵽳�����ͼ.
    // pMaxNode:
    //     Max�������еĴ��м��������ݵĽڵ�ָ��.
    // pSENode:
    //     ��ǰ�������ڵ�ĸ��ڵ�ָ��.

    Swing::Spatial* pSEChild = NULL;
    Swing::Spatial* pSELink = NULL;

    // ��Max������,������ڵ�������м������ӽڵ�,
    // ����Swing Engine������,���м�����ڵ������Ҷ�ӽڵ�.
    // �����ǰMax�ڵ��м������ӽڵ�,��Ϊ��ǰMax�ڵ㴴��һ�������Ӧ��"link"��ڵ�,
    // �Ӷ�����ǰMax�ڵ�ļ������ӽڵ������ΪҶ�ӽڵ�������뵽Swing Engine����.
    for( int i = 0; i < pMaxNode->NumberOfChildren(); i++ )
    {
        ObjectState tempObjectState = pMaxNode->EvalWorldState(m_iTimeStart); 
        if( tempObjectState.obj->SuperClassID() == GEOMOBJECT_CLASS_ID )
        {
            pSELink = BuildSpatial(pMaxNode, pSENode);

            break;
        }
    }

    if( pSELink == NULL )
    {
        pSELink = pSENode;
    }

    if( m_pSettings->IncludeMeshes && !pMaxNode->IsHidden() )
    {
        pSEChild = BuildMesh(pMaxNode, pSELink);
    }
    else
    {
        pSEChild = BuildSpatial(pMaxNode, pSELink);
    }

    if( pSELink != pSENode )
    {
        return pSELink;
    }

    return pSEChild;
}
//----------------------------------------------------------------------------
Swing::Spatial* Max8SceneBuilder::BuildSpatial(INode* pMaxNode,
    Swing::Spatial* pSENode)
{
    Swing::Node* pSEChild = new Swing::Node;
    pSEChild->SetName(pMaxNode->GetName());

    pSEChild->Local = GetLocalTransform(pMaxNode, m_iTimeStart);

    assert( pSENode->IsDerived(Swing::Node::TYPE) );
    ((Swing::Node*)pSENode)->AttachChild(pSEChild);

    return pSEChild;
}
//----------------------------------------------------------------------------
Swing::Transformation Max8SceneBuilder::GetLocalTransform(INode* pNode,
    TimeValue iTime)
{
    // ����ڵ��local�任.
    // Max�Ľڵ�任���ʺ������ṩ��Խڵ������ռ�任,
    // ������Ǳ�����һЩ����Ӷ���ýڵ��local�任.
    // ͬ��,MaxҲ�ṩ�ڵ��pivot�任,ͨ������GetNodeTM,
    // �����任,ͨ������GetObjectTM,GetObjTMBeforeWSM,GetObjTMAfterWSM.
    // "object offset"ָ���ǽڵ���������ı任,������ڽڵ��pivot��˵��,
    // ��˽ڵ��pivot�任�������Զ�λ�������local����ϵ�µ���̬.
    // ע�����任�ķ��ʺ������ص��ǽڵ��pivot�任�Ͷ���offset�任�����ս��.

    // ����ڵ�Ķ�����local����ϵ�µı任.
    Matrix3 tempMaxLocal = pNode->GetObjTMAfterWSM(iTime) * 
        Inverse(pNode->GetParentNode()->GetObjTMAfterWSM(iTime));

    // �ֽ�local�任,����������任����.
    AffineParts tempAffParts;
    decomp_affine(tempMaxLocal, &tempAffParts);

    // �ж�ƽ�Ʒ����Ƿ���������.
    bool bTranslationIsZero =
        fabsf(tempAffParts.t.x) < MIN_DIFFERENCE &&
        fabsf(tempAffParts.t.y) < MIN_DIFFERENCE &&
        fabsf(tempAffParts.t.z) < MIN_DIFFERENCE;

    // �ж���ת�����Ƿ�Ϊ��λ����.
    float fQSign = ( tempAffParts.q.w >= 0.0f ? 1.0f : -1.0f );
    bool bRotationIsIdentity =
        fabsf(fQSign*tempAffParts.q.w - 1.0f) < MIN_DIFFERENCE &&
        fabsf(tempAffParts.q.x) < MIN_DIFFERENCE &&
        fabsf(tempAffParts.q.y) < MIN_DIFFERENCE &&
        fabsf(tempAffParts.q.z) < MIN_DIFFERENCE;

    // �жϱ任�Ƿ��������任.
    bool bHasReflection = ( tempAffParts.f < 0.0f );

    // �ж����ŷ����Ƿ�Ϊͳһ����.
    bool bScaleIsUniform =
        fabsf(tempAffParts.k.x - tempAffParts.k.y) < MIN_DIFFERENCE &&
        fabsf(tempAffParts.k.y - tempAffParts.k.z) < MIN_DIFFERENCE;

    // ������ŷ�����ͳһ����,�ж�����ϵ���Ƿ�Ϊ1.
    bool bScaleIsUnity = bScaleIsUniform &&
        fabsf(tempAffParts.k.x - 1.0f) < MIN_DIFFERENCE;

    // �ж�scale-orientation�Ƿ�Ϊ��λ����.
    float fUSign = ( tempAffParts.u.w >= 0.0f ? 1.0f : -1.0f );
    bool bOrientIsIdentity = bScaleIsUniform || (
        fabsf(fUSign*tempAffParts.u.w - 1.0f) < MIN_DIFFERENCE &&
        fabsf(tempAffParts.u.x) < MIN_DIFFERENCE &&
        fabsf(tempAffParts.u.y) < MIN_DIFFERENCE &&
        fabsf(tempAffParts.u.z) < MIN_DIFFERENCE);

    // ����������Ͻ����Ӧ��Swing Engine�任.
    // ���ڽ����������Ͻ��Ĳ���,
    // �Ӷ�������ȷ������Swing Engine��Transformation�����ر�־.
    Swing::Transformation tempSELocal;

    if( !bTranslationIsZero )
    {
        tempSELocal.SetTranslate(Swing::Vector3f(tempAffParts.t.x, tempAffParts.t.z,
            tempAffParts.t.y));
    }

    if( bHasReflection )
    {
        tempAffParts.k *= -1.0f;
    }

    if( bScaleIsUniform )
    {
        // (s*I)*R�ľ�����ʽ,s��ͳһ����ϵ��.
        if( !bRotationIsIdentity )
        {
            Swing::Matrix3f tempSERot;
            Swing::Quaternionf(tempAffParts.q.w, +tempAffParts.q.x, +tempAffParts.q.z,
                +tempAffParts.q.y).ToRotationMatrix(tempSERot);
            tempSELocal.SetRotate(tempSERot);
        }
        if( !bScaleIsUnity )
        {
            tempSELocal.SetUniformScale(tempAffParts.k.x);
        }
    }
    else if( bOrientIsIdentity )
    {
        // S*R�ľ�����ʽ, S�Ǿ߱���ͳһ����ϵ���ĶԽǾ���.
        if( !bRotationIsIdentity )
        {
            Swing::Matrix3f tempSERot;
            Swing::Quaternionf(tempAffParts.q.w, +tempAffParts.q.x, +tempAffParts.q.z,
                +tempAffParts.q.y).ToRotationMatrix(tempSERot);
            tempSELocal.SetRotate(tempSERot);
        }

        tempSELocal.SetScale(Swing::Vector3f(tempAffParts.k.x, tempAffParts.k.z,
            tempAffParts.k.y));
    }
    else
    {
        // �����.
        // U*S*U^t*R�ľ�����ʽ,����U�Ƕ�λ����,S�Ǿ߱���ͳһ����ϵ���ĶԽǾ���.
        Swing::Matrix3f tempSEMatrix(
            tempMaxLocal.GetAddr()[0][0],
            tempMaxLocal.GetAddr()[0][2],
            tempMaxLocal.GetAddr()[0][1],
            tempMaxLocal.GetAddr()[2][0],
            tempMaxLocal.GetAddr()[2][2],
            tempMaxLocal.GetAddr()[2][1],
            tempMaxLocal.GetAddr()[1][0],
            tempMaxLocal.GetAddr()[1][2],
            tempMaxLocal.GetAddr()[1][1]);

        tempSELocal.SetMatrix(tempSEMatrix);
    }

    return tempSELocal;
}
//----------------------------------------------------------------------------



