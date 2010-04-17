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

#ifndef Swing_Culler_H
#define Swing_Culler_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SECamera.h"
#include "SEPlane3.h"
#include "SESpatial.h"
#include "SEUnculledSet.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:������ͼ�����޳�����
// ˵��:
// ����:Sun Che
// ʱ��:20080707
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Culler
{
public:
    // ǰ�����������ڴ���Ǳ�ڿ�������������.
    // ���û�д���camera,�Ժ�Ӧ��ʹ��ComputeUnculledSet֮ǰ,
    // ͨ��SetCamera����camera.
    Culler(int iMaxCount = 0, int iGrowBy = 0, const Camera* pCamera = 0);
    virtual ~Culler(void);

    // ����camera,frustum copy,Ǳ�ڿ�������������.
    inline void SetCamera(const Camera* pCamera);
    inline const Camera* GetCamera(void) const;
    void SetFrustum(const float* afFrustum);
    inline const float* GetFrustum(void) const;
    inline UnculledSet& GetVisibleSet(void);
    inline UnculledSet& GetAudibleSet(void);

    // ����Ĭ����Ϊ�Ǹ��ݴ����������һ��UnculledObject,
    // ��������ӵ�UnculledObject����ĩβ.
    // ������������ش˺���,����UnculledObject������Ա�ά��Ϊ��������,
    // �Ӷ�ʹ��Ⱦ״̬�ı���С��,
    // ����Ҳ���Գ�Ϊһ������unique list��portalϵͳʹ��.
    virtual void Insert(Spatial* pObject, Effect* pGlobalEffect,
        bool bIsAudible = false);

    // ����ռ��޳�ƽ���ջ����.
    // �û������ڽ�Ͷ��ƽ�����������û��Զ���ƽ��,
    // PushPlane�����������������ռ�ƽ��.
    enum { US_MAX_PLANE_COUNT = 32 };
    inline int GetPlaneCount(void) const;
    inline const SEPlane3f* GetPlanes(void) const;
    inline void SetPlaneState(unsigned int uiPlaneState);
    inline unsigned int GetPlaneState(void) const;
    inline void PushPlane(const SEPlane3f& rPlane);
    inline void PopPlane(void);

    // �Ѷ��������ռ�BV�͸����޳�ƽ��Ƚ�,
    // ����¼����ƽ����֮�ȽϵĽ��,���޳�ϵͳ�ڳ����޳�����������ʹ��.
    // ֻ��Spatial����Ҫ�����������.
    bool IsInFrustum(const BoundingVolume* pBound);

    // ֧��Portal::GetUnculledSet.
    bool IsInFrustum(int iVertexCount, const SEVector3f* aVertex,
        bool bIgnoreNearPlane);

    // �Ѷ��������ռ�BV�͸����޳�ƽ��Ƚ�,
    // ֻ�жϸö����Ƿ���frustum��,����¼�ȽϽ��.
    // ֧��RoamTerrainPage::Simplify.
    bool IsSingleInFrustum(const BoundingVolume* pBound) const;

    // ֧��BspNode::GetUnculledSet.
    // �жϽ�Ͷ����ƽ���λ�����,
    // ��ƽ������ռ䷵��+1,����ռ䷵��-1,�ཻ����0.
    // ����ƽ��Ϊ����ռ�ƽ��.
    int OnWhichSide(const SEPlane3f& rPlane) const;

    // �޳�ϵͳ���.
    // ͨ����������ĳ�����ͼ��,����Ǳ�ڿ��������󼯺�.
    void ComputeUnculledSet(Spatial* pScene);

protected:
    // �޳�ϵͳ��Ҫ֪����ǰ������������Ϣ.
    const Camera* m_pCamera;

    // �����������Ͷ������Ŀ���ֵ.
    // �⽫���������ϵͳ���޳�������(����portalϵͳ),
    // �ı��Ͷ���������Ӱ�쵱ǰ�����.
    float m_Frustum[Camera::VF_COUNT];

    // ����ռ�ƽ������,����6����Ͷ��ƽ��,����26���û��Զ���ƽ��.
    // m_uiPlaneState��32λ��Ӧ32���޳�ƽ���״̬,
    // ״̬λΪ1ʱ������ƽ��Ӧ�ò����޳��Ƚ�,Ϊ0ʱ���ò���.
    // ��������Ŀ����������޳�ϵͳ��Ч��,
    // ����ĳ���ڵ��BV��ȫ����ĳƽ���һ����ռ���,
    // ��ɶ϶��������ӽڵ�Ҳ�����ڸ�ƽ��ĸð�ռ���,
    // �Ӷ������ӽڵ���ظ����޳��Ƚ�.
    int m_iPlaneCount;
    SEPlane3f m_aPlane[US_MAX_PLANE_COUNT];
    unsigned int m_uiPlaneState;

    // ����ComputeUnculledSet�������Ǳ�ڿ��Ӷ��󼯺�.
    UnculledSet m_VisibleSet;

    // ����ComputeUnculledSet�������Ǳ�ڿ������󼯺�.
    UnculledSet m_AudibleSet;
};

#include "SECuller.inl"

}

#endif
