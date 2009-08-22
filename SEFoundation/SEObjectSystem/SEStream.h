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

#ifndef Swing_Stream_H
#define Swing_Stream_H

#include "SEFoundationLIB.h"
#include "SESystem.h"
#include "SEStreamVersion.h"
#include "SEStreamMCR.h"
#include "SESmartPointer.h"
#include "SEHashTable.h"

// ��Ҫ��streaming��class��Ա
#include "SEColorRGB.h"
#include "SEColorRGBA.h"
#include "SEMatrix2.h"
#include "SEMatrix3.h"
#include "SEMatrix4.h"
#include "SEPlane3.h"
#include "SEQuaternion.h"
#include "SETransformation.h"
#include "SEVector2.h"
#include "SEVector3.h"
#include "SEVector4.h"
#include "SEString.h"

namespace Swing
{

class Object;

//----------------------------------------------------------------------------
// ����:���л���
// ˵��:
// ����:Sun Che
// ʱ��:20080329
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Stream
{
public:
    Stream(void);
    ~Stream(void);

    // top level object����
    bool Insert(Object* pObject);
    bool Remove(Object* pObject);
    void RemoveAll(void);
    int GetObjectCount(void);
    Object* GetObjectAt(int i) const;
    bool IsTopLevel(const Object* pObject);

    // �ڴ�load/save
    bool Load(char* pBuffer, int iSize);
    bool Save(char*& rpBuffer, int& riSize);

    // �ļ�load/save
    bool Load(const char* pFileName);
    bool Save(const char* pFileName);

    // ��unique object�б�ķ���,�б�Ԫ��˳������register�׶ε�������ȱ���˳��,
    // ������load��save���ú���ʸ��б�,
    // �����load֮�����,��ͨ��GetOrderedObject���б�Ԫ�ص��޸�����Ч��,
    // �����save֮�����,��ͨ��GetOrderedObject���б�Ԫ�ص��޸Ľ�����Ӱ��
    // save���,��ֻ�ı�ԭʼ����ʵ��.
    inline int GetOrderedCount(void) const;
    inline Object* GetOrderedObject(int i) const;

    // ֧��disk usage
    int GetDiskUsed(void) const;

    // file save (ASCII text)
    bool SaveText(const char* pFileName, int iTabSize = 4);

    // �������һ���ļ�load�������ļ��汾,
    // ���û��load���ļ�,�򷵻�ֵΪ-1
    inline StreamVersion GetVersion(void) const;

    // link��
    class SE_FOUNDATION_API Link
    {
    public:
        Link(Object* pObject);

	public:
        void SetObject(Object* pObject);
        Object* GetObject(void);

        int GetCount(void) const;
        Object* GetLinkID(void);

        void Add(Object* pLinkID);

    protected:
        Object* m_pObject;
        int m_iCurrent;
        std::vector<Object*> m_LinkID;
    };

protected:
    friend class Object;

    bool InsertInMap(Object* pObject, Link* pLink);
    void InsertInOrdered(Object* pObject);

    // ���һ���ļ���ȡ�İ汾
    StreamVersion m_Version;

    // top level object�б�
    std::vector<Object*> m_pTopLevel;

    // saveʱʹ�õĶ���ע���,
    // ʹ�ô˱��Ŀ�����ڿ��ټ������ö����Ƿ��Ѿ�ע���
    mutable HashTable<Object*, Link*> m_Map;

    // Ψһ�����б�,��scene graph��register�׶�����,
    // ����ȷ���������������ȵı���˳��洢,
    // ����������hash��˳��洢(m_Map),
    // �����˳�򽫻���ÿ�γ�������ʱ�ڴ��ַ�����������˳��Ψһ
    mutable std::vector<Object*> m_Ordered;

    // read/write��������Ӧ�����ڴ�buffer,
    // Ŀ�����ڼ��ٴ��̷�����д��������
    int m_iBufferSize, m_iBufferNext;
    char* m_pBuffer;

    static const char* ms_pTopLevel;

// �ڲ�ʹ��
public:
    // ֧��linking
    Object* GetFromMap(Object* pLinkID);

    // ֧��debugging
    inline int GetBufferSize(void) const;
    inline int GetBufferNext(void) const;

    void Read(Object*& rpValue);
    void Read(int iCount, Object** ppValue);
    void Read(bool& rValue);
    void Read(int iCount, bool* pValue);
    void Read(char& rValue);
    void Read(int iCount, char* pValue);
    void Read(unsigned char& rValue);
    void Read(int iCount, unsigned char* pValue);
    void Read(short& rValue);
    void Read(int iCount, short* pValue);
    void Read(unsigned short& rValue);
    void Read(int iCount, unsigned short* pValue);
    void Read(int& rValue);
    void Read(int iCount, int* pValue);
    void Read(unsigned int& rValue);
    void Read(int iCount, unsigned int* pValue);
    void Read(long& rValue);
    void Read(int iCount, long* pValue);
    void Read(unsigned long& rValue);
    void Read(int iCount, unsigned long* pValue);
    void Read(float& rValue);
    void Read(int iCount, float* pValue);
    void Read(double& rValue);
    void Read(int iCount, double* pValue);

    void Read(String& rValue);
    void Read(int iCount, String* pValue);
    void Read(ColorRGB& rValue);
    void Read(int iCount, ColorRGB* pValue);
    void Read(ColorRGBA& rValue);
    void Read(int iCount, ColorRGBA* pValue);
    void Read(Matrix2f& rValue);
    void Read(int iCount, Matrix2f* pValue);
    void Read(Matrix3f& rValue);
    void Read(int iCount, Matrix3f* pValue);
    void Read(Matrix4f& rValue);
    void Read(int iCount, Matrix4f* pValue);
    void Read(Plane3f& rValue);
    void Read(int iCount, Plane3f* pValue);
    void Read(Quaternionf& rValue);
    void Read(int iCount, Quaternionf* pValue);
    void Read(Vector2f& rValue);
    void Read(int iCount, Vector2f* pValue);
    void Read(Vector3f& rValue);
    void Read(int iCount, Vector3f* pValue);
    void Read(Vector4f& rValue);
    void Read(int iCount, Vector4f* pValue);
    void Read(Transformation& rValue);
    void Read(int iCount, Transformation* pValue);

    void Write(const Object* pValue);
    void Write(int iCount, Object** const ppValue);
    void Write(const SmartPointer<Object>& rspValue);
    void Write(int iCount, const SmartPointer<Object>* pspValue);
    void Write(bool bValue);
    void Write(int iCount, const bool* pValue);
    void Write(char cValue);
    void Write(int iCount, const char* pValue);
    void Write(unsigned char ucValue);
    void Write(int iCount, const unsigned char* pValue);
    void Write(short sValue);
    void Write(int iCount, const short* pValue);
    void Write(unsigned short usValue);
    void Write(int iCount, const unsigned short* pValue);
    void Write(int iValue);
    void Write(int iCount, const int* pValue);
    void Write(unsigned int uiValue);
    void Write(int iCount, const unsigned int* pValue);
    void Write(long lValue);
    void Write(int iCount, const long* pValue);
    void Write(unsigned long ulValue);
    void Write(int iCount, const unsigned long* pValue);
    void Write(float fValue);
    void Write(int iCount, const float* pValue);
    void Write(double dValue);
    void Write(int iCount, const double* pValue);

    void Write(const String& rValue);
    void Write(int iCount, const String* pValue);
    void Write(const ColorRGB& rValue);
    void Write(int iCount, const ColorRGB* pValue);
    void Write(const ColorRGBA& rValue);
    void Write(int iCount, const ColorRGBA* pValue);
    void Write(const Matrix2f& rValue);
    void Write(int iCount, const Matrix2f* pValue);
    void Write(const Matrix3f& rValue);
    void Write(int iCount, const Matrix3f* pValue);
    void Write(const Matrix4f& rValue);
    void Write(int iCount, const Matrix4f* pValue);
    void Write(const Plane3f& rValue);
    void Write(int iCount, const Plane3f* pValue);
    void Write(const Quaternionf& rValue);
    void Write(int iCount, const Quaternionf* pValue);
    void Write(const Vector2f& rValue);
    void Write(int iCount, const Vector2f* pValue);
    void Write(const Vector3f& rValue);
    void Write(int iCount, const Vector3f* pValue);
    void Write(const Vector4f& rValue);
    void Write(int iCount, const Vector4f* pValue);
    void Write(const Transformation& rValue);
    void Write(int iCount, const Transformation* pValue);
};

typedef Object* (*FactoryFunction)(Stream&);

#include "SEStream.inl"

}

#endif
