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

#ifndef Swing_Geometry_H
#define Swing_Geometry_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SESpatial.h"
#include "SEVertexBuffer.h"
#include "SEIndexBuffer.h"
#include "SELightingEffect.h"
#include "SERenderStateBlock.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:������ͼ������ڵ������
// ˵��:һЩ������ϣ��Geometry�ڵ��������ж�����ڵ�,�Ӷ�ʹ������ͼ��ΪDAG.
//      ʵ���ϲ�������,������ͼ�ṹ��һ�����ͽṹ.
//      Geometry�༰����������ֻ����Ϊ������ͼ����Ҷ�ӽڵ�.
//      Geometry��Ҳ�ǿ���Ⱦ�ڵ�������.
// ����:Sun Che
// ʱ��:20080723
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Geometry : public Spatial
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    // �����.
    virtual ~Geometry(void);

    // Geometry������ʹ�õ�lighting����.
    // GLM_PIPELINE_VERTEX��GLM_PIPELINE_PIXEL������LightingEffect�е�
    // LM_VERTEX��LM_PIXELö��˳�򱣳�һ��.
    // GLM_USER��ʾ�û�ʹ���Զ���lighting,�Ӷ����û�����UpdateRS����ʱ,
    // Geometry���󽫲��ᴴ��ϵͳĬ�ϵ�LightingEffect����.
    // GLM_DISABLE��ʾ��Geometry�����ֹ�κι���Ч��.
    enum GeometryLightingMode
    {
        GLM_PIPELINE_VERTEX,
        GLM_PIPELINE_PIXEL,
        GLM_USER,
        GLM_DISABLE,
        GLM_MAX_COUNT
    };

    // ��Ա����.
    VertexBufferPtr VBuffer;
    IndexBufferPtr IBuffer;
    BoundingVolumePtr ModelBound;
    GeometryLightingMode LightingMode; // default: GLM_PIPELINE_PIXEL

    // ���������ݸ������.
    virtual void UpdateMS(bool bUpdateNormals = true);

    // Picking support.
    // ������������ά������������������.
    class SE_FOUNDATION_API PickRecord : public Spatial::PickRecord
    {
    protected:
        PickRecord(Geometry* pIObject, float fT);
    };

protected:
    Geometry(void);
    Geometry(VertexBuffer* pVBuffer, IndexBuffer* pIBuffer);

    // ���������ݸ���.
    virtual void UpdateModelBound(void);
    virtual void UpdateModelNormals(void);
    virtual void UpdateWorldBound(void);

    // ��Ⱦ״̬����,ʵ�ֻ���ӿ�.
    virtual void UpdateState(std::vector<GlobalState*>* aGStack,
        std::vector<Light*>* pLStack);

    // ֧���޳�ϵͳ,�ռ�����Ⱦ����.
    virtual void GetUnculledSet(Culler& rCuller, bool bNoCull);

    // ���߶�̬����.
    // ���߹���Ч��������Spatial��effect���鲢��Ϊ��Ԫ��,
    // ��˹��߹�����Ⱦ����������effect֮ǰ.
    // LightingEffect���ɸ��ڵ㵽�ﵱǰ������ڵ�ʱ,
    // ���ռ���������Light��������պϲ���ʽ.
    LightingEffectPtr m_spLEffect;

// �ڲ�ʹ��
public:
    // ������ڵ��render state block,��ʾӰ��ýڵ������state,
    // ��UpdateRSʱ�ռ����ڵ㵽�ﵱǰ������ڵ�����ж�ջ������Ⱦ״̬.
    // ע��:
    // ����ǰ�ڵ���Я����effect�ṩ����������Ⱦ��ǰ�ڵ��������Ⱦ״̬,
    // light�Լ�������Ϣ.

    // ������ڵ��lights,��ʾӰ��ýڵ������light����,
    // ��UpdateRSʱ�ռ����ڵ㵽�ﵱǰ������ڵ��light��ջ�е�Ԫ��.

    enum GeometryType
    {
        GT_POLYPOINT,
        GT_POLYLINE_SEGMENTS,
        GT_POLYLINE_OPEN,
        GT_POLYLINE_CLOSED,
        GT_TRIMESH,
        GT_MAX_COUNT
    };

    // ��ǰ����ͼԪ����.
    GeometryType Type;

    // ͨ��UpdateRS�����ռ�����Ӱ�쵱ǰ������ڵ��render states.
    RenderStateBlockPtr RStateBlock;

    // ͨ��UpdateRS�����ռ�����Ӱ�쵱ǰ������ڵ��lights.
    std::vector<LightPtr> Lights;
};

typedef SmartPointer<Geometry> GeometryPtr;

#include "SEGeometry.inl"

}

#endif
