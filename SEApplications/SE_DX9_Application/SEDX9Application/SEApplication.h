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

#ifndef Swing_Application_H
#define Swing_Application_H

#include "SwingFoundation.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ˵��:
// ����:Sun Che
// ʱ��:20080809
//----------------------------------------------------------------------------
class SEApplication
{
public:
    // �����
    virtual ~SEApplication(void);

    // ΨһӦ�ó������
    static SEApplication* TheApplication;

    // Ψһ�����в�������
	static SECommand* TheCommand;

    // ��������������������ָ��.
    // int�����������в�������.char**�����ǲ����ַ�������.
    typedef int (*EntryPoint)(int, char**);
    static EntryPoint Run;

    // ƽ̨���ʵ����ʹ�õĶ������ݴ洢.
    void SetExtraData(int iIndex, int iSize, const void* pvData);
    void GetExtraData(int iIndex, int iSize, void* pvData) const;

    bool LaunchFileDialog(void) const;

    // ��ʵ��:
    // �������Ŀǰֻ֧��Microsoft Windows.
    // ����ƽ̨�в�֧��.
    void LaunchTreeControl(SESpatial* pScene, int iXPos, int iYPos,
        int iXSize, int iYSize);
    void ShutdownTreeControl(void);

    // ���ڲ���disk-streaming��string-treeϵͳ.
    // ������󱻴洢�����̲����Ժ�װ��.��debug�汾��,
    // �����洢��װ�����ݲ�ƥ��ʱ,����������.
    // ��װ��֮��,LaunchTreeControl����������,���ڲ���string-treeϵͳ.
    // �������Ŀǰֻ֧��Microsoft Windows.
    void TestStreaming(SESpatial* pScene, int iXPos, int iYPos, int iXSize,
        int iYSize, const char* acFilename);

protected:
    SEApplication(void);

    // ��������
    enum { APP_EXTRA_DATA_COUNT = 128 };
    char m_acExtraData[APP_EXTRA_DATA_COUNT];

    // ֧���ļ��Ի���
    bool m_bLaunchFileDialog;
};

#include "SEApplicationMCR.h"

}

#endif
