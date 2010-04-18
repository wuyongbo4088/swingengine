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

namespace Swing
{

class SECuller;
class SELight;

//----------------------------------------------------------------------------
// ˵��:
// ����:Sun Che
// ʱ��:20080707
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SESpatial : public SEObject
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    // �����
    virtual ~SESpatial(void);

    // Local��World�任.
    // ĳЩ�������Ҳ����Ҫ����SESpatial::UpdateGS()ģ��Ӷ�ֱ������world�任,
    // ��ʱӦ����WorldIsCurrentΪtrue.
    // ����IK controller��Skin controller����Ҫ��������.
    SETransformation Local;
    SETransformation World;
    bool WorldIsCurrent;

    // ����ռ�BV.
    // ĳЩ�������Ҳ����Ҫ����SESpatial::UpdateGS()ģ��Ӷ�ֱ����������ռ�BV,
    // ��ʱӦ����WorldBoundIsCurrentΪtrue.
    SEBoundingVolumePtr WorldBound;
    bool WorldBoundIsCurrent;

    // ע��:
    // SESpatial����Ҫ����ģ�Ϳռ�BV.

    // �޳�ģʽ
    enum CullingMode
    {
        // ��������ռ�BV������ռ��޳�ƽ��,���ж�̬�޳�.
        CULL_DYNAMIC,

        // ǿ���޳�,�����һ��SENode�ڵ�,�������������������޳�.
        CULL_ALWAYS,

        // ǿ�Ʋ��޳�,�����һ��SENode�ڵ�,�������������������޳�.
        // ���״�������SENode�ڵ��,bNoCull����������Ϊtrue,
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
    void UpdateGS(double dAppTime = -SEMathd::MAX_REAL,
        bool bInitiator = true);
    void UpdateBS(void);

    // global state
    inline int GetGlobalStateCount(void) const;
    inline SEGlobalState* GetGlobalState(int i) const;
    SEGlobalState* GetGlobalState(SEGlobalState::StateType eType) const;
    void AttachGlobalState(SEGlobalState* pState);
    void DetachGlobalState(SEGlobalState::StateType eType);
    inline void DetachAllGlobalStates(void);

    // light state
    inline int GetLightCount(void) const;
    inline SELight* GetLight(int i) const;
    void AttachLight(SELight* pLight);
    void DetachLight(SELight* pLight);
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
    virtual void UpdateRS(std::vector<SEGlobalState*>* aGStack = 0,
        std::vector<SELight*>* pLStack = 0);

    // ���ʸ��ڵ�.
    inline SESpatial* GetParent(void);

    // Picking system.
    // ÿ��������SESpatial������඼���Խ�һ�������������Ӧ��SEPickRecord����.
    // ����ЩSEPickRecord������,�ɼ����κ���������Ҫ����Ϣ,DoPick��������ʱ��ȡ.
    // ��DoPick�������ر�ʾȫ���ཻ���SEPickRecord�����,
    // ÿ��SEPickRecord�����߲���t����Ϊ���������key,�Ӷ���ӳ���ɽ���Զ���ཻ˳��.
    //
    // SEPickRecord�౾����Ȼ���߱�RTTI����,
    // �����ǿ��Դ����ԱIObject�����ӻ�ȡ��RTTI��Ϣ.
    // ֻҪ����֪����IObject�ľ�������,
    // ���SEPickRecord���Ա���̬����ת��Ϊ��Ӧ��SEPickRecord������.
    class SE_FOUNDATION_API SEPickRecord
    {
    public:
        virtual ~SEPickRecord(void);

        // �������ཻ�Ķ���.
        SESmartPointer<SESpatial> IObject;

        // �ཻ�㴦,�������ߵĲ���tֵ,�Ǹ�.
        // ��,�������������P + t*D,��t >= 0.
        float T;

    protected:
        SEPickRecord(SESpatial* pIObject, float fT);
    };

    typedef std::vector<SEPickRecord*> PickArray;

    // ���ߵ�ԭ��ͷ�����������������������ϵ��.
    // Ӧ�ó������ͷ�PickArray�е�����pick record,
    // ��Ϊ��Щpick record������ȫ���ڴ�صĶ��ڴ�.
    virtual void DoPick(const SERay3f& rRay, PickArray& rResults);

    static SEPickRecord* GetClosest(PickArray& rResults);

protected:
    SESpatial(void);

    // �������ݸ���.
    virtual void UpdateWorldData(double dAppTime); // ������SENodeʵ�����½���AB�ݹ�
    virtual void UpdateWorldBound(void) = 0; // ���ϼ�������ռ�BV,������ʵ�־�����Ϊ
    void PropagateBoundToRoot(void);

    // ��Ⱦ״̬����.
    void PropagateStateFromRoot(std::vector<SEGlobalState*>* aGStack,
        std::vector<SELight*>* pLStack);
    void PushState(std::vector<SEGlobalState*>* aGStack,
        std::vector<SELight*>* pLStack);
    void PopState(std::vector<SEGlobalState*>* aGStack,
        std::vector<SELight*>* pLStack);
    // ������SENodeʵ�����½���AB�ݹ�,
    // ������SEGeometryʵ���ռ�������Ⱦ״̬.
    virtual void UpdateState(std::vector<SEGlobalState*>* aGStack,
        std::vector<SELight*>* pLStack) = 0;

    SESpatial* m_pParent;

    // ȫ����Ⱦ״̬����.
    std::vector<SEGlobalStatePtr> m_GlobalStates;

    // �ƹ�����.
    std::vector<SEObjectPtr> m_Lights; // ʹ��SEObjectPtr����ͷ�ļ������������

    // effect����.
    // ����SEGeometry����ʱ,���������ڸö���,
    // ����SENode����ʱ,�����ڸ�SENode��������SEGeometry����.
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
    inline void SetParent(SESpatial* pParent);

    // �޳�ϵͳ��AB�ݹ麯��.
    void OnGetUnculledSet(SECuller& rCuller, bool bNoCull);
    virtual void GetUnculledSet(SECuller& rCuller, bool bNoCull) = 0;
};

typedef SESmartPointer<SESpatial> SpatialPtr;

#include "SESpatial.inl"

}

#endif
