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

#ifndef Swing_Image_H
#define Swing_Image_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEObject.h"
#include "SEColorRGBA.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:֧��1D,2D,3Dͼ��.
//     ע��,ͼ���ÿһ��ά�ȵĳ��ȶ�������2^n,����n>=0.
// Author:Sun Che
// Date:20080629
// ����ʱ��:20090305
// ����˵��:Ϊ��֧������width,height��render target����,
//     ȡ������ά�ȱ�����power of two������.
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SEImage : public SEObject
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    enum FormatMode
    {
        IT_RGB888,
        IT_RGBA8888,
        IT_DEPTH16,
        IT_DEPTH24,
        IT_DEPTH32,
        IT_CUBE_RGB888,
        IT_CUBE_RGBA8888,
        IT_RGB32,
        IT_RGBA32,
        IT_L8,
        IT_L16,
        IT_R32,
        IT_RGB16,
        IT_RGBA16,
        IT_RGB565,   // (high)   B G R (low)
        IT_RGBA5551, // (high) A B G R (low)
        IT_RGBA4444, // (high) A B G R (low)
        IT_PVRTC,
        IT_COUNT
    };

    // ��ɫͼ���ݵ��ڴ�洢��ʽΪr,g,b,a�����������ڴ�͵�ַ���ߵ�ַ.
    // ����,��һ�����ص��ڴ�����˳��:
    //
    //   IT_RGB888:
    //     r = pData[0];
    //     g = pData[1];
    //     b = pData[2];
    //
    //   IT_RGBA8888:
    //     r = pData[0];
    //     g = pData[1];
    //     b = pData[2];
    //     a = pData[3];
    //
    // ���ͼ����ʹ����[0,1]�����ϵ�float���ݴ洢.
    // ָ����16,24,��32λλ��Ϣ��������ȷ����Ӳ����depth buffer��.
    // ����б�Ҫ�Ļ�,��depth buffer��ȡ�����ͼ�ϵ�����,
    // ���ᱻת��Ϊ[0,1]����floatֵ.

    // ���������.
    // SEImage������ɾ���������������.
    // pImageName����image����ʱ��ΨһID.
    // ���캯���ĵ����߿����ṩһ��image name.
    // ���û���ṩ,���캯�����Զ�����һ������"imageN.seif"��ΨһID,
    // ����N����SEObject::m_uiID.
    // ����image�����Ŀ��,ϵͳά����һ��ȫ��image map.
    // Ĭ�������,image���ɹ�������,�������뵽ȫ��image map,
    // ���Ժ���������Դʱʹ��.Ȼ���û�Ҳ��������bInsert����Ϊfalse,
    // �Ӷ���image���ᱻ����ȫ��image map(������û��ȫ��image mapʱ).

    // 1D image.
    SEImage(FormatMode eFormat, int iBound0, unsigned char* pData, 
        const char* pImageName, bool bInsert = true);

    // 2D image.
    SEImage(FormatMode eFormat, int iBound0, int iBound1,
        unsigned char* pData, const char* pImageName, bool bInsert = true);

    // 3D image.
    SEImage(FormatMode eFormat, int iBound0, int iBound1, int iBound2,
        unsigned char* pData, const char* pImageName, bool bInsert = true);

    virtual ~SEImage(void);

    inline FormatMode GetFormat(void) const;
    inline const std::string& GetFormatName(void) const;
    inline static std::string GetFormatName(int eFormat);
    inline bool IsDepthImage(void) const;
    inline bool IsCubeImage(void) const;
    inline int GetBytesPerPixel(void) const;
    inline static int GetBytesPerPixel(int eFormat);
    inline int GetDimension(void) const;
    inline int GetBound(int i) const;
    inline int GetCount(void) const;
    inline unsigned char* GetData(void) const;
    inline unsigned char* operator() (int i);

    // ����һ����������ΪSEColorRGBA��image.
    // ���������뵱ǰimage����ͬ��width��height��SEColorRGBA����.
    // ���ڸ������Ƕ�̬�����,��˵������������ͷŸ�����.
    SEColorRGBA* CreateRGBA(void) const;

    // �����ݸ��Ƹ�һ���Ѿ����ڵ�SEColorRGBA������.
    // ����������������͵�ǰimage��ͬ����ά����Ϣ.
    void CopyRGBA(SEColorRGBA* pColorImage) const;

    // Streaming support.����Щ����������ʱ,����ϵͳ�����Զ�����.
    // ����Load����,�������filename��image�Ѿ������ڴ�,
    // ���image���ҵ�����Ϊ����ֵ(����,���ڹ����Ŀ��).
    // ����,����һ���µ�image������.filename������image name.
    // �ο�SEImageCatalog::Find�����ж���������ĵ���.
    static SEImage* Load(const char* pImageName);

    // �����������֧�ֱ���������ɵ�image����,
    // ���߿�������֧�ְ��������͵�image��ʽ������ת��Ϊseif��ʽ.
    // �����filename�������ԭ�е�image name,ֻ���������ɵ��ļ���.
    bool Save(const char* pFileName);

    // ����һ��2D random image.
    static SEImage* GenerateRandomImage(FormatMode eFormat, int iBound0, 
        int iBound1, unsigned int uiSeed, const char* acImageName, 
        bool bInsert = true);

    // ����һ��2D color image.
    static SEImage* GenerateColorImage(FormatMode eFormat, int iBound0, 
        int iBound1, const SEColorRGBA& rColor, const char* acImageName, 
        bool bInsert = true);

protected:
    // support for streaming.
    SEImage(void);

    FormatMode m_eFormat;
    int m_iDimension;
    int m_Bound[3];
    int m_iCount;  // ���ظ���
    unsigned char* m_pData;

    bool m_bIsInCatalog;

    static int ms_BytesPerPixel[IT_COUNT];
    static std::string ms_FormatName[IT_COUNT];
};

typedef SESmartPointer<SEImage> SEImagePtr;

#include "SEImage.inl"

}

#endif
