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

class SEAttributes;
class SEBindable;
class SECamera;
class Effect;
class SEGeometry;
class SEGeometryProgram;
class SEIndexBuffer;
class SELight;
class SEPixelProgram;
class SEResourceIdentifier;
class ShaderEffect;
class SESpatial;
class SETexture;
class SEVertexBuffer;
class SEVertexProgram;
class SEUnculledObject;
class SEUnculledSet;
class SERenderStateBlock;

//----------------------------------------------------------------------------
// Description:Renderer abstract base class
// Author:Sun Che
// Date:20080320
//----------------------------------------------------------------------------
class SE_FOUNDATION_API SERenderer
{
public:
    // Abstract base class.
    virtual ~SERenderer(void);

    // RTTI.
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
    virtual int GetType(void) const = 0;

    // SECamera access.
    void SetCamera(SECamera* pCamera);
    inline SECamera* GetCamera(void) const;

    // Frame buffer parameters.
    inline SEFrameBuffer::FormatType GetFormatType(void) const;
    inline SEFrameBuffer::DepthType GetDepthType(void) const;
    inline SEFrameBuffer::StencilType GetStencilType(void) const;
    inline SEFrameBuffer::BufferingType GetBufferingType(void) const;
    inline SEFrameBuffer::MultisamplingType GetMultisamplingType(void) const;

    // Window parameters.
    inline int GetWidth(void) const;
    inline int GetHeight(void) const;
    virtual void Resize(int iWidth, int iHeight);
    virtual void ToggleFullscreen(void);

    // Back buffer clear color.
    inline void SetClearColor(const SEColorRGBA& rClearColor);
    inline const SEColorRGBA& GetClearColor(void) const;

    // Depth buffer clear value.
    inline void SetClearDepth(float fClearDepth);
    inline float GetClearDepth(void) const;

    // Stencil buffer clear value.
    inline void SetClearStencil(unsigned int uiClearValue);
    inline unsigned int GetClearStencil(void) const;

    // Pre-draw/Post-draw, DIRECTX9 API specific stuff.
    virtual bool BeginScene(void);
    virtual void EndScene(void);

    // Full window buffer operation.
    virtual void ClearBackBuffer(void) = 0;
    virtual void ClearZBuffer(void) = 0;
    virtual void ClearStencilBuffer(void) = 0;
    virtual void ClearBuffers(void) = 0;
    virtual void DisplayBackBuffer(void) = 0;

    // Sub-window buffer operation.
    virtual void ClearBackBuffer(int iXPos, int iYPos, int iWidth,
        int iHeight) = 0;
    virtual void ClearZBuffer(int iXPos, int iYPos, int iWidth,
        int iHeight) = 0;
    virtual void ClearStencilBuffer(int iXPos, int iYPos, int iWidth,
        int iHeight) = 0;
    virtual void ClearBuffers(int iXPos, int iYPos, int iWidth,
        int iHeight) = 0;

    // SEObject rendering entry point.
    virtual void DrawScene(SEUnculledSet& rVisibleSet);
    virtual void Draw(SEGeometry* pGeometry);

    // Text rendering.
    virtual int LoadFont(const char* acFace, int iSize, bool bBold,
        bool bItalic) = 0;
    virtual void UnloadFont(int iFontID) = 0;
    virtual bool SelectFont(int iFontID) = 0;
    virtual void Draw(int iX, int iY, const SEColorRGBA& rColor,
        const char* pText) = 0;

    // 2D rendering.
    virtual void Draw(const unsigned char* aucBuffer) = 0;

    // Vertex shader profiles.
    enum
    {
        VS_2_0,   // DirectX9
        VS_3_0,   // DirectX9
        VS_4_0,   // DirectX10
        ARBVP1,   // OpenGL
        VP40,     // OpenGL
        OES2VP1,  // OpenGL ES2.0
        SFTVP1,   // Software
        MAX_VP_TYPES
    };

    // SEGeometry shader profiles.
    // SEGeometry shader stage is an optional stage,
    // DirectX10 and OpenGL3 could support it for now.
    enum
    {
        GS_UNSUPPORTED, // Unsupported by current renderer
        GS_4_0,         // DirectX10
        GLSLG,          // OpenGL
        SFTGP1,         // Software
        MAX_GP_TYPES
    };

    // Pixel shader profile.
    enum
    {
        PS_2_0,    // DirectX9
        PS_3_0,    // DirectX9
        PS_4_0,    // DirectX10
        ARBFP1,    // OpenGL
        FP40,      // OpenGL
        OES2FP1,   // OpenGL ES2.0
        SFTFP1,    // Software
        MAX_PP_TYPES
    };

    // Renderer capability query.
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

    // Shader compiler existence query.
    inline bool HasShaderCompiler(void) const;

