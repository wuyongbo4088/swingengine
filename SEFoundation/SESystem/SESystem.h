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

#ifndef Swing_System_H
#define Swing_System_H

// C++��׼��
#include <cassert>
#include <cctype>
#include <cfloat>
#include <cmath>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <fstream>

// STL �������ݽṹ
#include <algorithm>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEAssert.h"
#include "SEMemory.h"

namespace Swing
{

class String;
//----------------------------------------------------------------------------
// ����:ϵͳ��
// ˵��:
// ����:Sun Che
// ʱ��:20080202
//----------------------------------------------------------------------------
class SE_FOUNDATION_API System
{
public:
    // ����һ��C�����������ݵ��ֽ��ڴ��ŷ�λ��(iSize = 2,4,8)
    static void SE_SwapBytes(int iSize, void* pValue);
	// ����һ��C�����������ݵ��ֽ��ڴ��ŷ�λ��(iSize = 2,4,8)
    static void SE_SwapBytes(int iSize, int iCount, void* pValue);

    // ͳһ�������Ϊlittle endian��ʽ
    static bool SE_IsBigEndian(void);
    static void SE_EndianCopy(int iSize, const void* pSrc, void* pDst);
    static void SE_EndianCopy(int iSize, int iCount, const void* pSrc, void* pDst);

    // ���ص�λΪ��
    static double SE_GetTime(void);

    // ֧���ļ���load,save
    static bool SE_Load(const char* pFileName, char*& rDstBuffer, int& riSize);
    static bool SE_Save(const char* pFileName, const char* pSrcBuffer, int iSize);
    static bool SE_Append(const char* pFileName, const char* pSrcBuffer, int iSize);

    // buffer��д,�ļ���д,���ֽ�������,endianness�޹�
    static int SE_Read1(const char* pSrcBuffer, int iCount, void* pDstData);
    static int SE_Write1(char* pDstBuffer, int iCount, const void* pSrcData);
    static int SE_Read1(FILE* pSrcFile, int iCount, void* pDstData);
    static int SE_Write1(FILE* pDstFile, int iCount, const void* pSrcData);

    // buffer��д,�ļ���д,2,4,8�ֽ�������,Դ����Ϊlittle endian
    static int SE_Read2le(const char* pSrcBuffer, int iCount, void* pDstData);
    static int SE_Read4le(const char* pSrcBuffer, int iCount, void* pDstData);
    static int SE_Read8le(const char* pSrcBuffer, int iCount, void* pDstData);
    static int SE_Write2le(char* pDstBuffer, int iCount, const void* pSrcData);
    static int SE_Write4le(char* pDstBuffer, int iCount, const void* pSrcData);
    static int SE_Write8le(char* pDstBuffer, int iCount, const void* pSrcData);
    static int SE_Read2le(FILE* pSrcFile, int iCount, void* pDstData);
    static int SE_Read4le(FILE* pSrcFile, int iCount, void* pDstData);
    static int SE_Read8le(FILE* pSrcFile, int iCount, void* pDstData);
    static int SE_Write2le(FILE* pDstFile, int iCount, const void* pSrcData);
    static int SE_Write4le(FILE* pDstFile, int iCount, const void* pSrcData);
    static int SE_Write8le(FILE* pDstFile, int iCount, const void* pSrcData);

    // buffer��д,�ļ���д,2,4,8�ֽ�������,Դ����Ϊbig endian
    static int SE_Read2be(const char* pSrcBuffer, int iCount, void* pDstData);
    static int SE_Read4be(const char* pSrcBuffer, int iCount, void* pDstData);
    static int SE_Read8be(const char* pSrcBuffer, int iCount, void* pDstData);
    static int SE_Write2be(char* pDstBuffer, int iCount, const void* pSrcData);
    static int SE_Write4be(char* pDstBuffer, int iCount, const void* pSrcData);
    static int SE_Write8be(char* pDstBuffer, int iCount, const void* pSrcData);
    static int SE_Read2be(FILE* pSrcFile, int iCount, void* pDstData);
    static int SE_Read4be(FILE* pSrcFile, int iCount, void* pDstData);
    static int SE_Read8be(FILE* pSrcFile, int iCount, void* pDstData);
    static int SE_Write2be(FILE* pDstFile, int iCount, const void* pSrcData);
    static int SE_Write4be(FILE* pDstFile, int iCount, const void* pSrcData);
    static int SE_Write8be(FILE* pDstFile, int iCount, const void* pSrcData);

