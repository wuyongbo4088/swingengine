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
#include "SEManagedCameraNode.h"
#include "SEManagedObjectFactory.h"

using namespace Swing;
using namespace Swing::Tools::ManagedFramework;

//---------------------------------------------------------------------------
ManagedCameraNode::ManagedCameraNode(ManagedCamera^ thCamera)
{
    SE_NULL_ARGUMENT_CHECK(thCamera, "thCamera");
    m_pspCameraNode = SE_NEW CameraNodePtr;
    (*m_pspCameraNode) = SE_NEW CameraNode(thCamera->GetNativeCamera());
}
//---------------------------------------------------------------------------
ManagedCameraNode::ManagedCameraNode(CameraNode* pCameraNode)
{
    m_pspCameraNode = SE_NEW CameraNodePtr;
    (*m_pspCameraNode) = pCameraNode;
}
//---------------------------------------------------------------------------
ManagedCameraNode::~ManagedCameraNode()
{
    SE_DELETE m_pspCameraNode;
    m_pspCameraNode = 0;
}
//---------------------------------------------------------------------------
void ManagedCameraNode::SetCamera(ManagedCamera^ thCamera)
{
    SE_NULL_ARGUMENT_CHECK(thCamera, "thCamera");
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    (*m_pspCameraNode)->SetCamera(thCamera->GetNativeCamera());
}
//---------------------------------------------------------------------------
ManagedCamera^ ManagedCameraNode::GetCamera()
{
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    return gcnew ManagedCamera((*m_pspCameraNode)->GetCamera());
}
//---------------------------------------------------------------------------
int ManagedCameraNode::GetCount()
{
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    return (*m_pspCameraNode)->GetCount();
}
//---------------------------------------------------------------------------
int ManagedCameraNode::AttachChild(INativeSpatial^ thSpatial)
{
    SE_NULL_ARGUMENT_CHECK(thSpatial, "thSpatial");
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    return (*m_pspCameraNode)->AttachChild(thSpatial->GetNativeSpatial());
}
//---------------------------------------------------------------------------
int ManagedCameraNode::DetachChild(INativeSpatial^ thSpatial)
{
    SE_NULL_ARGUMENT_CHECK(thSpatial, "thSpatial");
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    return (*m_pspCameraNode)->DetachChild(thSpatial->GetNativeSpatial());
}
//---------------------------------------------------------------------------
void ManagedCameraNode::SetLocalRotate(ManagedMatrix3f^ thRotate)
{
    SE_NULL_ARGUMENT_CHECK(thRotate, "thRotate");
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    Matrix3f mat3fRot;
    thRotate->ToMatrix3f(mat3fRot);
    (*m_pspCameraNode)->Local.SetRotate(mat3fRot);
}
//---------------------------------------------------------------------------
ManagedMatrix3f^ ManagedCameraNode::GetLocalRotate()
{
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    return gcnew ManagedMatrix3f((*m_pspCameraNode)->Local.GetRotate());
}
//---------------------------------------------------------------------------
void ManagedCameraNode::SetLocalMatrix(ManagedMatrix3f^ thMatrix)
{
    SE_NULL_ARGUMENT_CHECK(thMatrix, "thMatrix");
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    Matrix3f mat3fM;
    thMatrix->ToMatrix3f(mat3fM);
    (*m_pspCameraNode)->Local.SetMatrix(mat3fM);
}
//---------------------------------------------------------------------------
ManagedMatrix3f^ ManagedCameraNode::GetLocalMatrix()
{
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    return gcnew ManagedMatrix3f((*m_pspCameraNode)->Local.GetMatrix());
}
//---------------------------------------------------------------------------
void ManagedCameraNode::SetLocalTranslate(ManagedVector3f^ thTranslate)
{
    SE_NULL_ARGUMENT_CHECK(thTranslate, "thTranslate");
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    Vector3f vec3fTrn;
    thTranslate->ToVector3f(vec3fTrn);
    (*m_pspCameraNode)->Local.SetTranslate(vec3fTrn);
}
//---------------------------------------------------------------------------
ManagedVector3f^ ManagedCameraNode::GetLocalTranslate()
{
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    return gcnew ManagedVector3f((*m_pspCameraNode)->Local.GetTranslate());
}
//---------------------------------------------------------------------------
void ManagedCameraNode::SetLocalScale(ManagedVector3f^ thScale)
{
    SE_NULL_ARGUMENT_CHECK(thScale, "thScale");
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    Vector3f vec3fScl;
    thScale->ToVector3f(vec3fScl);
    (*m_pspCameraNode)->Local.SetScale(vec3fScl);
}
//---------------------------------------------------------------------------
ManagedVector3f^ ManagedCameraNode::GetLocalScale()
{
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    return gcnew ManagedVector3f((*m_pspCameraNode)->Local.GetScale());
}
//---------------------------------------------------------------------------
void ManagedCameraNode::SetLocalUniformScale(float fScale)
{
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    (*m_pspCameraNode)->Local.SetUniformScale(fScale);
}
//---------------------------------------------------------------------------
float ManagedCameraNode::GetLocalUniformScale()
{
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    return (*m_pspCameraNode)->Local.GetUniformScale();
}
//---------------------------------------------------------------------------
void ManagedCameraNode::UpdateGS(double dAppTime)
{
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    (*m_pspCameraNode)->UpdateGS(dAppTime);
}
//---------------------------------------------------------------------------
void ManagedCameraNode::UpdateRS()
{
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    (*m_pspCameraNode)->UpdateRS();
}
//---------------------------------------------------------------------------
void ManagedCameraNode::AttachEffect(INativeEffect^ thEffect)
{
    SE_NULL_ARGUMENT_CHECK(thEffect, "thEffect");
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    (*m_pspCameraNode)->AttachEffect(thEffect->GetNativeEffect());
}
//---------------------------------------------------------------------------
void ManagedCameraNode::DetachEffect(INativeEffect^ thEffect)
{
    SE_NULL_ARGUMENT_CHECK(thEffect, "thEffect");
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    (*m_pspCameraNode)->DetachEffect(thEffect->GetNativeEffect());
}
//---------------------------------------------------------------------------
void ManagedCameraNode::DetachAllEffects()
{
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    (*m_pspCameraNode)->DetachAllEffects();
}
//---------------------------------------------------------------------------
int ManagedCameraNode::GetGlobalStateCount()
{
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    return (*m_pspCameraNode)->GetGlobalStateCount();
}
//---------------------------------------------------------------------------
INativeGlobalState^ ManagedCameraNode::GetGlobalState(int i)
{
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    GlobalState* pState = (*m_pspCameraNode)->GetGlobalState(i);

    return ManagedObjectFactory::CreateGlobalState(pState);
}
//---------------------------------------------------------------------------
INativeGlobalState^ ManagedCameraNode::GetGlobalState(
    INativeGlobalState::StateType eType)
{
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    GlobalState* pState = (*m_pspCameraNode)->GetGlobalState(
        (GlobalState::StateType)eType);

    return ManagedObjectFactory::CreateGlobalState(pState);
}
//---------------------------------------------------------------------------
void ManagedCameraNode::AttachGlobalState(INativeGlobalState^ thState)
{
    SE_NULL_ARGUMENT_CHECK(thState, "thState");
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    (*m_pspCameraNode)->AttachGlobalState(thState->GetNativeGlobalState());
}
//---------------------------------------------------------------------------
void ManagedCameraNode::DetachGlobalState(INativeGlobalState::StateType eType)
{
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    (*m_pspCameraNode)->DetachGlobalState((GlobalState::StateType)eType);
}
//---------------------------------------------------------------------------
void ManagedCameraNode::DetachAllGlobalStates()
{
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    (*m_pspCameraNode)->DetachAllGlobalStates();
}
//---------------------------------------------------------------------------
int ManagedCameraNode::GetNativeReferences()
{
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    return (*m_pspCameraNode)->GetReferences();
}
//---------------------------------------------------------------------------
Spatial* ManagedCameraNode::GetNativeSpatial()
{
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    return (Spatial*)(*m_pspCameraNode);
}
//---------------------------------------------------------------------------
Node* ManagedCameraNode::GetNativeNode()
{
    SE_NULL_REFERENCE_CHECK(m_pspCameraNode, "Native pointer is null");
    return (Node*)(*m_pspCameraNode);
}
//---------------------------------------------------------------------------