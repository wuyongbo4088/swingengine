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
#include "SEMax8UnimaterialMesh.h"
#include <set>

using namespace std;

// ע��Maxʹ����������ϵ,��Swing Engineʹ����������ϵ.

//----------------------------------------------------------------------------
TriObject* Max8SceneBuilder::GetTriObject(INode* pNode, bool* pbNeedDel)
{
    // �ж�һ��Max�ڵ��Ƿ����mesh����.
    //
    // pNode:
    //     �����Ե�nodeָ��.
    // pbNeedDel:
    //     Ϊtrueʱ,���ô˺�����������ɾ�����ص�meshָ��,������.
    // ����������mesh����,û����Ϊ��.

    Object* pObj = pNode->EvalWorldState(m_iTimeStart).obj;

    // �жϵ�ǰ�ڵ����������Ƿ����ת��Ϊ�����������.
    if( !pObj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)) )
    {
        return NULL;
    }

    // �������ת��,��Max���ܻ�Ϊ��ǰ�����½�һ�������������.
    TriObject* pTriObj = (TriObject*)pObj->ConvertToType(m_iTimeStart,
        Class_ID(TRIOBJ_CLASS_ID, 0));
    if( pTriObj == NULL )
    {
        return NULL;
    }

    // �жϵ������Ƿ���Ҫɾ���������������.
    *pbNeedDel = (pTriObj != pObj ? true : false);

    return pTriObj;
}
//----------------------------------------------------------------------------
Swing::Spatial* Max8SceneBuilder::BuildMesh(INode* pMaxNode,
    Swing::Spatial* pSENode)
{
    // ��һ��Max��������ת���һ������Swing Engine��������.
    //
    // pMaxNode:
    //     Max�����е�Mesh�ڵ�ָ��.
    // pSENode:
    //     ����������Swing Engine��������ĸ��ڵ�ָ��.
    // �����´�����Swing Engine�����ӽڵ�,
    // �����ǰMax��Mesh�ڵ�û�б����,��ֱ�ӷ����½���Swing Engine��������ڵ�,
    // ���򷵻�һ��Swing Engine��"link"��ڵ�,
    // ����ֵ�Max�ڵ���Mesh������Ϊ��"link"�ڵ���ӽڵ�.

    bool bNeedDel = false;
    TriObject* pTriObj = GetTriObject(pMaxNode, &bNeedDel);
    if( !pTriObj )
    {
        return NULL;
    }
    Mesh* pMaxMesh = &pTriObj->GetMesh();

    Mtl* pMtl = NULL;
    int iMtlID = -1;
    pMtl = pMaxNode->GetMtl();
    if( pMtl )
	{
        iMtlID = m_MtlList.GetID(pMtl);
        assert( iMtlID >= 0 );
	}

    Swing::Spatial* pSELink;
    bool bHasLink = ((int)pSENode->GetName().length() > 0 &&
        strcmp(pMaxNode->GetName(), pSENode->GetName().c_str()) == 0);
    char* acMaxName = pMaxNode->GetName();
    int i;

    // ��ΪSwing Engine����ֻ�ܴ���һ������,
    // ���Ҫ�жϵ�ǰMax�����Ƿ���ж������,���������Ҫ��ָ�����,
    // ��ֺ����ɵ�������ֻ�ܴ���һ������.
    // �������Ҫ���Max����,��ֱ�Ӵ���Swing Engine�������Ӹ�Swing Engine���ڵ�,
    // ���򴴽�һ��Swing Engine"link"��ڵ���Ϊ��ֺ��������ĸ��ڵ�.
    vector<Max8UnimaterialMesh*> tempUMeshes;
    SplitGeometry(pMaxMesh, iMtlID, tempUMeshes);
    if( tempUMeshes.size() > 1 )
    {
        // ��������,����һ��Swing Engine"link"��ڵ�.
        if( !bHasLink )
        {
            pSELink = BuildSpatial(pMaxNode, pSENode);
        }
        else
        {
            pSELink = pSENode;
        }

        assert( pSELink->IsDerived(Swing::Node::TYPE) );

        // �Ѳ�ֺ��õ�Swing Engine��������Ϊ"link"��ڵ���ӽڵ�.
        for( i = 0; i < (int)tempUMeshes.size(); i++ )
        {
            Swing::TriMesh* pSETriMesh = tempUMeshes[i]->ToTriMesh();
            char acMeshNumber[6];
            Swing::System::SE_Sprintf(acMeshNumber, 6, "_%d", i + 1);
            size_t uiSize = strlen(acMaxName) + strlen(acMeshNumber) + 1;
            char* acSEName = new char[uiSize];
            Swing::System::SE_Strcpy(acSEName, uiSize, acMaxName);
            Swing::System::SE_Strcat(acSEName, uiSize, acMeshNumber);         
            pSETriMesh->SetName(acSEName);
            delete[] acSEName;
            ((Swing::Node*)pSELink)->AttachChild(pSETriMesh);
        }
    }
    else
    {
        // ���û�в��,��ѻ�ȡ��Swing Engine������ΪpSENode���ӽڵ�.
        Swing::TriMesh* pSETriMesh = tempUMeshes[0]->ToTriMesh();
        if( !bHasLink )
        {
            pSETriMesh->SetName(acMaxName);
            pSETriMesh->Local = GetLocalTransform(pMaxNode, m_iTimeStart);
        }
        else
        {
            size_t uiSize = strlen(acMaxName) + 3;
            char* acSEName = new char[uiSize];
            Swing::System::SE_Strcpy(acSEName, uiSize, acMaxName);
            Swing::System::SE_Strcat(acSEName, uiSize, "_1");
            pSETriMesh->SetName(acSEName);
            delete[] acSEName;
        }
        assert( pSENode->IsDerived(Swing::Node::TYPE) );
        ((Swing::Node*)pSENode)->AttachChild(pSETriMesh);
        pSELink = pSETriMesh;
    }
    for( i = 0; i < (int)tempUMeshes.size(); i++ )
    {
        delete tempUMeshes[i];
    }

    if( bNeedDel )
    {
        delete pTriObj;
    }

    return pSELink;
}
//----------------------------------------------------------------------------
void Max8SceneBuilder::SplitGeometry(Mesh* pMaxMesh, int iMtlID, 
    vector<Max8UnimaterialMesh*>& rUMeshes)
{
    // ���һ��Max����ʹ���˶���Ӳ���,
    // ����Ҫ��ֳ����ɸ�Swing Engine����,��ΪSwing Engine�������ֻ֧��һ������.
    //
    // pMaxMesh:
    //     ����ֵ�Max�������ָ��.
    // rUMeshes:
    //     ��ֺ�洢�����������.
    // bHasReflection:
    //     Ϊtrueʱ,�����ڵ�ı任��������任,����Ϊfalse.

    int i, j;

    // ���㶥�㷨��.
    Swing::Vector3f* aNormal = NULL;
    if( m_pSettings->IncludeNormals )
    {
        pMaxMesh->buildNormals();
        aNormal = new Swing::Vector3f[pMaxMesh->numVerts];
        for( i = 0; i < pMaxMesh->numFaces; i++ )
        {
            Face& rFace = pMaxMesh->faces[i];
            for( j = 0; j < 3; j++ )
            {
                int iVertex = rFace.getVert(j);
                aNormal[iVertex] += GetVertexNormal(pMaxMesh, i, iVertex); // �����
            }
        }
    }

    // �Ӳ�����
    int iSubCount = 0;
    Max8MaterialTree* pTree = 0;
    if( iMtlID >= 0 )
    {
        pTree = &m_MtlTree[iMtlID];
        iSubCount = pTree->GetMChildCount();
    }

    // ����������ʹ�õ�����Ӳ�������.
    int iFace, iSubID, iMaxSubID = -1;
    for( iFace = 0; iFace < pMaxMesh->numFaces; iFace++ )
    {
        iSubID = pMaxMesh->faces[iFace].getMatID();
        if( iSubID >= iSubCount )
        {
            if( iSubCount > 0 )
            {
                iSubID = iSubID % iSubCount;
            }
            else
            {
                iSubID = 0;
            }
        }
        if( iSubID > iMaxSubID )
        {
            iMaxSubID = iSubID;
        }
    }

    // ���ݲ���ID��������ƽ��.
    vector<int>* aPartition = new vector<int>[iMaxSubID + 1];
    for( iFace = 0; iFace < pMaxMesh->numFaces; iFace++ )
    {
        iSubID = pMaxMesh->faces[iFace].getMatID();
        if( iSubID >= iSubCount )
        {
            if( iSubCount > 0 )
            {
                iSubID = iSubID % iSubCount;
            }
            else
            {
                iSubID = 0;
            }
        }
        aPartition[iSubID].push_back(iFace);
    }

    // ����unimaterial����.
    for( iSubID = 0; iSubID <= iMaxSubID; iSubID++ )
    {
        if( aPartition[iSubID].size() == 0 )
        {
            // û���κ�����ƽ��ʹ�ø��Ӳ���.
            continue;
        }

        Max8UnimaterialMesh* pMesh = new Max8UnimaterialMesh;
        if( iMtlID >= 0 )
        {
            // ��ʵ��.
            // Ŀǰ,ֻ�Ǽ򵥵�ʹ���˵�һ���ǿ�����.
            // �Ժ�Ӧ֧�ֶ�������.
            if( iSubCount > 0 )
            {
                Max8MaterialTree& rSubtree = pTree->GetMChild(iSubID);
                pMesh->MState() = rSubtree.GetMaterial();
                for( i = 0; i < (int)rSubtree.GetTChildCount(); i++ )
                {
                    if( rSubtree.GetTexture(i) )
                    {
                        pMesh->TState() = rSubtree.GetTexture(i);

                        break;
                    }
                }
            }
            else
            {
                pMesh->MState() = pTree->GetMaterial();
                for( i = 0; i < (int)pTree->GetTChildCount(); i++ )
                {
                    if( pTree->GetTexture(i) )
                    {
                        pMesh->TState() = pTree->GetTexture(i);

                        break;
                    }
                }
            }
        }

        // �������.
        PackVertices(pMesh, pMaxMesh, aPartition[iSubID], aNormal);

        // �����ɫ.
        if( m_pSettings->IncludeVertexColors && pMaxMesh->numCVerts > 0 )
        {
            PackColors(pMesh, pMaxMesh, aPartition[iSubID]);
        }

        // �����������.
        if( m_pSettings->IncludeTexCoords &&  pMaxMesh->numTVerts > 0
            && pMesh->TState() )
        {
            PackTextures(pMesh, pMaxMesh, aPartition[iSubID]);
        }

        rUMeshes.push_back(pMesh);
    }

    delete[] aPartition;
    delete[] aNormal;

    // ����Swing Engine������������Ҫ��,���ƶ�������.
    for( i = 0; i < (int)rUMeshes.size(); i++ )
    {
        rUMeshes[i]->DuplicateGeometry();
    }
}
//----------------------------------------------------------------------------
void Max8SceneBuilder::PackVertices(Max8UnimaterialMesh* pUniMesh,
    Mesh* pMaxMesh, vector<int>& rPartition, Swing::Vector3f* aNormal)
{
    // ����Max�����һ����(��һ��ƽ�����������ʾ),�ò���ʹ��ͬһ����.
    // ����Ӧ�Ķ���ͷ������ݴ����Max8UnimaterialMesh.
    //
    // pUniMesh:
    //     ���ڽ������ݵ�Max8UnimaterialMeshָ��.
    // pMaxMesh:
    //     ���ڻ�ȡ�������ݵ�Max����ָ��.
    // rPartition:
    //     ��ʾMax����һ���ֵ�ƽ����������.
    // aNormal:
    //     ���ڻ�ȡ���ݵ�Max�����ȫ�����㷨��.

    // ��ǰ������ʹ�õĶ�������,setȷ������Ψһ��.
    set<int> tempVIndexSet;
    int i, j;
    for( i = 0; i < (int)rPartition.size(); i++ )
    {
        Face& rFace = pMaxMesh->faces[rPartition[i]];
        for( j = 0; j < 3; j++ )
        {
            tempVIndexSet.insert(rFace.v[j]);
        }
    }

    // ȫ�����񶥵������Ƕ���������Max���񶥵���Դ�����е�����.
    // ����һ��ȫ�����񶥵������ķ���ӳ���,
    // ��Ե�ǰʹ��ͬһ���ʵ�������,����һ�������񶥵���Դ����,
    // �ö��������±�0��n,��ʾn+1�����ظ��Ķ���),
    // ��������ӳ������(�����±�0��m,��ʾ�����ȫ�����񶥵�����,mΪ�������ֵ).
    // STL set��Ĭ�ϰѲ����Ԫ�ذ����������д洢,
    // ��˵�ǰset��ĩβԪ�رض�Ϊ��ǰ����������ȫ�����񶥵�����.
    // ���tempVIndexSet[i] = j,��aiVMap[j] = i.
    // ����ǵ�ǰ������û���õ���ȫ�����񶥵�����,��aiVMap[j] = -1.
    int iVMax = *tempVIndexSet.rbegin();
    int* aiVMap = new int[iVMax + 1];
    memset(aiVMap, 0xFF, (iVMax + 1)*sizeof(int));

    pUniMesh->VCount() = tempVIndexSet.size();
    pUniMesh->Vertex() = new Swing::Vector3f[pUniMesh->VCount()];
    pUniMesh->Normal() = new Swing::Vector3f[pUniMesh->VCount()];

    set<int>::iterator tempIter = tempVIndexSet.begin();
    for( i = 0; i < (int)tempVIndexSet.size(); i++, tempIter++ )
    {
        j = *tempIter;
        aiVMap[j] = i;

        pUniMesh->Vertex()[i].X = pMaxMesh->verts[j].x;
        pUniMesh->Vertex()[i].Y = pMaxMesh->verts[j].z;
        pUniMesh->Vertex()[i].Z = pMaxMesh->verts[j].y;
        pUniMesh->Normal()[i].X = aNormal[j].X;
        pUniMesh->Normal()[i].Y = aNormal[j].Z;
        pUniMesh->Normal()[i].Z = aNormal[j].Y;
    }

    // �����������涥������.
    pUniMesh->FCount() = rPartition.size();
    pUniMesh->Face() = new int[3 * pUniMesh->FCount()];
    for( i = 0; i < (int)rPartition.size(); i++ )
    {
        Face& rFace = pMaxMesh->faces[rPartition[i]];

        pUniMesh->Face()[3*i    ] = aiVMap[rFace.v[0]];
        pUniMesh->Face()[3*i + 1] = aiVMap[rFace.v[2]];
        pUniMesh->Face()[3*i + 2] = aiVMap[rFace.v[1]];
    }
    delete[] aiVMap;
}
//----------------------------------------------------------------------------
void Max8SceneBuilder::PackColors(Max8UnimaterialMesh* pUniMesh, Mesh* pMaxMesh, 
    vector<int>& rPartition)
{
    // ����Max�����һ����(��һ��ƽ�����������ʾ),�ò���ʹ��ͬһ����.
    // ����Ӧ�Ķ�����ɫ�����Max8UnimaterialMesh.
    //
    // pUniMesh:
    //     ���ڽ������ݵ�Max8UnimaterialMeshָ��.
    // pMaxMesh:
    //     ���ڻ�ȡ��ɫ���ݵ�Max����ָ��.
    // rPartition:
    //     ��ʾMax����һ���ֵ�ƽ����������.

    // ��ǰ������ʹ�õ���ɫ����,setȷ����ɫΨһ��.
    set<int> tempCIndexSet;
    int i, j;
    for( i = 0; i < (int)rPartition.size(); i++ )
    {
        TVFace& rCFace = pMaxMesh->vcFace[rPartition[i]];
        for( j = 0; j < 3; j++ )
        {
            tempCIndexSet.insert(rCFace.t[j]);
        }
    }

    // ȫ��������ɫ��������ɫ������Max������ɫ��Դ�����е�����.
    // ����һ��ȫ��������ɫ�����ķ���ӳ���,
    // ��Ե�ǰʹ��ͬһ���ʵ�������,����һ����������ɫ��Դ����,
    // ����ɫ�����±�0��n,��ʾn+1�����ظ�����ɫ),
    // ��������ӳ������(�����±�0��m,��ʾ��ɫ��ȫ��������ɫ����,mΪ�������ֵ).
    // STL set��Ĭ�ϰѲ����Ԫ�ذ����������д洢,
    // ��˵�ǰset��ĩβԪ�رض�Ϊ��ǰ����������ȫ��������ɫ����.
    // ���tempCIndexSet[i] = j,��aiCMap[j] = i.
    // ����ǵ�ǰ������û���õ���ȫ��������ɫ����,��aiCMap[j] = -1.
    int iCMax = *tempCIndexSet.rbegin();
    int* aiCMap = new int[iCMax + 1];
    memset(aiCMap, 0xFF, (iCMax + 1)*sizeof(int));

    pUniMesh->CCount() = tempCIndexSet.size();
    pUniMesh->Color() = new Swing::ColorRGB[pUniMesh->CCount()];

    set<int>::iterator tempIter = tempCIndexSet.begin();
    for( i = 0; i < (int)tempCIndexSet.size(); i++, tempIter++ )
    {
        j = *tempIter;
        aiCMap[j] = i;

        pUniMesh->Color()[i].R = pMaxMesh->vertCol[j].x;
        pUniMesh->Color()[i].G = pMaxMesh->vertCol[j].y;
        pUniMesh->Color()[i].B = pMaxMesh->vertCol[j].z;
    }

    // ��������������ɫ����.
    pUniMesh->CFace() = new int[3*pUniMesh->FCount()];
    for( i = 0; i < (int)rPartition.size(); i++ )
    {
        TVFace& rCFace = pMaxMesh->vcFace[rPartition[i]];

        pUniMesh->CFace()[3*i    ] = aiCMap[rCFace.t[0]];
        pUniMesh->CFace()[3*i + 1] = aiCMap[rCFace.t[2]];
        pUniMesh->CFace()[3*i + 2] = aiCMap[rCFace.t[1]];
    }
    delete[] aiCMap;
}
//----------------------------------------------------------------------------
void Max8SceneBuilder::PackTextures(Max8UnimaterialMesh* pUniMesh,
    Mesh* pMaxMesh, vector<int>& rPartition)
{
    // ����Max�����һ����(��һ��ƽ�����������ʾ),�ò���ʹ��ͬһ����.
    // ����Ӧ�Ķ���������������Max8UnimaterialMesh.
    //
    // pUniMesh:
    //     ���ڽ������ݵ�Max8UnimaterialMeshָ��.
    // pMaxMesh:
    //     ���ڻ�ȡ�����������ݵ�Max����ָ��.
    // rPartition:
    //     ��ʾMax����һ���ֵ�ƽ����������.

    // ��ǰ������ʹ�õ�������������,setȷ����������Ψһ��.
    set<int> tempTIndexSet;
    int i, j;
    for( i = 0; i < (int)rPartition.size(); i++ )
    {
        TVFace& rTFace = pMaxMesh->tvFace[rPartition[i]];
        for( j = 0; j < 3; j++ )
        {
            tempTIndexSet.insert(rTFace.t[j]);
        }
    }

    // ȫ����������������������������������Max��������������Դ�����е�����.
    // ����һ��ȫ�������������������ķ���ӳ���,
    // ��Ե�ǰʹ��ͬһ���ʵ�������,����һ������������������Դ����,
    // ���������������±�0��n,��ʾn+1�����ظ�����������),
    // ��������ӳ������(�����±�0��m,��ʾ���������ȫ������������������,mΪ�������ֵ).
    // STL set��Ĭ�ϰѲ����Ԫ�ذ����������д洢,
    // ��˵�ǰset��ĩβԪ�رض�Ϊ��ǰ����������ȫ������������������.
    // ���tempTIndexSet[i] = j,��aiTMap[j] = i.
    // ����ǵ�ǰ������û���õ���ȫ������������������,��aTMap[j] = -1.
    int iTMax = *tempTIndexSet.rbegin();
    int* aiTMap = new int[iTMax + 1];
    memset(aiTMap, 0xFF, (iTMax + 1)*sizeof(int));

    pUniMesh->TCount() = tempTIndexSet.size();
    pUniMesh->Texture() = new Swing::Vector2f[pUniMesh->TCount()];

    set<int>::iterator tempIter = tempTIndexSet.begin();
    for( i = 0; i < (int)tempTIndexSet.size(); i++, tempIter++ )
    {
        j = *tempIter;
        aiTMap[j] = i;

        pUniMesh->Texture()[i].X = pMaxMesh->tVerts[j].x;
        pUniMesh->Texture()[i].Y = pMaxMesh->tVerts[j].y;
    }

    // ������������������������.
    pUniMesh->TFace() = new int[3 * pUniMesh->FCount()];
    for( i = 0; i < (int)rPartition.size(); i++ )
    {
        TVFace& rTFace = pMaxMesh->tvFace[rPartition[i]];

        pUniMesh->TFace()[3*i    ] = aiTMap[rTFace.t[0]];
        pUniMesh->TFace()[3*i + 1] = aiTMap[rTFace.t[2]];
        pUniMesh->TFace()[3*i + 2] = aiTMap[rTFace.t[1]];
    }
    delete[] aiTMap;
}
//----------------------------------------------------------------------------
Swing::Vector3f Max8SceneBuilder::GetVertexNormal(Mesh* pMaxMesh, int iFace,
    int iVertex)
{
    // ��ȡ���㷨��,���һ��Max���񶥵�����.
    // 
    // pMaxMesh:
    //     �����ö����Max����.
    // iFace:
    //     �ö�������ƽ�������.
    // iVertex:
    //     �ö��������.
    // ���ض��㷨��.

    Point3 tempNormal;

    RVertex* pRV = pMaxMesh->getRVertPtr(iVertex);
    
    if( pRV->rFlags & SPECIFIED_NORMAL )
    {
        tempNormal = pRV->rn.getNormal();
    }
    else
    {
        // ���RVertex��������֮��صķ�������,
        // ��ͨ��һ��smoothing group������÷���,
        // ������smoothing groupҲ������,��ʹ���淨��.
        Face& rFace = pMaxMesh->faces[iFace];
        DWORD dwSmoothingGroup = rFace.smGroup;
        int iNumNormals = pRV->rFlags & NORCT_MASK;
        if( iNumNormals && dwSmoothingGroup )
        {
            if( iNumNormals == 1 )
            {
                // ���group��ֻ��һ������,��rRV���Ѿ��洢�˸÷���.
                tempNormal = pRV->rn.getNormal();
            }
            else
            {
                // ��smoothing group���ҵ����跨��.
                for( int i = 0; i < iNumNormals; i++ )
                {
                    if( pRV->ern[i].getSmGroup() & dwSmoothingGroup )
                    {
                        tempNormal = pRV->ern[i].getNormal();

                        break;
                    }
                }
            }
        }
        else
        {
            // û��smoothing groupʱ��ʹ�����ƽ��ķ���.
            tempNormal = pMaxMesh->FaceNormal(iFace, true);
        }
    }

    return Swing::Vector3f(tempNormal.x, tempNormal.y, tempNormal.z);
}
//----------------------------------------------------------------------------