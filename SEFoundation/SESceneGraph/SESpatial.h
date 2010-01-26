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

#ifndef Swing_Spatial_H
#define Swing_Spatial_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEObject.h"
#include "SEBoundingVolume.h"
#include "SEEffect.h"
#include "SEGlobalState.h"
#include "SETransformation.h"
#include "SECloneable.h"

namespace Swing
{

class Culler;
class Light;

//----------------------------------------------------------------------------
// ����:������ͼ�ռ�ڵ������
// ˵��:
// ����:Sun Che
// ʱ��:20080707
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Spatial : public Object, public Cloneable
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    // �����
    virtual ~Spatial(void);

    // Local��World�任.
    // ��ĳЩ�������Ҳ����Ҫ����Spatial::UpdateGS()ģ��Ӷ�ֱ������world�任,
    // ��ʱӦ����WorldIsCurrentΪtrue.
    // ����IK controller��Skin controller����Ҫ��������.
    Transformation Local;
    Transformation World;
    bool WorldIsCurrent;

    // ����ռ�BV.
    // ��ĳЩ�������Ҳ����Ҫ����Spatial::UpdateGS()ģ��Ӷ�ֱ����������ռ�BV,
    // ��ʱӦ����WorldBoundIsCurrentΪtrue.
    BoundingVolumePtr WorldBound;
    bool WorldBoundIsCurrent;

    // ע��:
    // Spatial����Ҫ����ģ�Ϳռ�BV.

    // �޳�ģʽ
    enum CullingMode
    {
        // ��������ռ�BV������ռ��޳�ƽ��,���ж�̬�޳�.
        CULL_DYNAMIC,

        // ǿ���޳�,�����һ��Node�ڵ�,�������������������޳�.
        CULL_ALWAYS,

        // ǿ�Ʋ��޳�,�����һ��Node�ڵ�,�������������������޳�.
        // ���״�������Node�ڵ��,bNoCull����������Ϊtrue,
        // �����ݸ�GetUnculledSet/OnGetUnculledSet AB�ݹ���,
        // �Ӷ�ʹ�ݹ�·���ϵ������ӽڵ㶼���ᱻ�޳�.
        CULL_NEVER,

        MAX_CULLING_MODE
    };

    CullingMode Culling;

    // Geometric state��Controller�������.
    // UpdateGS�������¼���ÿ���ڵ������任,���ϼ�������BV.
    // UpdateBS����ֻ���ϼ�������BV,��ģ�����ݸı�ʱ,ֻ��ı�ģ��BV������BV,
    // �������¼���ռ�任.
    void UpdateGS(double dAppTime = -Mathd::MAX_REAL,
        bool bInitiator = true);
    void UpdateBS(void);

    // global state
    inline int GetGlobalStateCount(void) const;
    inline GlobalState* GetGlobalState(int i) const;
    GlobalState* GetGlobalState(GlobalState::StateType eType) const;
    void AttachGlobalState(GlobalState* pState);
    void DetachGlobalState(GlobalState::StateType eType);
    inline void DetachAllGlobalStates(void);

    // light state
    inline int GetLightCount(void) const;
    inline Light* GetLight(int i) const;
    void AttachLight(Light* pLight);
    void DetachLight(Light* pLight);
    inline void DetachAllLights(void);

    // effect state
    inline int GetEffectCount(void) const;
    inline Effect* GetEffect(int i) const;
    void AttachEffect(Effect* pEffect);
    void DetachEffect(Effect* pEffect);
    inline void DetachAllEffects(void);
    inline void SetStartEffect(int i);
    inline int GetStartEffect(void) const;

    // Render state�������.
    virtual void UpdateRS(std::vector<GlobalState*>* aGStack = 0,
        std::vector<Light*>* pLStack = 0);

    // ���ʸ��ڵ�.
    inline Spatial* GetParent(void);

