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
#include "SEGeometry.h"
#include "SECuller.h"
#include "SELight.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SEGeometry, SESpatial);
SE_IMPLEMENT_ABSTRACT_STREAM(SEGeometry);

//SE_REGISTER_STREAM(SEGeometry);

//----------------------------------------------------------------------------
SEGeometry::SEGeometry()
    :
    ModelBound(SEBoundingVolume::Create())
{
    RStateBlock = SE_NEW SERenderStateBlock;
    LightingMode = GLM_PIPELINE_PIXEL;
}
//----------------------------------------------------------------------------
SEGeometry::SEGeometry(SEVertexBuffer* pVBuffer, SEIndexBuffer* pIBuffer)
    :
    VBuffer(pVBuffer),
    IBuffer(pIBuffer),
    ModelBound(SEBoundingVolume::Create())
{
    RStateBlock = SE_NEW SERenderStateBlock;
    LightingMode = GLM_PIPELINE_PIXEL;
    UpdateModelBound();
}
//----------------------------------------------------------------------------
SEGeometry::~SEGeometry()
{
}
//----------------------------------------------------------------------------
void SEGeometry::UpdateMS(bool bUpdateNormals)
{
    UpdateModelBound();
    if( bUpdateNormals )
    {
        UpdateModelNormals();
    }
}
//----------------------------------------------------------------------------
void SEGeometry::UpdateModelBound()
{
    ModelBound->ComputeFromData(VBuffer);
}
//----------------------------------------------------------------------------
void SEGeometry::UpdateWorldBound()
{
    ModelBound->TransformBy(World, WorldBound);
}
//----------------------------------------------------------------------------
void SEGeometry::UpdateState(std::vector<SEGlobalState*>* aGStack,
    std::vector<SELight*>* pLStack)
{
    // ���µ�ǰ�������render state block.
    // �ռ���Ӱ�쵱ǰ�ڵ������ջ��Ԫ��.
    if( RStateBlock )
    {
        // ���֮ǰstate block�����豸��Դ,���ͷŵ�.
        // Ŀǰֻ����֧��DirectX 10��render state objects��Դ.
        RStateBlock->Release();
    }
    else
    {
        RStateBlock = SE_NEW SERenderStateBlock;
    }

    int i;
    for( i = 0; i < SEGlobalState::MAX_STATE_TYPE; i++ )
    {
        SEGlobalState* pGState = 0;
        pGState = aGStack[i].back();
        RStateBlock->States[i] = pGState;
    }

    // ����lighting effect.
    // �ռ���Ӱ�쵱ǰ�ڵ������light����.
    Lights.clear();
    int iLCount = (int)pLStack->size();
    if( LightingMode < GLM_USER )
	{
        // ����ϵͳĬ��lighting effect.

        if( iLCount > 0 )
        {
            if( m_spLEffect )
            {
                m_spLEffect->DetachAllLights();

                if( m_Effects.size() == 0 )
                {
                    m_Effects.push_back(StaticCast<SEEffect>(m_spLEffect));
                }
                else if( m_Effects[0] != m_spLEffect )
                {
                    m_Effects.insert(m_Effects.begin(), StaticCast<SEEffect>(
                        m_spLEffect));
                }
            }
            else
            {
                m_spLEffect = SE_NEW SELightingEffect;
                m_Effects.insert(m_Effects.begin(), StaticCast<SEEffect>(
                    m_spLEffect));
            }

            for( i = 0; i < iLCount; i++ )
            {
                m_spLEffect->AttachLight((*pLStack)[i]);
                Lights.push_back((*pLStack)[i]);
            }

            // ֻҪö��˳��һ��,�����ǿ���ȷ���������ת���ǰ�ȫ��.
            m_spLEffect->Configure(
                (SELightingEffect::LightingMode)LightingMode);
        }
        else
        {
            if( m_spLEffect )
            {
                SE_ASSERT( *m_Effects.begin() == m_spLEffect );

                m_Effects.erase(m_Effects.begin());
                m_spLEffect = 0;
            }
        }
    }
    else if( LightingMode == GLM_USER )
    {
        // �û��Զ���lighting effect.
        // �����Զ���lighting effect������ʹ����Ⱦ���ṩ�ĵƹ�����.

        if( m_spLEffect )
        {
            SE_ASSERT( *m_Effects.begin() == m_spLEffect );

            m_Effects.erase(m_Effects.begin());
            m_spLEffect = 0;
        }

        for( i = 0; i < iLCount; i++ )
        {
            Lights.push_back((*pLStack)[i]);
        }
    }
    else
    {
        // ��ֹ����Ч��.

        if( m_spLEffect )
        {
            SE_ASSERT( *m_Effects.begin() == m_spLEffect );

            m_Effects.erase(m_Effects.begin());
            m_spLEffect = 0;
        }
    }

    // ��ʵ��.
    // Ŀǰֻ֧��effect passЯ����alpha state.
    // ����effects��render state blocks.
    int iECount = (int)m_Effects.size();
    for( int i = 0; i < iECount; i++ )
    {
        m_Effects[i]->OnUpdateData((void*)RStateBlock);
    }
}
//----------------------------------------------------------------------------
void SEGeometry::GetUnculledSet(SECuller& rCuller, bool)
{
    rCuller.Insert(this, 0);
}
//----------------------------------------------------------------------------
SEGeometry::SEPickRecord::SEPickRecord(SEGeometry* pIObject, float fT)
    :
    SESpatial::SEPickRecord(pIObject, fT)
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// name and unique id
//----------------------------------------------------------------------------
SEObject* SEGeometry::GetObjectByName(const std::string& rName)
{
    SEObject* pFound = SESpatial::GetObjectByName(rName);
    if( pFound )
    {
        return pFound;
    }

    if( ModelBound )
    {
        pFound = ModelBound->GetObjectByName(rName);
        if( pFound )
        {
            return pFound;
        }
    }

    if( VBuffer )
    {
        pFound = VBuffer->GetObjectByName(rName);
        if( pFound )
        {
            return pFound;
        }
    }

    if( IBuffer )
    {
        pFound = IBuffer->GetObjectByName(rName);
        if( pFound )
        {
            return pFound;
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
void SEGeometry::GetAllObjectsByName(const std::string& rName,
    std::vector<SEObject*>& rObjects)
{
    SESpatial::GetAllObjectsByName(rName, rObjects);

    if( ModelBound )
    {
        ModelBound->GetAllObjectsByName(rName, rObjects);
    }

    if( VBuffer )
    {
        VBuffer->GetAllObjectsByName(rName, rObjects);
    }

    if( IBuffer )
    {
        IBuffer->GetAllObjectsByName(rName, rObjects);
    }
}
//----------------------------------------------------------------------------
SEObject* SEGeometry::GetObjectByID(unsigned int uiID)
{
    SEObject* pFound = SESpatial::GetObjectByID(uiID);
    if( pFound )
    {
        return pFound;
    }

    if( ModelBound )
    {
        pFound = ModelBound->GetObjectByID(uiID);
        if( pFound )
        {
            return pFound;
        }
    }

    if( VBuffer )
    {
        pFound = VBuffer->GetObjectByID(uiID);
        if( pFound )
        {
            return pFound;
        }
    }

    if( IBuffer )
    {
        pFound = IBuffer->GetObjectByID(uiID);
        if( pFound )
        {
            return pFound;
        }
    }

    return 0;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// streaming
//----------------------------------------------------------------------------
void SEGeometry::Load(SEStream& rStream, SEStream::SELink* pLink)
{
    SE_BEGIN_DEBUG_STREAM_LOAD;

    SESpatial::Load(rStream, pLink);

    // native data
    rStream.Read((int&)LightingMode);

    // link data
    SEObject* pObject;
    rStream.Read(pObject);  // ModelBound
    pLink->Add(pObject);

    rStream.Read(pObject);  // VBuffer
    pLink->Add(pObject);

    rStream.Read(pObject);  // IBuffer
    pLink->Add(pObject);

    rStream.Read(pObject);  // m_spLEffect
    pLink->Add(pObject);

    SE_END_DEBUG_STREAM_LOAD(SEGeometry);
}
//----------------------------------------------------------------------------
void SEGeometry::Link(SEStream& rStream, SEStream::SELink* pLink)
{
    SESpatial::Link(rStream, pLink);

    SEObject* pLinkID = pLink->GetLinkID();
    ModelBound = (SEBoundingVolume*)rStream.GetFromMap(pLinkID);

    pLinkID = pLink->GetLinkID();
    VBuffer = (SEVertexBuffer*)rStream.GetFromMap(pLinkID);

    pLinkID = pLink->GetLinkID();
    IBuffer = (SEIndexBuffer*)rStream.GetFromMap(pLinkID);

    pLinkID = pLink->GetLinkID();
    m_spLEffect = (SELightingEffect*)rStream.GetFromMap(pLinkID);
}
//----------------------------------------------------------------------------
bool SEGeometry::Register(SEStream& rStream) const
{
    if( !SESpatial::Register(rStream) )
    {
        return false;
    }

    if( ModelBound )
    {
        ModelBound->Register(rStream);
    }

    if( VBuffer )
    {
        VBuffer->Register(rStream);
    }

    if( IBuffer )
    {
        IBuffer->Register(rStream);
    }

    if( m_spLEffect )
    {
        m_spLEffect->Register(rStream);
    }

    return true;
}
//----------------------------------------------------------------------------
void SEGeometry::Save(SEStream& rStream) const
{
    SE_BEGIN_DEBUG_STREAM_SAVE;

    SESpatial::Save(rStream);

    // native data
    rStream.Write((int)LightingMode);

    // link data
    rStream.Write(ModelBound);
    rStream.Write(VBuffer);
    rStream.Write(IBuffer);
    rStream.Write(m_spLEffect);

    SE_END_DEBUG_STREAM_SAVE(SEGeometry);
}
//----------------------------------------------------------------------------
int SEGeometry::GetDiskUsed(const SEStreamVersion& rVersion) const
{
    return SESpatial::GetDiskUsed(rVersion) +
        sizeof(LightingMode) +
        SE_PTRSIZE(ModelBound) +
        SE_PTRSIZE(VBuffer) +
        SE_PTRSIZE(IBuffer) +
        SE_PTRSIZE(m_spLEffect);
}
//----------------------------------------------------------------------------
SEStringTree* SEGeometry::SaveStrings(const char*)
{
    SEStringTree* pTree = SE_NEW SEStringTree;

    // strings
    pTree->Append(Format(&TYPE, GetName().c_str()));
    pTree->Append(Format("lighting mode =", (int)LightingMode));

    // children
    pTree->Append(SESpatial::SaveStrings());

    if( ModelBound )
    {
        pTree->Append(ModelBound->SaveStrings());
    }

    if( VBuffer )
    {
        pTree->Append(VBuffer->SaveStrings("vertices"));
    }

    if( IBuffer )
    {
        pTree->Append(IBuffer->SaveStrings("indices"));
    }

    return pTree;
}
//----------------------------------------------------------------------------
