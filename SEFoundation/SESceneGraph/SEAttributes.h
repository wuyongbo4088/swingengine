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

#ifndef Swing_Attributes_H
#define Swing_Attributes_H

#include "SEFoundationLIB.h"
#include "SESystem.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:������
// ˵��:����ָ��VB�����ʽʱ������FVF,���⻹����ָ��shader input output��������,
//      offsetΪָ��������VertexBuffer�е�float����ƫ����
// ����:Sun Che
// ʱ��:20080312
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Attributes
{
public:
    Attributes(void);
    ~Attributes(void);

    // ��ǰ֧�ֵĶ�������:
    //    p  = position
    //         3��4������ (xyz,xyzw)
    //    n  = normal vector
    //         3��4������ (xyz,xyzw)
    //    ci = color
    //         1��4������ (r,rg,rgb,rgba)
    //    ti = texture coordinate
    //         1��4������ (s,st,str,strq)
    // �������Ե��ڲ���֯˳��:
    // p, n, c0, c1, ..., t0, t1, ...

    void SetPositionChannels(int iPChannels);
    void SetNormalChannels(int iNChannels);
    void SetColorChannels(int iUnit, int iCChannels);
    void SetTCoordChannels(int iUnit, int iTChannels);

    // ��ȡ�������Ե�floatͨ������
    inline int GetChannelCount(void) const;

    // ����position��Ϣ
    inline int GetPositionOffset(void) const;
    inline int GetPositionChannels(void) const;
    inline bool HasPosition(void) const;

    // ����normal��Ϣ
    inline int GetNormalOffset(void) const;
    inline int GetNormalChannels(void) const;
    inline bool HasNormal(void) const;

    // ����color��Ϣ
    inline int GetMaxColors(void) const; // ����color
    int GetColorOffset(int iUnit) const;
    int GetColorChannels(int iUnit) const;
    bool HasColor(int iUnit) const;

    // ����texture coordinate��Ϣ.
    inline int GetMaxTCoords(void) const; // ����texture coord
    int GetTCoordOffset(int iUnit) const;
    int GetTCoordChannels(int iUnit) const;
    bool HasTCoord(int iUnit) const;

    // ��vertex program outputs��pixel program inputs���бȽ�
    bool Matches(const Attributes& rAttributes, bool bIncludeP, bool bIncludeN,
        bool bIncludeC, bool bIncludeT) const;

    // ��vertex buffer��vertex format��vertex program input��vertex format���бȽ�
    bool operator == (const Attributes& rAttributes) const;
    bool operator != (const Attributes& rAttributes) const;

private:
    void UpdateOffsets(void);

    // �������Ե�floatͨ������
    int m_iChannelCount;

    // positions
    int m_iPositionOffset;
    int m_iPositionChannels;

    // normals
    int m_iNormalOffset;
    int m_iNormalChannels;

    // colors
    std::vector<int> m_ColorOffset;
    std::vector<int> m_ColorChannels;

    // texture coordinates
    std::vector<int> m_TCoordOffset;
    std::vector<int> m_TCoordChannels;
};

#include "SEAttributes.inl"

}

#endif
