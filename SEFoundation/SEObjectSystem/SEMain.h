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

#ifndef Swing_Main_H
#define Swing_Main_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SESystem.h"
#include "SEMainMCR.h"

namespace Swing
{

class ImageCatalog;
class VertexProgramCatalog;
class GeometryProgramCatalog;
class PixelProgramCatalog;

//----------------------------------------------------------------------------
// ����:Ӧ�ó��������
// ˵��:������Ӧ�ó�������ǰ�ͽ�������������в���,
//      һϵ��pre-main,post-main�������������ڴ������ͷŸ������������ȫ������,
//      pre-main,post-main��������������ݸ�������ʵ��
// ����:Sun Che
// ʱ��:20080324
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Main
{
public:
    typedef void (*Initializer)(void);  // ����pre-main����ָ������
    typedef std::vector<Initializer> InitializerArray; // ����pre-main����ָ����������
    static void AddInitializer(Initializer FuncInitialize);
    static void Initialize(void);

    typedef void (*Terminator)(void); // ����post-main����ָ������
    typedef std::vector<Terminator> TerminatorArray; // ����post-main����ָ����������
    static void AddTerminator(Terminator FuncTerminate);
    static void Terminate(void);

private:
    static InitializerArray* ms_pInitializers; // ȫ��pre-main����ָ������
    static TerminatorArray* ms_pTerminators; // ȫ��post-main����ָ������
    static int ms_iStartObjects;
    static int ms_iFinalObjects;

    static ImageCatalog* ms_pImageCatalog;
    static VertexProgramCatalog* ms_pVertexProgramCatalog;
    static GeometryProgramCatalog* ms_pGeometryProgramCatalog;
    static PixelProgramCatalog* ms_pPixelProgramCatalog;
};

}

#endif
