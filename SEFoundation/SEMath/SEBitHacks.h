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

// Bit hacks are available at
//     http://graphics.stanford.edu/~seander/bithacks.html

#ifndef Swing_BitHacks_H
#define Swing_BitHacks_H

// һЩλ����������λ����ʵ�ֵ���ѧ����.
namespace Swing
{

// �ж�iValue�Ƿ�Ϊ����.
inline bool IsOdd(int iValue);

// �ж�iValue�Ƿ�Ϊż��.
inline bool IsEven(int iValue);

// �ж�iValue�Ƿ�Ϊ2^n.
inline bool IsPowerOfTwo(int iValue);

// uiPowerOfTwo = 2^n,��n.
inline unsigned int Log2OfPowerOfTwo(unsigned int uiPowerOfTwo);

// ��ָ��λ����Ϊ1.
inline void SetBitToOne(unsigned int& rValue, int i);

// ��ָ��λ����Ϊ0.
inline void SetBitToZero(unsigned int& rValue, int i);

#include "SEBitHacks.inl"

}

#endif
