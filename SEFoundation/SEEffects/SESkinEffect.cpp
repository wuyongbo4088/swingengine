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
#include "SESkinEffect.h"
#include "SEVertexProgramCatalog.h"
#include "SENode.h"
#include "SETransformation.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SESkinEffect, SEShaderEffect);
SE_IMPLEMENT_ABSTRACT_STREAM(SESkinEffect);
SE_IMPLEMENT_DEFAULT_NAME_ID(SESkinEffect, SEShaderEffect);
SE_IMPLEMENT_INITIALIZE(SESkinEffect);
SE_IMPLEMENT_TERMINATE(SESkinEffect);

//SE_REGISTER_STREAM(SESkinEffect);
//SE_REGISTER_INITIALIZE(SESkinEffect);
//SE_REGISTER_TERMINATE(SESkinEffect);

float* SESkinEffect::ms_apSkinMatrix = 0;

//----------------------------------------------------------------------------
void SESkinEffect::Initialize()
{
    ms_apSkinMatrix = SE_NEW float[SM_COUNT*16];
}
//----------------------------------------------------------------------------
void SESkinEffect::Terminate()
{
    SE_DELETE ms_apSkinMatrix;
    ms_apSkinMatrix = 0;
}
//----------------------------------------------------------------------------
SESkinEffect::SESkinEffect(int iBoneCount, SENode** apBones, 
    SETransformation* aOffset)
    :
    SEShaderEffect(1)
{
    SE_ASSERT( iBoneCount < SM_COUNT );

    m_iBoneCount = iBoneCount;
    m_apBones = apBones;
    m_aOffset = aOffset;
}
//----------------------------------------------------------------------------
SESkinEffect::SESkinEffect()
{
    m_iBoneCount = 0;
    m_apBones = 0;
    m_aOffset = 0;
}
//----------------------------------------------------------------------------
SESkinEffect::~SESkinEffect()
{
    SE_DELETE[] m_apBones;
    SE_DELETE[] m_aOffset;
}
//----------------------------------------------------------------------------
int SESkinEffect::GetBoneCount() const
{
    return m_iBoneCount;
}
//----------------------------------------------------------------------------
SENode** SESkinEffect::GetBones()
{
    return m_apBones;
}
//----------------------------------------------------------------------------
SETransformation* SESkinEffect::GetOffsets()
{
    return m_aOffset;
}
//----------------------------------------------------------------------------
void SESkinEffect::InitializeUserConstants(SEProgram* pVProgram)
{
    int iMaxCount = 0;
    int iVProfile = SEVertexProgramCatalog::GetActive()->GetProfile();

    if( iVProfile == SERenderer::OES2VP1 )
    {
        char tempName[16]; // "SkinArray[xx].M"

        iMaxCount = OES2VP1_COUNT;
        for( int i = 0; i < iMaxCount; i++ )
        {
            SESystem::SE_Sprintf(tempName, 16, "SkinArray[%d].M", i);
            SEUserConstant* pUC = pVProgram->GetUC(tempName);
            SE_ASSERT( pUC );
        
            if( pUC )
            {
                pUC->SetDataSource((float*)&ms_apSkinMatrix[i*16]);
            }
        }
    }
    else
    {
        char tempName[15]; // "SkinMatrix[xx]"

        switch( iVProfile )
        {
        case SERenderer::VS_2_0:
            iMaxCount = VS_2_0_COUNT;
            break;
        case SERenderer::VS_3_0:
            iMaxCount = VS_3_0_COUNT;
            break;
        case SERenderer::ARBVP1:
            iMaxCount = ARBVP1_COUNT;
            break;
        case SERenderer::VP40:
            iMaxCount = VP40_COUNT;
            break;
        case SERenderer::SFTVP1:
            iMaxCount = SFTVP1_COUNT;
            break;
        }

        for( int i = 0; i < iMaxCount; i++ )
        {
            SESystem::SE_Sprintf(tempName, 15, "SkinMatrix[%d]", i);
            SEUserConstant* pUC = pVProgram->GetUC(tempName);
            SE_ASSERT( pUC );
        
            if( pUC )
            {
                pUC->SetDataSource((float*)&ms_apSkinMatrix[i*16]);
            }
        }
    }
}
//----------------------------------------------------------------------------
void SESkinEffect::OnLoadPrograms(int, SEProgram*, SEProgram*, SEProgram*)
{
    // Stub for derived classes.
}
//----------------------------------------------------------------------------
void SESkinEffect::OnPreApplyEffect(SERenderer*, bool)
{
    SETransformation tempT;
    SEMatrix4f tempMat;
    for( int i = 0; i < m_iBoneCount; i++ )
    {
        tempT.Product(m_aOffset[i], m_apBones[i]->World);
        tempT.GetHomogeneous(tempMat);

        int iIndex = i*16;
        ms_apSkinMatrix[iIndex    ] = tempMat[0][0];
        ms_apSkinMatrix[iIndex + 1] = tempMat[0][1];
        ms_apSkinMatrix[iIndex + 2] = tempMat[0][2];
        ms_apSkinMatrix[iIndex + 3] = tempMat[0][3];

        ms_apSkinMatrix[iIndex + 4] = tempMat[1][0];
        ms_apSkinMatrix[iIndex + 5] = tempMat[1][1];
        ms_apSkinMatrix[iIndex + 6] = tempMat[1][2];
        ms_apSkinMatrix[iIndex + 7] = tempMat[1][3];

        ms_apSkinMatrix[iIndex +  8] = tempMat[2][0];
        ms_apSkinMatrix[iIndex +  9] = tempMat[2][1];
        ms_apSkinMatrix[iIndex + 10] = tempMat[2][2];
        ms_apSkinMatrix[iIndex + 11] = tempMat[2][3];

        ms_apSkinMatrix[iIndex + 12] = tempMat[3][0];
        ms_apSkinMatrix[iIndex + 13] = tempMat[3][1];
        ms_apSkinMatrix[iIndex + 14] = tempMat[3][2];
        ms_apSkinMatrix[iIndex + 15] = tempMat[3][3];
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// streaming
//----------------------------------------------------------------------------
void SESkinEffect::Load(SEStream& rStream, SEStream::SELink* pLink)
{
    SE_BEGIN_DEBUG_STREAM_LOAD;

    SEShaderEffect::Load(rStream, pLink);

    // native data
    rStream.Read(m_iBoneCount);

    int i;

    m_aOffset = SE_NEW SETransformation[m_iBoneCount];
    for( i = 0; i < m_iBoneCount; i++ )
    {
        rStream.Read(m_aOffset[i]);
    }

    // link data
    for( i = 0; i < m_iBoneCount; i++ )
    {
        SEObject* pObject;
        rStream.Read(pObject);  // m_apBones[i]
        pLink->Add(pObject);
    }

    SE_END_DEBUG_STREAM_LOAD(SESkinEffect);
}
//----------------------------------------------------------------------------
void SESkinEffect::Link(SEStream& rStream, SEStream::SELink* pLink)
{
    SEShaderEffect::Link(rStream, pLink);

    m_apBones = SE_NEW SENode*[m_iBoneCount];
    for( int i = 0; i < m_iBoneCount; i++ )
    {
        SEObject* pLinkID = pLink->GetLinkID();
        m_apBones[i] = (SENode*)rStream.GetFromMap(pLinkID);
    }
}
//----------------------------------------------------------------------------
bool SESkinEffect::Register(SEStream& rStream) const
{
    if( !SEShaderEffect::Register(rStream) )
    {
        return false;
    }

    for( int i = 0; i < m_iBoneCount; i++ )
    {
        if( m_apBones[i] )
        {
            m_apBones[i]->Register(rStream);
        }
    }

    return true;
}
//----------------------------------------------------------------------------
void SESkinEffect::Save(SEStream& rStream) const
{
    SE_BEGIN_DEBUG_STREAM_SAVE;

    SEShaderEffect::Save(rStream);

    // native data
    rStream.Write(m_iBoneCount);

    int i;

    for( i = 0; i < m_iBoneCount; i++ )
    {
        rStream.Write(m_aOffset[i]);
    }

    // link data
    for( i = 0; i < m_iBoneCount; i++ )
    {
        rStream.Write(m_apBones[i]);
    }

    SE_END_DEBUG_STREAM_SAVE(SESkinEffect);
}
//----------------------------------------------------------------------------
int SESkinEffect::GetDiskUsed(const SEStreamVersion& rVersion) const
{
    return SEShaderEffect::GetDiskUsed(rVersion) +
        sizeof(m_iBoneCount) + 
        m_iBoneCount*SETransformation::DISK_USED +
        m_iBoneCount*SE_PTRSIZE(m_apBones[0]);
}
//----------------------------------------------------------------------------
SEStringTree* SESkinEffect::SaveStrings(const char*)
{
    SEStringTree* pTree = SE_NEW SEStringTree;

    // strings
    pTree->Append(Format(&TYPE, GetName().c_str()));
    pTree->Append(Format("bone count = ", m_iBoneCount));

    // children
    pTree->Append(SEShaderEffect::SaveStrings());
    pTree->Append(Format("bones", m_iBoneCount, m_apBones));

    return pTree;
}
//----------------------------------------------------------------------------