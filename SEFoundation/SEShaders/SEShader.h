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
#include "SEInterfaceDescriptor.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:����shader������Ⱦ������Դ
// Author:Sun Che
// Date:20080701
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SEShader : public SEObject
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    // �����
    virtual ~SEShader(void);

    inline const std::string& GetShaderName(void) const;

    // ����textures��image names
    void SetTextureCount(int iCount);
    inline int GetTextureCount(void) const;
    SETexture* GetTexture(int i);
    SETexture* GetTexture(const std::string& rName);
    const SETexture* GetTexture(int i) const;
    const SETexture* GetTexture(const std::string& rName) const;
    void SetTexture(int i, SETexture* pTexture);  // ����������Դ����
    void SetImageName(int i, const std::string& rName);
    const std::string& GetImageName(int i) const;

    // ����interface descriptor.
    inline void SetInterfaceDescriptor(SEInterfaceDescriptor* pDescriptor);
    inline SEInterfaceDescriptor* GetInterfaceDescriptor(void) const;

protected:
    // Support for streaming.
    SEShader(void);

    // ���캯�������������
    SEShader(const std::string& rShaderName);

protected:
    // shader������shader�����Ψһ��ʶ
    std::string m_ShaderName;

    // shader����,�����ھ���ͼ��API(DirectX,OpenGL)
    SEProgramPtr m_spProgram;

    // shader��������interfaces.
    SEInterfaceDescriptorPtr m_spInterfaces;

    // ÿһ��shader����ʵ�����������Լ���user-defined��������,
    // program����ֻ����������������Щ����������,�Ĵ������,�Ĵ�������,
    // �����ṩ�Ĵ洢�ռ���Ϊ�˸�shader�������ṩ����,
    // ��Ȼ,shader��������ȫ����ͨ��SEUserConstant::SetDataSource����,
    // ���user-defined����ָ�����ݴ洢λ��
    std::vector<float> m_UserData;

    // ��shader����ʵ����ʹ�õ�image����,
    // ��Щtexture����洢��ʵ�ʵ�images��samplers
    std::vector<std::string> m_ImageNames;
    std::vector<SETexturePtr> m_Textures;

// �ڲ�ʹ��
public:
    void OnLoadProgram(SEProgram* pProgram);
    void OnReleaseProgram(void);
};

typedef SESmartPointer<SEShader> SEShaderPtr;

#include "SEShader.inl"

}

#endif
