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

#ifndef Swing_StringTree_H
#define Swing_StringTree_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SELine3.h"
#include "SEMatrix2.h"
#include "SEMatrix3.h"
#include "SEMatrix4.h"
#include "SEPlane3.h"
#include "SEQuaternion.h"

namespace Swing
{

class BoundingVolume;
class ColorRGB;
class ColorRGBA;
class RTTI;

//----------------------------------------------------------------------------
// ����:�ַ�������
// ˵��:
// ����:Sun Che
// ʱ��:20080311
//----------------------------------------------------------------------------
class SE_FOUNDATION_API StringTree
{
public:
    StringTree(void);
    ~StringTree(void);

    // �Լ���strings
    inline int GetStringCount(void) const;
    inline void SetString(int i, char* pString);
    inline char* GetString(int i);
    inline void Append(char* pString);

    // �ӽڵ�
    inline int GetChildCount(void) const;
    inline void SetChild(int i, StringTree* pChild);
    inline StringTree* GetChild(int i);
    inline void Append(StringTree* pChild);

    // streaming
    bool Save(const char* pFileName, int iTabSize = 4); // iTabSize��ÿ�������Ĳ�����

    static void FormatFloat(float fValue, size_t uiStringSize, char* pString);
    static void FormatDouble(double dValue, size_t uiStringSize, char* pString);

private:
    // streaming(�ݹ�)
    void Save(FILE* pFile, int iLevel, int iTabSize);

    std::vector<char*> m_Strings;
    std::vector<StringTree*> m_Children;
};

// ���ñ������͵��ַ������ɺ���
SE_FOUNDATION_API char* Format(const RTTI* pRTTI, const char* pName);
SE_FOUNDATION_API char* Format(const char* pString);
SE_FOUNDATION_API char* Format(const char* pPrefix, bool);
SE_FOUNDATION_API char* Format(const char* pPrefix, char);
SE_FOUNDATION_API char* Format(const char* pPrefix, unsigned char);
SE_FOUNDATION_API char* Format(const char* pPrefix, short);
SE_FOUNDATION_API char* Format(const char* pPrefix, unsigned short);
SE_FOUNDATION_API char* Format(const char* pPrefix, int);
SE_FOUNDATION_API char* Format(const char* pPrefix, unsigned int);
SE_FOUNDATION_API char* Format(const char* pPrefix, long);
SE_FOUNDATION_API char* Format(const char* pPrefix, unsigned long);
SE_FOUNDATION_API char* Format(const char* pPrefix, float);
SE_FOUNDATION_API char* Format(const char* pPrefix, double);
SE_FOUNDATION_API char* Format(const char* pPrefix, void*);
SE_FOUNDATION_API char* Format(const char* pPrefix, const char*);

// �����ñ������͵��ַ������ɺ���
SE_FOUNDATION_API char* Format(const char* pPrefix, const BoundingVolume*);
SE_FOUNDATION_API char* Format(const char* pPrefix, const ColorRGBA&);
SE_FOUNDATION_API char* Format(const char* pPrefix, const ColorRGB&);
SE_FOUNDATION_API char* Format(const char* pPrefix, const Line3f&);
SE_FOUNDATION_API char* Format(const char* pPrefix, const Matrix2f&);
SE_FOUNDATION_API char* Format(const char* pPrefix, const Matrix3f&);
SE_FOUNDATION_API char* Format(const char* pPrefix, const Matrix4f&);
SE_FOUNDATION_API char* Format(const char* pPrefix, const Plane3f&);
SE_FOUNDATION_API char* Format(const char* pPrefix, const Quaternionf&);
SE_FOUNDATION_API char* Format(const char* pPrefix, const Vector2f&);
SE_FOUNDATION_API char* Format(const char* pPrefix, const Vector3f&);
SE_FOUNDATION_API char* Format(const char* pPrefix, const Vector4f&);

template <class T>
StringTree* Format(const char* pTitle, int iCount, const T* pValue);

#include "SEStringTree.inl"

}

#endif