    // Macintoshƽ̨��Ҫ�˺����������临�ӵ�Ӧ�ó���ṹ,
    // �ر��,�˺�������Xcode��Ŀ���Һ���Ŀ¼��,������Ӧ�ó�������
    // �������ļ�����Ŀ����Դ�ļ�����,
    // ������ƽ̨����Ŀ¼�����ļ���,���ֵ�������ྲ̬ȫ���ڴ���(���߳�ʱ��ע��)
    static const char* SE_GetPath(const char* pDirectory, const char* pFileName);

    // ά��һ��Ŀ¼�б�,��Ӧ�ó����ѯ,�б��ʼΪ��,
    // ��Ӧ�ó��������������Ŀ¼,Ŀ¼��������"/"��β,
    // SE_InsertDirectory��SE_RemoveDirectory�����Զ�������ַ�
    static void SE_Initialize(void);
    static void SE_Terminate(void);
    static int SE_GetDirectoryCount(void);
    static const char* SE_GetDirectory(int i);
    static bool SE_InsertDirectory(const char* pDirectory);
    static bool SE_RemoveDirectory(const char* pDirectory);
    static void SE_RemoveAllDirectories(void);

    // SE_GetPath����Ŀ¼�б�,���ָ���ļ�������,�򷵻�������·������,���򷵻�null,
    // eMode������ö��ֵ
    enum
    {
        SM_READ,
        SM_WRITE,
        SM_READ_WRITE
    };
    static const char* SE_GetPath(const char* pFileName, int eMode);

    // ����һ����ɫֵ,����endianϸ��
    static unsigned int SE_MakeRGB(unsigned char ucR, unsigned char ucG, unsigned char ucB);
    static unsigned int SE_MakeRGBA(unsigned char ucR, unsigned char ucG, unsigned char ucB, unsigned char ucA);

    // ��װ��׼I/O�����Ӷ����buffer��д���������,��߰�ȫ����,
    // ĿǰMSVC8.0ʵ�ֵķ�ʽ�������ȵı�׼I/O��
    static FILE* SE_Fopen(const char* pFileName, const char* pMode);
    static int SE_Fprintf(FILE* pFile, const char* pFormat, ...);
    static int SE_Fclose(FILE* pFile);
    static const char* SE_GetEnv(const char* pEnvVarName);  // ���ݻ�����������,�������Ӧֵ
    static void* SE_Memcpy(void* pDst, size_t uiDstSize, const void* pSrc, size_t uiSrcSize);
    static int SE_Sprintf(char* pDst, size_t uiDstSize, const char* pFormat, ...);
    static char* SE_Strcpy(char* pDst, size_t uiDstSize, const char* pSrc);
    static char* SE_Strcat(char* pDst, size_t uiDstSize, const char* pSrc);
    static char* SE_Strncpy(char* pDst, size_t uiDstSize, const char* pSrc, size_t uiSrcSize);
    static char* SE_Strtok(char* pToken, const char* pDelimiters, char*& rpNextToken);

private:
    enum { SYSTEM_MAX_PATH = 1024, SYSTEM_MAX_ENVVAR = 1024 };
    static char Path[SYSTEM_MAX_PATH];
    static char EnvVar[SYSTEM_MAX_ENVVAR];
    static std::vector<String>* Directories;   // ϵͳά����Ŀ¼�б�

public:
    static char SE_PATH[SYSTEM_MAX_ENVVAR];
};

// 2D����ķ������ͷ�,�ͷ�ʱ����ָ�뱻��Ϊnull
template <class T> void Allocate(int iCols, int iRows, T**& raatArray);
template <class T> static void Deallocate(T**& raatArray);

// 3D����ķ������ͷ�,�ͷ�ʱ����ָ�뱻��Ϊnull
template <class T> void Allocate(int iCols, int iRows, int iSlices, T***& raaatArray);
template <class T> void Deallocate(T***& raaatArray);

#include "SESystem.inl"

}

#endif