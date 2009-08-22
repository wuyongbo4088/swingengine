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

#ifndef Swing_LightNode_H
#define Swing_LightNode_H

#include "SEFoundationLIB.h"
#include "SELight.h"
#include "SENode.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:light node��
// ˵��:
// ����:Sun Che
// ʱ��:20090224
//----------------------------------------------------------------------------
class SE_FOUNDATION_API LightNode : public Node
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    // ���������.
    // �ڵ������ƽ�Ʊ任������Ϊlight��λ�õ�.
    // �ڵ��������ת�任������Ϊlight��R,U,D����.
    // ����,������ת�����Row0��R��,Row1��U��,Row2��D��.
    //
    // �ڹ���ʱ,�ڵ�ľֲ��任������Ϊlight�ĵ�ǰ������ϵ.
    //   local translation       = light location
    //   local rotation row 0 = light right
    //   local rotation row 1 = light up
    //   local rotation row 2 = light direction
    LightNode(Light* pLight = 0);
    virtual ~LightNode(void);

    // ������lightʱ,�ڵ�ľֲ��任������Ϊlight�ĵ�ǰ������ϵ.
    // �ڵ������任���������,Ȼ��light��������ϵ������Ϊʹ�ýڵ������任.
    void SetLight(Light* pLight);

    inline Light* GetLight(void);
    inline const Light* GetLight(void) const;

protected:
    // geometric updates
    virtual void UpdateWorldData(double dAppTime);

    LightPtr m_spLight;
};

typedef SmartPointer<LightNode> LightNodePtr;

#include "SELightNode.inl"

}

#endif
