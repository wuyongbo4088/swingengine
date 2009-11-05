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

#ifndef Swing_WindowApplication_H
#define Swing_WindowApplication_H

#include "SEApplication.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:����Ӧ�ó������
// ˵��:for iPhone application
// ����:jazzboysc
// ʱ��:20090604
//----------------------------------------------------------------------------
class WindowApplication : public Application
{
public:
    // �����
    WindowApplication(const char* acWindowTitle, int iXPosition,
        int iYPosition, int iWidth, int iHeight,
        const ColorRGBA& rBackgroundColor);
    virtual ~WindowApplication(void);

    // Ӧ�ó�����ʵ����ں���,����ֵΪexit code.
    virtual int Main(int iCount, char** apcArgument);

    // ��Ա����
    inline const char* GetWindowTitle(void) const;
    inline int GetXPosition(void) const;
    inline int GetYPosition(void) const;
    inline int GetWidth(void) const;
    inline int GetHeight(void) const;
    inline void SetRenderer(Renderer* pRenderer);
    inline Renderer* GetRenderer(void);
    inline void SetWindowID(int iWindowID);
    inline int GetWindowID(void) const;

    // �¼��ص�����
    virtual bool OnPrecreate(void);
    virtual bool OnInitialize(void);
    virtual void OnTerminate(void);
    virtual void OnMove(int iX, int iY);
    virtual void OnResize(int iWidth, int iHeight);
    virtual void OnDisplay(void);
    virtual void OnIdle(void);
    virtual bool OnKeyDown(unsigned char ucKey, int iX, int iY);
    virtual bool OnKeyUp(unsigned char ucKey, int iX, int iY);
    virtual bool OnSpecialKeyDown(int iKey, int iX, int iY);
    virtual bool OnSpecialKeyUp(int iKey, int iX, int iY);
    virtual bool OnMouseClick(int iButton, int iState, int iX, int iY,
        unsigned int uiModifiers);
    virtual bool OnMotion(int iButton, int iX, int iY,
        unsigned int uiModifiers);
    virtual bool OnPassiveMotion(int iX, int iY);

    // ���λ��
    void SetMousePosition(int iX, int iY);
    void GetMousePosition(int& riX, int& riY) const;

    // ������Ϣ.
    // ��Щ��ƽ̨��ص�,��������ฺ��ʵ����Щ�ӿں���.
    // ������WindowApplication��δ������.
    int GetStringWidth(const char* acText) const;
    int GetCharacterWidth(const char cCharacter) const;
    int GetFontHeight(void) const;

    // cocoa�������ڴ���rendererʱʹ��.
    inline FrameBuffer::FormatType GetFormat(void);
    inline FrameBuffer::DepthType GetDepth(void);
    inline FrameBuffer::StencilType GetStencil(void);
    inline FrameBuffer::BufferingType GetBuffering(void);
    inline FrameBuffer::MultisamplingType GetMultisampling(void);	

    // Key ID.
    // ��Щ��ƽ̨��ص�,��������ฺ������Щ����.
    // ������WindowApplication��δ������.
    int KEY_TERMINATE;  // default KEY_ESCAPE, redefine as desired
    static const int KEY_ESCAPE;
    static const int KEY_LEFT_ARROW;
    static const int KEY_RIGHT_ARROW;
    static const int KEY_UP_ARROW;
    static const int KEY_DOWN_ARROW;
    static const int KEY_HOME;
    static const int KEY_END;
    static const int KEY_PAGE_UP;
    static const int KEY_PAGE_DOWN;
    static const int KEY_INSERT;
    static const int KEY_DELETE;
    static const int KEY_F1;
    static const int KEY_F2;
    static const int KEY_F3;
    static const int KEY_F4;
    static const int KEY_F5;
    static const int KEY_F6;
    static const int KEY_F7;
    static const int KEY_F8;
    static const int KEY_F9;
    static const int KEY_F10;
    static const int KEY_F11;
    static const int KEY_F12;
    static const int KEY_BACKSPACE;
    static const int KEY_TAB;
    static const int KEY_ENTER;
    static const int KEY_RETURN;

    // keyboard modifiers
    static const int KEY_SHIFT;
    static const int KEY_CONTROL;
    static const int KEY_ALT;
    static const int KEY_COMMAND;

    // mouse buttons
    static const int MOUSE_LEFT_BUTTON;
    static const int MOUSE_MIDDLE_BUTTON;
    static const int MOUSE_RIGHT_BUTTON;

    // mouse state
    static const int MOUSE_UP;
    static const int MOUSE_DOWN;

    // mouse modifiers
    static const int MODIFIER_CONTROL;
    static const int MODIFIER_LBUTTON;
    static const int MODIFIER_MBUTTON;
    static const int MODIFIER_RBUTTON;
    static const int MODIFIER_SHIFT;

protected:
    // ���Ӻ���������ں���Main.
    static int Run(int iArgCount, char** apcArgument);

    // ���캯���������
    const char* m_acWindowTitle;
    int m_iXPosition, m_iYPosition, m_iWidth, m_iHeight;
    ColorRGBA m_BackgroundColor;

    // ����ID(��ʾ��ʽ��ƽ̨��ص�).
    int m_iWindowID;

    // ������Ⱦ�������frame buffer����.
    // ����������Ӧ�ó����캯��������Ϊ�����ֵ.
    FrameBuffer::FormatType m_eFormat;
    FrameBuffer::DepthType m_eDepth;
    FrameBuffer::StencilType m_eStencil;
    FrameBuffer::BufferingType m_eBuffering;
    FrameBuffer::MultisamplingType m_eMultisampling;

    // ��Ⱦ��(2D��3DӦ�ó���).
    Renderer* m_pRenderer;
};

#include "SEWindowApplication.inl"

}

#endif
