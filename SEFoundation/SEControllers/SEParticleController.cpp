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
#include "SEParticleController.h"
#include "SEParticles.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SEParticleController, SEController);
SE_IMPLEMENT_STREAM(SEParticleController);
SE_IMPLEMENT_DEFAULT_NAME_ID(SEParticleController, SEController);

//SE_REGISTER_STREAM(SEParticleController);

//----------------------------------------------------------------------------
SEParticleController::SEParticleController()
    :
    SystemLinearAxis(SEVector3f::UNIT_Z),
    SystemAngularAxis(SEVector3f::UNIT_Z)
{
    SystemLinearSpeed = 0.0f;
    SystemAngularSpeed = 0.0f;
    m_iLCount = 0;
    m_afPointLinearSpeed = 0;
    m_aPointLinearAxis = 0;
    SystemSizeChange = 0.0f;
    m_afPointSizeChange = 0;
}
//----------------------------------------------------------------------------
SEParticleController::~SEParticleController()
{
    SE_DELETE[] m_afPointLinearSpeed;
    SE_DELETE[] m_aPointLinearAxis;
    SE_DELETE[] m_afPointSizeChange;
}
//----------------------------------------------------------------------------
void SEParticleController::Reallocate(int iLCount)
{
    SystemLinearSpeed = 0.0f;
    SystemAngularSpeed = 0.0f;
    SystemLinearAxis = SEVector3f::UNIT_Z;
    SystemAngularAxis = SEVector3f::UNIT_Z;
    SystemSizeChange = 0.0f;

    SE_DELETE[] m_afPointLinearSpeed;
    SE_DELETE[] m_aPointLinearAxis;
    SE_DELETE[] m_afPointSizeChange;

    m_iLCount = iLCount;
    if( m_iLCount > 0 )
    {
        m_afPointLinearSpeed = SE_NEW float[m_iLCount];
        m_aPointLinearAxis = SE_NEW SEVector3f[m_iLCount];
        m_afPointSizeChange = SE_NEW float[m_iLCount];

        memset(m_afPointLinearSpeed, 0, m_iLCount*sizeof(float));
        for( int i = 0; i < m_iLCount; i++ )
        {
            m_aPointLinearAxis[i] = SEVector3f::UNIT_Z;
        }

        memset(m_afPointSizeChange, 0, m_iLCount*sizeof(float));
    }
    else
    {
        m_afPointLinearSpeed = 0;
        m_aPointLinearAxis = 0;
        m_afPointSizeChange = 0;
    }
}
//----------------------------------------------------------------------------
void SEParticleController::SetObject(SEObject* pObject)
{
    SEController::SetObject(pObject);

    if( pObject )
    {
        SE_ASSERT( pObject->IsDerived(Particles::TYPE) );
        Particles* pParticles = StaticCast<Particles>(pObject);
        Reallocate(pParticles->Locations->GetCount());
    }
    else
    {
        Reallocate(0);
    }
}
//----------------------------------------------------------------------------
void SEParticleController::UpdateSystemMotion(float fCtrlTime)
{
    Particles* pParticle = StaticCast<Particles>(m_pObject);

    float fDSize = fCtrlTime*SystemSizeChange;
    pParticle->SizeAdjust += fDSize;
    if( pParticle->SizeAdjust < 0.0f )
    {
        pParticle->SizeAdjust = 0.0f;
    }

    float fDistance = fCtrlTime*SystemLinearSpeed;
    SEVector3f vec3fDTrn = fDistance*SystemLinearAxis;
    pParticle->Local.SetTranslate(pParticle->Local.GetTranslate() + vec3fDTrn);

    float fAngle = fCtrlTime*SystemAngularSpeed;
    SEMatrix3f mat3fDRot(SystemAngularAxis, fAngle);
    pParticle->Local.SetRotate(mat3fDRot);
}
//----------------------------------------------------------------------------
void SEParticleController::UpdatePointMotion(float fCtrlTime)
{
    Particles* pParticle = StaticCast<Particles>(m_pObject);
    int i, iACount = pParticle->GetActiveCount();
    float* afSize = pParticle->Sizes->GetData();

    for( i = 0; i < iACount; i++ )
    {
        float fDSize = fCtrlTime*m_afPointSizeChange[i];
        afSize[i] += fDSize;
    }

    SEVector3f* aLocation = pParticle->Locations->GetData();
    for( i = 0; i < iACount; i++ )
    {
        float fDistance = fCtrlTime*m_afPointLinearSpeed[i];
        SEVector3f vec3fDTrn = fDistance*m_aPointLinearAxis[i];
        aLocation[i] += vec3fDTrn;
    }
}
//----------------------------------------------------------------------------
bool SEParticleController::Update(double dAppTime)
{
    if( !SEController::Update(dAppTime) )
    {
        return false;
    }

    float fCtrlTime = (float)GetControlTime(dAppTime);

    UpdateSystemMotion(fCtrlTime);
    UpdatePointMotion(fCtrlTime);
    return true;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// streaming
//----------------------------------------------------------------------------
void SEParticleController::Load(SEStream& rStream, SEStream::Link* pLink)
{
    SE_BEGIN_DEBUG_STREAM_LOAD;

    SEController::Load(rStream, pLink);

    // native data
    rStream.Read(SystemLinearSpeed);
    rStream.Read(SystemAngularSpeed);
    rStream.Read(SystemLinearAxis);
    rStream.Read(SystemAngularAxis);
    rStream.Read(SystemSizeChange);

    rStream.Read(m_iLCount);
    if( m_iLCount > 0 )
    {
        Reallocate(m_iLCount);
        rStream.Read(m_iLCount, m_afPointLinearSpeed);
        rStream.Read(m_iLCount, m_aPointLinearAxis);
        rStream.Read(m_iLCount, m_afPointSizeChange);
    }

    SE_END_DEBUG_STREAM_LOAD(SEParticleController);
}
//----------------------------------------------------------------------------
void SEParticleController::Link(SEStream& rStream, SEStream::Link* pLink)
{
    SEController::Link(rStream, pLink);
}
//----------------------------------------------------------------------------
bool SEParticleController::Register(SEStream& rStream) const
{
    return SEController::Register(rStream);
}
//----------------------------------------------------------------------------
void SEParticleController::Save(SEStream& rStream) const
{
    SE_BEGIN_DEBUG_STREAM_SAVE;

    SEController::Save(rStream);

    // native data
    rStream.Write(SystemLinearSpeed);
    rStream.Write(SystemAngularSpeed);
    rStream.Write(SystemLinearAxis);
    rStream.Write(SystemAngularAxis);
    rStream.Write(SystemSizeChange);

    // 把以下数据写入磁盘,从而不必等待被控制对象被装载和连接后,
    // 所造成的重新动态分配以下数据.
    rStream.Write(m_iLCount);
    if( m_iLCount > 0 )
    {
        rStream.Write(m_iLCount, m_afPointLinearSpeed);
        rStream.Write(m_iLCount, m_aPointLinearAxis);
        rStream.Write(m_iLCount, m_afPointSizeChange);
    }

    SE_END_DEBUG_STREAM_SAVE(SEParticleController);
}
//----------------------------------------------------------------------------
int SEParticleController::GetDiskUsed(const SEStreamVersion& rVersion) const
{
    int iSize = SEController::GetDiskUsed(rVersion) +
        sizeof(SystemLinearSpeed) +
        sizeof(SystemAngularSpeed) +
        sizeof(SystemLinearAxis) +
        sizeof(SystemAngularAxis) +
        sizeof(SystemSizeChange) +
        sizeof(m_iLCount);

    if( m_iLCount > 0 )
    {
        iSize += m_iLCount*sizeof(m_afPointLinearSpeed[0]);
        iSize += m_iLCount*sizeof(m_aPointLinearAxis[0]);
        iSize += m_iLCount*sizeof(m_afPointSizeChange[0]);
    }

    return iSize;
}
//----------------------------------------------------------------------------
SEStringTree* SEParticleController::SaveStrings(const char*)
{
    SEStringTree* pTree = SE_NEW SEStringTree;

    // strings
    pTree->Append(Format(&TYPE, GetName().c_str()));
    pTree->Append(Format("sys lin speed =", SystemLinearSpeed));
    pTree->Append(Format("sys ang speed =", SystemAngularSpeed));
    pTree->Append(Format("sys lin axis =", SystemLinearAxis));
    pTree->Append(Format("sys ang axis =", SystemAngularAxis));
    pTree->Append(Format("sys size change =", SystemSizeChange));

    // children
    pTree->Append(SEController::SaveStrings());

    pTree->Append(Format("point lin speed", m_iLCount,
        m_afPointLinearSpeed));

    pTree->Append(Format("point lin axis", m_iLCount,
        m_aPointLinearAxis));

    pTree->Append(Format("point size change", m_iLCount,
        m_afPointSizeChange));

    return pTree;
}
//----------------------------------------------------------------------------
