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

#ifndef Swing_VertexBuffer_H
#define Swing_VertexBuffer_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEObject.h"
#include "SEAttributes.h"
#include "SEBindable.h"
#include "SEColorRGB.h"
#include "SEColorRGBA.h"
#include "SEVector2.h"
#include "SEVector3.h"
#include "SEVector4.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:���㻺������
// ˵��:VBԭʼ���ݱ�����ϵͳ�ڴ���,
//      ���Դ��п��Դ��ڶ����VB��ʵ��,�ɾ���ͼ��API���𴴽�,
//      ÿ���Դ�ʵ�����Ǹ�VB���ݵ��Ӽ�,��ĳeffectʹ��.
// ����:Sun Che
// ʱ��:20080319
//----------------------------------------------------------------------------
class SE_FOUNDATION_API VertexBuffer : public SEObject, public Bindable
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    VertexBuffer(const Attributes& rAttributes, int iVertexCount);
    VertexBuffer(const VertexBuffer* pVBuffer);
    virtual ~VertexBuffer(void);

    // ��ȡ�������Ķ����ʽ.
    inline const Attributes& GetAttributes(void) const;

    // ����ÿ�������С,��λ��"float",
    // �ֽ�����С��GetVertexSize()*sizeof(float)���.
    inline int GetVertexSize(void) const;

    // ��ȡ�������Ķ������
    inline int GetVertexCount(void) const;

    // ����positions.
    float* PositionTuple(int i);
    const float* PositionTuple(int i) const;

    // ����normals.
    float* NormalTuple(int i);
    const float* NormalTuple(int i) const;

    // ����colors.
    float* ColorTuple(int iUnit, int i);
    const float* ColorTuple(int iUnit, int i) const;

    // ����texture coordinates.
    float* TCoordTuple(int iUnit, int i);
    const float* TCoordTuple(int iUnit, int i) const;

    // ֱ�ӷ��ʶ��㻺��������,channel count��floatԪ������,
    // ���������ֽ���ΪGetChannelCount()*sizeof(float).
    inline int GetChannelCount(void) const;
    inline float* GetData(void);
    inline const float* GetData(void) const;

    // û�з�Χ���,�û�����ȷ����Ҫ���ʵĶ������Դ����Ҳ�����channel��Χ.
    Vector3f& Position3(int i);
    Vector3f Position3(int i) const;
    Vector3f& Normal3(int i);
    Vector3f Normal3(int i) const;
    ColorRGB& Color3(int iUnit, int i);
    ColorRGB Color3(int iUnit, int i) const;
    ColorRGBA& Color4(int iUnit, int i);
    ColorRGBA Color4(int iUnit, int i) const;
    float& TCoord1(int iUnit, int i);
    float TCoord1(int iUnit, int i) const;
    SEVector2f& TCoord2(int iUnit, int i);
    SEVector2f TCoord2(int iUnit, int i) const;
    Vector3f& TCoord3(int iUnit, int i);
    Vector3f TCoord3(int iUnit, int i) const;
    Vector4f& TCoord4(int iUnit, int i);
    Vector4f TCoord4(int iUnit, int i) const;

    // �������ݵ�ǰVB����һ����vertex program inputs���ݵ���������,
    // rCompatible����ü�������,���rCompatible�����ָ��,��˺�����̬Ϊ������ڴ�,
    // ������������ɾ�����ڴ�,Ҳ���Դ���ǿ�ָ��,ǰ�������Ѿ�֪������������С.
    //
    // bPackARGBĿǰ���DirectX9,��ΪDirectX9��ҪARGB(8-bit,��32-bit)��ɫͨ��,
    // ������OpenGL��[0,1]���両��ֵ��ɫͨ��(32-bit,��128-bit),
    // DirectX10Ҳ֧��[0,1]���両��ֵ��ɫͨ��(32-bit,��128-bit).
    // ���bPackARGBΪtrueʱΪDirectX9,����ΪOpenGL/DirectX10/software renderers,
    // ��������Ҫ֧��������ʽ,����չ�˱�־Ϊ��״̬��־.
    void BuildCompatibleArray(const Attributes& rIAttributes, bool bPackARGB,
        int& rChannels, float*& rCompatible) const;

    // �û����������ö����"active count",ʹ�ô˺���������,
    // ����û�иı�ʵ�����ݴ洢,ֻ������m_iVertexCount,
    // �û������α���ʵ�ʶ�����Ŀ,����һϵ�в�����ɺ�ָ�ʵ�ʶ�����Ŀ,
    // active count���ܳ���ʵ�ʶ�����Ŀ.
    inline void SetVertexCount(int iVCount);

private:
    // streaming support
    VertexBuffer(void);

    // ��������.
    Attributes m_Attributes;

    // ÿ�������С,��λ��"float".
    int m_iVertexSize;

    // ������.
    int m_iVertexCount;

    // ���㻺��������.
    int m_iChannelCount;  // = m_iVertexCount*m_iVertexSize
    float* m_pChannel;
};

typedef SESmartPointer<VertexBuffer> VertexBufferPtr;

#include "SEVertexBuffer.inl"

}

#endif
