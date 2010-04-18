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

#include "SEMax8SceneBuilder.h"
#include "decomp.h"
#include <algorithm>

using namespace std;
using namespace Swing;

//----------------------------------------------------------------------------
bool Max8SceneBuilder::AreEqual(const Point3& rPoint1, const Point3& rPoint2)
{   
    // ��ָ����ȷ���µ�ģ���Ƚ�.

    return fabsf(rPoint1.x - rPoint2.x) <= MIN_DIFFERENCE
        && fabsf(rPoint1.y - rPoint2.y) <= MIN_DIFFERENCE
        && fabsf(rPoint1.z - rPoint2.z) <= MIN_DIFFERENCE;
}
//----------------------------------------------------------------------------
bool Max8SceneBuilder::AreEqual(const Quat& rQuat1, const Quat& rQuat2)
{
    // ��ָ����ȷ���µ�ģ���Ƚ�.

    return fabsf(rQuat1.x - rQuat2.x) <= MIN_DIFFERENCE
        && fabsf(rQuat1.y - rQuat2.y) <= MIN_DIFFERENCE
        && fabsf(rQuat1.z - rQuat2.z) <= MIN_DIFFERENCE
        && fabsf(rQuat1.w - rQuat2.w) <= MIN_DIFFERENCE;
}
//----------------------------------------------------------------------------
bool Max8SceneBuilder::CompareKeyTimes(KeyInfo* pFirst, KeyInfo* pSecond)
{
    return pFirst->Time < pSecond->Time;
}
//----------------------------------------------------------------------------
void Max8SceneBuilder::BuildKeyFrameController(INode* pMaxNode,
    SESpatial* pSENode)
{
    // Ϊһ���߱��ؼ��嶯���Ľڵ����һ��key frame controller.
    // ע�����ǲ�֧�ַ�ͳһ���ŵĶ���.
    //
    // pMaxNode:
    //     �߱��ؼ��嶯����Max�ڵ�ָ��.
    // pSENode:
    //     ������ӹؼ��嶯����Swing Engine������ͼ�ڵ�ָ��.

    assert( pSENode );
    assert( pMaxNode );

    // ����Max�ڵ��Ƿ���йؼ��嶯��.
    Control* pTMC = pMaxNode->GetTMController();

    assert( pTMC );

    if( !pTMC->IsKeyable() )
    {
        // �ڵ㲻�߱��ؼ��嶯��,���Կ��ܺ��ж�����Ϣ,
        // �����п�����һ��IK�ڵ�.
        // �����Ļ�,�����ڵ㶯����������.
        BuildFrameController(pMaxNode, pSENode);

        return;
    }

    // �����Ż���Ŀ��,���ýڵ��ƽ��,��ת�������Ƿ�������ѡ���������Ϸ����˱仯.
    AnimationTiming tempTrnTiming, tempRotTiming, tempScaTiming;
    if( !GetAnimationTiming(pMaxNode, tempTrnTiming, tempRotTiming, tempScaTiming) )
    {
        return;
    }

    vector<KeyInfo*> tempKeyInfo;

    // ����ƽ�ƹؼ���
    int iNumTrnKeys = 0;
    if( tempTrnTiming.Active )
    {
        Control* pPC = pTMC->GetPositionController();
        if( pPC )
        {		
            IKeyControl* pKeyCon = GetKeyControlInterface(pPC);
            if( pKeyCon )
            {
                iNumTrnKeys = pKeyCon->GetNumKeys();
                GetTrnKeyInfo(iNumTrnKeys, pPC->ClassID(), pKeyCon, tempTrnTiming,
                    tempKeyInfo);
            }
        }
    }

    // ������ת�ؼ���
    int iNumRotKeys = 0;
    if( tempRotTiming.Active )
    {
        Control* pRC = pTMC->GetRotationController();
        if( pRC )
        {
            IKeyControl* pKeyCon = GetKeyControlInterface(pRC);
            if( pKeyCon )
            {
                iNumRotKeys = pKeyCon->GetNumKeys();
                GetRotKeyInfo(iNumRotKeys, pRC->ClassID(), pKeyCon, tempRotTiming,
                    tempKeyInfo);
            }
        }
    }

    // �������Źؼ���
    int iNumScaKeys = 0;
    if( tempScaTiming.Active )
    {
        Control* pSC = pTMC->GetScaleController();
        if( pSC )
        {
            IKeyControl* pKeyCon = GetKeyControlInterface(pSC);
            if( pKeyCon )
            {
                iNumScaKeys = pKeyCon->GetNumKeys();
                GetScaleKeyInfo(iNumScaKeys, pSC->ClassID(), pKeyCon, tempScaTiming,
                    tempKeyInfo);
            }
        }
    }

    // ���һ������ʹ���˳��������,��û��ʹ�ùؼ��������,
    // ���������е�GetKeyControlInterface�������᷵��NULL,
    // ��Ϊ�����������û��"�ؼ���"�ĸ���.
    // �����Ļ�,����û�йؼ�����Ϣ.
    if( !iNumTrnKeys && !iNumRotKeys && !iNumScaKeys )
    {
        BuildFrameController(pMaxNode, pSENode);

        return;
    }

    // ���ݻ�õ�Max�ؼ�����Ϣ,������Ӧ��Swing Engine�ؼ�������������ռ�.
    SEKeyframeController* pSEKFC = new SEKeyframeController;
    if( iNumTrnKeys > 0 )
    {
        pSEKFC->TranslationTimes = new SEFloatArray(iNumTrnKeys,
            new float[iNumTrnKeys]);
        pSEKFC->TranslationData = new SEVector3fArray(iNumTrnKeys,
            new SEVector3f[iNumTrnKeys]);
    }
    if( iNumRotKeys > 0 )
    {
        pSEKFC->RotationTimes = new SEFloatArray(iNumRotKeys,
            new float[iNumRotKeys]);
        pSEKFC->RotationData = new SEQuaternionfArray(iNumRotKeys,
            new SEQuaternionf[iNumRotKeys]);
    }
    if( iNumScaKeys > 0 )
    {
        pSEKFC->ScaleTimes = new SEFloatArray(iNumScaKeys,
            new float[iNumScaKeys]);
        pSEKFC->ScaleData = new SEFloatArray(iNumScaKeys,
            new float[iNumScaKeys]);
    }

    // ����ʱ��ֵ������������.
    sort(tempKeyInfo.begin(), tempKeyInfo.end(), CompareKeyTimes);

    SEVector3f* pTKey = (pSEKFC->TranslationData ? 
        pSEKFC->TranslationData->GetData() : NULL);
    float* pfTTime = (pSEKFC->TranslationTimes ?
        pSEKFC->TranslationTimes->GetData() : NULL);

    SEQuaternionf* pRKey = (pSEKFC->RotationData ?
        pSEKFC->RotationData->GetData() : NULL);
    float* pfRTime = (pSEKFC->RotationTimes ?
        pSEKFC->RotationTimes->GetData() : NULL);

    float* pfSKey = (pSEKFC->ScaleData ?
        pSEKFC->ScaleData->GetData() : NULL);
    float* pfSTime = (pSEKFC->ScaleTimes ?
        pSEKFC->ScaleTimes->GetData() : NULL);

    TimeValue iTimeNow = -1;
    SETransformation tempTransform;
    for( int i = 0; i < (int)tempKeyInfo.size(); i++ )
    {
        KeyInfo* pInfo = tempKeyInfo[i];

        if( iTimeNow != pInfo->Time )
        {
            iTimeNow = pInfo->Time;
            tempTransform = GetLocalTransform(pMaxNode, iTimeNow);
        }

        switch( pInfo->Type )
        {
        case KT_TRANSLATION:
            assert( pTKey );
            assert( pfTTime );

            *pTKey++ = tempTransform.GetTranslate();
            *pfTTime++ = TicksToSec(iTimeNow - m_iTimeOffset);

            break;
        case KT_ROTATION:
            // ��ʵ��.
            // ��δ֧�ַ�SR�任.
            if( tempTransform.IsSRMatrix() )
            {
                assert( pRKey );
                assert( pfRTime );

                // �����.
                // q��-q��ʾͬ������ת�任.
                // ��Ϊ�˱���q��next q֮�����С�н�,���ʹ�䱣�ַ���һ��.
                pRKey->FromRotationMatrix(tempTransform.GetRotate());
                if( pRKey->W < 0.0f )
                {
                    *pRKey = -*pRKey;
                }
                pRKey++;
                *pfRTime++ = TicksToSec(iTimeNow - m_iTimeOffset);
            }

            break;
        case KT_SCALE:
        {
            // ��ʵ��.
            // ��δ֧�ַ�ͳһ���ű任������ϵ���任.
            float fMax;
            if( tempTransform.IsUniformScale() )
            {
                fMax = tempTransform.GetUniformScale();
            }
            else
            {
                fMax = SEMathf::FAbs(tempTransform.GetScale().X);
                float fAbs = SEMathf::FAbs(tempTransform.GetScale().Y);
                if( fAbs > fMax )
                {
                    fMax = fAbs;
                }
                fAbs = SEMathf::FAbs(tempTransform.GetScale().Z);
                if( fAbs > fMax )
                {
                    fMax = fAbs;
                }
            }

            assert( pfSKey );
            assert( pfSTime );

            *pfSKey++ = fMax;
            *pfSTime++ = TicksToSec(iTimeNow - m_iTimeOffset);

            break;
        }
        }

        delete pInfo;
    }

    pSEKFC->Repeat = SEController::RT_WRAP;
    pSEKFC->MinTime = TicksToSec(m_iTimeStart - m_iTimeOffset);
    pSEKFC->MaxTime = TicksToSec(m_iTimeEnd - m_iTimeOffset);
    pSENode->AttachController(pSEKFC);
}
//----------------------------------------------------------------------------
void Max8SceneBuilder::BuildFrameController(INode* pNode,
    SESpatial* pSENode)
{
    // Ϊһ���߱�������Ϣ�Ľڵ����һ��"�������".
    // ������������ýڵ��������,���������ǹؼ���.
    // Ϊ�ýڵ���������SEKeyframeController���иýڵ�ÿ���SRT�任��Ϣ.
    // ����Ȼ�������ŵ�,��Swing Engine������,�ýڵ����ڵ�����任֮����ȻҪ����ֵ,
    // Ȼ��,�ڶ���ʦʹ��IK����ʱ,����������,���ܵ�����Ŀǰ����֧��IK������.
    // ע�����ǲ�֧�ַ�ͳһ���ű任�Ķ���.
    //
    // pNode:
    //     ���ж�����Ϣ��Max�ڵ�ָ��.
    // pSENode:
    //     Swing Engine������ͼ�еĴ��ж�����Ϣ�Ľڵ�.

    // �洢����Ψһ��������(ʹ��ģ���Ƚ�).
    vector<int> tempTTime, tempRTime, tempSTime;
    vector<Point3> tempTData, tempSData;
    vector<Quat> tempRData;

    // ��ȡ��ʼ�任����.
    // ÿ����̵ı任��������֮ǰ�ı任�������бȽ�,�Ӷ��ж��Ƿ����˱仯.
    // ��������˱仯,���µı任�����������б�.������Ե�.
    // ���,���һ������ֻ��һ��Ԫ��,�򲻴����κ�keyframe����.
    int iTime = m_iTimeStart;
    Matrix3 tempLocal = pNode->GetNodeTM(iTime) * Inverse(pNode->GetParentTM(iTime));
    AffineParts tempAffParts;
    decomp_affine(tempLocal, &tempAffParts);
    tempTTime.push_back(iTime);
    tempRTime.push_back(iTime);
    tempSTime.push_back(iTime);
    tempTData.push_back(tempAffParts.t);
    tempRData.push_back(tempAffParts.q);
    tempSData.push_back(tempAffParts.k);

    // ��ʵ��.
    // �ж�tempAffParts.f�Ƿ�Ϊ��.
    // ���ʾ������ϵ������任.

    // ������������������.
    for( iTime += m_iTicksPerFrame; iTime <= m_iTimeEnd; iTime += m_iTicksPerFrame )
    {
        tempLocal = pNode->GetNodeTM(iTime) * Inverse(pNode->GetParentTM(iTime));
        decomp_affine(tempLocal, &tempAffParts);

        if( !AreEqual(tempAffParts.t, tempTData.back()) )
        {
            tempTTime.push_back(iTime);
            tempTData.push_back(tempAffParts.t);
        }

        if( !AreEqual(tempAffParts.q, tempRData.back()) )
        {
            // q��-q��ʾͬ������ת�任.
            if( !AreEqual(-tempAffParts.q, tempRData.back()) )
            {
                tempRTime.push_back(iTime);

                // ���Ǵ�����������Ԫ��,�Ӷ�ʹ����֮��ļнǱ�����С��.
                float fDot = tempAffParts.q % tempRData.back();
                if( fDot > 0.0f )
                    tempRData.push_back(tempAffParts.q);
                else
                    tempRData.push_back(-tempAffParts.q);
            }
        }

        if( !AreEqual(tempAffParts.k, tempSData.back()) )
        {
            tempSTime.push_back(iTime);
            tempSData.push_back(tempAffParts.f*tempAffParts.k);
        }

        // �ж��Ƿ�Ϊͳһ���ű任.
        bool bScaleIsUniform =
            fabsf(tempAffParts.k.x - tempAffParts.k.y) < MIN_DIFFERENCE &&
            fabsf(tempAffParts.k.y - tempAffParts.k.z) < MIN_DIFFERENCE;

        // �ж�����ϵ���Ƿ�Ϊ1.
        bool bScaleIsUnity = bScaleIsUniform &&
            fabsf(tempAffParts.k.x - 1.0f) < MIN_DIFFERENCE;

        // �ж�scale-orientation�Ƿ�Ϊ��λ�任.
        if( tempAffParts.u.w < 0.0f )
            tempAffParts.u = -tempAffParts.u;

        bool bOrientIsIdentity = bScaleIsUniform || (
            fabsf(tempAffParts.u.w - 1.0f) < MIN_DIFFERENCE &&
            fabsf(tempAffParts.u.x) < MIN_DIFFERENCE &&
            fabsf(tempAffParts.u.y) < MIN_DIFFERENCE &&
            fabsf(tempAffParts.u.z) < MIN_DIFFERENCE);

        int iStopHere = 0;
    }

    if( tempTData.size() == 1 && tempRData.size() == 1 && tempSData.size() == 1 )
    {
        // ����ڵ�û�ж�����Ϣ
        return;
    }

    // ��ʵ��.
    // �ж��Ƿ�����ȫ��ͬ������,�Ӷ����ǿ��Ա�����.

    SEKeyframeController* pSEKFCtrl = new SEKeyframeController;
    pSEKFCtrl->MinTime = TicksToSec(m_iTimeStart - m_iTimeOffset);
    pSEKFCtrl->MaxTime = TicksToSec(m_iTimeEnd - m_iTimeOffset);
    pSEKFCtrl->Repeat = SEController::RT_WRAP;

    if( tempTData.size() > 1 )
    {
        int iNumTKeys = (int)tempTData.size();
        float* afTTime = new float[iNumTKeys];
        SEVector3f* aTData = new SEVector3f[iNumTKeys];
        for( int i = 0; i < iNumTKeys; i++ )
        {
            afTTime[i] = TicksToSec(tempTTime[i] - m_iTimeOffset);
            aTData[i].X = tempTData[i].x;
            aTData[i].Y = tempTData[i].z;
            aTData[i].Z = tempTData[i].y;
        }

        pSEKFCtrl->TranslationTimes = new SEFloatArray(iNumTKeys, afTTime);
        pSEKFCtrl->TranslationData = new SEVector3fArray(iNumTKeys, aTData);
    }

    if( tempRData.size() > 1 )
    {
        int iNumRKeys = (int)tempRData.size();
        float* afRTime = new float[iNumRKeys];
        SEQuaternionf* aRData = new SEQuaternionf[iNumRKeys];
        for( int i = 0; i < iNumRKeys; i++ )
        {
            afRTime[i] = TicksToSec(tempRTime[i] - m_iTimeOffset);
            aRData[i].W = tempRData[i].w;
            aRData[i].X = +tempRData[i].x;
            aRData[i].Y = +tempRData[i].z;
            aRData[i].Z = +tempRData[i].y;

            // DEBUGGING
            SEVector3f tempAxis;
            float fAngle;
            aRData[i].ToAxisAngle(tempAxis, fAngle);
            int iStopHere = 0;
        }

        pSEKFCtrl->RotationTimes = new SEFloatArray(iNumRKeys, afRTime);
        pSEKFCtrl->RotationData = new SEQuaternionfArray(iNumRKeys, aRData);
    }

    if( tempSData.size() > 1 )
    {
        int iNumSKeys = (int)tempSData.size();
        float* afSTime = new float[iNumSKeys];
        float* afSData = new float[iNumSKeys];
        for( int i = 0; i < iNumSKeys; i++ )
        {
            afSTime[i] = TicksToSec(tempSTime[i] - m_iTimeOffset);
            afSData[i] = tempSData[i].x;
        }

        pSEKFCtrl->ScaleTimes = new SEFloatArray(iNumSKeys, afSTime);
        pSEKFCtrl->ScaleData = new SEFloatArray(iNumSKeys, afSData);
    }

    pSENode->AttachController(pSEKFCtrl);
}
//----------------------------------------------------------------------------
bool Max8SceneBuilder::GetAnimationTiming(INode* pNode,
    AnimationTiming& rTTiming, AnimationTiming& rRTiming,
    AnimationTiming& rSTiming)
{
    // ͨ�����һ���ڵ���ָ��ʱ���������Ƿ����˽ϴ�ı任(����С���),
    // �Ӷ������ýڵ��Ƿ��ж�����Ϣ.

    int iTicksPerFrame = GetTicksPerFrame();
    Matrix3 tempNodeTM;
    AffineParts tempAffParts;
    Point3 tempPrevTrans, tempRotAxis, tempPrevRotAxis, tempPrevScaleFactor;
    float fRotAngle, fPrevRotAngle;
    rTTiming.Active = FALSE;
    rRTiming.Active = FALSE;
    rSTiming.Active = FALSE;

    // ��ȡ�ڵ�ĳ�ʼ�任������.
    tempNodeTM = pNode->GetNodeTM(m_iTimeStart) * Inverse(pNode->GetParentTM(
        m_iTimeStart));
    decomp_affine(tempNodeTM, &tempAffParts);
    AngAxisFromQ(tempAffParts.q, &fPrevRotAngle, tempPrevRotAxis);
    tempPrevTrans = tempAffParts.t;
    tempPrevScaleFactor = tempAffParts.k;

    // ѭ������������,ȷ�Ͻڵ�ĳ�任�����Ƿ����ı�.
    TimeValue iTimeNow = m_iTimeStart + iTicksPerFrame;
    for( /**/; iTimeNow <= m_iTimeEnd; iTimeNow += iTicksPerFrame )
    {
        tempNodeTM = pNode->GetNodeTM(iTimeNow) * Inverse(pNode->GetParentTM(iTimeNow));
        decomp_affine(tempNodeTM, &tempAffParts);
        AngAxisFromQ(tempAffParts.q, &fRotAngle, tempRotAxis);

        if( !rTTiming.Active )
        {
            // �Ƚϵ�ǰ���֮ǰ���ƽ�Ʒ����Ƿ����.
            if( !AreEqual(tempAffParts.t, tempPrevTrans) )
            {
                // ƽ�Ʒ����ı�,��¼�ı�ǰ����ʱ��.
                rTTiming.Active = TRUE;
                rTTiming.Start = iTimeNow - iTicksPerFrame;
                rTTiming.End = iTimeNow;
            }
        }
        else
        {
            // ���ƽ�Ʒ������ڸı�,�����ƽ�ƶ����Ľ���ʱ��.
            if( !AreEqual(tempAffParts.t, tempPrevTrans) )
            {
                rTTiming.End = iTimeNow;
            }
        }

        if( !rRTiming.Active )
        {
            // �Ƚϵ�ǰ���֮ǰ�����ת�����Ƿ����.
            if( fabsf(fRotAngle - fPrevRotAngle) > MIN_DIFFERENCE
            ||   !AreEqual(tempRotAxis, tempPrevRotAxis) )
            {
                // ��ת�����ı�,��¼�ı�ǰ����ʱ��.
                rRTiming.Active = TRUE;
                rRTiming.Start = iTimeNow - iTicksPerFrame;
                rRTiming.End = iTimeNow;
            }
        }
        else
        {
            // �����ת�������ڸı�,�������ת�����Ľ���ʱ��.
            if( fabsf(fRotAngle - fPrevRotAngle) > MIN_DIFFERENCE
            ||   !AreEqual(tempRotAxis, tempPrevRotAxis) )
            {
                rRTiming.End = iTimeNow;
            }
        }

        if( !rSTiming.Active )
        {
            // �Ƚϵ�ǰ���֮ǰ������ŷ����Ƿ����.
            if( !AreEqual(tempAffParts.k, tempPrevScaleFactor) )
            {
                // ���ŷ����ı�,��¼�ı�ǰ����ʱ��.
                rSTiming.Active = TRUE;
                rSTiming.Start = iTimeNow - iTicksPerFrame;
                rSTiming.End = iTimeNow;
            }
        }
        else
        {
            // ������ŷ������ڸı�,��������Ŷ����Ľ���ʱ��.
            if( !AreEqual(tempAffParts.k, tempPrevScaleFactor) )
            {
                rSTiming.End = iTimeNow;
            }
        }

        fPrevRotAngle = fRotAngle;
        tempPrevRotAxis = tempRotAxis;
        tempPrevTrans = tempAffParts.t;
        tempPrevScaleFactor = tempAffParts.k;
    }

    return rTTiming.Active || rRTiming.Active || rSTiming.Active;
}
//----------------------------------------------------------------------------
void Max8SceneBuilder::GetTrnKeyInfo(int& riNumKeys, Class_ID& rClassID, 
    IKeyControl* pKeyCon, AnimationTiming& rTTiming,
    vector<KeyInfo*>& rKeyInfo)
{       
    // ��Max�ڵ��ƽ�Ʊ任�ؼ����������ȡƽ�Ʊ任�ؼ���.
    //
    // riNumKeys:
    //     ��Ϊ�������ʱ,����Ҫ����Ĺؼ�����Ŀ.
    //     ��Ϊ�������ʱ,���������Swing Engine�ؼ�����Ŀ.
    // class_ID:
    //     ƽ�Ʊ任�ؼ����������Max class ID.
    // pKeyCon:
    //     Max�ڵ��ƽ�Ʊ任�ؼ��������ָ��.
    // rTTiming:
    //     �ڵ��ƽ�ƶ�������ʱ�����ϸ��Ϣ.
    // rKeyInfo:
    //     KeyInfoָ�����������洢�ؼ������ͺ�ʱ����Ϣ.

    int i, iKeys = riNumKeys;
    riNumKeys = 0;

    if( rClassID == Class_ID(TCBINTERP_POSITION_CLASS_ID, 0) )
    {
        // ����Tension/Continuity/Bias������
        ITCBPoint3Key tempKey;
        for( i = 0; i < iKeys; i++ )
        {
            pKeyCon->GetKey(i, &tempKey);
            if( tempKey.time < rTTiming.Start )
            {
                continue;
            }
            else if( tempKey.time > rTTiming.End )
            {
                break;
            }
    
            rKeyInfo.push_back(new KeyInfo(tempKey.time, KT_TRANSLATION));
            riNumKeys++;
        }
    }
    else if( rClassID == Class_ID(HYBRIDINTERP_POSITION_CLASS_ID, 0) )
    {
        // ����Bezier������
        IBezPoint3Key tempKey;
        for( i = 0; i < iKeys; i++ )
        {
            pKeyCon->GetKey(i, &tempKey);
            if( tempKey.time < rTTiming.Start )
            {
                continue;
            }
            else if( tempKey.time > rTTiming.End )
            {
                break;
            }

            rKeyInfo.push_back(new KeyInfo(tempKey.time, KT_TRANSLATION));
            riNumKeys++;
        }
    }
    else if( rClassID == Class_ID(LININTERP_POSITION_CLASS_ID, 0) )
    {
        // ����Linear������
        ILinPoint3Key tempKey;
        for( int i = 0; i < iKeys; i++ )
        {
            pKeyCon->GetKey(i, &tempKey);
            if( tempKey.time < rTTiming.Start )
            {
                continue;
            }
            else if( tempKey.time > rTTiming.End )
            {
                break;
            }

            rKeyInfo.push_back(new KeyInfo(tempKey.time, KT_TRANSLATION));
            riNumKeys++;
        }   
    }
}
//----------------------------------------------------------------------------
void Max8SceneBuilder::GetRotKeyInfo(int& riNumKeys, Class_ID& rClassID, 
    IKeyControl* pKeyCon, AnimationTiming& rRTiming,
    vector<KeyInfo*>& rKeyInfo)
{
    // ��Max�ڵ����ת�任�ؼ����������ȡ��ת�任�ؼ���.
    //
    // riNumKeys:
    //     ��Ϊ�������ʱ,����Ҫ����Ĺؼ�����Ŀ.
    //     ��Ϊ�������ʱ,���������Swing Engine�ؼ�����Ŀ.
    // class_ID:
    //     ��ת�任�ؼ����������Max class ID.
    // pKeyCon:
    //     Max�ڵ����ת�任�ؼ��������ָ��.
    // rRTiming:
    //     �ڵ����ת��������ʱ�����ϸ��Ϣ.
    // rKeyInfo:
    //     KeyInfoָ�����������洢�ؼ������ͺ�ʱ����Ϣ.

    int i, iKeys = riNumKeys;
    riNumKeys = 0;

    if( rClassID == Class_ID(TCBINTERP_ROTATION_CLASS_ID, 0) )
    {
        // ����Tension/Continuity/Bias������
        ITCBRotKey tempKey;
        for( i = 0; i < iKeys; i++ )
        {
            pKeyCon->GetKey(i, &tempKey);
            if( tempKey.time < rRTiming.Start )
            {
                continue;
            }
            else if( tempKey.time > rRTiming.End )
            {
                break;
            }
            
            rKeyInfo.push_back(new KeyInfo(tempKey.time, KT_ROTATION));
            riNumKeys++;
        }
    }
    else if( rClassID == Class_ID(HYBRIDINTERP_ROTATION_CLASS_ID, 0) )
    {
        // ����Bezier������
        IBezQuatKey tempKey;
        for( i = 0; i < iKeys; i++ )
        {
            pKeyCon->GetKey(i, &tempKey);
            if( tempKey.time < rRTiming.Start )
            {
                continue;
            }
            else if( tempKey.time > rRTiming.End )
            {
                break;
            }

            rKeyInfo.push_back(new KeyInfo(tempKey.time, KT_ROTATION));
            riNumKeys++;
        }
    }
    else if( rClassID == Class_ID(LININTERP_ROTATION_CLASS_ID, 0) )
    {
        // ����Linear������
        ILinRotKey tempKey;
        for( i = 0; i < iKeys; i++ )
        {
            pKeyCon->GetKey(i, &tempKey);
            if( tempKey.time < rRTiming.Start )
            {
                continue;
            }
            else if( tempKey.time > rRTiming.End )
            {
                break;
            }

            rKeyInfo.push_back(new KeyInfo(tempKey.time, KT_ROTATION));
            riNumKeys++;
        }   
    }
}
//----------------------------------------------------------------------------
void Max8SceneBuilder::GetScaleKeyInfo(int& riNumKeys, Class_ID& rClassID,
    IKeyControl* pKeyCon, AnimationTiming& rSTiming,
    vector<KeyInfo*>& rKeyInfo)
{
    // ��Max�ڵ�����ű任�ؼ����������ȡ���ű任�ؼ���.
    //
    // riNumKeys:
    //     ��Ϊ�������ʱ,����Ҫ����Ĺؼ�����Ŀ.
    //     ��Ϊ�������ʱ,���������Swing Engine�ؼ�����Ŀ.
    // class_ID:
    //     ���ű任�ؼ����������Max class ID.
    // pKeyCon:
    //     Max�ڵ�����ű任�ؼ��������ָ��.
    // rTTiming:
    //     �ڵ�����Ŷ�������ʱ�����ϸ��Ϣ.
    // rKeyInfo:
    //     KeyInfoָ�����������洢�ؼ������ͺ�ʱ����Ϣ.

    int i, iKeys = riNumKeys;
    riNumKeys = 0;

    if( rClassID == Class_ID(TCBINTERP_SCALE_CLASS_ID, 0) )
    {
        // ����Tension/Continuity/Bias������
        ITCBScaleKey tempKey;
        for( i = 0; i < iKeys; i++ )
        {
            pKeyCon->GetKey(i, &tempKey);
            if( tempKey.time < rSTiming.Start )
            {
                continue;
            }
            else if( tempKey.time > rSTiming.End )
            {
                break;
            }

            rKeyInfo.push_back(new KeyInfo(tempKey.time, KT_SCALE));
            riNumKeys++;
        }
    }
    else if( rClassID == Class_ID(HYBRIDINTERP_SCALE_CLASS_ID, 0) )
    {
        // ����Bezier������
        IBezScaleKey tempKey;
        for( i = 0; i < iKeys; i++ )
        {
            pKeyCon->GetKey(i, &tempKey);
            if( tempKey.time < rSTiming.Start )
            {
                continue;
            }
            else if( tempKey.time > rSTiming.End )
            {
                break;
            }

            rKeyInfo.push_back(new KeyInfo(tempKey.time, KT_SCALE));
            riNumKeys++;
        }
    }
    else if( rClassID == Class_ID(LININTERP_SCALE_CLASS_ID, 0) )
    {
        // ����Linear������
        ILinScaleKey tempKey;
        for( i = 0; i < iKeys; i++ ) 
        {
            pKeyCon->GetKey(i, &tempKey);
            if( tempKey.time < rSTiming.Start )
            {
                continue;
            }
            else if( tempKey.time > rSTiming.End )
            {
                break;
            }

            rKeyInfo.push_back(new KeyInfo(tempKey.time, KT_SCALE));
            riNumKeys++;
        }
    }
}
//----------------------------------------------------------------------------
