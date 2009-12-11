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

#include "SEFoundationPCH.h"
#include "SEObject.h"
#include "SEStream.h"

using namespace Swing;

const char* Stream::ms_pTopLevel = "Top Level";

//----------------------------------------------------------------------------
Stream::Stream()
    :
    m_Map(1024)
{
    m_iBufferSize = 0;
    m_iBufferNext = 0;
    m_pBuffer = 0;
}
//----------------------------------------------------------------------------
Stream::~Stream()
{
    m_Map.RemoveAll();
    m_Ordered.clear();
    RemoveAll();
}
//----------------------------------------------------------------------------
bool Stream::Insert(Object* pObject)
{
    if( pObject )
    {
        // ֻ�ܲ���һ��
        for( int i = 0; i < (int)m_pTopLevel.size(); i++ )
        {
            if( pObject == m_pTopLevel[i] )
            {
                return false;
            }
        }

        m_pTopLevel.push_back(pObject);
        pObject->IncrementReferences();

        return true;
    }

    return false;
}
//----------------------------------------------------------------------------
bool Stream::Remove(Object* pObject)
{
    if( pObject )
    {
        std::vector<Object*>::iterator pIter = m_pTopLevel.begin();
        for( /**/; pIter != m_pTopLevel.end(); pIter++ )
        {
            if( pObject == *pIter )
            {
                m_pTopLevel.erase(pIter);
                pObject->DecrementReferences();

                return true;
            }
        }
    }

    return false;
}
//----------------------------------------------------------------------------
void Stream::RemoveAll()
{
    for( int i = 0; i < (int)m_pTopLevel.size(); i++ )
    {
        m_pTopLevel[i]->DecrementReferences();
    }

    m_pTopLevel.clear();
}
//----------------------------------------------------------------------------
int Stream::GetObjectCount()
{
    return (int)m_pTopLevel.size();
}
//----------------------------------------------------------------------------
Object* Stream::GetObjectAt(int i) const
{
    if( 0 <= i && i < (int)m_pTopLevel.size() )
    {
        return m_pTopLevel[i];
    }

    return 0;
}
//----------------------------------------------------------------------------
bool Stream::IsTopLevel(const Object* pObject)
{
    for( int i = 0; i < (int)m_pTopLevel.size(); i++ )
    {
        if( pObject == m_pTopLevel[i] )
        {
            return true;
        }
    }

    return false;
}
//----------------------------------------------------------------------------
bool Stream::Load(char* pBuffer, int iSize)
{
    if( !Object::ms_pFactory )
    {
        // �������ȴ�������������
        SE_ASSERT( false );

        return false;
    }

    m_Map.RemoveAll();
    m_Ordered.clear();
    // ���֮ǰ����top level object
    RemoveAll();

    // ��ʼ���ڲ�stream buffer,m_iBufferNext�Ѿ���ʼ��Ϊ0
    m_iBufferSize = iSize;
    m_pBuffer = pBuffer;

    // װ��unique object�б�
    std::string StrTopLevel(ms_pTopLevel);
    Object* pObject;
    while( m_iBufferNext < m_iBufferSize )
    {
        // ��������"Top Level"��RTTI name
        std::string StrTemp;
        Read(StrTemp);
        bool bTopLevel = (StrTemp == StrTopLevel);
        if( bTopLevel )
        {
            // ������ȡRTTI name
            Read(StrTemp);
        }

        // RTTI name������"Swing.xxx"
        SE_ASSERT( ((int)StrTemp.length()) >= 7 );

        // ����RTTI name,�ҵ�������ȡ��object�Ĺ�������
        FactoryFunction* pFactory = Object::ms_pFactory->Find(StrTemp);
        if( !pFactory )
        {
            // �޷��ҵ������͵Ĺ�������,
            // ����ȷ��������ù�SE_REGISTER_STREAM(someclass)
            SE_ASSERT( false );

            return false;
        }

        // װ�ظ�object
        pObject = (*pFactory)(*this);

        // ΪӦ�ó���ʹ�ö�׷������top level object,
        // ����top level�����
        if( bTopLevel )
        {
            Insert(pObject);
        }
    }

    // link�׶�,Ϊ��������Ķ��������ӹ�ϵ,����ԭʼscene graph,
    // ά��һ������Ӧ�ó���ʹ�õĶ����б�,
    // ���Ե���Ψһ����hash��,Ϊ���ж��������ӹ�ϵ
    Link** ppLink = m_Map.GetFirst(&pObject);
    while( ppLink )
    {
        pObject = (*ppLink)->GetObject();  // ��ȡ�ö���ǰ�ڴ��ַ
        pObject->Link(*this, *ppLink);     // Ϊ�ö��������ӹ�ϵ
        m_Ordered.push_back(pObject);      // ��ʱ��˳��Ϊ����hash��ķ���˳��
        ppLink = m_Map.GetNext(&pObject);
    }

    // ������Ҫlink����
    ppLink = m_Map.GetFirst(&pObject);
    while( ppLink )
    {
        SE_DELETE *ppLink;
        ppLink = m_Map.GetNext(&pObject);
    }

    // �����ڲ�����
    m_pBuffer = 0;
    m_iBufferSize = 0;
    m_iBufferNext = 0;

    return true;
}
//----------------------------------------------------------------------------
bool Stream::Save(char*& rpBuffer, int& riSize)
{
    m_Map.RemoveAll();
    m_Ordered.clear();

    // register�׶�,
    // ����Ψһ�����б�
    int i;
    for( i = 0; i < (int)m_pTopLevel.size(); i++ )
    {
        m_pTopLevel[i]->Register(*this);
    }

    // ͳ�����ж���Ĵ����ֽ���
    std::string StrTopLevel(ms_pTopLevel);
    int iTLGetDiskUsed = sizeof(int) + (int)StrTopLevel.length();
    m_iBufferSize = ((int)m_pTopLevel.size())*iTLGetDiskUsed;
    for( i = 0; i < (int)m_Ordered.size(); i++ )
    {
        m_iBufferSize += m_Ordered[i]->GetDiskUsed(StreamVersion::CURRENT);
    }

    // ���������ڴ�buffer
    m_pBuffer = SE_NEW char[m_iBufferSize];
    m_iBufferNext = 0;

    // ����Ψһ�����б��ڴ�buffer��
    for( i = 0; i < (int)m_Ordered.size(); i++ )
    {
        const Object* pObject = m_Ordered[i];
        if( IsTopLevel(pObject) )
        {
            Write(StrTopLevel);
        }
        pObject->Save(*this);
    }

    // ȷ���ڴ�buffer����ֽ���������󱨸������Ǻ�
    SE_ASSERT( m_iBufferNext == m_iBufferSize );
    if( m_iBufferNext != m_iBufferSize )
    {
        // ���ֲ��Ǻ�,
        // Ҫô��ĳ������󱨸�����������ֽ���(ͨ��GetDiskUsed����),
        // Ҫô��ĳ����д���Ա����ʱ����
        SE_ASSERT( false );

        SE_DELETE[] m_pBuffer;
        rpBuffer = 0;
        riSize = 0;

        return false;
    }

    // ���ڴ�buffer����Ȩ���ݸ�������������
    rpBuffer = m_pBuffer;
    riSize = m_iBufferSize;

    // �����ڲ�����
    m_pBuffer = 0;
    m_iBufferSize = 0;
    m_iBufferNext = 0;

    return true;
}
//----------------------------------------------------------------------------
bool Stream::Load(const char* pFileName)
{
    // �Ӵ������뵽�ڴ�
    char* pBuffer;
    int iSize;
    if( !System::SE_Load(pFileName, pBuffer, iSize) )
    {
        return false;
    }

    // ��ȡ�ļ��汾
    if( iSize < StreamVersion::LENGTH )
    {
        // �ļ������ڻ�û���㹻��С
        SE_DELETE[] pBuffer;

        return false;
    }
    m_Version = StreamVersion(pBuffer);
    if( !m_Version.IsValid() )
    {
        // TO DO.  This should not abort if the input stream version is less
        // than current version.

        SE_DELETE[] pBuffer;

        return false;
    }

    // ƫ�Ƶ�������,�ع�scene graph
    iSize -= StreamVersion::LENGTH;
    if( !Load(pBuffer+StreamVersion::LENGTH, iSize) )
    {
        SE_DELETE[] pBuffer;

        return false;
    }

    SE_DELETE[] pBuffer;

    return true;
}
//----------------------------------------------------------------------------
bool Stream::Save(const char* pFileName)
{
    // �����ļ��汾������
    if( !System::SE_Save(pFileName, StreamVersion::LABEL, StreamVersion::LENGTH) )
    {
        return false;
    }

    // �ֽ�scene graph���洢���ڴ�buffer
    char* pBuffer;
    int iSize;
    if( !Save(pBuffer, iSize) )
    {
        return false;
    }

    // ��scene graph���ڴ�洢������
    if( !System::SE_Append(pFileName, pBuffer, iSize) )
    {
        SE_DELETE[] pBuffer;

        return false;
    }

    SE_DELETE[] pBuffer;

    return true;
}
//----------------------------------------------------------------------------
bool Stream::SaveText(const char* pFileName, int iTabSize)
{
    StringTree Root;
    Root.Append(Format(pFileName));

    const int iCount = GetObjectCount();
    for( int i = 0; i < iCount; i++ )
    {
        Object* pObject = m_pTopLevel[i];
        SE_ASSERT( pObject );
        Root.Append(pObject->SaveStrings());
    }

    return Root.Save(pFileName, iTabSize);
}
//----------------------------------------------------------------------------
bool Stream::InsertInMap(Object* pObject, Link* pLink)
{
    return m_Map.Insert(pObject, pLink);
}
//----------------------------------------------------------------------------
Object* Stream::GetFromMap(Object* pLinkID)
{
    Link** ppLink = m_Map.Find(pLinkID);

    return (ppLink ? (*ppLink)->GetObject() : 0);
}
//----------------------------------------------------------------------------
void Stream::InsertInOrdered(Object* pObject)
{
    m_Ordered.push_back(pObject);
}
//----------------------------------------------------------------------------
int Stream::GetDiskUsed() const
{
    int i;
    for( i = 0; i < (int)m_pTopLevel.size(); i++ )
    {
        m_pTopLevel[i]->Register((Stream&)*this);
    }

    int iSize = 0;
    for( i = 0; i < (int)m_Ordered.size(); i++ )
    {
        iSize += m_Ordered[i]->GetDiskUsed(StreamVersion::CURRENT);
    }

    m_Map.RemoveAll();
    m_Ordered.clear();

    return iSize;
}
//----------------------------------------------------------------------------
Stream::Link::Link(Object* pObject)
{
    m_pObject = pObject;
    m_iCurrent = 0;
}
//----------------------------------------------------------------------------
void Stream::Link::Add(Object* pLinkID)
{
    m_LinkID.push_back(pLinkID);
}
//----------------------------------------------------------------------------
void Stream::Link::SetObject(Object* pObject)
{
    m_pObject = pObject;
}
//----------------------------------------------------------------------------
Object* Stream::Link::GetObject()
{
    return m_pObject;
}
//----------------------------------------------------------------------------
int Stream::Link::GetCount() const
{
    return (int)m_LinkID.size();
}
//----------------------------------------------------------------------------
Object* Stream::Link::GetLinkID()
{
    SE_ASSERT( m_iCurrent < (int)m_LinkID.size() );

    return m_LinkID[m_iCurrent++];
}
//----------------------------------------------------------------------------
void Stream::Read(Object*& rpValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, 1, &rpValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(int iCount, Object** ppValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, iCount,
        ppValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(bool& rValue)
{
    char* pSrc = m_pBuffer + m_iBufferNext;
    m_iBufferNext++;

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );

    rValue = (*pSrc != 0 ? true : false);
}
//----------------------------------------------------------------------------
void Stream::Read(int iCount, bool* pValue)
{
    if( iCount > 0 )
    {
        char* pSrc = m_pBuffer + m_iBufferNext;
        m_iBufferNext += iCount;

        SE_ASSERT( m_iBufferNext <= m_iBufferSize );

        for( int i = 0; i < iCount; i++ )
        {
            pValue[i] = (pSrc[i] != 0 ? true : false);
        }
    }
}
//----------------------------------------------------------------------------
void Stream::Read(char& rValue)
{
    char* pSrc = m_pBuffer + m_iBufferNext;
    m_iBufferNext++;

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );

    rValue = *pSrc;
}
//----------------------------------------------------------------------------
void Stream::Read(int iCount, char* pValue)
{
    if( iCount > 0 )
    {
        char* pSrc = m_pBuffer + m_iBufferNext;
        m_iBufferNext += iCount;

        SE_ASSERT( m_iBufferNext <= m_iBufferSize );

        size_t uiSize = (size_t)iCount;
        System::SE_Memcpy(pValue, uiSize, pSrc, uiSize);
    }
}
//----------------------------------------------------------------------------
void Stream::Read(unsigned char& rValue)
{
    unsigned char* pSrc = (unsigned char*)(m_pBuffer + m_iBufferNext);
    m_iBufferNext++;

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );

    rValue = *pSrc;
}
//----------------------------------------------------------------------------
void Stream::Read(int iCount, unsigned char* pValue)
{
    if( iCount > 0 )
    {
        unsigned char* pSrc = (unsigned char*)(m_pBuffer + m_iBufferNext);
        m_iBufferNext += iCount;

        SE_ASSERT( m_iBufferNext <= m_iBufferSize );

        size_t uiSize = (size_t)iCount;
        System::SE_Memcpy(pValue, uiSize, pSrc, uiSize);
    }
}
//----------------------------------------------------------------------------
void Stream::Read (short& rValue)
{
    m_iBufferNext += System::SE_Read2le(m_pBuffer+m_iBufferNext,1,&rValue);
    SE_ASSERT(m_iBufferNext <= m_iBufferSize);
}
//----------------------------------------------------------------------------
void Stream::Read (int iCount, short* pValue)
{
    m_iBufferNext += System::SE_Read2le(m_pBuffer+m_iBufferNext,iCount,
        pValue);
    SE_ASSERT(m_iBufferNext <= m_iBufferSize);
}
//----------------------------------------------------------------------------
void Stream::Read(unsigned short& rValue)
{
    m_iBufferNext += System::SE_Read2le(m_pBuffer+m_iBufferNext, 1, &rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(int iCount, unsigned short* pValue)
{
    m_iBufferNext += System::SE_Read2le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(int& rValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, 1, &rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(int iCount, int* pValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(unsigned int& rValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, 1, &rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(int iCount, unsigned int* pValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(long& rValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, 1, &rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(int iCount, long* pValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(unsigned long& rValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, 1, &rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(int iCount, unsigned long* pValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(float& rValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, 1, &rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(int iCount, float* pValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(double& rValue)
{
    m_iBufferNext += System::SE_Read8le(m_pBuffer+m_iBufferNext, 1, &rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(int iCount, double* pValue)
{
    m_iBufferNext += System::SE_Read8le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(std::string& rValue)
{
    int iLength;
    Read(iLength);

    if( iLength > 0 )
    {
        char* pText = m_pBuffer + m_iBufferNext;
        m_iBufferNext += iLength;
        SE_ASSERT( m_iBufferNext <= m_iBufferSize );

        rValue.assign(pText, iLength);
    }
    else
    {
        rValue.clear();
    }
}
//----------------------------------------------------------------------------
void Stream::Read(int iCount, std::string* pValue)
{
    for( int i = 0; i < iCount; i++ )
    {
        Read(pValue[i]);
    }
}
//----------------------------------------------------------------------------
void Stream::Read(ColorRGB& rValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, 3, 
        (float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(int iCount, ColorRGB* pValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, 3*iCount, 
        (float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(ColorRGBA& rValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, 4, 
        (float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(int iCount, ColorRGBA* pValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, 4*iCount, 
        (float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(Matrix2f& rValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, 4, 
        (float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(int iCount, Matrix2f* pValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, 4*iCount, 
        (float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(Matrix3f& rValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, /*12*/9, 
        (float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(int iCount, Matrix3f* pValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, /*12*/9*iCount, 
        (float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(Matrix4f& rValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, 16, 
        (float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(int iCount, Matrix4f* pValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, 16*iCount, 
        (float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(Plane3f& rValue)
{
    Read(rValue.Normal);
    Read(rValue.Constant);
}
//----------------------------------------------------------------------------
void Stream::Read(int iCount, Plane3f* pValue)
{
    for( int i = 0; i < iCount; i++ )
    {
        Read(pValue[i].Normal);
        Read(pValue[i].Constant);
    }
}
//----------------------------------------------------------------------------
void Stream::Read(Quaternionf& rValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, 4, 
        (float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(int iCount, Quaternionf* pValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, 4*iCount, 
        (float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(Vector2f& rValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, 2, 
        (float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(int iCount, Vector2f* pValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, 2*iCount, 
        (float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(Vector3f& rValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, 3, 
        (float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(int iCount, Vector3f* pValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, 3*iCount, 
        (float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(Vector4f& rValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, 4, 
        (float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(int iCount, Vector4f* pValue)
{
    m_iBufferNext += System::SE_Read4le(m_pBuffer+m_iBufferNext, 4*iCount, 
        (float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Read(Transformation& rValue)
{
    Read(rValue.m_Matrix);
    Read(rValue.m_Translate);
    Read(rValue.m_Scale);
    Read(rValue.m_bIsIdentity);
    Read(rValue.m_bIsSRMatrix);
    Read(rValue.m_bIsUniformScale);
}
//----------------------------------------------------------------------------
void Stream::Read(int iCount, Transformation* pValue)
{
    for( int i = 0; i < iCount; i++ )
    {
        Read(pValue[i].m_Matrix);
        Read(pValue[i].m_Translate);
        Read(pValue[i].m_Scale);
        Read(pValue[i].m_bIsIdentity);
        Read(pValue[i].m_bIsSRMatrix);
        Read(pValue[i].m_bIsUniformScale);
    }
}
//----------------------------------------------------------------------------
void Stream::Write(const Object* pValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, 1, &pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, Object** const ppValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, iCount, 
        ppValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(const SmartPointer<Object>& rspValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, 1, &rspValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, const SmartPointer<Object>* pspValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, iCount, 
        pspValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(bool bValue)
{
    char* pDst = m_pBuffer + m_iBufferNext;
    m_iBufferNext++;

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );

    *pDst = (bValue ? 1 : 0);
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, const bool* pValue)
{
    if( iCount > 0 )
    {
        char* pDst = m_pBuffer + m_iBufferNext;
        m_iBufferNext += iCount;

        SE_ASSERT( m_iBufferNext <= m_iBufferSize );

        for( int i = 0; i < iCount; i++ )
        {
            pDst[i] = (pValue[i] ? 1 : 0);
        }
    }
}
//----------------------------------------------------------------------------
void Stream::Write(char cValue)
{
    char* pDst = m_pBuffer + m_iBufferNext;
    m_iBufferNext++;

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );

    *pDst = cValue;
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, const char* pValue)
{
    if( iCount > 0 )
    {
        char* pDst = m_pBuffer + m_iBufferNext;
        m_iBufferNext += iCount;

        SE_ASSERT( m_iBufferNext <= m_iBufferSize );

        size_t uiSize = (size_t)iCount;
        System::SE_Memcpy(pDst, uiSize, pValue, uiSize);
    }
}
//----------------------------------------------------------------------------
void Stream::Write(unsigned char ucValue)
{
    unsigned char* pDst = (unsigned char*)(m_pBuffer + m_iBufferNext);
    m_iBufferNext++;

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );

    *pDst = ucValue;
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, const unsigned char* pValue)
{
    if( iCount > 0 )
    {
        unsigned char* pDst = (unsigned char*)(m_pBuffer + m_iBufferNext);
        m_iBufferNext += iCount;

        SE_ASSERT( m_iBufferNext <= m_iBufferSize );

        size_t uiSize = (size_t)iCount;
        System::SE_Memcpy(pDst, uiSize, pValue, uiSize);
    }
}
//----------------------------------------------------------------------------
void Stream::Write(short sValue)
{
    m_iBufferNext += System::SE_Write2le(m_pBuffer+m_iBufferNext, 1, &sValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, const short* pValue)
{
    m_iBufferNext += System::SE_Write2le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(unsigned short usValue)
{
    m_iBufferNext += System::SE_Write2le(m_pBuffer+m_iBufferNext, 1, &usValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, const unsigned short* pValue)
{
    m_iBufferNext += System::SE_Write2le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(int iValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, 1, &iValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, const int* pValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(unsigned int uiValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, 1, &uiValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, const unsigned int* pValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(long lValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, 1, &lValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, const long* pValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(unsigned long ulValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, 1, &ulValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, const unsigned long* pValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(float fValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, 1, &fValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, const float* pValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(double dValue)
{
    m_iBufferNext += System::SE_Write8le(m_pBuffer+m_iBufferNext, 1, &dValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, const double* pValue)
{
    m_iBufferNext += System::SE_Write8le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(const std::string& rValue)
{
    Write((int)rValue.length());
    Write((int)rValue.length(), rValue.c_str());
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, const std::string* pValue)
{
    for( int i = 0; i < iCount; i++ )
    {
        Write(pValue[i]);
    }
}
//----------------------------------------------------------------------------
void Stream::Write(const ColorRGB& rValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, 3, 
        (const float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, const ColorRGB* pValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, 3*iCount, 
        (const float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(const ColorRGBA& rValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, 4, 
        (const float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, const ColorRGBA* pValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, 4*iCount, 
        (const float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(const Matrix2f& rValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, 4, 
        (const float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, const Matrix2f* pValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, 4*iCount, 
        (const float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(const Matrix3f& rValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, /*12*/9, 
        (const float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, const Matrix3f* pValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, /*12*/9*iCount, 
        (const float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(const Matrix4f& rValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, 16, 
        (const float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, const Matrix4f* pValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, 16*iCount, 
        (const float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(const Plane3f& rValue)
{
    Write(rValue.Normal);
    Write(rValue.Constant);
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, const Plane3f* pValue)
{
    for( int i = 0; i < iCount; i++ )
    {
        Write(pValue[i].Normal);
        Write(pValue[i].Constant);
    }
}
//----------------------------------------------------------------------------
void Stream::Write(const Quaternionf& rValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, 4, 
        (const float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, const Quaternionf* pValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, 4*iCount, 
        (const float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(const Vector2f& rValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, 2, 
        (const float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, const Vector2f* pValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, 2*iCount, 
        (const float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(const Vector3f& rValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, 3, 
        (const float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, const Vector3f* pValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, 3*iCount, 
        (const float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(const Vector4f& rValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, 4, 
        (const float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, const Vector4f* pValue)
{
    m_iBufferNext += System::SE_Write4le(m_pBuffer+m_iBufferNext, 4*iCount, 
        (const float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void Stream::Write(const Transformation& rValue)
{
    Write(rValue.m_Matrix);
    Write(rValue.m_Translate);
    Write(rValue.m_Scale);
    Write(rValue.m_bIsIdentity);
    Write(rValue.m_bIsSRMatrix);
    Write(rValue.m_bIsUniformScale);
}
//----------------------------------------------------------------------------
void Stream::Write(int iCount, const Transformation* pValue)
{
    for( int i = 0; i < iCount; i++ )
    {
        Write(pValue[i].m_Matrix);
        Write(pValue[i].m_Translate);
        Write(pValue[i].m_Scale);
        Write(pValue[i].m_bIsIdentity);
        Write(pValue[i].m_bIsSRMatrix);
        Write(pValue[i].m_bIsUniformScale);
    }
}
//----------------------------------------------------------------------------