    // Picking system.
    // ÿ��������spatial������඼���Խ�һ�������������Ӧ��PickRecord����.
    // ����ЩPickRecord������,�ɼ����κ���������Ҫ����Ϣ,DoPick��������ʱ��ȡ.
    // ��DoPick�������ر�ʾȫ���ཻ���PickRecord�����,
    // ÿ��PickRecord�����߲���t����Ϊ���������key,�Ӷ���ӳ���ɽ���Զ���ཻ˳��.
    //
    // PickRecord�౾����Ȼ���߱�RTTI����,
    // �����ǿ��Դ����ԱIObject�����ӻ�ȡ��RTTI��Ϣ.
    // ֻҪ����֪����IObject�ľ�������,
    // ���PickRecord���Ա���̬����ת��Ϊ��Ӧ��PickRecord������.
    class SE_FOUNDATION_API PickRecord
    {
    public:
        virtual ~PickRecord(void);

        // �������ཻ�Ķ���.
        SmartPointer<Spatial> IObject;

        // �ཻ�㴦,�������ߵĲ���tֵ,�Ǹ�.
        // ��,�������������P + t*D,��t >= 0.
        float T;

    protected:
        PickRecord(Spatial* pIObject, float fT);
    };

    typedef std::vector<PickRecord*> PickArray;

    // ���ߵ�ԭ��ͷ�����������������������ϵ��.
    // Ӧ�ó������ͷ�PickArray�е�����pick record,
    // ��Ϊ��Щpick record������ȫ���ڴ�صĶ��ڴ�.
    virtual void DoPick(const Ray3f& rRay, PickArray& rResults);

    static PickRecord* GetClosest(PickArray& rResults);

protected:
    Spatial(void);

    // �������ݸ���.
    virtual void UpdateWorldData(double dAppTime); // ������Nodeʵ�����½���AB�ݹ�
    virtual void UpdateWorldBound(void) = 0; // ���ϼ�������ռ�BV,������ʵ�־�����Ϊ
    void PropagateBoundToRoot(void);

    // ��Ⱦ״̬����.
    void PropagateStateFromRoot(std::vector<GlobalState*>* aGStack,
        std::vector<Light*>* pLStack);
    void PushState(std::vector<GlobalState*>* aGStack,
        std::vector<Light*>* pLStack);
    void PopState(std::vector<GlobalState*>* aGStack,
        std::vector<Light*>* pLStack);
    // ������Nodeʵ�����½���AB�ݹ�,
    // ������Geometryʵ���ռ�������Ⱦ״̬.
    virtual void UpdateState(std::vector<GlobalState*>* aGStack,
        std::vector<Light*>* pLStack) = 0;

    Spatial* m_pParent;

    // ȫ����Ⱦ״̬����.
    std::vector<GlobalStatePtr> m_GlobalStates;

    // �ƹ�����.
    std::vector<ObjectPtr> m_Lights; // ʹ��ObjectPtr����ͷ�ļ������������

    // effect����.
    // ����Geometry����ʱ,���������ڸö���,
    // ����Node����ʱ,�����ڸ�Node��������Geometry����.
    std::vector<EffectPtr> m_Effects;

    // ͨ�������,���������ڶ����effect������Ӧ���ڸö���.
    // ����ʱΪ������һЩeffect,������Ҫһ����ʼ����ֵ��Ӧ�ó���ʹ��,
    // ���ڸ��ӵ�effectЧ��,�⽫��ǳ�����,�ܹ����Ե���ǰ����Щeffect,
    // ֻʹ��ĳ���ض�effect��Ⱦ����.
    // ���û�������ʼ����ֵ,�ͱ����Ƴ���ǰ����effect����ʱ����,
    // Ȼ������ض�effect����Ⱦ,֮���Ƴ��ض�effect,����ٴλָ�֮ǰ������effect.
    // ͨ����ʼ����ֵ,�Ϳ�������ض�effect��effect����ĩβ,
    // Ȼ��ָ����λ��Ϊ��ʼӦ��λ��,��Ⱦ��,������ʼλ��Ϊ0,���Ƴ����ض�effect.
    int m_iStartEffect;

private:
    static const char* ms_aacCullingMode[MAX_CULLING_MODE];

// �ڲ�ʹ��
public:
    inline void SetParent(Spatial* pParent);

    // �޳�ϵͳ��AB�ݹ麯��.
    void OnGetUnculledSet(Culler& rCuller, bool bNoCull);
    virtual void GetUnculledSet(Culler& rCuller, bool bNoCull) = 0;
};

typedef SmartPointer<Spatial> SpatialPtr;

#include "SESpatial.inl"

}

#endif
