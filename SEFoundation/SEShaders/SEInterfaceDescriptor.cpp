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
#include "SEInterfaceDescriptor.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, InterfaceDescriptor, SEObject);
SE_IMPLEMENT_STREAM(InterfaceDescriptor);
SE_IMPLEMENT_DEFAULT_NAME_ID(InterfaceDescriptor, SEObject);

//SE_REGISTER_STREAM(InterfaceDescriptor);

//----------------------------------------------------------------------------
InterfaceDescriptor::InterfaceDescriptor()
{
}
//----------------------------------------------------------------------------
InterfaceDescriptor::~InterfaceDescriptor()
{
}
//----------------------------------------------------------------------------
void InterfaceDescriptor::AttachItem(DescriptorItem* pItem)
{
    SE_ASSERT( pItem );

    // �Ƿ���ڸ�item.
    for( int i = 0; i < (int)m_DescriptorItems.size(); i++ )
    {
        if( m_DescriptorItems[i] == pItem )
        {
            // �����򷵻�.
            return;
        }
    }

    m_DescriptorItems.push_back(pItem);
}
//----------------------------------------------------------------------------
void InterfaceDescriptor::DetachItem(DescriptorItem* pItem)
{
    SE_ASSERT( pItem );

    // �Ƿ���ڸ�item.
    std::vector<DescriptorItemPtr>::iterator pIter = m_DescriptorItems.begin();
    for( /**/ ; pIter != m_DescriptorItems.end(); pIter++ )
    {
        if( *pIter == pItem )
        {
            // ������ɾ��.
            m_DescriptorItems.erase(pIter);
            return;
        }
    }
}
//----------------------------------------------------------------------------
DescriptorItem* InterfaceDescriptor::GetItem(int i) const
{
    SE_ASSERT( i >= 0 && i < (int)m_DescriptorItems.size() );

    return m_DescriptorItems[i];
}
//----------------------------------------------------------------------------
void InterfaceDescriptor::GetDescription(std::string& rDesc) const
{
    for( int i = 0; i < (int)m_DescriptorItems.size(); i++ )
    {
        std::string tempItemDesc;
        m_DescriptorItems[i]->GetDescription(tempItemDesc);
        rDesc += tempItemDesc;
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// streaming
//----------------------------------------------------------------------------
void InterfaceDescriptor::Load(SEStream& rStream, SEStream::Link* pLink)
{
    SE_BEGIN_DEBUG_STREAM_LOAD;

    SEObject::Load(rStream, pLink);

    // link data
    int iCount;
    rStream.Read(iCount);
    m_DescriptorItems.resize(iCount);
    for( int i = 0; i < iCount; i++ )
    {
        SEObject* pObject;
        rStream.Read(pObject);  // m_DescriptorItems[i]
        pLink->Add(pObject);
    }

    SE_END_DEBUG_STREAM_LOAD(InterfaceDescriptor);
}
//----------------------------------------------------------------------------
void InterfaceDescriptor::Link(SEStream& rStream, SEStream::Link* pLink)
{
    SEObject::Link(rStream, pLink);

    for( int i = 0; i < (int)m_DescriptorItems.size(); i++ )
    {
        SEObject* pLinkID;
        pLinkID = pLink->GetLinkID();
        m_DescriptorItems[i] = (DescriptorItem*)rStream.GetFromMap(pLinkID);
    }
}
//----------------------------------------------------------------------------
bool InterfaceDescriptor::Register(SEStream& rStream) const
{
    if( !SEObject::Register(rStream) )
    {
        return false;
    }

    for( int i = 0; i < (int)m_DescriptorItems.size(); i++ )
    {
        if( m_DescriptorItems[i] )
        {
            m_DescriptorItems[i]->Register(rStream);
        }
    }

    return true;
}
//----------------------------------------------------------------------------
void InterfaceDescriptor::Save(SEStream& rStream) const
{
    SE_BEGIN_DEBUG_STREAM_SAVE;

    SEObject::Save(rStream);

    // link data
    int iCount = (int)m_DescriptorItems.size();
    rStream.Write(iCount);
    for( int i = 0; i < iCount; i++ )
    {
        rStream.Write(m_DescriptorItems[i]);
    }

    SE_END_DEBUG_STREAM_SAVE(InterfaceDescriptor);
}
//----------------------------------------------------------------------------
int InterfaceDescriptor::GetDiskUsed(const SEStreamVersion& rVersion) const
{
    int iSize = SEObject::GetDiskUsed(rVersion);

    int iCount = (int)m_DescriptorItems.size();
    iSize += sizeof(int) + iCount*sizeof(m_DescriptorItems[0]);

    return iSize;
}
//----------------------------------------------------------------------------
SEStringTree* InterfaceDescriptor::SaveStrings(const char*)
{
    SEStringTree* pTree = SE_NEW SEStringTree;

    // strings
    pTree->Append(Format(&TYPE, GetName().c_str()));

    // children
    pTree->Append(SEObject::SaveStrings());

    for( int i = 0; i < (int)m_DescriptorItems.size(); i++ )
    {
        pTree->Append(m_DescriptorItems[i]->SaveStrings());
    }

    return pTree;
}
//----------------------------------------------------------------------------