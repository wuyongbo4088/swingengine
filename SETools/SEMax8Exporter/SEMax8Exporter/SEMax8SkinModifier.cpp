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
#include "iparamb2.h"
#include "iskin.h"

//----------------------------------------------------------------------------
void Max8SceneBuilder::ProcessSkin(INode* pMaxNode, Modifier* pSkinMod)
{
    // ��Ƥ�������ǻ���CPU����Ƥ����.
    // ��ƤEffect�ǻ���GPU����Ƥ����.
    // ������Ƥ����������ƤEffect.���Max�����ڵ���ʱ��Ҫ�����,
    // ���ڲ�ֹ����д�����Swing Engine��������Ҫ�����Ƥ����������ƤEffect.
    // ��Ƥ����ƫ��������Ƥ�����ڹ����ֲ�����ϵ�µ�����,
    // ��Ƥ������ʹ����Ƥ����ƫ����.
    // ͨ��������ʼʱ�䴦�����񶥵���������͹�������任��������.
    //
    // pMaxNode:
    //     ����Max��Ƥ�޸�����Max�ڵ�ָ��.
    // pSkinMod:
    //     Max��Ƥ�޸�������ָ��.

    bool bNeedDel;
    TriObject* pTriObj = GetTriObject(pMaxNode, &bNeedDel);
    Mesh* pMesh = &pTriObj->GetMesh();

    // ��ȡMax��Ƥ�޸�����ӿ�ָ��.
    ISkin* pSkin = (ISkin*)pSkinMod->GetInterface(I_SKIN);
    ISkinContextData* pSkinData = pSkin->GetContextInterface(pMaxNode);

    // ѭ���������й���,�洢��ʼ������ת�����ű任,
    // ���ӳ�����ͼ�ҵ���֮��Ӧ��Swing Engine�ڵ�.
    int iB, iBoneCount = pSkin->GetNumBones();
    Swing::Node** apSEBones = new Swing::Node*[iBoneCount];
    for( iB = 0; iB < iBoneCount; iB++ )
    {
        INode* pBoneNode = pSkin->GetBone(iB);
        apSEBones[iB] = Swing::StaticCast<Swing::Node>(
            m_spSEScene->GetObjectByName(pBoneNode->GetName()));
    }

    // ����Ӧ��Max��Ƥ�����Swing Engine�ڵ�,
    // �����������һ������ڵ�,
    // ��˵����֮ǰ�ĳ�������������,��֮��Ӧ��Max����û�б����,
    // ������Ӧ����һ��"link"�ڵ�,�����Ų�����ɵ�Swing Engine������ڵ�.
    vector<Swing::TriMesh*> aSEMeshes;
    Swing::Object* pSEObject = m_spSEScene->GetObjectByName(pMaxNode->GetName());
    if( pSEObject->IsExactly(Swing::TriMesh::TYPE) )
    {
        aSEMeshes.push_back(Swing::StaticCast<Swing::TriMesh>(pSEObject));
    }
    else
    {
        Swing::Node* pSENode = Swing::StaticCast<Swing::Node>(pSEObject);
        const char* acName = pSENode->GetName();
        for( int iC = 0; iC < pSENode->GetCount(); iC++ )
        {
            Swing::Spatial* pSEChild = pSENode->GetChild(iC);
            const char* acCName = pSEChild->GetName();
            if( strncmp(acCName, acName, strlen(acName)) == 0 )
            {
                aSEMeshes.push_back(Swing::StaticCast<Swing::TriMesh>(pSEChild));
            }
        }
    }

    // Ϊ��ӦMax���������Swing Engine�����񴴽���Ƥ����������Ƥ�޸���.
    int* aiVerticesPerBone = new int[iBoneCount];
    for( int iM = 0; iM < (int)aSEMeshes.size(); iM++ )
    {
        Swing::TriMesh* pSEMesh = aSEMeshes[iM];

        // �ж�Max�������Щ���㱻�����ڵ�ǰSwing Engine��������,
        // ��������ЩMax��������.
        // ��ʵ��:
        // �Ƚϲ����Ƕ���Զ����.���ԭʼ���������������ظ�������ô��?
        // ��PackVertices��������ʹ�õ�hash���ݽṹӦ�ñ���������,
        // �����Ļ����ǿ���������Щ����,�Ӷ����ý���Ǳ�ڵ�Ч�ʵ�˫��ѭ���㷨.
        int iVCount = pSEMesh->VBuffer->GetVertexCount();
        vector<int> tempVIArray;
        int iV, i, j, k;
        for( iV = 0; iV < iVCount; iV++ )
        {
            for( i = 0; i < pMesh->getNumVerts(); i++ )
            {
                if( pMesh->verts[i].x == (*(Swing::Vector3f*)pSEMesh->VBuffer->PositionTuple(iV)).X
                &&  pMesh->verts[i].y == (*(Swing::Vector3f*)pSEMesh->VBuffer->PositionTuple(iV)).Z
                &&  pMesh->verts[i].z == (*(Swing::Vector3f*)pSEMesh->VBuffer->PositionTuple(iV)).Y )
                {
                    tempVIArray.push_back(i);

                    break;
                }
            }
        }

        // �ж�ÿ�������ֱ�Ӱ���˶��ٶ���.
        iVCount = (int)tempVIArray.size();
        assert( iVCount == pSEMesh->VBuffer->GetVertexCount() );
        memset(aiVerticesPerBone, 0, sizeof(int)*iBoneCount);
        for( i = 0; i < iVCount; i++ )
        {
            iV = tempVIArray[i];
            for( j = 0; j < pSkinData->GetNumAssignedBones(iV); j++ )
            {
                iB = pSkinData->GetAssignedBone(iV, j);
                aiVerticesPerBone[iB]++;
            }
        }

        // ���Max���񱻲����,����п�����Щ����û��Ӱ�쵱ǰSwing Engine������,
        // ���������Ҫ�ж�Ӱ�쵱ǰSwing Engine������Ĺ�������.
        int iActiveBoneCount = 0;
        for( iB = 0; iB < iBoneCount; iB++ )
        {
            if( aiVerticesPerBone[iB] > 0 )
            {
                iActiveBoneCount++;
            }
        }
        assert( iActiveBoneCount > 0 );
        if( iActiveBoneCount == 0 )
        {
            // ��ǰ������û���ܵ��κι���Ӱ��,�����������һ��������
            continue;
        }

        // Ϊ��ǰSwing Engine���������Ƥ����������ƤEffect�����������.
        Swing::Node** apSEActiveBones = new Swing::Node*[iActiveBoneCount];
        float** aafWeight;
        Swing::Allocate(iActiveBoneCount, iVCount, aafWeight);
        memset(aafWeight[0], 0, iActiveBoneCount*iVCount*sizeof(float));
        Swing::Vector3f** aaOffset;
        Swing::Allocate(iActiveBoneCount, iVCount, aaOffset);
        memset(aaOffset[0], 0, iActiveBoneCount*iVCount*sizeof(Swing::Vector3f));

        // Ϊ��ǰSwing Engine������洢Ӱ���������й���,��Щ��������Swing Engine�ڵ�.
        vector<int> tempBIArray(iBoneCount);
        for( iB = 0, k = 0; iB < iBoneCount; iB++ )
        {
            if( aiVerticesPerBone[iB] > 0 )
            {
                apSEActiveBones[k] = apSEBones[iB];
                tempBIArray[iB] = k;
                k++;
            }
        }

        // ѭ��������ǰSwing Engine���������õ���Max�����б�,
        // ��ȡ�������Ȩ�غͶ���ƫ����.
        for( i = 0; i < iVCount; i++ )
        {
            iV = tempVIArray[i];
            for( j = 0; j < pSkinData->GetNumAssignedBones(iV); j++ )
            {
                iB = pSkinData->GetAssignedBone(iV, j);
                k = tempBIArray[iB];
                aafWeight[i][k] = pSkinData->GetBoneWeight(iV, j);

                Swing::Vector3f tempWorldVertex;
                pSEMesh->World.ApplyForward(*(Swing::Vector3f*)pSEMesh->VBuffer->PositionTuple(i), 
                    tempWorldVertex);
                apSEActiveBones[k]->World.ApplyInverse(tempWorldVertex, aaOffset[i][k]);
            }
        }
        
        if( !m_pSettings->UseGPUSkin )
        {
            // Ϊ��ǰSwing Engine�����񴴽���Ƥ������.

            Swing::SkinController* pSESkinController = new Swing::SkinController(
                iVCount, iActiveBoneCount, apSEActiveBones, aafWeight, aaOffset);
            pSESkinController->MinTime = 0.0f;
            pSESkinController->MaxTime = TicksToSec(m_iTimeEnd - m_iTimeStart);
            pSEMesh->AttachController(pSESkinController);
        }
        else
        {
            // Ϊ��ǰSwing Engine�����񴴽���ƤEffect.

            enum SkinEffect
            {
                ST_DEFAULT,
                ST_MATERIAL,
                ST_MATERIALTEXTURE,
                ST_COUNT
            };

            vector<BoneWeight>* aBWArray;
            Swing::Transformation* aOffset;

            // �ж�Ӧ�����ɵ���Ƥeffect����.
            SkinEffect eSkinEffect = ST_DEFAULT;
            Swing::Effect* pSaveEffect = 0;
            for( i = 0; i < pSEMesh->GetEffectCount(); i++ )
            {
                Swing::Effect* pTempEffect = pSEMesh->GetEffect(i);
                if( Swing::DynamicCast<Swing::MaterialEffect>(pTempEffect) )
                {
                    eSkinEffect = ST_MATERIAL;
                    pSaveEffect = pTempEffect;

                    break;
                }
                else if( Swing::DynamicCast<Swing::MaterialTextureEffect>(pTempEffect) )
                {
                    eSkinEffect = ST_MATERIALTEXTURE;
                    pSaveEffect = pTempEffect;

                    break;
                }
                else if( Swing::DynamicCast<Swing::DefaultShaderEffect>(pTempEffect ))
                {
                    eSkinEffect = ST_DEFAULT;
                    pSaveEffect = pTempEffect;

                    break;
                }
            }

            // ������ǰSwing Engine�����������ÿ����������Ա任.
            aOffset = new Swing::Transformation[iActiveBoneCount];
            for( i = 0; i < iActiveBoneCount; i++ )
            {
                Swing::Transformation tempInvT;
                apSEActiveBones[i]->World.GetInverse(tempInvT);
                aOffset[i].Product(pSEMesh->World, tempInvT);
            }

            // ������ǰSwing Engine�������������VB.
            Swing::Attributes tempAttr;
            switch( eSkinEffect )
            {
            case ST_DEFAULT:
		        tempAttr.SetPositionChannels(3);
                tempAttr.SetTCoordChannels(0, 4); // BlendWeight
                tempAttr.SetTCoordChannels(1, 4); // BlendIndices

                break;
            case ST_MATERIAL:
		        tempAttr.SetPositionChannels(3);
                tempAttr.SetNormalChannels(3);
                tempAttr.SetTCoordChannels(0, 4); // BlendWeight
                tempAttr.SetTCoordChannels(1, 4); // BlendIndices

                break;
            case ST_MATERIALTEXTURE:
		        tempAttr.SetPositionChannels(3);
                tempAttr.SetNormalChannels(3);
                tempAttr.SetTCoordChannels(0, 2); // BaseTexture
                tempAttr.SetTCoordChannels(1, 4); // BlendWeight
                tempAttr.SetTCoordChannels(2, 4); // BlendIndices

                break;
            default :
                assert( false );
                break;
            }
            Swing::VertexBuffer* pVBNew = new Swing::VertexBuffer(tempAttr, 
                pSEMesh->VBuffer->GetVertexCount());
            int iChannels;
            float* afData = pVBNew->GetData();
            pSEMesh->VBuffer->BuildCompatibleArray(tempAttr, false, iChannels, afData);
            assert( iChannels == pVBNew->GetChannelCount() );
            pSEMesh->VBuffer = pVBNew;

            // ���������񶥵�Ȩ�ر�,�Ӷ�Ϊÿ������ѡ������Ӱ��Ч������4������.
            aBWArray = new vector<BoneWeight>[iVCount];
            for( i = 0; i < iVCount; i++ )
            {
                for( j = 0; j < iActiveBoneCount; j++ )
                {
                    BoneWeight tempBW;
                    tempBW.BoneID = j;
                    tempBW.Weight = aafWeight[i][j];

                    aBWArray[i].push_back(tempBW);
                }
                sort(aBWArray[i].begin(), aBWArray[i].end());

                float* pWeight = 0;
                float* pIndices = 0;
                switch( eSkinEffect )
                {
                case ST_DEFAULT:
                    pWeight = pSEMesh->VBuffer->TCoordTuple(0, i);
                    pIndices = pSEMesh->VBuffer->TCoordTuple(1, i);

                    break;
                case ST_MATERIAL:
                    pWeight = pSEMesh->VBuffer->TCoordTuple(0, i);
                    pIndices = pSEMesh->VBuffer->TCoordTuple(1, i);

                    break;
                case ST_MATERIALTEXTURE:
                    pWeight = pSEMesh->VBuffer->TCoordTuple(1, i);
                    pIndices = pSEMesh->VBuffer->TCoordTuple(2, i);

                    break;
                default :
                    assert( false );

                    break;
                }
                for( j = 0, k = iActiveBoneCount-1; k >= 0 && j < 4 ; k--, j++ )
                {
                    *pWeight++ = aBWArray[i][k].Weight;
                    *pIndices++ = (float)aBWArray[i][k].BoneID;
                }

                int iStopHere = 0;
            }

            Swing::Effect* pNewEffect = 0;
            Swing::String tempImageName;
            switch( eSkinEffect )
            {
            case ST_DEFAULT:
                pNewEffect = 
                    new Swing::SkinDefaultEffect(iActiveBoneCount, apSEActiveBones, aOffset);

                break;
            case ST_MATERIAL:
                pNewEffect = 
                    new Swing::SkinMaterialEffect(iActiveBoneCount, apSEActiveBones, aOffset);

                break;
            case ST_MATERIALTEXTURE:
                tempImageName = 
                    Swing::StaticCast<Swing::ShaderEffect>(pSaveEffect)->GetPImageName(0, 0);
                pNewEffect = 
                    new Swing::SkinMaterialTextureEffect(tempImageName, iActiveBoneCount, apSEActiveBones, aOffset);

                break;
            default :
                assert( false );

                break;
            }

            if( pSaveEffect )
            {
                pSEMesh->DetachEffect(pSaveEffect);
            }

            pSEMesh->AttachEffect(pNewEffect);

            delete[] aBWArray;
            Swing::Deallocate(aafWeight);
            Swing::Deallocate(aaOffset);
        }
    }
    
    if( bNeedDel )
    {
        delete pTriObj;
    }

    delete[] apSEBones;
    delete[] aiVerticesPerBone;
}
//----------------------------------------------------------------------------