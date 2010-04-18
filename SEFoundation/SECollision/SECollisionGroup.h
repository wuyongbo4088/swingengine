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

#ifndef Swing_CollisionGroup_H
#define Swing_CollisionGroup_H

#include "SEFoundationLIB.h"
#include "SESystem.h"

namespace Swing
{

class SECollisionRecord;

//----------------------------------------------------------------------------
// Description:
// Author:Sun Che
// Date:20081223
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SECollisionGroup
{
public:
    SECollisionGroup(void);
    ~SECollisionGroup(void);

    // SECollisionGroup������Ϊ������ɾ�����������collision record.
    // ��˴����collision recordӦ���Ƕ�̬�����.
    bool Add(SECollisionRecord* pRecord);
    bool Remove(SECollisionRecord* pRecord);

    // Intersection��ѯ.
    // �����ײ���е�������������ײ,�������Ӧ��record�����������Ϣ.

    // ���Ƚ϶��󱻼���Ϊ��ֹ״̬(�ٶȱ�����),���ж��󱻳ɶԽ��бȽ�.
    void TestIntersection(void);
    void FindIntersection(void);
    
    // ���Ƚ϶��󱻼���Ϊ�˶�״̬.���ж���Я���ٶ�����ʱ(������������),
    // ���ٶȵĶ������������󱻳ɶԽ��бȽ�.
    void TestIntersection(float fTMax);
    void FindIntersection(float fTMax);

protected:
    std::vector<SECollisionRecord*> m_Record;
};

}

#endif
