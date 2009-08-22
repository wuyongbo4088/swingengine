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

#ifndef Swing_FrameBuffer_H
#define Swing_FrameBuffer_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"

namespace Swing
{

class Renderer;
class Texture;

//----------------------------------------------------------------------------
// ����:frame buffer��
// ˵��:��װ��ȾĿ���йصĲ���,
//      ֧����Ⱦ������.
// ����:Sun Che
// ʱ��:20080702
// ����ʱ��:20090306
// ����˵��:��ֻ֧�ֵ�һ��ȾĿ��,����Ϊ֧�ֶ���ȾĿ��(MRT).
//     ����ȾĿ����������ܳ�����ǰ��Ⱦ��������������������.
//     �����������ȾĿ�������ָ������Ӧ�ɵ������ڶ�̬�ڴ��д���,
//     frame buffer�����ͷŸ�����.
//
//     ÿ��������Ҫ��ͬ��������ߴ���������,
//     ��,�������ͬ������λ��Ĳ�ͬ�������͵�����ͬʱʹ��.
//     DirectX9��OpenGL2�����ոñ�׼Ҫ��MRT.
//
//     frame buffer����֧�ֶ������Ƕ�׵���,��:
//
//        FrameBuffer A,B;
//        A.Enable();
//        rendering code...
//            B.Enable();
//            rendering code...
//            B.Disable();
//        rendering code...
//        A.Disable();
//----------------------------------------------------------------------------
class SE_FOUNDATION_API FrameBuffer
{
public:
    // ֧��MRT��,framebuffer�ľ����ʽ��ÿ��target�������,���ö��û��������.
    enum FormatType
    {
        FT_FORMAT_NONE,
        FT_FORMAT_RGB,
        FT_FORMAT_RGBA,
        FT_FORMAT_R32,
        FT_FORMAT_RGBA16,
        FT_FORMAT_RGBA32,
        FT_FORMAT_DEPTH
    };

    enum DepthType
    {
        DT_DEPTH_NONE,
        DT_DEPTH_8,
        DT_DEPTH_16,
        DT_DEPTH_24,
        DT_DEPTH_32
    };

    enum StencilType
    {
        ST_STENCIL_NONE,
        ST_STENCIL_8
    };

    enum BufferingType
    {
        BT_BUFFERED_SINGLE,
        BT_BUFFERED_DOUBLE
    };

    enum MultisamplingType
    {
        MT_SAMPLING_NONE,
        MT_SAMPLING_2,
        MT_SAMPLING_4,
        MT_SAMPLING_8
    };

    // �����,FrameBuffer::Create��FrameBuffer::Destroy���ɹ�������,
    // ��������apTargets������һ��SAMPLER_2DĿ��,frame buffer��һ��2D������,
    // �û�������ȷ����������������е�ÿ��������󶼾�����ͬ��width,height,
    // �Լ���������,�����ʽ.
    // ���������ָ������������ɵ����߶�̬������,��FrameBuffer����ɾ��.
    static FrameBuffer* Create(FormatType eFormat, DepthType eDepth,
        StencilType eStencil, BufferingType eBuffering,
        MultisamplingType eMultisampling, Renderer* pRenderer,
        int iTCount, Texture** apTargets);

    static void Destroy(FrameBuffer* pBuffer);

    FrameBuffer(FormatType eFormat, DepthType eDepth, StencilType eStencil,
        BufferingType eBuffering, MultisamplingType eSampling,
        Renderer* pRenderer, int iTCount, Texture** apTargets);

    virtual ~FrameBuffer(void);

    inline FormatType GetFormatType(void) const;
    inline DepthType GetDepthType(void) const;
    inline StencilType GetStencilType(void) const;
    inline BufferingType GetBufferingType(void) const;
    inline MultisamplingType GetMultisamplingType(void) const;
    inline Renderer* GetRenderer(void);
    inline int GetCount(void) const;
    inline Texture* GetTarget(int i);

    virtual void Enable(void) = 0;   // �ɾ�����Ⱦ��������ʵ��
    virtual void Disable(void) = 0;  // �ɾ�����Ⱦ��������ʵ��

    // ��ָ��target�����ݴ�frame buffer���Ƶ�texture��ϵͳ�ڴ���.
    virtual void CopyToTexture(int i) = 0;  // �ɾ�����Ⱦ��������ʵ��

protected:
    // ֧��MRT��,framebuffer�ľ����ʽ��ÿ��target�������,m_eFormatû��������.
    FormatType m_eFormat;
    DepthType m_eDepth;
    StencilType m_eStencil;
    BufferingType m_eBuffering;
    MultisamplingType m_eMultisampling;
    Renderer* m_pRenderer;   // ��ǰ��Ⱦ��

    int m_iCount;
    Texture** m_apTargets;

    // frame buffer������������,ÿ��������ʵ��һ��Creator��Destroyer,
    // ���ҰѺ���ָ�븳�����¾�̬�����Ա
    typedef FrameBuffer* (*Creator)(FormatType, DepthType, StencilType,
        BufferingType, MultisamplingType, Renderer*, int, Texture**);
    typedef void (*Destroyer)(FrameBuffer*);

    static Creator ms_aoCreator[];
    static Destroyer ms_aoDestroyer[];
};

#include "SEFrameBuffer.inl"

}

#endif
