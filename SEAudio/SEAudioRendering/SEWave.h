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

#ifndef Swing_Wave_H
#define Swing_Wave_H

#include "SEAudioLIB.h"
#include "SEObject.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:wave��
// ˵��:֧��wave�����ļ�.
// ����:Sun Che
// ʱ��:20090617
//----------------------------------------------------------------------------
class SE_AUDIO_API Wave : public Object
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    enum FormatMode
    {
        WT_MONO4 = 0,
        WT_MONO8,
        WT_MONO16,
        WT_STEREO4,
        WT_STEREO8,
        WT_STEREO16,
        WT_QUAD16,
        WT_REAR16,
        WT_51CHN16,
        WT_61CHN16,
        WT_71CHN16,
        WT_COUNT
    };

    // ע��:16λwave���ݴ���endian����.
    // 8λ�����������ǰ��յ��ֽ����ݶ�ȡ/д�����.
    // 16λ�����������ǰ���little endian��˫�ֽ�����д�����,
    // �����д��/��ȡ16λ��������ʱ,�����ǰϵͳ��big endian��,������swap.

    // ���������.
    // Wave������ɾ���������������.
    // pWaveName����wave����ʱ��ΨһID.
    // ���캯���ĵ����߿����ṩһ��wave name.
    // ���û���ṩ,���캯�����Զ�����һ������"waveN.sewf"��ΨһID,
    // ����N����Object::m_uiID.
    // ����wave�����Ŀ��,ϵͳά����һ��ȫ��wave map.
    // Ĭ�������,wave���ɹ�������,�������뵽ȫ��wave map,
    // ���Ժ���������Դʱʹ��.Ȼ���û�Ҳ��������bInsert����Ϊfalse,
    // �Ӷ���wave���ᱻ����ȫ��wave map(������û��ȫ��wave mapʱ).

    Wave(FormatMode eFormat, unsigned int uiFrequency, int iDataSize,
        unsigned char* pData, const char* pWaveName, bool bInsert = true);

    virtual ~Wave(void);

    // ��Ա����.
    inline FormatMode GetFormat(void) const;
    inline const std::string& GetFormatName(void) const;
    inline static std::string GetFormatName(int eFormat);
    inline int GetBytesPerSample(void) const;
    inline static int GetBytesPerSample(int eFormat);
    inline int GetChannelsPerSample(void) const;
    inline static int GetChannelsPerSample(int eFormat);
    inline unsigned int GetFrequency(void) const;
    inline int GetDataSize(void) const;
    inline unsigned char* GetData(void) const;

    // Streaming support.����Щ����������ʱ,����ϵͳ�����Զ�����.
    // ����Load����,�������filename��wave�Ѿ������ڴ�,
    // ���wave���ҵ�����Ϊ����ֵ(����,���ڹ����Ŀ��).
    // ����,����һ���µ�wave������.filename������wave name.
    // �ο�WaveCatalog::Find�����ж���������ĵ���.
    static Wave* Load(const char* pWaveName);

    // �����������֧�ֱ���������ɵ�wave����,
    // ���߿�������֧�ְ��������͵�audio��ʽ������ת��Ϊsewf��ʽ.
    // �����filename�������ԭ�е�wave name,ֻ���������ɵ��ļ���.
    bool Save(const char* pFileName);

protected:
    // support for streaming.
    Wave(void);

    FormatMode m_eFormat;
    unsigned int m_uiFrequency;
    int m_iDataSize;
    unsigned char* m_pData;

    bool m_bIsInCatalog;

    static int ms_BytesPerSample[WT_COUNT];
    static int ms_ChannelsPerSample[WT_COUNT];
    static std::string ms_FormatName[WT_COUNT];
};

typedef SmartPointer<Wave> WavePtr;

#include "SEWave.inl"

}

#endif
