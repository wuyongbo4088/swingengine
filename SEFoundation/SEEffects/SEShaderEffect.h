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

#ifndef Swing_ShaderEffect_H
#define Swing_ShaderEffect_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEEffect.h"
#include "SEVertexShader.h"
#include "SEGeometryShader.h"
#include "SEPixelShader.h"
#include "SERenderStateBlock.h"

namespace Swing
{

class Renderer;

//----------------------------------------------------------------------------
// ����:shader effect����
// ˵��:������ʵ����������֪��effect����shader program profile�������Դ.
// ����:Sun Che
// ʱ��:20080702
// ����ʱ��:20090513
// ��������:����geometry shader֧��,Ŀǰ����һ����ѡstage.
//----------------------------------------------------------------------------
class SE_FOUNDATION_API ShaderEffect : public Effect
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    ShaderEffect(int iPassCount);
    virtual ~ShaderEffect(void);

    // shader effect��vertex/geometry/pixel shader�Ĺ�����.
    // ���·�װʵ����shader��������ݵķ��ʽӿ�,
    // �Ӷ������˽ϳ���shader��������ݵ������������ʽ.
    // ������Ӧ�����乹�캯����:
    // (1) ָ������pass����.
    // (2) Ϊÿ��passָ������vertex/geometry/pixel������.
    // (3) Ϊÿ��pass��shader����ָ������������Դ��.
    //
    // ÿ��pass��vertex/geometry/pixel shader�����shader�����ı�������,
    // ������Ϊ��Դ,��pass��������Ⱦ����������ǰ,����Ⱦ���Զ�װ��.
    // ��Ȼ,Ҳ�������û��ֶ�ָ��װ��.

    // vertex/geometry/pixel shader����.
    // SetPassCount�����·���vertex/geometry/pixel shader,alpha state�����С.
    virtual void SetPassCount(int iPassCount);
    int GetPassCount(void) const;

    // multipass effect�Ļ��ģʽ.
    // ģʽi����pass i-1��pass i��������ɫ����λ�ϵ�.
    // ����һ��Ӧ����geometry�����single effect��˵,
    // ���ģʽ0���޹ؽ�Ҫ��,�������Դģʽϵ��ΪSBF_ONE,
    // Ŀ��ģʽϵ��ΪSDF_ZERO;Ҳ����˵,frame buffer������ֵ��shader������ظ���.
    // ������effectӦ����һ��geometry����,
    // ����ģʽ0������ǰeffect������������frame buffer����ֵ���л��.
    AlphaState* GetBlending(int iPass);

    // per-pass render state block����.
    RenderStateBlock* GetRStateBlock(int iPass);

    // ����pass������κ�global state�����Ժ�(��Ⱦ���)�ָ���Ӧstate.
    // ����Ĭ��ʵ��:
    // ����multipass��multieffect��Ⱦ����Ҫ,�����͹ر�alpha���״̬,
    // �Ӷ�ʹ��ǰpass���������ֵ�ܹ���frame buffer����ֵ��ȷ���.
    virtual void SetGlobalState(int iPass, Renderer* pRenderer,
        bool bPrimaryEffect);
    virtual void RestoreGlobalState(int iPass, Renderer* pRenderer,
        bool bPrimaryEffect);

    // vertex shader�ӿ�
    void SetVShader(int iPass, VertexShader* pVShader);
    VertexShader* GetVShader(int iPass);
    VertexProgram* GetVProgram(int iPass);
    const std::string& GetVShaderName(int iPass) const;
    int GetVConstantCount(int iPass) const;
    UserConstant* GetVConstant(int iPass, int i);
    UserConstant* GetVConstant(int iPass, const std::string& rName);
    int GetVTextureCount(int iPass) const;
    Texture* GetVTexture(int iPass, int i);
    Texture* GetVTexture(int iPass, const std::string& rName);
    void SetVTexture(int iPass, int i, Texture* pTexture); // ����Ӧ�ó���ָ��������.
    void SetVImageName(int iPass, int i, const std::string& rName);
    const std::string& GetVImageName(int iPass, int i) const;

    // geometry shader�ӿ�
    void SetGShader(int iPass, GeometryShader* pGShader);
    GeometryShader* GetGShader(int iPass);
    GeometryProgram* GetGProgram(int iPass);
    const std::string& GetGShaderName(int iPass) const;
    int GetGConstantCount(int iPass) const;
    UserConstant* GetGConstant(int iPass, int i);
    UserConstant* GetGConstant(int iPass, const std::string& rName);
    int GetGTextureCount(int iPass) const;
    Texture* GetGTexture(int iPass, int i);
    Texture* GetGTexture(int iPass, const std::string& rName);
    void SetGTexture(int iPass, int i, Texture* pTexture); // ����Ӧ�ó���ָ��������.
    void SetGImageName(int iPass, int i, const std::string& rName);
    const std::string& GetGImageName(int iPass, int i) const;

