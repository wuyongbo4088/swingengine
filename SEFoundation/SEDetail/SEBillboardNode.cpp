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

SE_IMPLEMENT_RTTI(Swing, BillboardNode, Node);
SE_IMPLEMENT_STREAM(BillboardNode);

//SE_REGISTER_STREAM(BillboardNode);

//----------------------------------------------------------------------------
BillboardNode::BillboardNode(Camera* pCamera)
    :
    m_spCamera(pCamera)
{
}
//----------------------------------------------------------------------------
BillboardNode::~BillboardNode()
{
}
//----------------------------------------------------------------------------
void BillboardNode::UpdateWorldData(double dAppTime)
{
    // ����billboard��local���丸�ڵ��world�任������world�任.
    // ע�ⲻ�ܵ���Node::UpdateWorldData,����ú��������µ�ǰ�ڵ���ӽڵ�.
    // �Ժ����Ǹ��������������ǰbillboard�ڵ���̬��,���ܸ�����Щ�ӽڵ�.
    Spatial::UpdateWorldData(dAppTime);

    if( m_spCamera )
    {
        // �������location��任��billboard��ģ�Ϳռ�.
        Vector3f vec3fCLoc;
        World.ApplyInverse(m_spCamera->GetLocation(), vec3fCLoc);

        // Ϊ�˶���billboard,�����������billboardģ�Ϳռ�xzƽ���ͶӰ,
        // �ܹ�����billboard�����ģ�Ϳռ�y�����ת�Ƕ�.
        // ����������ͶӰ��ģ�Ϳռ�y����(x = 0, z = 0),
        // ��ATan2����������0,���û��Ҫ�����ж��������.
        float fAngle = Mathf::ATan2(vec3fCLoc.X, -vec3fCLoc.Z);
        Matrix3f mat3fOrient(Vector3f::UNIT_Y, -fAngle);
        World.SetRotate(mat3fOrient*World.GetRotate());
    }

    // billboard�ı任�Ѿ�����camera����,�Ӷ������������ӽڵ�.
    for( int i = 0; i < (int)m_Child.size(); i++ )
    {
        Spatial* pChild = m_Child[i];
        if( pChild )
        {
            pChild->UpdateGS(dAppTime, false);
        }
    }
}
//----------------------------------------------------------------------------
ObjectPtr BillboardNode::Clone()
{
    // TODO:
    // Impliment this function.
    return 0;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// name and unique id
//----------------------------------------------------------------------------
Object* BillboardNode::GetObjectByName(const std::string& rName)
{
    Object* pFound = Object::GetObjectByName(rName);
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
void BillboardNode::GetAllObjectsByName(const std::string& rName,
    std::vector<Object*>& rObjects)
{
    Node::GetAllObjectsByName(rName, rObjects);

    if( m_spCamera )
    {
        m_spCamera->GetAllObjectsByName(rName, rObjects);
    }
}
//----------------------------------------------------------------------------
Object* BillboardNode::GetObjectByID(unsigned int uiID)
{
    Object* pFound = Object::GetObjectByID(uiID);
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
void BillboardNode::Load(Stream& rStream, Stream::Link* pLink)
{
    SE_BEGIN_DEBUG_STREAM_LOAD;

    Node::Load(rStream, pLink);

    // link data
    Object* pObject;
    rStream.Read(pObject);  // m_spCamera
    pLink->Add(pObject);

    SE_END_DEBUG_STREAM_LOAD(BillboardNode);
}
//----------------------------------------------------------------------------
void BillboardNode::Link(Stream& rStream, Stream::Link* pLink)
{
    Node::Link(rStream, pLink);

    Object* pLinkID = pLink->GetLinkID();
    m_spCamera = (Camera*)rStream.GetFromMap(pLinkID);
}
//----------------------------------------------------------------------------
bool BillboardNode::Register(Stream& rStream) const
{
    if( !Node::Register(rStream) )
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
void BillboardNode::Save(Stream& rStream) const
{
    SE_BEGIN_DEBUG_STREAM_SAVE;

    Node::Save(rStream);

    // link data
    rStream.Write(m_spCamera);

    SE_END_DEBUG_STREAM_SAVE(BillboardNode);
}
//----------------------------------------------------------------------------
int BillboardNode::GetDiskUsed(const StreamVersion& rVersion) const
{
    return Node::GetDiskUsed(rVersion) +
        sizeof(m_spCamera);
}
//----------------------------------------------------------------------------
StringTree* BillboardNode::SaveStrings(const char*)
{
    StringTree* pTree = SE_NEW StringTree;

    // strings
    pTree->Append(Format(&TYPE, GetName().c_str()));

    // children
    pTree->Append(Node::SaveStrings());

    if( m_spCamera )
    {
        pTree->Append(m_spCamera->SaveStrings());
    }

    return pTree;
}
//----------------------------------------------------------------------------