    // Render state management.
    virtual void SetAlphaState(SEAlphaState* pState);
    virtual void SetCullState(SECullState* pState);
    virtual void SetMaterialState(SEMaterialState* pState);
    virtual void SetPolygonOffsetState(SEPolygonOffsetState* pState);
    virtual void SetStencilState(SEStencilState* pState);
    virtual void SetWireframeState(SEWireframeState* pState);
    virtual void SetZBufferState(SEZBufferState* pState);
    inline SEAlphaState* GetAlphaState(void);
    inline SECullState* GetCullState(void);
    inline SEMaterialState* GetMaterialState(void);
    inline SEPolygonOffsetState* GetPolygonOffsetState(void);
    inline SEStencilState* GetStencilState(void);
    inline SEWireframeState* GetWireframeState(void);
    inline SEZBufferState* GetZBufferState(void);
    inline void SetReverseCullFace(bool bReverseCullFace);
    inline bool GetReverseCullFace(void) const;

    // Prototype of bindable resource release function.
    typedef void (SERenderer::*ReleaseFunction)(SEBindable*);

    // Resource loading/releasing functions.
    void LoadAllResources(SESpatial* pScene);
    void ReleaseAllResources(SESpatial* pScene);
    void LoadResources(SEGeometry* pGeometry);
    void ReleaseResources(SEGeometry* pGeometry);
    void LoadResources(Effect* pEffect);
    void ReleaseResources(Effect* pEffect);
    void LoadVProgram(SEVertexProgram* pVProgram);
    void ReleaseVProgram(SEBindable* pVProgram);
    void LoadGProgram(SEGeometryProgram* pGProgram);
    void ReleaseGProgram(SEBindable* pGProgram);
    void LoadPProgram(SEPixelProgram* pPProgram);
    void ReleasePProgram(SEBindable* pPProgram);
    void LoadTexture(SETexture* pTexture);
    void ReleaseTexture(SEBindable* pTexture);
    void LoadVBuffer(const SEAttributes& rIAttributes, 
        const SEAttributes& rOAttributes,
        SEVertexBuffer* pVBuffer, SEVertexProgram* pVProgram);
    void ReleaseVBuffer(SEBindable* pVBuffer);
    void LoadIBuffer(SEIndexBuffer* pIBuffer);
    void ReleaseIBuffer(SEBindable* pIBuffer);
    // DirectX10 specific functions.
    void LoadRenderStateBlock(SERenderStateBlock* pRStateBlock);
    void ReleaseRenderStateBlock(SEBindable* pRStateBlock);

    // Resource enabling/disabling functions.
    void EnableVProgram(SEVertexProgram* pVProgram);
    void DisableVProgram(SEVertexProgram* pVProgram);
    void EnableGProgram(SEGeometryProgram* pGProgram);
    void DisableGProgram(SEGeometryProgram* pGProgram);
    void EnablePProgram(SEPixelProgram* pPProgram);
    void DisablePProgram(SEPixelProgram* pPProgram);
    void EnableTexture(SETexture* pTexture);
    void DisableTexture(SETexture* pTexture);
    SEResourceIdentifier* EnableVBuffer(const SEAttributes& rIAttributes,
        const SEAttributes& rOAttributes, SEVertexProgram* pVProgram = 0);
    void DisableVBuffer(SEResourceIdentifier* pID, 
        SEVertexProgram* pVProgram = 0);
    void EnableIBuffer(void);
    void DisableIBuffer(void);
    // DirectX10 specific functions.
    void EnableRenderStateBlock(SERenderStateBlock* pRStateBlock);
    void DisableRenderStateBlock(SERenderStateBlock* pRStateBlock);

    // For use by effects with lights.
    inline void SetLightCount(int iCount);
    inline void SetLight(int i, SELight* pLight);
    inline SELight* GetLight(int i);

    // For use by effects with projectors.
    inline void SetProjector(SECamera* pProjector);
    inline SECamera* GetProjector(void);

    // Enable or disable which color channels will be written to the color
    // buffer.
    virtual void SetColorMask(bool bAllowRed, bool bAllowGreen,
        bool bAllowBlue, bool bAllowAlpha);
    virtual void GetColorMask(bool& rbAllowRed, bool& rbAllowGreen,
        bool& rbAllowBlue, bool& rbAllowAlpha);

    // Include additional clip planes.  The input plane must be in model
    // coordinates.  It is transformed internally to camera coordinates to
    // support clipping in clip space.
    virtual void EnableUserClipPlane(int i, const SEPlane3f& rPlane) = 0;
    virtual void DisableUserClipPlane(int i) = 0;

    // Support for model-to-world transformation management.
    void SetWorldTransformation(void);
    void RestoreWorldTransformation(void);

