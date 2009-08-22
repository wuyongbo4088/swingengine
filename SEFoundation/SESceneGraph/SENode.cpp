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
#include "SENode.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, Node, Spatial);
SE_IMPLEMENT_STREAM(Node);

//SE_REGISTER_STREAM(Node);

//----------------------------------------------------------------------------
Node::Node()
{
}
//----------------------------------------------------------------------------
Node::~Node()
{
    for( int i = 0; i < (int)m_Child.size(); i++ )
    {
        SpatialPtr spChild = DetachChildAt(i);
        spChild = 0;
    }
}
//----------------------------------------------------------------------------
int Node::AttachChild(Spatial* pChild)
{
    // һЩ������ϣ��node�ڵ��������ж�����ڵ�,�Ӷ�ʹ������ͼ��ΪDAG.
    // ʵ���ϲ�������,������ͼ�ṹ��һ�����ͽṹ.
    // ������Ե�ʹ��,ʹ��������ʶ����ǰ�ӽڵ��ԭ�����ӹ�ϵ��Ҫ���ƻ�,
    // Ϊ�˰�ȫ����,��Ӧ�����ȵ���DetachChild����ʹ�ӽڵ�����ԭ�����ӹ�ϵ,
    // ֮���ٵ���AttachChild��SetChild����.
    // ����һ��Ҫע�⵽,����DetachChildʱ�п���ʹ����ӽڵ����ü���Ϊ0�Ӷ���������,
    // ��ȫ��������,��һ������ָ�����ø��ӽڵ�,Ȼ�����DetachChild,������ʾ:
    //
    //     Node* pNode0 = SE_NEW Node;
    //     Spatial* pChild0 = <...>;
    //     pNode0->AttachChild(pChild0);  // child at index 0
    //     Node* pNode1 = <...>;
    //
    //     // �⽫���������,��ΪpChild0�Ѿ��и��ڵ�(pNode0).
    //     pNode1->AttachChild(pChild0);
    //
    //     // ȡ����֮����Ӧ������,ͬʱ��ֹ�ӽڵ���������.
    //     SpatialPtr spSaveChild = pNode0->GetChild(0);
    //     pNode0->DetachChild(spSaveChild);
    //     pNode1->AttachChild(spSaveChild);

    SE_ASSERT( pChild && !pChild->GetParent() );

    pChild->SetParent(this);

    // �Ƿ��п�slot
    int iCount = (int)m_Child.size();
    for( int i = 0; i < iCount; i++ )
    {
        if( m_Child[i] == 0 )
        {
            // �������
            m_Child[i] = pChild;

            return i;
        }
    }

    // ������β��
    m_Child.push_back(pChild);

    return iCount;
}
//----------------------------------------------------------------------------
int Node::DetachChild(Spatial* pChild)
{
    if( pChild )
    {
        // �Ƿ����
        for( int i = 0; i < (int)m_Child.size(); i++ )
        {
            if( m_Child[i] == pChild )
            {
                // �������Ƴ�
                pChild->SetParent(0);
                m_Child[i] = 0;

                return i;
            }
        }
    }

    return -1;
}
//----------------------------------------------------------------------------
SpatialPtr Node::DetachChildAt(int i)
{
    if( 0 <= i && i < (int)m_Child.size() )
    {
        SpatialPtr spChild = m_Child[i];
        if( spChild )
        {
            // �������Ƴ�
            spChild->SetParent(0);
            m_Child[i] = 0;
        }

        return spChild;
    }

    return 0;
}
//----------------------------------------------------------------------------
SpatialPtr Node::SetChild(int i, Spatial* pChild)
{
    // һЩ������ϣ��node�ڵ��������ж�����ڵ�,�Ӷ�ʹ������ͼ��ΪDAG.
    // ʵ���ϲ�������,������ͼ�ṹ��һ�����ͽṹ.
    // ������Ե�ʹ��,ʹ��������ʶ����ǰ�ӽڵ��ԭ�����ӹ�ϵ��Ҫ���ƻ�,
    // Ϊ�˰�ȫ����,��Ӧ�����ȵ���DetachChild����ʹ�ӽڵ�����ԭ�����ӹ�ϵ,
    // ֮���ٵ���AttachChild��SetChild����.
    // ����һ��Ҫע�⵽,����DetachChildʱ�п���ʹ����ӽڵ����ü���Ϊ0�Ӷ���������,
    // ��ȫ��������,��һ������ָ�����ø��ӽڵ�,Ȼ�����DetachChild,������ʾ:
    //
    //     Node* pNode0 = SE_NEW Node;
    //     Spatial* pChild0 = <...>;
    //     pNode0->AttachChild(pChild0);  // child at index 0
    //     Node* pNode1 = <...>;
    //
    //     // �⽫���������,��ΪpChild0�Ѿ��и��ڵ�(pNode0).
    //     pNode1->AttachChild(pChild0);
    //
    //     // ȡ����֮����Ӧ������,ͬʱ��ֹ�ӽڵ���������.
    //     SpatialPtr spSaveChild = pNode0->GetChild(0);
    //     pNode0->DetachChild(spSaveChild);
    //     pNode1->AttachChild(spSaveChild);

    if( pChild )
    {
        SE_ASSERT( !pChild->GetParent() );
    }

    if( 0 <= i && i < (int)m_Child.size() )
    {
        // �Ƴ�slot�о����ӽڵ�
        SpatialPtr spPreviousChild = m_Child[i];
        if( spPreviousChild )
        {
            spPreviousChild->SetParent(0);
        }

        // �����µ��ӽڵ㵽slot
        if( pChild )
        {
            pChild->SetParent(this);
        }

        m_Child[i] = pChild;

        return spPreviousChild;
    }

    // ����������Χ,�����β��
    pChild->SetParent(this);
    m_Child.push_back(pChild);

    return 0;
}
//----------------------------------------------------------------------------
SpatialPtr Node::GetChild(int i)
{
    if( 0 <= i && i < (int)m_Child.size() )
    {
        return m_Child[i];
    }

    return 0;
}
//----------------------------------------------------------------------------
void Node::UpdateWorldData(double dAppTime)
{
    Spatial::UpdateWorldData(dAppTime);

    // Node�ڵ������ν���AB�ݹ�,�Ӷ�������ı���.
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
void Node::UpdateWorldBound()
{
    if( !WorldBoundIsCurrent )
    {
        // ����һ��̰���㷨��BV�ϲ�,
        // ���ղ�����BV���������ӽڵ��BV,�������ܱ�֤����СBV,
        // �ܹ�������СBV���㷨ʱ�临�ӶȽϸ��Ҽ��临��,��˲������.
        bool bFoundFirstBound = false;
        for( int i = 0; i < (int)m_Child.size(); i++ )
        {
            Spatial* pChild = m_Child[i];
            if( pChild )
            {
                if( bFoundFirstBound )
                {
                    // �õ�ǰworld bound���ӽڵ�world bound�ϲ�
                    WorldBound->GrowToContain(pChild->WorldBound);
                }
                else
                {
                    // ���Ȱ�world bound����Ϊ��һ���ǿ��ӽڵ��world bound
                    bFoundFirstBound = true;
                    WorldBound->CopyFrom(pChild->WorldBound);
                }
            }
        }
    }
}
//----------------------------------------------------------------------------
void Node::UpdateState(std::vector<GlobalState*>* aGStack,
    std::vector<Light*>* pLStack)
{
    // Node�ڵ������ν���AB�ݹ�,�Ӷ�������ı���.
    for( int i = 0; i < (int)m_Child.size(); i++ )
    {
        Spatial* pChild = m_Child[i];
        if( pChild )
        {
            pChild->UpdateRS(aGStack, pLStack);
        }
    }
}
//----------------------------------------------------------------------------
void Node::GetUnculledSet(Culler& rCuller, bool bNoCull)
{
    int i;
    for( i = 0; i < (int)m_Effects.size(); i++ )
    {
        // ����һ��global effect.
        // �ڿɼ������м���һ��'��ʼ'���,����һ��global effect������Ŀ�ʼ.
        rCuller.Insert(this, m_Effects[i]);
    }

    // Node�ڵ������ν���AB�ݹ�,�Ӷ���ɳ�����ͼ���ı���.
    // ���б�����ɼ������е�Geometry�ӽڵ㶼���ڵ�ǰ�ڵ�global effect����������,
    // ��ЩGeometry�ӽڵ㶼��ʹ�ø�global effect������Ⱦ.
    for( i = 0; i < (int)m_Child.size(); i++ )
    {
        Spatial* pChild = m_Child[i];
        if( pChild )
        {
            pChild->OnGetUnculledSet(rCuller, bNoCull);
        }
    }

    for( i = 0; i < (int)m_Effects.size(); i++ )
    {
        // �ڿɼ������м���һ��'����'���,����һ��global effect������Ľ���.
        rCuller.Insert(0, 0);
    }
}
//----------------------------------------------------------------------------
void Node::DoPick(const Ray3f& rRay, PickArray& rResults)
{
    if( WorldBound->TestIntersection(rRay) )
    {
        for( int i = 0; i < (int)m_Child.size(); i++ )
        {
            Spatial* pChild = m_Child[i];
            if( pChild )
            {
                pChild->DoPick(rRay, rResults);
            }
        }
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// name and unique id
//----------------------------------------------------------------------------
Object* Node::GetObjectByName(const String& rName)
{
    Object* pFound = Spatial::GetObjectByName(rName);
    if( pFound )
    {
        return pFound;
    }

    for( int i = 0; i < (int)m_Child.size(); i++ )
    {
        Spatial* pChild = m_Child[i];
        if( pChild )
        {
            pFound = pChild->GetObjectByName(rName);
            if( pFound )
            {
                return pFound;
            }
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
void Node::GetAllObjectsByName(const String& rName,
    std::vector<Object*>& rObjects)
{
    Spatial::GetAllObjectsByName(rName, rObjects);

    for( int i = 0; i < (int)m_Child.size(); i++ )
    {
        Spatial* pChild = m_Child[i];
        if( pChild )
        {
            pChild->GetAllObjectsByName(rName, rObjects);
        }
    }
}
//----------------------------------------------------------------------------
Object* Node::GetObjectByID(unsigned int uiID)
{
    Object* pFound = Spatial::GetObjectByID(uiID);
    if( pFound )
    {
        return pFound;
    }

    for( int i = 0; i < (int)m_Child.size(); i++ )
    {
        Spatial* pChild = m_Child[i];
        if( pChild )
        {
            pFound = pChild->GetObjectByID(uiID);
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
void Node::Load(Stream& rStream, Stream::Link* pLink)
{
    SE_BEGIN_DEBUG_STREAM_LOAD;

    Spatial::Load(rStream, pLink);

    // link data
    int iMaxCount, iGrowBy, iCount;
    rStream.Read(iMaxCount);
    m_Child.resize(iMaxCount);

    // TO DO:  The growby parameter no longer exists, but to remove it
    // requires fixing the streaming system "IsValid" behavior.
    rStream.Read(iGrowBy);

    rStream.Read(iCount);
    for( int i = 0; i < iMaxCount; i++ )
    {
        Object* pObject;
        rStream.Read(pObject);
        pLink->Add(pObject);
    }

    SE_END_DEBUG_STREAM_LOAD(Node);
}
//----------------------------------------------------------------------------
void Node::Link(Stream& rStream, Stream::Link* pLink)
{
    Spatial::Link(rStream, pLink);

    for( int i = 0; i < (int)m_Child.size(); i++ )
    {
        Object* pLinkID = pLink->GetLinkID();
        Spatial* pChild = (Spatial*)rStream.GetFromMap(pLinkID);
        if( pChild )
        {
            SetChild(i, pChild);
        }
    }
}
//----------------------------------------------------------------------------
bool Node::Register(Stream& rStream) const
{
    if( !Spatial::Register(rStream) )
    {
        return false;
    }

    for( int i = 0; i < (int)m_Child.size(); i++ )
    {
        if( m_Child[i] )
        {
            m_Child[i]->Register(rStream);
        }
    }

    return true;
}
//----------------------------------------------------------------------------
void Node::Save(Stream& rStream) const
{
    SE_BEGIN_DEBUG_STREAM_SAVE;

    Spatial::Save(rStream);

    // link data
    rStream.Write((int)m_Child.size());  // was maxcount

    // TO DO: Delete these lines once streaming is fixed.
    int iGrowBy = 0;
    rStream.Write(iGrowBy);

    rStream.Write((int)m_Child.size());
    for( int i = 0; i < (int)m_Child.size(); i++ )
    {
        rStream.Write(m_Child[i]);
    }

    SE_END_DEBUG_STREAM_SAVE(Node);
}
//----------------------------------------------------------------------------
int Node::GetDiskUsed(const StreamVersion& rVersion) const
{
    return Spatial::GetDiskUsed(rVersion) +
        3*sizeof(int) +  // m_Child maxcount, growby, count
        ((int)m_Child.size())*sizeof(m_Child[0]);
}
//----------------------------------------------------------------------------
StringTree* Node::SaveStrings(const char*)
{
    StringTree* pTree = SE_NEW StringTree;

    // strings
    pTree->Append(Format(&TYPE, (const char*)GetName()));
    pTree->Append(Format("child count =", (int)m_Child.size()));

    // children
    pTree->Append(Spatial::SaveStrings());
    for( int i = 0; i < (int)m_Child.size(); i++ )
    {
        Spatial* pChild = m_Child[i];
        if( pChild )
        {
            pTree->Append(pChild->SaveStrings());
        }
        else
        {
            StringTree* pEmpty = SE_NEW StringTree;
            pEmpty->Append(Format("unused slot"));
            pTree->Append(pEmpty);
        }
    }

    return pTree;
}
//----------------------------------------------------------------------------
