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
#include "SECollisionGroup.h"
#include "SECollisionRecord.h"

using namespace Swing;

//----------------------------------------------------------------------------
CollisionGroup::CollisionGroup()
{
}
//----------------------------------------------------------------------------
CollisionGroup::~CollisionGroup()
{
    for( int i = 0; i < (int)m_Record.size(); i++ )
    {
        SE_DELETE m_Record[i];
    }
}
//----------------------------------------------------------------------------
bool CollisionGroup::Add(CollisionRecord* pRecord)
{
    for( int i = 0; i < (int)m_Record.size(); i++ )
    {
        if( pRecord == m_Record[i] )
        {
            return false;
        }
    }

    m_Record.push_back(pRecord);

    return true;
}
//----------------------------------------------------------------------------
bool CollisionGroup::Remove(CollisionRecord* pRecord)
{
    std::vector<CollisionRecord*>::iterator tempIter = m_Record.begin();
    for( /**/; tempIter != m_Record.end(); tempIter++ )
    {
        if( pRecord == *tempIter )
        {
            m_Record.erase(tempIter);
            SE_DELETE pRecord;

            return true;
        }
    }

    return false;
}
//----------------------------------------------------------------------------
void CollisionGroup::TestIntersection()
{
    // ���ж��󱻼���Ϊ��ֹ״̬,�ɶԽ��бȽ�.
    for( int i0 = 0; i0 < (int)m_Record.size(); i0++ )
    {
        CollisionRecord* pRecord0 = m_Record[i0];
        for( int i1 = i0 + 1; i1 < (int)m_Record.size(); i1++ )
        {
            CollisionRecord* pRecord1 = m_Record[i1];
            pRecord0->TestIntersection(*pRecord1);
        }
    }
}
//----------------------------------------------------------------------------
void CollisionGroup::FindIntersection()
{
    // ���ж��󱻼���Ϊ��ֹ״̬,�ɶԽ��бȽ�.
    for( int i0 = 0; i0 < (int)m_Record.size(); i0++ )
    {
        CollisionRecord* pRecord0 = m_Record[i0];
        for( int i1 = i0 + 1; i1 < (int)m_Record.size(); i1++ )
        {
            CollisionRecord* pRecord1 = m_Record[i1];
            pRecord0->FindIntersection(*pRecord1);
        }
    }
}
//----------------------------------------------------------------------------
void CollisionGroup::TestIntersection(float fTMax)
{
    // ���ж��󱻼���Ϊ�˶�״̬,�ɶԽ��бȽ�.
    for( int i0 = 0; i0 < (int)m_Record.size(); i0++ )
    {
        CollisionRecord* pRecord0 = m_Record[i0];
        for( int i1 = i0 + 1; i1 < (int)m_Record.size(); i1++ )
        {
            CollisionRecord* pRecord1 = m_Record[i1];
            if( pRecord0->GetVelocity() || pRecord1->GetVelocity() )
                pRecord0->TestIntersection(fTMax, *pRecord1);
        }
    }
}
//----------------------------------------------------------------------------
void CollisionGroup::FindIntersection(float fTMax)
{
    // ���ж��󱻼���Ϊ�˶�״̬,�ɶԽ��бȽ�.
    for( int i0 = 0; i0 < (int)m_Record.size(); i0++ )
    {
        CollisionRecord* pRecord0 = m_Record[i0];
        for( int i1 = i0 + 1; i1 < (int)m_Record.size(); i1++ )
        {
            CollisionRecord* pRecord1 = m_Record[i1];
            if( pRecord0->GetVelocity() || pRecord1->GetVelocity() )
                pRecord0->FindIntersection(fTMax, *pRecord1);
        }
    }
}
//----------------------------------------------------------------------------
