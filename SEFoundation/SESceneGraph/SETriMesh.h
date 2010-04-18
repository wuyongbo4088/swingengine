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

#ifndef Swing_TriMesh_H
#define Swing_TriMesh_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEGeometry.h"
#include "SEIntArray.h"
#include "SETriangle3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ˵��:
// ����:Sun Che
// ʱ��:20080803
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SETriMesh : public SEGeometry
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    SETriMesh(SEVertexBuffer* pVBuffer, SEIndexBuffer* pIBuffer);
    virtual ~SETriMesh(void);

    // ��Ա����.
    inline int GetTriangleCount(void) const;
    void GetModelTriangle(int i, SETriangle3f& rMTri) const;
    void GetWorldTriangle(int i, SETriangle3f& rWTri) const;

    // ���ɷ���.
    void GenerateNormals(void);

    // ��������.
    // ����ָ����һ��2ά������������(s,t),���ɶ����T,B,N�淶������,
    // ����T��Ӧ����ռ�s��,B��Ӧ����ռ�t��.
    // �û�������ȷ����ǰ����߱�һ��2ά������������,�����ɹ�����true,
    // ���򷵻�false.
    bool GenerateTangents(int iSrcTCoordUnit, int iTangentUnit, 
        int iBiTangentUnit = -1);

    // Picking support.
    // ��pick record�����ഢ�����������ཻ������������ֵ.
    // ͬʱ���������ཻ�������Ȩ��ֵ.
    // �⽫����Ӧ�ó����ֵ������ཻ��Ķ������Ժ�������������.
    class SE_FOUNDATION_API SEPickRecord : public SEGeometry::SEPickRecord
    {
    public:
        SEPickRecord(SETriMesh* pIObject, float fT, int iTriangle, float fBary0,
            float fBary1, float fBary2);
        SEPickRecord(void);

        // �������ཻ������������ֵ.
        int Triangle;

        // �ཻ�����������е�����Ȩ������.
        // ȫ����[0,1]֮��,��b0 + b1 + b2 = 1.
        float Bary0, Bary1, Bary2;
    };

    // ���ߵ�ԭ��ͷ�����������������������ϵ��.
    // Ӧ�ó������ͷ�PickArray�е�����pick record,
    // ��Ϊ��Щpick record������ȫ���ڴ�صĶ��ڴ�.
    virtual void DoPick(const SERay3f& rRay, PickArray& rResults);

    // SEPickRecord�ڴ��,�������DoPick����ʱ��Ƶ����̬new��delete����.
    // Ӧ�ó����ʼ��ʱ,�������InitializePickRecordPool����.
    // Ӧ�ó�����ֹʱ,�������TerminatePickRecordPool����.
    // Ӧ�ó������DoPick��������������ͼǰ,Ӧ�ȵ���ResetPickRecordPool����.
    static void InitializePickRecordPool(int iMaxCount = 32, int iGrowBy = 32);
    static void TerminatePickRecordPool(void);
    static void ResetPickRecordPool(void);

protected:
    SETriMesh(void);

    // ���������ݸ���.
    // ����ģ�Ϳռ䷨��.
    virtual void UpdateModelNormals(void);

    // SEPickRecord�ڴ����ز���.
    inline SEPickRecord* AllocatePickRecord(void);
    static std::vector<SEPickRecord> ms_PickRecordPool;
    static int ms_iMaxCount;
    static int ms_iGrowBy;
    static int ms_iCount;
};

typedef SESmartPointer<SETriMesh> SETriMeshPtr;

#include "SETriMesh.inl"

}

#endif
