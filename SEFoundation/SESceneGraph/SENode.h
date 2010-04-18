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

#ifndef Swing_Node_H
#define Swing_Node_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SESpatial.h"
#include "SECuller.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:������ͼ��ϵ�ڵ����
// ˵��:
// ����:Sun Che
// ʱ��:20080723
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SENode : public SESpatial
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    SENode(void);
    virtual ~SENode(void);

    // �ӽڵ����.
    inline int GetCount(void) const;
    int AttachChild(SESpatial* pChild);
    int DetachChild(SESpatial* pChild);
    SpatialPtr DetachChildAt(int i);
    SpatialPtr SetChild(int i, SESpatial* pChild);
    SpatialPtr GetChild(int i);

    // Picking support.
    // ���ߵ�ԭ��ͷ�����������������������ϵ��.
    // Ӧ�ó��������ε����ͷ�PickArray�е�����pick record.
    virtual void DoPick(const SERay3f& rRay, PickArray& rResults);

protected:
    // �������ݸ���
    virtual void UpdateWorldData(double dAppTime);
    virtual void UpdateWorldBound(void);

    // ��Ⱦ״̬����
    virtual void UpdateState(std::vector<SEGlobalState*>* aGStack,
        std::vector<SELight*>* pLStack);

    // ֧���޳�ϵͳ
    virtual void GetUnculledSet(SECuller& rCuller, bool bNoCull);

    // �ӽڵ�����
    std::vector<SpatialPtr> m_Child;
};

typedef SESmartPointer<SENode> SENodePtr;

#include "SENode.inl"

}

#endif
