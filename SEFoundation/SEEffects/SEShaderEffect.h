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

class SERenderer;

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
    SEAlphaState* GetBlending(int iPass);

    // per-pass render state block����.
    SERenderStateBlock* GetRStateBlock(int iPass);

    // ����pass������κ�global state�����Ժ�(��Ⱦ���)�ָ���Ӧstate.
    // ����Ĭ��ʵ��:
    // ����multipass��multieffect��Ⱦ����Ҫ,�����͹ر�alpha���״̬,
    // �Ӷ�ʹ��ǰpass���������ֵ�ܹ���frame buffer����ֵ��ȷ���.
    virtual void SetGlobalState(int iPass, SERenderer* pRenderer,
        bool bPrimaryEffect);
    virtual void RestoreGlobalState(int iPass, SERenderer* pRenderer,
        bool bPrimaryEffect);

    // vertex shader�ӿ�
    void SetVShader(int iPass, SEVertexShader* pVShader);
    SEVertexShader* GetVShader(int iPass);
    SEVertexProgram* GetVProgram(int iPass);
    const std::string& GetVShaderName(int iPass) const;
    int GetVConstantCount(int iPass) const;
    SEUserConstant* GetVConstant(int iPass, int i);
    SEUserConstant* GetVConstant(int iPass, const std::string& rName);
    int GetVTextureCount(int iPass) const;
    SETexture* GetVTexture(int iPass, int i);
    SETexture* GetVTexture(int iPass, const std::string& rName);
    void SetVTexture(int iPass, int i, SETexture* pTexture); // ����Ӧ�ó���ָ��������.
    void SetVImageName(int iPass, int i, const std::string& rName);
    const std::string& GetVImageName(int iPass, int i) const;

    // geometry shader�ӿ�
    void SetGShader(int iPass, SEGeometryShader* pGShader);
    SEGeometryShader* GetGShader(int iPass);
    SEGeometryProgram* GetGProgram(int iPass);
    const std::string& GetGShaderName(int iPass) const;
    int GetGConstantCount(int iPass) const;
    SEUserConstant* GetGConstant(int iPass, int i);
    SEUserConstant* GetGConstant(int iPass, const std::string& rName);
    int GetGTextureCount(int iPass) const;
    SETexture* GetGTexture(int iPass, int i);
    SETexture* GetGTexture(int iPass, const std::string& rName);
    void SetGTexture(int iPass, int i, SETexture* pTexture); // ����Ӧ�ó���ָ��������.
    void SetGImageName(int iPass, int i, const std::string& rName);
    const std::string& GetGImageName(int iPass, int i) const;

    // pixel shader�ӿ�
    void SetPShader(int iPass, SEPixelShader* pPShader);
    SEPixelShader* GetPShader(int iPass);
    SEPixelProgram* GetPProgram(int iPass);
    const std::string& GetPShaderName(int iPass) const;
    int GetPConstantCount(int iPass) const;
    SEUserConstant* GetPConstant(int iPass, int i);
    SEUserConstant* GetPConstant(int iPass, const std::string& rName);
    int GetPTextureCount(int iPass) const;
    SETexture* GetPTexture(int iPass, int i);
    SETexture* GetPTexture(int iPass, const std::string& rName);
    void SetPTexture(int iPass, int i, SETexture* pTexture); // ����Ӧ�ó���ָ��������.
    void SetPImageName(int iPass, int i, const std::string& rName);
    const std::string& GetPImageName(int iPass, int i) const;

    // ��ʵ��.
    // Ŀǰֻ֧��effect passЯ����alpha state.
    // ��scene graph����UpdateRS����ʱ,��SEGeometry�����,
    // ���µ�ǰshader effect��pass��render state block.
    virtual void OnUpdateData(void* pUserData);

    // ��Щ������SERenderer::LoadResources��SERenderer::ReleaseResources����.
    // Ҳ������Ӧ�ó��������û��ֶ����ó�ʼ��������Դ.
    virtual void LoadResources(SERenderer* pRenderer, SEGeometry* pGeometry);
    virtual void ReleaseResources(SERenderer* pRenderer, SEGeometry* pGeometry);

protected:
    // streaming support
    ShaderEffect(void);

    void SetDefaultAlphaState(void);

    int m_iPassCount;
    std::vector<SEVertexShaderPtr> m_VShader;
    std::vector<SEGeometryShaderPtr> m_GShader;
    std::vector<SEPixelShaderPtr> m_PShader;
    std::vector<SERenderStateBlockPtr> m_RStateBlocks;

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
    void LoadPrograms(int iPass, SERenderer* pRenderer);

    void ReleasePrograms(int iPass);

    // ��LoadPrograms������,shader���򱻴�����,������������.
    // �⽫����ShaderEffect���������ظú���,�Ӷ�ʵ���Զ�����Ϊ.
    // ������Ӧ��������Ϊ�û��Զ��峣��ָ���洢λ��.
    virtual void OnLoadPrograms(int iPass, SEProgram* pVProgram,
        SEProgram* pPProgram, SEProgram* pGProgram);

    // ��ReleasePrograms������,shader�����ͷ�ǰ,������������.
    // �⽫����ShaderEffect���������ظú���,�Ӷ�ʵ���Զ�����Ϊ.
    virtual void OnReleasePrograms(int iPass, SEProgram* pVProgram,
        SEProgram* pPProgram, SEProgram* pGProgram);

    // ����Ⱦ��Ӧ�õ�ǰeffect������Ⱦǰ(��),�������Щ����.
    // �⽫����ShaderEffect������������Щ����,�Ӷ�ʵ���Զ�����Ϊ.
    virtual void OnPreApplyEffect(SERenderer* pRenderer, bool bPrimaryEffect);
    virtual void OnPostApplyEffect(SERenderer* pRenderer, bool bPrimaryEffect);

    // ����Ⱦ��Ӧ�õ�ǰeffect��ÿ��pass������Ⱦǰ(��),�������Щ����.
    // �⽫����ShaderEffect������������Щ����,�Ӷ�ʵ���Զ�����Ϊ.
    virtual void OnPreApplyPass(int iPass, SERenderer* pRenderer, 
        bool bPrimaryEffect);
    virtual void OnPostApplyPass(int iPass, SERenderer* pRenderer, 
        bool bPrimaryEffect);
};

typedef SESmartPointer<ShaderEffect> ShaderEffectPtr;

}

#endif
