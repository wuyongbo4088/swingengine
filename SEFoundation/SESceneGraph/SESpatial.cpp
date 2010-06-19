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

#include "SEFoundationPCH.h"
#include "SESpatial.h"
#include "SECamera.h"
#include "SECuller.h"
#include "SELight.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SESpatial, SEAdvancedObject);
SE_IMPLEMENT_ABSTRACT_STREAM(SESpatial);

//SE_REGISTER_STREAM(SESpatial);

const char* SESpatial::ms_aacCullingMode[MAX_CULLING_MODE] =
{
    "CULL_DYNAMIC",
    "CULL_ALWAYS",
    "CULL_NEVER"
};

//----------------------------------------------------------------------------
SESpatial::SESpatial()
    :
    WorldBound(SEBoundingVolume::Create())
{
    Culling = CULL_DYNAMIC;
    WorldIsCurrent = false;
    WorldBoundIsCurrent = false;
    m_pParent = 0;
    m_iStartEffect = 0;
}
//----------------------------------------------------------------------------
SESpatial::~SESpatial()
{
    DetachAllGlobalStates();
    DetachAllLights();
    DetachAllEffects();
}
//----------------------------------------------------------------------------
void SESpatial::UpdateGS(double dAppTime, bool bInitiator)
{
    // ����controller,��������任,
    // ����,������SENodeͨ�����麯��ʵ��AB�ݹ��������,
    // SEGeometryû�����ش˺���.
    UpdateWorldData(dAppTime);
    // �����Ϻ������غ�,���ܽ�������ռ�BV����.
    UpdateWorldBound();

    if( bInitiator )
    {
        // ��ʼ���������������ϴ�������ռ�BV����.
        PropagateBoundToRoot();
    }
}
//----------------------------------------------------------------------------
void SESpatial::UpdateBS()
{
    UpdateWorldBound();
    PropagateBoundToRoot();
}
//----------------------------------------------------------------------------
void SESpatial::UpdateWorldData(double dAppTime)
{
    // �������е�ǰ�����controller
    UpdateControllers(dAppTime);

    int i;
    for( i = 0; i < (int)m_GlobalStates.size(); i++ )
    {
        m_GlobalStates[i]->UpdateControllers(dAppTime);
    }

    for( i = 0; i < (int)m_Lights.size(); i++ )
    {
        ((SELight*)(SEObject*)m_Lights[i])->UpdateControllers(dAppTime);
    }

    for( i = 0; i < (int)m_Effects.size(); i++ )
    {
        m_Effects[i]->UpdateControllers(dAppTime);
    }

    // ��������任
    if( !WorldIsCurrent )
    {
        if( m_pParent )
        {
            World.Product(Local, m_pParent->World);
        }
        else
        {
            World = Local;
        }
    }
}
//----------------------------------------------------------------------------
void SESpatial::PropagateBoundToRoot()
{
    if( m_pParent )
    {
        m_pParent->UpdateWorldBound();
        m_pParent->PropagateBoundToRoot();
    }
}
//----------------------------------------------------------------------------
SEGlobalState* SESpatial::GetGlobalState(SEGlobalState::StateType eType) const
{
    // �Ƿ���ڸ����͵���Ⱦ״̬
    for( int i = 0; i < (int)m_GlobalStates.size(); i++ )
    {
        if( m_GlobalStates[i]->GetStateType() == eType )
        {
            // �����򷵻�
            return m_GlobalStates[i];
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
void SESpatial::AttachGlobalState(SEGlobalState* pState)
{
    SE_ASSERT( pState );

    // �Ƿ���ڸ����͵���Ⱦ״̬
    for( int i = 0; i < (int)m_GlobalStates.size(); i++ )
    {
        if( m_GlobalStates[i]->GetStateType() == pState->GetStateType() )
        {
            // �����򸲸�
            m_GlobalStates[i] = pState;

            return;
        }
    }

    // ����������Ӹ�����
    m_GlobalStates.push_back(pState);
}
//----------------------------------------------------------------------------
void SESpatial::DetachGlobalState(SEGlobalState::StateType eType)
{
    std::vector<SEGlobalStatePtr>::iterator pIter = m_GlobalStates.begin();
    for( /**/; pIter != m_GlobalStates.end(); pIter++ )
    {
        SEGlobalState* pState = *pIter;
        if( pState->GetStateType() == eType )
        {
            m_GlobalStates.erase(pIter);

            return;
        }
    }
}
//----------------------------------------------------------------------------
void SESpatial::AttachLight(SELight* pLight)
{
    SE_ASSERT( pLight );

    // �Ƿ���ڸ�light
    for( int i = 0; i < (int)m_Lights.size(); i++ )
    {
        if( m_Lights[i] == pLight )
        {
            // �����򷵻�
            return;
        }
    }

    // ����������Ӹ�light
    m_Lights.push_back(pLight);
}
//----------------------------------------------------------------------------
void SESpatial::DetachLight(SELight* pLight)
{
    std::vector<SEObjectPtr>::iterator pIter = m_Lights.begin();
    for( /**/; pIter != m_Lights.end(); pIter++ )
    {
        if( pLight == *pIter )
        {
            m_Lights.erase(pIter);

            return;
        }
    }
}
//----------------------------------------------------------------------------
void SESpatial::AttachEffect(SEEffect* pEffect)
{
    SE_ASSERT( pEffect );

    // �Ƿ���ڸ�effect
    for( int i = 0; i < (int)m_Effects.size(); i++ )
    {
        if( m_Effects[i] == pEffect )
        {
            // �����򷵻�
            return;
        }
    }

    // ����������Ӹ�effect
    m_Effects.push_back(pEffect);
}
//----------------------------------------------------------------------------
void SESpatial::DetachEffect(SEEffect* pEffect)
{
    std::vector<SEEffectPtr>::iterator pIter = m_Effects.begin();
    for( /**/; pIter != m_Effects.end(); pIter++ )
    {
        if( pEffect == *pIter )
        {
            m_Effects.erase(pIter);

            return;
        }
    }
}
//----------------------------------------------------------------------------
void SESpatial::UpdateRS(std::vector<SEGlobalState*>* aGStack,
    std::vector<SELight*>* pLStack)
{
    bool bInitiator = (aGStack == 0);

    if( bInitiator )
    {
        // render stateӦ��˳��:
        //   (1) Ĭ��global state.
        //   (2) SEGeometry����Ĭ��global state.
        //   (3) SEEffect����state������������.
        aGStack = SE_NEW std::vector<SEGlobalState*>[
            SEGlobalState::MAX_STATE_TYPE];
        for( int i = 0; i < SEGlobalState::MAX_STATE_TYPE; i++ )
        {
            aGStack[i].push_back(0);
        }

        // ��ʼʱstackû��light.
        pLStack = SE_NEW std::vector<SELight*>;

        // ׷�ݵ���ǰ·������,�ռ���;render state,light,
        // ѹջ˳��Ϊ��������.
        PropagateStateFromRoot(aGStack, pLStack);
    }
    else
    {
        // ����ǰ�ڵ�render state,lightѹջ.
        PushState(aGStack, pLStack);
    }

    // �������ݹ鴫��render state,light,
    // ������SENode����AB�ݹ�,
    // ������SEGeometry�����ռ����������ϵ�����render state,light.
    UpdateState(aGStack, pLStack);

    if( bInitiator )
    {
        SE_DELETE[] aGStack;
        SE_DELETE pLStack;
    }
    else
    {
        // ����ǰ�ڵ�render state,light��ջ.
        PopState(aGStack, pLStack);
    }
}
//----------------------------------------------------------------------------
void SESpatial::PropagateStateFromRoot(std::vector<SEGlobalState*>* aGStack,
    std::vector<SELight*>* pLStack)
{
    // ����һֱ׷�ݵ�����.
    if( m_pParent )
    {
        m_pParent->PropagateStateFromRoot(aGStack, pLStack);
    }

    // �ýڵ�render state,lightѹջ.
    PushState(aGStack, pLStack);
}
//----------------------------------------------------------------------------
void SESpatial::PushState(std::vector<SEGlobalState*>* aGStack,
    std::vector<SELight*>* pLStack)
{
    int i;
    for( i = 0; i < (int)m_GlobalStates.size(); i++ )
    {
        int eType = m_GlobalStates[i]->GetStateType();
        aGStack[eType].push_back(m_GlobalStates[i]);
    }

    for( i = 0; i < (int)m_Lights.size(); i++ )
    {
        SELight* pLight = StaticCast<SELight>(m_Lights[i]);
        pLStack->push_back(pLight);
    }
}
//----------------------------------------------------------------------------
void SESpatial::PopState(std::vector<SEGlobalState*>* aGStack,
    std::vector<SELight*>* pLStack)
{
    int i;
    for( i = 0; i < (int)m_GlobalStates.size(); i++ )
    {
        int eType = m_GlobalStates[i]->GetStateType();
        aGStack[eType].pop_back();
    }

    for( i = 0; i < (int)m_Lights.size(); i++ )
    {
        pLStack->pop_back();
    }
}
//----------------------------------------------------------------------------
void SESpatial::OnGetUnculledSet(SECuller& rCuller, bool bNoCull)
{
    if( Culling == CULL_ALWAYS )
    {
        return;
    }

    if( Culling == CULL_NEVER )
    {
        bNoCull = true;
    }

    unsigned int uiSavePlaneState = rCuller.GetPlaneState();
    if( bNoCull || rCuller.IsInFrustum(WorldBound) )
    {
        // ������SENodeʵ��AB�ݹ�,�ռ�global effect,�����޳����̴���������,
        // ������SEGeometryʵ��δ���޳��Ŀ���Ⱦ�����ռ�
        GetUnculledSet(rCuller, bNoCull);
    }
    rCuller.SetPlaneState(uiSavePlaneState);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// picking support
//----------------------------------------------------------------------------
SESpatial::SEPickRecord::SEPickRecord(SESpatial* pIObject, float fT)
    :
    IObject(pIObject)
{
    T = fT;
}
//----------------------------------------------------------------------------
SESpatial::SEPickRecord::~SEPickRecord()
{
    // �������ฺ��ʵ��.
}
//----------------------------------------------------------------------------
void SESpatial::DoPick(const SERay3f&, PickArray&)
{
    // �������ฺ��ʵ��.
}
//----------------------------------------------------------------------------
SESpatial::SEPickRecord* SESpatial::GetClosest(PickArray& rResults)
{
    if( rResults.size() == 0 )
    {
        return 0;
    }

    float fTMin = rResults[0]->T;
    int iMin = 0;
    for( int i = 1; i < (int)rResults.size(); i++ )
    {
        float fT = rResults[i]->T;
        if( fT < fTMin )
        {
            fTMin = fT;
            iMin = i;
        }
    }

    return rResults[iMin];
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// name and unique id
//----------------------------------------------------------------------------
SEObject* SESpatial::GetObjectByName(const std::string& rName)
{
    SEObject* pFound = SEAdvancedObject::GetObjectByName(rName);
    if( pFound )
    {
        return pFound;
    }

    if( WorldBound )
    {
        pFound = WorldBound->GetObjectByName(rName);
        if( pFound )
        {
            return pFound;
        }
    }

    int i;
    for( i = 0; i < (int)m_GlobalStates.size(); i++ )
    {
        if( m_GlobalStates[i] )
        {
            pFound = m_GlobalStates[i]->GetObjectByName(rName);
            if( pFound )
            {
                return pFound;
            }
        }
    }

    for( i = 0; i < (int)m_Lights.size(); i++ )
    {
        if( m_Lights[i] )
        {
            pFound = m_Lights[i]->GetObjectByName(rName);
            if( pFound )
            {
                return pFound;
            }
        }
    }

    for( i = 0; i < (int)m_Effects.size(); i++ )
    {
        if( m_Effects[i] )
        {
            pFound = m_Effects[i]->GetObjectByName(rName);
            if( pFound )
            {
                return pFound;
            }
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
void SESpatial::GetAllObjectsByName(const std::string& rName,
    std::vector<SEObject*>& rObjects)
{
    SEAdvancedObject::GetAllObjectsByName(rName, rObjects);

    if( WorldBound )
    {
        WorldBound->GetAllObjectsByName(rName, rObjects);
    }

    int i;
    for( i = 0; i < (int)m_GlobalStates.size(); i++ )
    {
        if( m_GlobalStates[i] )
        {
            m_GlobalStates[i]->GetAllObjectsByName(rName, rObjects);
        }
    }

    for( i = 0; i < (int)m_Lights.size(); i++ )
    {
        if( m_Lights[i] )
        {
            m_Lights[i]->GetAllObjectsByName(rName, rObjects);
        }
    }

    for( i = 0; i < (int)m_Effects.size(); i++ )
    {
        if( m_Effects[i] )
        {
            m_Effects[i]->GetAllObjectsByName(rName, rObjects);
        }
    }
}
//----------------------------------------------------------------------------
SEObject* SESpatial::GetObjectByID(unsigned int uiID)
{
    SEObject* pFound = SEAdvancedObject::GetObjectByID(uiID);
    if( pFound )
    {
        return pFound;
    }

    if( WorldBound )
    {
        pFound = WorldBound->GetObjectByID(uiID);
        if( pFound )
        {
            return pFound;
        }
    }

    int i;
    for( i = 0; i < (int)m_GlobalStates.size(); i++ )
    {
        if( m_GlobalStates[i] )
        {
            pFound = m_GlobalStates[i]->GetObjectByID(uiID);
            if( pFound )
            {
                return pFound;
            }
        }
    }

    for( i = 0; i < (int)m_Lights.size(); i++ )
    {
        if( m_Lights[i] )
        {
            pFound = m_Lights[i]->GetObjectByID(uiID);
            if( pFound )
            {
                return pFound;
            }
        }
    }

    for( i = 0; i < (int)m_Effects.size(); i++ )
    {
        if( m_Effects[i] )
        {
            pFound = m_Effects[i]->GetObjectByID(uiID);
            if( pFound )
            {
                return pFound;
            }
        }
    }

    return 0;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// streaming
//----------------------------------------------------------------------------
void SESpatial::Load(SEStream& rStream, SEStream::SELink* pLink)
{
    SE_BEGIN_DEBUG_STREAM_LOAD;

    SEAdvancedObject::Load(rStream, pLink);

    // native data
    rStream.Read(Local);
    rStream.Read(World);
    int iTemp;
    rStream.Read(iTemp);
    Culling = (CullingMode)iTemp;

    rStream.Read(WorldIsCurrent);
    rStream.Read(WorldBoundIsCurrent);

    // link data
    SEObject* pObject;
    rStream.Read(pObject);  // WorldBound
    pLink->Add(pObject);

    int i, iCount;
    rStream.Read(iCount);
    m_GlobalStates.resize(iCount);
    for( i = 0; i < iCount; i++ )
    {
        SEObject* pObject;
        rStream.Read(pObject);  // m_GlobalStates[i]
        pLink->Add(pObject);
    }

    rStream.Read(iCount);
    m_Lights.resize(iCount);
    for( i = 0; i < iCount; i++ )
    {
        SEObject* pObject;
        rStream.Read(pObject);  // m_Lights[i]
        pLink->Add(pObject);
    }

    rStream.Read(iCount);
    m_Effects.resize(iCount);
    for( i = 0; i < iCount; i++ )
    {
        SEObject* pObject;
        rStream.Read(pObject);  // m_Effects[i]
        pLink->Add(pObject);
    }

    SE_END_DEBUG_STREAM_LOAD(SESpatial);
}
//----------------------------------------------------------------------------
void SESpatial::Link(SEStream& rStream, SEStream::SELink* pLink)
{
    SEAdvancedObject::Link(rStream, pLink);

    SEObject* pLinkID = pLink->GetLinkID();
    WorldBound = (SEBoundingVolume*)rStream.GetFromMap(pLinkID);

    int i;
    for( i = 0; i < (int)m_GlobalStates.size(); i++ )
    {
        pLinkID = pLink->GetLinkID();
        m_GlobalStates[i] = (SEGlobalState*)rStream.GetFromMap(pLinkID);
    }

    for( i = 0; i < (int)m_Lights.size(); i++ )
    {
        pLinkID = pLink->GetLinkID();
        m_Lights[i] = (SELight*)rStream.GetFromMap(pLinkID);
    }

    for( i = 0; i < (int)m_Effects.size(); i++ )
    {
        pLinkID = pLink->GetLinkID();
        m_Effects[i] = (SEEffect*)rStream.GetFromMap(pLinkID);
    }
}
//----------------------------------------------------------------------------
bool SESpatial::Register(SEStream& rStream) const
{
    if( !SEAdvancedObject::Register(rStream) )
    {
        return false;
    }

    if( WorldBound )
    {
        WorldBound->Register(rStream);
    }

    int i;
    for( i = 0; i < (int)m_GlobalStates.size(); i++ )
    {
        if( m_GlobalStates[i] )
        {
            m_GlobalStates[i]->Register(rStream);
        }
    }

    for( i = 0; i < (int)m_Lights.size(); i++ )
    {
        if( m_Lights[i] )
        {
            m_Lights[i]->Register(rStream);
        }
    }

    for( i = 0; i < (int)m_Effects.size(); i++ )
    {
        if( m_Effects[i] )
        {
            m_Effects[i]->Register(rStream);
        }
    }

    // m_pParent����Ҫ������register,��Ȼ���ݳ�����ͼ�ı���˳��,
    // ���ѱ������������������Register����������������ȫ���ӽڵ�,
    // ��ǰ�ڵ�������ӽڵ��е�һ��.
    return true;
}
//----------------------------------------------------------------------------
void SESpatial::Save(SEStream& rStream) const
{
    SE_BEGIN_DEBUG_STREAM_SAVE;

    SEAdvancedObject::Save(rStream);

    // native data
    rStream.Write(Local);
    rStream.Write(World);
    rStream.Write((int)Culling);
    rStream.Write(WorldIsCurrent);
    rStream.Write(WorldBoundIsCurrent);

    // link data
    rStream.Write(WorldBound);

    int iCount = (int)m_GlobalStates.size();
    rStream.Write(iCount);
    int i;
    for( i = 0; i < iCount; i++ )
    {
        rStream.Write(m_GlobalStates[i]);
    }

    iCount = (int)m_Lights.size();
    rStream.Write(iCount);
    for( i = 0; i < iCount; i++ )
    {
        rStream.Write(m_Lights[i]);
    }

    iCount = (int)m_Effects.size();
    rStream.Write(iCount);
    for( i = 0; i < iCount; i++ )
    {
        rStream.Write(m_Effects[i]);
    }

    // m_pParent����Ҫ������save���ļ�,
    // ��load�׶�,�ᱻ��̬����.

    SE_END_DEBUG_STREAM_SAVE(SESpatial);
}
//----------------------------------------------------------------------------
int SESpatial::GetDiskUsed(const SEStreamVersion& rVersion) const
{
    int iSize = SEAdvancedObject::GetDiskUsed(rVersion) +
        SETransformation::DISK_USED +  // Local
        SETransformation::DISK_USED +  // World
        sizeof(char) + // WorldIsCurrent
        sizeof(char) + // WorldBoundIsCurrent
        SE_PTRSIZE(WorldBound);

    iSize += sizeof(int); // Culling

    iSize +=
        sizeof(int) + ((int)m_GlobalStates.size())*
        SE_PTRSIZE(SEGlobalStatePtr) +
        sizeof(int) + ((int)m_Lights.size())*SE_PTRSIZE(SELightPtr) +
        sizeof(int) + ((int)m_Effects.size())*SE_PTRSIZE(SEEffectPtr);

    return iSize;
}
//----------------------------------------------------------------------------
SEStringTree* SESpatial::SaveStrings(const char*)
{
    SEStringTree* pTree = SE_NEW SEStringTree;

    // strings
    pTree->Append(Format(&TYPE, GetName().c_str()));

    if( Local.IsSRMatrix() )
    {
        pTree->Append(Format("local rotate =", Local.GetRotate()));
    }
    else
    {
        pTree->Append(Format("local matrix =", Local.GetMatrix()));
    }

    pTree->Append(Format("local trans =", Local.GetTranslate()));

    if( Local.IsSRMatrix() )
    {
        pTree->Append(Format("local scale =", Local.GetScale()));
    }
    else
    {
        pTree->Append(Format("local scale part of matrix"));
    }

    if( World.IsSRMatrix() )
    {
        pTree->Append(Format("world rotate =", World.GetRotate()));
    }
    else
    {
        pTree->Append(Format("world matrix =", World.GetMatrix()));
    }

    pTree->Append(Format("world trans =", World.GetTranslate()));

    if( World.IsSRMatrix() )
    {
        pTree->Append(Format("world scale =", World.GetScale()));
    }
    else
    {
        pTree->Append(Format("world scale part of matrix"));
    }

    pTree->Append(Format("world bound =", WorldBound));
    pTree->Append(Format("culling =", ms_aacCullingMode[Culling]));
    pTree->Append(Format("start effect =", m_iStartEffect));
    pTree->Append(Format("effect count =", (int)m_Effects.size()));

    // children
    pTree->Append(SEAdvancedObject::SaveStrings());

    int iCount = (int)m_GlobalStates.size();
    int i;
    for( i = 0; i < iCount; i++ )
    {
        pTree->Append(m_GlobalStates[i]->SaveStrings());
    }

    iCount = (int)m_Lights.size();
    for( i = 0; i < iCount; i++ )
    {
        pTree->Append(m_Lights[i]->SaveStrings());
    }

    iCount = (int)m_Effects.size();
    for( i = 0; i < iCount; i++ )
    {
        pTree->Append(m_Effects[i]->SaveStrings());
    }

    return pTree;
}
//----------------------------------------------------------------------------