    // The input transformation is applied to world-space vertices before
    // the view matrix is applied.
    void SetPostWorldTransformation(const SEMatrix4f& rMatrix);
    void RestorePostWorldTransformation(void);

    // Access the current state of the matrices.
    inline const SEMatrix4f& GetWorldMatrix(void) const;
    inline const SEMatrix4f& GetViewMatrix(void) const;
    inline const SEMatrix4f& GetProjectionMatrix(void) const;

protected:
    // Abstract base class.
    SERenderer(SEFrameBuffer::FormatType eFormat, SEFrameBuffer::DepthType eDepth,
        SEFrameBuffer::StencilType eStencil,
        SEFrameBuffer::BufferingType eBuffering,
        SEFrameBuffer::MultisamplingType eMultisampling,
        int iWidth, int iHeight);

    // Support for camera access and transformation setting.
    friend class SECamera;
    void OnFrameChange(void);
    void OnFrustumChange(void);
    virtual void OnViewportChange(void) = 0;
    virtual void OnDepthRangeChange(void) = 0;

    // Global render state management.
    void SetGlobalState(SEGlobalStatePtr aspState[]);
    void RestoreGlobalState(SEGlobalStatePtr aspState[]);

    // Per-geometry pre/post-draw entry point.
    // CAUTION:
    // SERenderer derived class should implements the setup of render state for
    // drawing a geometry.
    virtual void OnPreDrawGeometry(void) = 0;
    virtual void OnPostDrawGeometry(void) = 0;

    // Per-pass pre/post-draw entry point.
    // These functions are called by ShaderEffect class's OnPreApplyPass()/
    // OnPostApplyPass().
    // CAUTION:
    // SERenderer derived class should implements the setup of render state for
    // drawing a pass.
    friend class ShaderEffect;
    virtual void OnPreDrawPass(ShaderEffect* pEffect, int iPass,
        bool bPrimaryEffect) = 0;
    virtual void OnPostDrawPass(ShaderEffect* pEffect, int iPass,
        bool bPrimaryEffect) = 0;

    // Support for light array management.
    void SetLights(void);
    void RestoreLights(void);

    // The main entry point to drawing in the derived-class renderers.
    virtual void DrawElements(void) = 0;

    // Resource loading/releasing functions.
    virtual void OnLoadVProgram(SEResourceIdentifier*& rpID,
        SEVertexProgram* pVProgram) = 0;
    virtual void OnReleaseVProgram(SEResourceIdentifier* pID) = 0;
    virtual void OnLoadGProgram(SEResourceIdentifier*& rpID,
        SEGeometryProgram* pGProgram);
    virtual void OnReleaseGProgram(SEResourceIdentifier* pID);
    virtual void OnLoadPProgram(SEResourceIdentifier*& rpID,
        SEPixelProgram* pPProgram) = 0;
    virtual void OnReleasePProgram(SEResourceIdentifier* pID) = 0;
    virtual void OnLoadTexture(SEResourceIdentifier*& rpID,
        SETexture* pTexture) = 0;
    virtual void OnReleaseTexture(SEResourceIdentifier* pID) = 0;
    virtual void OnLoadVBuffer(SEResourceIdentifier*& rpID,
        const SEAttributes& rIAttributes, const SEAttributes& rOAttributes,
        SEVertexBuffer* pVBuffer, SEVertexProgram* pVProgram) = 0;
    virtual void OnReleaseVBuffer(SEResourceIdentifier* pID) = 0;
    virtual void OnLoadIBuffer(SEResourceIdentifier*& rpID,
        SEIndexBuffer* pIBuffer) = 0;
    virtual void OnReleaseIBuffer(SEResourceIdentifier* pID) = 0;
    // DirectX10 specific functions.
    virtual void OnLoadRenderStateBlock(SEResourceIdentifier*& rpID,
        SERenderStateBlock* pRStateBlock);
    virtual void OnReleaseRenderStateBlock(SEResourceIdentifier* pID);

