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

#ifndef Swing_Renderer_H
#define Swing_Renderer_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEFrameBuffer.h"
#include "SEAlphaState.h"
#include "SECullState.h"
#include "SEMaterialState.h"
#include "SEPolygonOffsetState.h"
#include "SEStencilState.h"
#include "SEWireframeState.h"
#include "SEZBufferState.h"
#include "SEColorRGB.h"
#include "SEColorRGBA.h"
#include "SEMatrix4.h"
#include "SERendererConstant.h"
#include "SEUserConstant.h"
#include "SESamplerInformation.h"

namespace Swing
{

class Attributes;
class Bindable;
class Camera;
class Effect;
class Geometry;
class GeometryProgram;
class IndexBuffer;
class Light;
class PixelProgram;
class ResourceIdentifier;
class ShaderEffect;
class Spatial;
class Texture;
class VertexBuffer;
class VertexProgram;
class UnculledObject;
class UnculledSet;
class RenderStateBlock;

//----------------------------------------------------------------------------
// ����:��Ⱦ������
// ˵��:
// ����:Sun Che
// ʱ��:20080320
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Renderer
{
public:
    // �����.
    // ��Ⱦ������API.����ͼ�ι���API��Ҫʵ�����.
    virtual ~Renderer(void);

    // RTTI��Ϣ.
    enum
    {
        OPENGL,
        OPENGLES1,
        OPENGLES2,
        DIRECTX9,
        DIRECTX10,
        SOFTWARE,
        MAX_RENDERER_TYPES
    };
    virtual int GetType(void) const = 0; // ����Ⱦ��������ʵ��

    // װ��shader����ʱ�õ�����Ⱦ����ʶ��.
    virtual const char* GetExtension(void) const = 0;
    virtual char GetCommentCharacter(void) const = 0;

    // ���������.
    void SetCamera(Camera* pCamera);
    inline Camera* GetCamera(void) const;

    // frame buffer����.
    inline FrameBuffer::FormatType GetFormatType(void) const;
    inline FrameBuffer::DepthType GetDepthType(void) const;
    inline FrameBuffer::StencilType GetStencilType(void) const;
    inline FrameBuffer::BufferingType GetBufferingType(void) const;
    inline FrameBuffer::MultisamplingType GetMultisamplingType(void) const;

    // window����.
    inline int GetWidth(void) const;
    inline int GetHeight(void) const;
    virtual void Resize(int iWidth, int iHeight);
    virtual void ToggleFullscreen(void);

    // back buffer clear color.
    inline void SetClearColor(const ColorRGBA& rClearColor);
    inline const ColorRGBA& GetClearColor(void) const;

    // depth buffer clear value.
    inline void SetClearDepth(float fClearDepth);
    inline float GetClearDepth(void) const;

    // stencil buffer clear value.
    inline void SetClearStencil(unsigned int uiClearValue);
    inline unsigned int GetClearStencil(void) const;

    // pre-draw��post-draw,DIRECTX9 API��Ҫ.
    virtual bool BeginScene(void);
    virtual void EndScene(void);

    // ȫ����buffer����.
    virtual void ClearBackBuffer(void) = 0;
    virtual void ClearZBuffer(void) = 0;
    virtual void ClearStencilBuffer(void) = 0;
    virtual void ClearBuffers(void) = 0;
    virtual void DisplayBackBuffer(void) = 0;

    // �ض�����buffer����.
    virtual void ClearBackBuffer(int iXPos, int iYPos, int iWidth,
        int iHeight) = 0;
    virtual void ClearZBuffer(int iXPos, int iYPos, int iWidth,
        int iHeight) = 0;
    virtual void ClearStencilBuffer(int iXPos, int iYPos, int iWidth,
        int iHeight) = 0;
    virtual void ClearBuffers(int iXPos, int iYPos, int iWidth,
        int iHeight) = 0;

    // ������Ⱦ���.
    virtual void DrawScene(UnculledSet& rVisibleSet);
    virtual void Draw(Geometry* pGeometry);

    // �ı���Ⱦ.
    virtual int LoadFont(const char* acFace, int iSize, bool bBold,
        bool bItalic) = 0;
    virtual void UnloadFont(int iFontID) = 0;
    virtual bool SelectFont(int iFontID) = 0;
    virtual void Draw(int iX, int iY, const ColorRGBA& rColor,
        const char* pText) = 0;

    // 2D��Ⱦ.
    virtual void Draw(const unsigned char* aucBuffer) = 0;

    // vertex shader profile��Ϣ.
    enum
    {
        VS_2_0,   // DirectX9
        VS_3_0,   // DirectX9
        VS_4_0,   // DirectX10
        ARBVP1,   // OpenGL
        VP40,     // OpenGL
        SFTVP1,   // Software
        MAX_VP_TYPES
    };

    // geometry shader profile��Ϣ.
    // geometry shader stage��һ����ѡ�Ĺ���stage,
    // ��ǰֻ��DirectX10,OpenGL2�����ϰ汾֧��.
    enum
    {
        GS_UNSUPPORTED, // Unsupported by current renderer
        GS_4_0,         // DirectX10
        GLSLG,          // OpenGL
        SFTGP1,         // Software
        MAX_GP_TYPES
    };

    // pixel shader profile��Ϣ.
    enum
    {
        PS_2_0,    // DirectX9
        PS_3_0,    // DirectX9
        PS_4_0,    // DirectX10
        ARBFP1,    // OpenGL
        FP40,      // OpenGL
        SFTFP1,    // Software
        MAX_PP_TYPES
    };

    // ��Ⱦ���������Ƽ��.
    inline int GetMaxLights(void) const;
    inline int GetMaxColors(void) const;
    inline int GetMaxTCoords(void) const;
    inline int GetMaxVShaderImages(void) const;
    inline int GetMaxGShaderImages(void) const;
    inline int GetMaxPShaderImages(void) const;
    inline int GetMaxStencilIndices(void) const;
    inline int GetMaxUserClipPlanes(void) const;
    inline int GetMaxVShaderProfile(void) const;
    inline int GetMaxGShaderProfile(void) const;
    inline int GetMaxPShaderProfile(void) const;
    inline int GetMaxRenderTargets(void) const;

    // shader compiler��ѯ.
    inline bool HasShaderCompiler(void) const;

    // ȫ����Ⱦ״̬����.
    virtual void SetAlphaState(AlphaState* pState);
    virtual void SetCullState(CullState* pState);
    virtual void SetMaterialState(MaterialState* pState);
    virtual void SetPolygonOffsetState(PolygonOffsetState* pState);
    virtual void SetStencilState(StencilState* pState);
    virtual void SetWireframeState(WireframeState* pState);
    virtual void SetZBufferState(ZBufferState* pState);
    inline AlphaState* GetAlphaState(void);
    inline CullState* GetCullState(void);
    inline MaterialState* GetMaterialState(void);
    inline PolygonOffsetState* GetPolygonOffsetState(void);
    inline StencilState* GetStencilState(void);
    inline WireframeState* GetWireframeState(void);
    inline ZBufferState* GetZBufferState(void);
    inline void SetReverseCullFace(bool bReverseCullFace);
    inline bool GetReverseCullFace(void) const;

    // ��Ⱦ������Դ�ͷź����ĺ���ָ������.
    typedef void (Renderer::*ReleaseFunction)(Bindable*);

    // ��Ⱦ����Դװ�����ͷ�.
    // ����Դ�����Դ����Դ��ͷ�.
    void LoadAllResources(Spatial* pScene);
    void ReleaseAllResources(Spatial* pScene);
    void LoadResources(Geometry* pGeometry);
    void ReleaseResources(Geometry* pGeometry);
    void LoadResources(Effect* pEffect);
    void ReleaseResources(Effect* pEffect);
    void LoadVProgram(VertexProgram* pVProgram);
    void ReleaseVProgram(Bindable* pVProgram);
    void LoadGProgram(GeometryProgram* pGProgram);
    void ReleaseGProgram(Bindable* pGProgram);
    void LoadPProgram(PixelProgram* pPProgram);
    void ReleasePProgram(Bindable* pPProgram);
    void LoadTexture(Texture* pTexture);
    void ReleaseTexture(Bindable* pTexture);
    void LoadVBuffer(const Attributes& rIAttributes, 
        const Attributes& rOAttributes,
        VertexBuffer* pVBuffer, VertexProgram* pVProgram);
    void ReleaseVBuffer(Bindable* pVBuffer);
    void LoadIBuffer(IndexBuffer* pIBuffer);
    void ReleaseIBuffer(Bindable* pIBuffer);
    void LoadRenderStateBlock(RenderStateBlock* pRStateBlock);
    void ReleaseRenderStateBlock(Bindable* pRStateBlock);

    // ��Դ������ر�.
    void EnableVProgram(VertexProgram* pVProgram);
    void DisableVProgram(VertexProgram* pVProgram);
    void EnableGProgram(GeometryProgram* pGProgram);
    void DisableGProgram(GeometryProgram* pGProgram);
    void EnablePProgram(PixelProgram* pPProgram);
    void DisablePProgram(PixelProgram* pPProgram);
    void EnableTexture(Texture* pTexture);
    void DisableTexture(Texture* pTexture);
    ResourceIdentifier* EnableVBuffer(const Attributes& rIAttributes,
        const Attributes& rOAttributes, VertexProgram* pVProgram = 0);
    void DisableVBuffer(ResourceIdentifier* pID);
    void EnableIBuffer(void);
    void DisableIBuffer(void);
    void EnableRenderStateBlock(RenderStateBlock* pRStateBlock);
    void DisableRenderStateBlock(RenderStateBlock* pRStateBlock);

    // ������lights��effectsʹ��.
    inline void SetLightCount(int iCount);
    inline void SetLight(int i, Light* pLight);
    inline Light* GetLight(int i);

    // ������ͶӰ����effectsʹ��.
    inline void SetProjector(Camera* pProjector);
    inline Camera* GetProjector(void);

    // color mask����,������Щ��ɫͨ��������д��color buffer.
    virtual void SetColorMask(bool bAllowRed, bool bAllowGreen,
        bool bAllowBlue, bool bAllowAlpha);
    virtual void GetColorMask(bool& rbAllowRed, bool& rbAllowGreen,
        bool& rbAllowBlue, bool& rbAllowAlpha);

    // ����/�ر��û��Զ���ü�ƽ��.
    // ����ƽ�������ģ�Ϳռ�ƽ��,�����ڲ����任��������ռ����clip�ռ�ü�.
    virtual void EnableUserClipPlane(int i, const Plane3f& rPlane) = 0;
    virtual void DisableUserClipPlane(int i) = 0;

    // ����任����Ӧ���ڶ�������ռ�任֮��,������ռ�任֮ǰ.
    void SetPostWorldTransformation(const Matrix4f& rMatrix);
    void RestorePostWorldTransformation(void);

    // ��ȡ���ι���world����,view����,���projection����.
    inline const Matrix4f& GetWorldMatrix(void) const;
    inline const Matrix4f& GetViewMatrix(void) const;
    inline const Matrix4f& GetProjectionMatrix(void) const;

protected:
    // �����
    Renderer(FrameBuffer::FormatType eFormat, FrameBuffer::DepthType eDepth,
        FrameBuffer::StencilType eStencil,
        FrameBuffer::BufferingType eBuffering,
        FrameBuffer::MultisamplingType eMultisampling,
        int iWidth, int iHeight);

    // ֧����������ʺ���ظ��²���.
    // �������������ݸı�ʱ,������Щ�ص�����.
    friend class Camera;
    void OnFrameChange(void);                  // �����{E:R,U,D}�ı�
    void OnFrustumChange(void);                // �������Ͷ������ı�
    virtual void OnViewportChange(void) = 0;   // ������ӿڲ����ı�
    virtual void OnDepthRangeChange(void) = 0; // ������ӿ���ȷ�Χ�����ı�

    // ȫ����Ⱦ״̬����.
    void SetGlobalState(GlobalStatePtr aspState[]);
    void RestoreGlobalState(GlobalStatePtr aspState[]);

    // per-geometry pre/post-draw�������.
    // ע��:
    // ��Ⱦ������������Ӧʵ�ֵ�ǰ���������Ⱦ״̬����.
    virtual void OnPreDrawGeometry(void) = 0;
    virtual void OnPostDrawGeometry(void) = 0;

    // per-pass pre/post-draw�������,
    // ��ShaderEffect���OnPreApplyPass/OnPostApplyPass����.
    // ע��:
    // ��Ⱦ������������Ӧʵ�ֵ�ǰpass����Ⱦ״̬����.
    friend class ShaderEffect;
    virtual void OnPreDrawPass(ShaderEffect* pEffect, int iPass,
        bool bPrimaryEffect) = 0;
    virtual void OnPostDrawPass(ShaderEffect* pEffect, int iPass,
        bool bPrimaryEffect) = 0;

    // ����/�ָ���Ⱦ������任.
    void SetWorldTransformation(void);
    void RestoreWorldTransformation(void);

    // ����/�ָ���Ⱦ���ƹ�����.
    void SetLights(void);
    void RestoreLights(void);

    // ��Ⱦ����������Ⱦ�������.
    virtual void DrawElements(void) = 0;

    // ��Ⱦ����Դװ�����ͷ�.
    // ����Դ�,��Ҫ����ͼ��API����ʵ��.
    virtual void OnLoadVProgram(ResourceIdentifier*& rpID,
        VertexProgram* pVProgram) = 0;
    virtual void OnReleaseVProgram(ResourceIdentifier* pID) = 0;
    virtual void OnLoadGProgram(ResourceIdentifier*& rpID,
        GeometryProgram* pGProgram);
    virtual void OnReleaseGProgram(ResourceIdentifier* pID);
    virtual void OnLoadPProgram(ResourceIdentifier*& rpID,
        PixelProgram* pPProgram) = 0;
    virtual void OnReleasePProgram(ResourceIdentifier* pID) = 0;
    virtual void OnLoadTexture(ResourceIdentifier*& rpID,
        Texture* pTexture) = 0;
    virtual void OnReleaseTexture(ResourceIdentifier* pID) = 0;
    virtual void OnLoadVBuffer(ResourceIdentifier*& rpID,
        const Attributes& rIAttributes, const Attributes& rOAttributes,
        VertexBuffer* pVBuffer, VertexProgram* pVProgram) = 0;
    virtual void OnReleaseVBuffer(ResourceIdentifier* pID) = 0;
    virtual void OnLoadIBuffer(ResourceIdentifier*& rpID,
        IndexBuffer* pIBuffer) = 0;
    virtual void OnReleaseIBuffer(ResourceIdentifier* pID) = 0;
    // DirectX 10��Ⱦ����Ҫ�������º���.
    virtual void OnLoadRenderStateBlock(ResourceIdentifier*& rpID,
        RenderStateBlock* pRStateBlock);
    virtual void OnReleaseRenderStateBlock(ResourceIdentifier* pID);

    // ��Ⱦ����Դ������ر�.
    virtual void SetVProgramRC(RendererConstant* pRC) = 0;
    virtual void SetVProgramUC(UserConstant* pUC) = 0;
    virtual void SetGProgramRC(RendererConstant* pRC) = 0;
    virtual void SetGProgramUC(UserConstant* pUC) = 0;
    virtual void SetPProgramRC(RendererConstant* pRC) = 0;
    virtual void SetPProgramUC(UserConstant* pUC) = 0;
    virtual void UpdateVProgramConstants(VertexProgram* pVProgram) = 0;
    virtual void UpdateGProgramConstants(GeometryProgram* pGProgram) = 0;
    virtual void UpdatePProgramConstants(PixelProgram* pPProgram) = 0;
    virtual void OnEnableVProgram(ResourceIdentifier* pID) = 0;
    virtual void OnDisableVProgram(ResourceIdentifier* pID) = 0;
    virtual void OnEnableGProgram(ResourceIdentifier* pID);
    virtual void OnDisableGProgram(ResourceIdentifier* pID);
    virtual void OnEnablePProgram(ResourceIdentifier* pID) = 0;
    virtual void OnDisablePProgram(ResourceIdentifier* pID) = 0;
    virtual void OnEnableTexture(ResourceIdentifier* pID) = 0;
    virtual void OnDisableTexture(ResourceIdentifier* pID) = 0;
    virtual void OnEnableVBuffer(ResourceIdentifier* pID) = 0;
    virtual void OnDisableVBuffer(ResourceIdentifier* pID) = 0;
    virtual void OnEnableIBuffer(ResourceIdentifier* pID) = 0;
    virtual void OnDisableIBuffer(ResourceIdentifier* pID) = 0;
    // DirectX 10��Ⱦ����Ҫ�������º���.
    virtual void OnEnableRenderStateBlock(ResourceIdentifier* pID);
    virtual void OnDisableRenderStateBlock(ResourceIdentifier* pID);

    // ��ѡshader��������.
    // ĳЩͼ�ι��ߵ�shaderϵͳҪ����Ѿ�װ�ز������shader�����������.
    // ��GLSL,ESSLϵͳ.
    // OpenGL ES2��Ⱦ����Ҫ�����������.
    // ���ӹ��̷����󷵻�true,Ĭ�������û�����ӹ���ʱ,����false.
    virtual bool OnLinkPrograms(VertexProgram* pVProgram,
        GeometryProgram* pGProgram, PixelProgram* pPProgram);

    // ����T&L������,��Ҫ������shader����������Ⱦ��������ʵ��.

    // Operation����
    //   0 = matrix
    //   1 = transpose of matrix
    //   2 = inverse of matrix
    //   3 = inverse-transpose of matrix
    void GetTransform(const Matrix4f& rMat, int iOperation, float* afData);
    void SetConstantWMatrix(int iOperation, float* afData);
    void SetConstantVMatrix(int iOperation, float* afData);
    void SetConstantPMatrix(int iOperation, float* afData);
    void SetConstantWVMatrix(int iOperation, float* afData);
    void SetConstantVPMatrix(int iOperation, float* afData);
    void SetConstantWVPMatrix(int iOperation, float* afData);

    // ��Щ������Ȼû��ʹ��option����,�������Ա�������,
    // �⽫������ľ�̬����ָ����������������shader����.
    void SetConstantMaterialEmissive(int, float* afData);
    void SetConstantMaterialAmbient(int, float* afData);
    void SetConstantMaterialDiffuse(int, float* afData);
    void SetConstantMaterialSpecular(int, float* afData);
    void SetConstantCameraModelPosition(int, float* afData);
    void SetConstantCameraModelRight(int, float* afData);
    void SetConstantCameraModelUp(int, float* afData);
    void SetConstantCameraModelDirection(int, float* afData);
    void SetConstantCameraWorldPosition(int, float* afData);
    void SetConstantCameraWorldRight(int, float* afData);
    void SetConstantCameraWorldUp(int, float* afData);
    void SetConstantCameraWorldDirection(int, float* afData);
    void SetConstantProjectorModelPosition(int, float* afData);
    void SetConstantProjectorModelRight(int, float* afData);
    void SetConstantProjectorModelUp(int, float* afData);
    void SetConstantProjectorModelDirection(int, float* afData);
    void SetConstantProjectorWorldPosition(int, float* afData);
    void SetConstantProjectorWorldRight(int, float* afData);
    void SetConstantProjectorWorldUp(int, float* afData);
    void SetConstantProjectorWorldDirection(int, float* afData);

    void SetConstantProjectorMatrix(int, float* afData);

    // ��Щ�������õƹ�״̬.
    // ����ֵiLight������0��7֮��.(��ǰͼ�ι���֧�ַ�Χ)
    void SetConstantLightModelPosition(int iLight, float* afData);
    void SetConstantLightModelDirection(int iLight, float* afData);
    void SetConstantLightWorldPosition(int iLight, float* afData);
    void SetConstantLightWorldDirection(int iLight, float* afData);
    void SetConstantLightAmbient(int iLight, float* afData);
    void SetConstantLightDiffuse(int iLight, float* afData);
    void SetConstantLightSpecular(int iLight, float* afData);
    void SetConstantLightSpotCutoff(int iLight, float* afData);
    void SetConstantLightAttenuation(int iLight, float* afData);

    void SetConstantLightCount(int, float* afData);

    enum { SC_COUNT = 37 };
    typedef void (Renderer::*SetConstantFunction)(int, float*);
    static SetConstantFunction ms_aoSCFunction[SC_COUNT];
    void SetRendererConstant(RendererConstant::Type eRCType, float* afData);

    // ������Ⱦ.
    void ApplyEffect(ShaderEffect* pEffect, bool& rbPrimaryEffect);

// ���ݳ�Ա:
protected:
    // �豸��������Դ����,����Ⱦ�������ฺ���Ⲣ����.
    int m_iMaxLights;
    int m_iMaxColors;
    int m_iMaxTCoords;
    int m_iMaxVShaderImages;
    int m_iMaxGShaderImages;
    int m_iMaxPShaderImages;
    int m_iMaxStencilIndices;
    int m_iMaxUserClipPlanes;
    int m_iMaxVShaderProfile;
    int m_iMaxGShaderProfile;
    int m_iMaxPShaderProfile;
    int m_iMaxRenderTargets;

    // Active samplers in shader programs, at most m_iMaxVShaderImages of them
    // for vertex programs and at most m_iMaxGShaderImages of them for geometry
    // programs and at most m_iMaxPShaderImages of them for pixel programs.
    int m_iNumActiveSamplers;
    int m_iCurrentSampler;
    int m_iMaxActiveSamplerCount;
    SamplerInformation** m_apActiveSamplers;

    // ��Ⱦ���Ƿ�߱�shader compiler,
    // shader compiler�����ṩshaderԴ������װ�ؽ׶ε�online compilation.
    // ��������OpenGL/DirectXϵͳ,���߱�shader compiler.
    // �����ƶ��豸���п��ܲ��߱�shader compiler.
    // Ŀǰ����֧��OpenGL ES2��shader compiler���.
    bool m_bHasShaderCompiler;  // default: true

    // ���ں�frame buffer����.
    FrameBuffer::FormatType m_eFormat;
    FrameBuffer::DepthType m_eDepth;
    FrameBuffer::StencilType m_eStencil;
    FrameBuffer::BufferingType m_eBuffering;
    FrameBuffer::MultisamplingType m_eMultisampling;
    int m_iWidth, m_iHeight;
    ColorRGBA m_ClearColor;
    float m_fClearDepth;
    unsigned int m_uiClearStencil;
    bool m_bAllowRed, m_bAllowGreen, m_bAllowBlue, m_bAllowAlpha;

    // ���ڽ�����Ͷ��ĵ�ǰ�����.
    Camera* m_pCamera;

    // ȫ����Ⱦ״̬����.
    GlobalStatePtr m_aspState[GlobalState::MAX_STATE_TYPE];

    // shader�������ú������ڲ��ҵ�light����.
    // ��Ⱦ������������ڹ����ڼ䴴���������,����m_iMaxLights������Ԫ��.
    // ��Ⱦ������������ʱ�ͷŸ�����.
    int m_iNumActiveLights;
    ObjectPtr* m_aspLight;

    // ΪĳЩ��Ч�ṩ�����ͶӰ��,
    // ����projected texture��shadow map.
    Camera* m_pProjector;

    // ��ǰ������Ⱦ�ļ��������.
    Geometry* m_pGeometry;

    // ������Ⱦ������ʹ�õľ���任.
    // ֧��1x4������������4x4����.
    Matrix4f m_WorldMatrix, m_SaveWorldMatrix;
    Matrix4f m_ViewMatrix, m_SaveViewMatrix;
    Matrix4f m_ProjectionMatrix, m_SaveProjectionMatrix;

    // ��ǰ��Ⱦ������ID.
    int m_iFontID;

    // ֧�־�����Ч(Ĭ��Ϊfalse).
    bool m_bReverseCullFace;

    // ȫ��/����ģʽ��־.
    bool m_bFullscreen;
};

#include "SERenderer.inl"

}

#endif