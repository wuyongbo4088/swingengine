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

#include "SEDX10RendererPCH.h"
#include "SEDX10Renderer.h"

using namespace Swing;

D3D10_COMPARISON_FUNC DX10Renderer::ms_aeZBufferCompare[ZBufferState::CF_COUNT] = 
{
    D3D10_COMPARISON_NEVER,           // ZBufferState::CF_NEVER
    D3D10_COMPARISON_LESS,            // ZBufferState::CF_LESS
    D3D10_COMPARISON_EQUAL,           // ZBufferState::CF_EQUAL
    D3D10_COMPARISON_LESS_EQUAL,      // ZBufferState::CF_LEQUAL
    D3D10_COMPARISON_GREATER,         // ZBufferState::CF_GREATER
    D3D10_COMPARISON_NOT_EQUAL,       // ZBufferState::CF_NOTEQUAL
    D3D10_COMPARISON_GREATER_EQUAL,   // ZBufferState::CF_GEQUAL
    D3D10_COMPARISON_ALWAYS           // ZBufferState::CF_ALWAYS
};

D3D10_COMPARISON_FUNC DX10Renderer::ms_aeStencilCompare[StencilState::CF_COUNT] = 
{
    D3D10_COMPARISON_NEVER,           // StencilState::CF_NEVER
    D3D10_COMPARISON_LESS,            // StencilState::CF_LESS
    D3D10_COMPARISON_EQUAL,           // StencilState::CF_EQUAL
    D3D10_COMPARISON_LESS_EQUAL,      // StencilState::CF_LEQUAL
    D3D10_COMPARISON_GREATER,         // StencilState::CF_GREATER
    D3D10_COMPARISON_NOT_EQUAL,       // StencilState::CF_NOTEQUAL
    D3D10_COMPARISON_GREATER_EQUAL,   // StencilState::CF_GEQUAL
    D3D10_COMPARISON_ALWAYS           // StencilState::CF_ALWAYS
};

D3D10_STENCIL_OP DX10Renderer::ms_aeStencilOperation[StencilState::OT_COUNT] =
{
    D3D10_STENCIL_OP_KEEP,      // StencilState::OT_KEEP
    D3D10_STENCIL_OP_ZERO,      // StencilState::OT_ZERO
    D3D10_STENCIL_OP_REPLACE,   // StencilState::OT_REPLACE
    D3D10_STENCIL_OP_INCR,      // StencilState::OT_INCREMENT
    D3D10_STENCIL_OP_DECR,      // StencilState::OT_DECREMENT
    D3D10_STENCIL_OP_INVERT     // StencilState::OT_INVERT
};

//----------------------------------------------------------------------------
void DX10Renderer::GenerateDepthStencilState(
    const RenderStateBlock* pRStateBlock, 
    ID3D10DepthStencilState*& rpDX10DSState)
{
    GlobalState* pState = pRStateBlock->States[GlobalState::ZBUFFER];
    ZBufferState* pZBufferState = (ZBufferState*)pState;
    pState = pRStateBlock->States[GlobalState::STENCIL];
    StencilState* pStencilState = (StencilState*)pState;

    // 检查是否需要创建一个depth stencil state对象.
    if( !pZBufferState && !pStencilState )
    {
        rpDX10DSState = 0;
        return;
    }

    D3D10_DEPTH_STENCIL_DESC tempDSSDesc;
    memset(&tempDSSDesc, 0, sizeof(D3D10_DEPTH_STENCIL_DESC));

    // 填充zbuffer state相关参数.
    if( !pZBufferState )
    {
        pState = GlobalState::Default[GlobalState::ZBUFFER];
        pZBufferState = (ZBufferState*)pState;
    }
    SE_ASSERT( pZBufferState );

    if( pZBufferState->Enabled )
    {
        tempDSSDesc.DepthEnable = true;
        tempDSSDesc.DepthFunc = ms_aeZBufferCompare[pZBufferState->Compare]; 
    }
    else
    {
        tempDSSDesc.DepthEnable = false;
    }

    if( pZBufferState->Writable )
    {
        tempDSSDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
    }
    else
    {
        tempDSSDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ZERO;
    }

    // 填充stencil state相关参数.
    if( !pStencilState )
    {
        pState = GlobalState::Default[GlobalState::STENCIL];
        pStencilState = (StencilState*)pState;
    }
    SE_ASSERT( pStencilState );

    if( pStencilState->Enabled )
    {
        tempDSSDesc.StencilEnable = true;
        tempDSSDesc.StencilReadMask = (UINT8)pStencilState->Mask;
        tempDSSDesc.StencilWriteMask = (UINT8)pStencilState->WriteMask;

        tempDSSDesc.FrontFace.StencilFunc = 
            ms_aeStencilCompare[pStencilState->Compare];
        tempDSSDesc.FrontFace.StencilPassOp = 
            ms_aeStencilOperation[pStencilState->OnZPass];
        tempDSSDesc.FrontFace.StencilFailOp = 
            ms_aeStencilOperation[pStencilState->OnFail];
        tempDSSDesc.FrontFace.StencilDepthFailOp = 
            ms_aeStencilOperation[pStencilState->OnZFail];

        // 目前Swing Engine只支持front/back face一致.
        tempDSSDesc.BackFace.StencilFunc = 
            ms_aeStencilCompare[pStencilState->Compare];
        tempDSSDesc.BackFace.StencilPassOp = 
            ms_aeStencilOperation[pStencilState->OnZPass];
        tempDSSDesc.BackFace.StencilFailOp = 
            ms_aeStencilOperation[pStencilState->OnFail];
        tempDSSDesc.BackFace.StencilDepthFailOp = 
            ms_aeStencilOperation[pStencilState->OnZFail];
    }
    else
    {
        tempDSSDesc.StencilEnable = false;
    }

    ms_hResult = m_pDX10Device->CreateDepthStencilState(&tempDSSDesc, 
        &rpDX10DSState);
    SE_ASSERT( ms_hResult );
}
//----------------------------------------------------------------------------