    // Resource enabling/disabling functions.
    virtual void SetVProgramRC(SERendererConstant* pRC) = 0;
    virtual void SetVProgramUC(SEUserConstant* pUC) = 0;
    virtual void SetGProgramRC(SERendererConstant* pRC) = 0;
    virtual void SetGProgramUC(SEUserConstant* pUC) = 0;
    virtual void SetPProgramRC(SERendererConstant* pRC) = 0;
    virtual void SetPProgramUC(SEUserConstant* pUC) = 0;
    virtual void UpdateVProgramConstants(SEVertexProgram* pVProgram) = 0;
    virtual void UpdateGProgramConstants(SEGeometryProgram* pGProgram) = 0;
    virtual void UpdatePProgramConstants(SEPixelProgram* pPProgram) = 0;
    virtual void OnEnableVProgram(SEResourceIdentifier* pID) = 0;
    virtual void OnDisableVProgram(SEResourceIdentifier* pID) = 0;
    virtual void OnEnableGProgram(SEResourceIdentifier* pID);
    virtual void OnDisableGProgram(SEResourceIdentifier* pID);
    virtual void OnEnablePProgram(SEResourceIdentifier* pID) = 0;
    virtual void OnDisablePProgram(SEResourceIdentifier* pID) = 0;
    virtual void OnEnableTexture(SEResourceIdentifier* pID) = 0;
    virtual void OnDisableTexture(SEResourceIdentifier* pID) = 0;
    virtual void OnEnableVBuffer(SEResourceIdentifier* pID, 
        SEVertexProgram* pVProgram) = 0;
    virtual void OnDisableVBuffer(SEResourceIdentifier* pID, 
        SEVertexProgram* pVProgram) = 0;
    virtual void OnEnableIBuffer(SEResourceIdentifier* pID) = 0;
    virtual void OnDisableIBuffer(SEResourceIdentifier* pID) = 0;
    // DirectX10 specific functions.
    virtual void OnEnableRenderStateBlock(SEResourceIdentifier* pID);
    virtual void OnDisableRenderStateBlock(SEResourceIdentifier* pID);

    // ��ѡshader��������.
    // ĳЩͼ�ι��ߵ�shaderϵͳҪ����Ѿ�װ�ز������shader�����������.
    // ��GLSL,ESSLϵͳ.
    // OpenGL ES2��Ⱦ����Ҫ�����������.
    // ���ӹ��̷����󷵻�true,Ĭ�������û�����ӹ���ʱ,����false.
    virtual bool OnLinkPrograms(SEVertexProgram* pVProgram,
        SEGeometryProgram* pGProgram, SEPixelProgram* pPProgram);

    // ����T&L������,��Ҫ������shader����������Ⱦ��������ʵ��.

    // Operation����
    //   0 = matrix
    //   1 = transpose of matrix
    //   2 = inverse of matrix
    //   3 = inverse-transpose of matrix
    void GetTransform(const SEMatrix4f& rMat, int iOperation, float* afData);
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
    typedef void (SERenderer::*SetConstantFunction)(int, float*);
    static SetConstantFunction ms_aoSCFunction[SC_COUNT];
    void SetRendererConstant(SERendererConstant::Type eRCType, float* afData);

    // ������Ⱦ.
    void ApplyEffect(ShaderEffect* pEffect, bool& rbPrimaryEffect);

// Data member:
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
    SESamplerInformation** m_apActiveSamplers;

    // ��Ⱦ���Ƿ�߱�shader compiler,
    // shader compiler�����ṩshaderԴ������װ�ؽ׶ε�online compilation.
    // ��������OpenGL/DirectXϵͳ,���߱�shader compiler.
    // �����ƶ��豸���п��ܲ��߱�shader compiler.
    // Ŀǰ����֧��OpenGL ES2��shader compiler���.
    bool m_bHasShaderCompiler;  // default: true

    // ���ں�frame buffer����.
    SEFrameBuffer::FormatType m_eFormat;
    SEFrameBuffer::DepthType m_eDepth;
    SEFrameBuffer::StencilType m_eStencil;
    SEFrameBuffer::BufferingType m_eBuffering;
    SEFrameBuffer::MultisamplingType m_eMultisampling;
    int m_iWidth, m_iHeight;
    SEColorRGBA m_ClearColor;
    float m_fClearDepth;
    unsigned int m_uiClearStencil;
    bool m_bAllowRed, m_bAllowGreen, m_bAllowBlue, m_bAllowAlpha;

    // ���ڽ�����Ͷ��ĵ�ǰ�����.
    SECamera* m_pCamera;

    // ȫ����Ⱦ״̬����.
    SEGlobalStatePtr m_aspState[SEGlobalState::MAX_STATE_TYPE];

    // shader�������ú������ڲ��ҵ�light����.
    // ��Ⱦ������������ڹ����ڼ䴴���������,����m_iMaxLights������Ԫ��.
    // ��Ⱦ������������ʱ�ͷŸ�����.
    int m_iNumActiveLights;
    SEObjectPtr* m_aspLight;

    // ΪĳЩ��Ч�ṩ�����ͶӰ��,
    // ����projected texture��shadow map.
    SECamera* m_pProjector;

    // ��ǰ������Ⱦ�ļ��������.
    SEGeometry* m_pGeometry;

    // ������Ⱦ������ʹ�õľ���任.
    // ֧��1x4������������4x4����.
    SEMatrix4f m_WorldMatrix, m_SaveWorldMatrix;
    SEMatrix4f m_ViewMatrix, m_SaveViewMatrix;
    SEMatrix4f m_ProjectionMatrix, m_SaveProjectionMatrix;

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