    // pixel shader�ӿ�
    void SetPShader(int iPass, PixelShader* pPShader);
    PixelShader* GetPShader(int iPass);
    PixelProgram* GetPProgram(int iPass);
    const std::string& GetPShaderName(int iPass) const;
    int GetPConstantCount(int iPass) const;
    UserConstant* GetPConstant(int iPass, int i);
    UserConstant* GetPConstant(int iPass, const std::string& rName);
    int GetPTextureCount(int iPass) const;
    Texture* GetPTexture(int iPass, int i);
    Texture* GetPTexture(int iPass, const std::string& rName);
    void SetPTexture(int iPass, int i, Texture* pTexture); // ����Ӧ�ó���ָ��������.
    void SetPImageName(int iPass, int i, const std::string& rName);
    const std::string& GetPImageName(int iPass, int i) const;

    // ��ʵ��.
    // Ŀǰֻ֧��effect passЯ����alpha state.
    // ��scene graph����UpdateRS����ʱ,��Geometry�����,
    // ���µ�ǰshader effect��pass��render state block.
    virtual void OnUpdateData(void* pUserData);

    // ��Щ������Renderer::LoadResources��Renderer::ReleaseResources����.
    // Ҳ������Ӧ�ó��������û��ֶ����ó�ʼ��������Դ.
    virtual void LoadResources(Renderer* pRenderer, Geometry* pGeometry);
    virtual void ReleaseResources(Renderer* pRenderer, Geometry* pGeometry);

protected:
    // streaming support
    ShaderEffect(void);

    void SetDefaultAlphaState(void);

    int m_iPassCount;
    std::vector<VertexShaderPtr> m_VShader;
    std::vector<GeometryShaderPtr> m_GShader;
    std::vector<PixelShaderPtr> m_PShader;
    std::vector<RenderStateBlockPtr> m_RStateBlocks;

// �ڲ�ʹ��
public:
    // ��Ⱦ�����ô˺����Ӵ���װ��effect��Ӧpass�����shader����.
    // ���ĳshader����װ��ʧ��,��ʹ��һ��Ĭ��shader.
    // ���ʧ�ܵĸ������:
    // (1) shader�����ļ�û�ҵ�.
    // (2) vertex�����������Ժ�pixel������������Բ�����.
    // (3) shader������Ҫ�������ɫ����,������Ⱦ��֧�ַ�Χ.
    // (4) shader������Ҫ�����texture image��Ԫ,������Ⱦ��֧�ַ�Χ.
    // (5) shader������Ҫ��������������,������Ⱦ��֧�ַ�Χ.
    void LoadPrograms(int iPass, Renderer* pRenderer);

    void ReleasePrograms(int iPass);

    // ��LoadPrograms������,shader���򱻴�����,������������.
    // �⽫����ShaderEffect���������ظú���,�Ӷ�ʵ���Զ�����Ϊ.
    // ������Ӧ��������Ϊ�û��Զ��峣��ָ���洢λ��.
    virtual void OnLoadPrograms(int iPass, Program* pVProgram,
        Program* pPProgram, Program* pGProgram);

    // ��ReleasePrograms������,shader�����ͷ�ǰ,������������.
    // �⽫����ShaderEffect���������ظú���,�Ӷ�ʵ���Զ�����Ϊ.
    virtual void OnReleasePrograms(int iPass, Program* pVProgram,
        Program* pPProgram, Program* pGProgram);

    // ����Ⱦ��Ӧ�õ�ǰeffect������Ⱦǰ(��),�������Щ����.
    // �⽫����ShaderEffect������������Щ����,�Ӷ�ʵ���Զ�����Ϊ.
    virtual void OnPreApplyEffect(Renderer* pRenderer, bool bPrimaryEffect);
    virtual void OnPostApplyEffect(Renderer* pRenderer, bool bPrimaryEffect);

    // ����Ⱦ��Ӧ�õ�ǰeffect��ÿ��pass������Ⱦǰ(��),�������Щ����.
    // �⽫����ShaderEffect������������Щ����,�Ӷ�ʵ���Զ�����Ϊ.
    virtual void OnPreApplyPass(int iPass, Renderer* pRenderer, 
        bool bPrimaryEffect);
    virtual void OnPostApplyPass(int iPass, Renderer* pRenderer, 
        bool bPrimaryEffect);
};

typedef SESmartPointer<ShaderEffect> ShaderEffectPtr;

}

#endif
