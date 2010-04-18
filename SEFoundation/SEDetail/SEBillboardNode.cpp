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
#include "SEBillboardNode.h"
#include "SERenderer.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SEBillboardNode, SENode);
SE_IMPLEMENT_STREAM(SEBillboardNode);

//SE_REGISTER_STREAM(SEBillboardNode);

//----------------------------------------------------------------------------
SEBillboardNode::SEBillboardNode(SECamera* pCamera)
    :
    m_spCamera(pCamera)
{
}
//----------------------------------------------------------------------------
SEBillboardNode::~SEBillboardNode()
{
}
//----------------------------------------------------------------------------
void SEBillboardNode::UpdateWorldData(double dAppTime)
{
    // ����billboard��local���丸�ڵ��world�任������world�任.
    // ע�ⲻ�ܵ���SENode::UpdateWorldData,����ú��������µ�ǰ�ڵ���ӽڵ�.
    // �Ժ����Ǹ��������������ǰbillboard�ڵ���̬��,���ܸ�����Щ�ӽڵ�.
    SESpatial::UpdateWorldData(dAppTime);

    if( m_spCamera )
    {
        // �������location��任��billboard��ģ�Ϳռ�.
        SEVector3f vec3fCLoc;
        World.ApplyInverse(m_spCamera->GetLocation(), vec3fCLoc);

        // Ϊ�˶���billboard,�����������billboardģ�Ϳռ�xzƽ���ͶӰ,
        // �ܹ�����billboard�����ģ�Ϳռ�y�����ת�Ƕ�.
        // ����������ͶӰ��ģ�Ϳռ�y����(x = 0, z = 0),
        // ��ATan2����������0,���û��Ҫ�����ж��������.
        float fAngle = SEMathf::ATan2(vec3fCLoc.X, -vec3fCLoc.Z);
        SEMatrix3f mat3fOrient(SEVector3f::UNIT_Y, -fAngle);
        World.SetRotate(mat3fOrient*World.GetRotate());
    }

    // billboard�ı任�Ѿ�����camera����,�Ӷ������������ӽڵ�.
    for( int i = 0; i < (int)m_Child.size(); i++ )
    {
        SESpatial* pChild = m_Child[i];
        if( pChild )
        {
            pChild->UpdateGS(dAppTime, false);
        }
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// name and unique id
//----------------------------------------------------------------------------
SEObject* SEBillboardNode::GetObjectByName(const std::string& rName)
{
    SEObject* pFound = SEObject::GetObjectByName(rName);
    if( pFound )
    {
        return pFound;
    }

    if( m_spCamera )
    {
        pFound = m_spCamera->GetObjectByName(rName);
        if( pFound )
        {
            return pFound;
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
void SEBillboardNode::GetAllObjectsByName(const std::string& rName,
    std::vector<SEObject*>& rObjects)
{
    SENode::GetAllObjectsByName(rName, rObjects);

    if( m_spCamera )
    {
        m_spCamera->GetAllObjectsByName(rName, rObjects);
    }
}
//----------------------------------------------------------------------------
SEObject* SEBillboardNode::GetObjectByID(unsigned int uiID)
{
    SEObject* pFound = SEObject::GetObjectByID(uiID);
    if( pFound )
    {
        return pFound;
    }

    if( m_spCamera )
    {
        pFound = m_spCamera->GetObjectByID(uiID);
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
void SEBillboardNode::Load(SEStream& rStream, SEStream::Link* pLink)
{
    SE_BEGIN_DEBUG_STREAM_LOAD;

    SENode::Load(rStream, pLink);

    // link data
    SEObject* pObject;
    rStream.Read(pObject);  // m_spCamera
    pLink->Add(pObject);

    SE_END_DEBUG_STREAM_LOAD(SEBillboardNode);
}
//----------------------------------------------------------------------------
void SEBillboardNode::Link(SEStream& rStream, SEStream::Link* pLink)
{
    SENode::Link(rStream, pLink);

    SEObject* pLinkID = pLink->GetLinkID();
    m_spCamera = (SECamera*)rStream.GetFromMap(pLinkID);
}
//----------------------------------------------------------------------------
bool SEBillboardNode::Register(SEStream& rStream) const
{
    if( !SENode::Register(rStream) )
    {
        return false;
    }

    if( m_spCamera )
    {
        m_spCamera->Register(rStream);
    }

    return true;
}
//----------------------------------------------------------------------------
void SEBillboardNode::Save(SEStream& rStream) const
{
    SE_BEGIN_DEBUG_STREAM_SAVE;

    SENode::Save(rStream);

    // link data
    rStream.Write(m_spCamera);

    SE_END_DEBUG_STREAM_SAVE(SEBillboardNode);
}
//----------------------------------------------------------------------------
int SEBillboardNode::GetDiskUsed(const SEStreamVersion& rVersion) const
{
    return SENode::GetDiskUsed(rVersion) +
        sizeof(m_spCamera);
}
//----------------------------------------------------------------------------
SEStringTree* SEBillboardNode::SaveStrings(const char*)
{
    SEStringTree* pTree = SE_NEW SEStringTree;

    // strings
    pTree->Append(Format(&TYPE, GetName().c_str()));

    // children
    pTree->Append(SENode::SaveStrings());

    if( m_spCamera )
    {
        pTree->Append(m_spCamera->SaveStrings());
    }

    return pTree;
}
//----------------------------------------------------------------------------
