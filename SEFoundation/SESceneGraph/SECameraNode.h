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

#ifndef Swing_CameraNode_H
#define Swing_CameraNode_H

#include "SEFoundationLIB.h"
#include "SECamera.h"
#include "SENode.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:camera node��
// ˵��:
// ����:Sun Che
// ʱ��:20090224
//----------------------------------------------------------------------------
class SE_FOUNDATION_API CameraNode : public Node
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    // ���������.
    // �ڵ������ƽ�Ʊ任������Ϊcamera��E��.
    // �ڵ��������ת�任������Ϊcamera��R,U,D����.
    // ����,������ת�����Row0��R��,Row1��U��,Row2��D��.
    //
    // �ڹ���ʱ,�ڵ�ľֲ��任������Ϊcamera�ĵ�ǰ������ϵ.
    //   local translation    = camera location
    //   local rotation row 0 = camera right
    //   local rotation row 1 = camera up
    //   local rotation row 2 = camera direction
    CameraNode(Camera* pCamera = 0);
    virtual ~CameraNode(void);

    // ������cameraʱ,�ڵ�ľֲ��任������Ϊcamera�ĵ�ǰ������ϵ.
    // �ڵ������任���������,Ȼ��camera��������ϵ������Ϊʹ�ýڵ������任.
    void SetCamera(Camera* pCamera);

    inline Camera* GetCamera(void);
    inline const Camera* GetCamera(void) const;

protected:
    // geometric updates
    virtual void UpdateWorldData(double dAppTime);

    CameraPtr m_spCamera;
};

typedef SmartPointer<CameraNode> CameraNodePtr;

#include "SECameraNode.inl"

}

#endif
