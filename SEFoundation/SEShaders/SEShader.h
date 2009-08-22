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

#ifndef Swing_Shader_H
#define Swing_Shader_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEObject.h"
#include "SEProgram.h"
#include "SETexture.h"
#include "SEString.h"
#include "SEInterfaceDescriptor.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:shader��
// ˵��:����shader������Ⱦ������Դ
// ����:Sun Che
// ʱ��:20080701
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Shader : public Object
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    // �����
    virtual ~Shader(void);

    inline const String& GetShaderName(void) const;

    // ����textures��image names
    void SetTextureCount(int iCount);
    inline int GetTextureCount(void) const;
    Texture* GetTexture(int i);
    Texture* GetTexture(const String& rName);
    const Texture* GetTexture(int i) const;
    const Texture* GetTexture(const String& rName) const;
    void SetTexture(int i, Texture* pTexture);  // ����������Դ����
    void SetImageName(int i, const String& rName);
    const String& GetImageName(int i) const;

    // ����interface descriptor.
    inline void SetInterfaceDescriptor(InterfaceDescriptor* pDescriptor);
    inline InterfaceDescriptor* GetInterfaceDescriptor(void) const;

protected:
    // Support for streaming.
    Shader(void);

    // ���캯�������������
    Shader(const String& rShaderName);

protected:
    // shader������shader�����Ψһ��ʶ
    String m_ShaderName;

    // shader����,�����ھ���ͼ��API(DirectX,OpenGL)
    ProgramPtr m_spProgram;

    // shader��������interfaces.
    InterfaceDescriptorPtr m_spInterfaces;

    // ÿһ��shader����ʵ�����������Լ���user-defined��������,
    // program����ֻ����������������Щ����������,�Ĵ������,�Ĵ�������,
    // �����ṩ�Ĵ洢�ռ���Ϊ�˸�shader�������ṩ����,
    // ��Ȼ,shader��������ȫ����ͨ��UserConstant::SetDataSource����,
    // ���user-defined����ָ�����ݴ洢λ��
    std::vector<float> m_UserData;

    // ��shader����ʵ����ʹ�õ�image����,
    // ��Щtexture����洢��ʵ�ʵ�images��samplers
    std::vector<String> m_ImageNames;
    std::vector<TexturePtr> m_Textures;

// �ڲ�ʹ��
public:
    void OnLoadProgram(Program* pProgram);
    void OnReleaseProgram(void);
};

typedef SmartPointer<Shader> ShaderPtr;

#include "SEShader.inl"

}

#endif
