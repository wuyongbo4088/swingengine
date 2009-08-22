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
#include "modstack.h"

using namespace std;

//----------------------------------------------------------------------------
void Max8SceneBuilder::ApplyModifiers()
{
    // ��֮ǰ����Max�������ռ������޸�����Ӹ�Swing Engine������ͼ��Ӧ�ڵ�.
    // ��ǰֻ֧��skin�޸���.

    vector<ModifierInfo*>::iterator pI;
    for( pI = m_ModList.begin(); pI < m_ModList.end(); pI++ )
    {
        ModifierInfo* pInfo = *pI;
        vector<Modifier*>::iterator pJ = pInfo->Modifiers.begin();
        for( /**/; pJ < pInfo->Modifiers.end(); pJ++ )
        {
            Modifier* pModifier = *pJ;
            Class_ID tempID = pModifier->ClassID();
            if( tempID == SKIN_CLASSID )
            {
                ProcessSkin(pInfo->Node, pModifier);
            }
        }
    }
}
//----------------------------------------------------------------------------
void Max8SceneBuilder::FindModifiers(INode* pMaxNode,
    vector<Modifier*>& rModifiers)
{
    // �ж�һ��Max�ڵ��Ƿ�����޸���.��������ռ�,���Ժ�ʹ��.
    //
    // pMaxNode:
    //     ������Max�ڵ�ָ��.
    // rModifiers:
    //     �����ռ����޸���ָ������.

    // �ӽڵ��ȡ�����.
    Object* pObj = pMaxNode->GetObjectRef();
    if( !pObj )
    {
        return;
    }

    // �Ƿ�Ϊ��������?
    while( pObj->SuperClassID() == GEN_DERIVOB_CLASS_ID )
    {
        // ����ת��.
        IDerivedObject* pDerObj = static_cast<IDerivedObject*>(pObj);

        // ���������޸�����ջ������Ԫ��.
        int iModStackIndex = 0;
        while( iModStackIndex < pDerObj->NumModifiers() )
        {
            // ��ȡ��ǰ�޸���.
            Modifier* pMod = pDerObj->GetModifier(iModStackIndex);
            if( pMod )
            {
                rModifiers.push_back(pMod);
            }

            iModStackIndex++;
        }
        pObj = pDerObj->GetObjRef();
    }
}
//----------------------------------------------------------------------------



