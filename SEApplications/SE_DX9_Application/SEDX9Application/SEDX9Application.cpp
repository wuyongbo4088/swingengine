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

#include "SEDX9ApplicationPCH.h"

#include <windows.h>

// ��̬����DirectX9��.
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "d3d9.lib")
//#ifdef _DEBUG
//#pragma comment(lib, "d3dx9d.lib")
//#else
#pragma comment(lib, "d3dx9.lib")
//#endif
#pragma comment(lib, "dxerr9.lib")

// ��̬����OpenAL1.0��.
#pragma comment(lib, "openal32.lib")

#include "SEWindowApplication.h"
#include "SEDX9Renderer.h"
#include "SETreeControl.h"
#include "SEWalRenderer.h"

using namespace Swing;

const int WindowApplication::KEY_ESCAPE = VK_ESCAPE;
const int WindowApplication::KEY_LEFT_ARROW = VK_LEFT;
const int WindowApplication::KEY_RIGHT_ARROW = VK_RIGHT;
const int WindowApplication::KEY_UP_ARROW = VK_UP;
const int WindowApplication::KEY_DOWN_ARROW = VK_DOWN;
const int WindowApplication::KEY_HOME = VK_HOME;
const int WindowApplication::KEY_END = VK_END;
const int WindowApplication::KEY_PAGE_UP = VK_PRIOR;
const int WindowApplication::KEY_PAGE_DOWN = VK_NEXT;
const int WindowApplication::KEY_INSERT = VK_INSERT;
const int WindowApplication::KEY_DELETE = VK_DELETE;
const int WindowApplication::KEY_F1 = VK_F1;
const int WindowApplication::KEY_F2 = VK_F2;
const int WindowApplication::KEY_F3 = VK_F3;
const int WindowApplication::KEY_F4 = VK_F4;
const int WindowApplication::KEY_F5 = VK_F5;
const int WindowApplication::KEY_F6 = VK_F6;
const int WindowApplication::KEY_F7 = VK_F7;
const int WindowApplication::KEY_F8 = VK_F8;
const int WindowApplication::KEY_F9 = VK_F9;
const int WindowApplication::KEY_F10 = VK_F10;
const int WindowApplication::KEY_F11 = VK_F11;
const int WindowApplication::KEY_F12 = VK_F12;
const int WindowApplication::KEY_BACKSPACE = VK_BACK;
const int WindowApplication::KEY_TAB = VK_TAB;
const int WindowApplication::KEY_ENTER = VK_RETURN;
const int WindowApplication::KEY_RETURN = VK_RETURN;

const int WindowApplication::KEY_SHIFT = MK_SHIFT;
const int WindowApplication::KEY_CONTROL = MK_CONTROL;
const int WindowApplication::KEY_ALT = 0;      // ��ǰ��δ����
const int WindowApplication::KEY_COMMAND = 0;  // ��ǰ��δ����

const int WindowApplication::MOUSE_LEFT_BUTTON = 0;
const int WindowApplication::MOUSE_MIDDLE_BUTTON = 1;
const int WindowApplication::MOUSE_RIGHT_BUTTON = 2;
const int WindowApplication::MOUSE_UP = 0;
const int WindowApplication::MOUSE_DOWN = 1;

const int WindowApplication::MODIFIER_CONTROL = MK_CONTROL;
const int WindowApplication::MODIFIER_LBUTTON = MK_LBUTTON;
const int WindowApplication::MODIFIER_MBUTTON = MK_MBUTTON;
const int WindowApplication::MODIFIER_RBUTTON = MK_RBUTTON;
const int WindowApplication::MODIFIER_SHIFT = MK_SHIFT;

// VC7.1 basetsd.h�ж�����,��VC6 basetsd.h��û��.
#ifndef IntToPtr
#define IntToPtr(i) ((VOID*)(INT_PTR)((int)i))
#endif

#ifndef PtrToUint
#define PtrToUint(p) ((UINT)(UINT_PTR)(p))
#endif

#ifndef PtrToInt
#define PtrToInt(p)  ((INT)(INT_PTR)(p))
#endif

//----------------------------------------------------------------------------
void WindowApplication::SetMousePosition(int iX, int iY)
{
    HWND hWnd = (HWND)IntToPtr(m_iWindowID);
    POINT tempPoint;
    tempPoint.x = (LONG)iX;
    tempPoint.y = (LONG)iY;
    ClientToScreen(hWnd, &tempPoint);
    SetCursorPos(tempPoint.x, tempPoint.y);
}
//----------------------------------------------------------------------------
void WindowApplication::GetMousePosition(int& riX, int& riY) const
{
    HWND hWnd = (HWND)IntToPtr(m_iWindowID);
    POINT tempPoint;
    GetCursorPos(&tempPoint);
    ScreenToClient(hWnd, &tempPoint);
    riX = (int)tempPoint.x;
    riY = (int)tempPoint.y;
}
//----------------------------------------------------------------------------
int WindowApplication::GetStringWidth(const char* acText) const
{
    if( !acText || strlen(acText) == 0 )
    {
        return 0;
    }

    HWND hWnd = (HWND)IntToPtr(m_iWindowID);
    HDC hDC = GetDC(hWnd);
    SIZE tempSize;
    GetTextExtentPoint32(hDC, acText, (int)strlen(acText), &tempSize);
    ReleaseDC(hWnd, hDC);

    return (int)tempSize.cx;
}
//----------------------------------------------------------------------------
int WindowApplication::GetCharacterWidth(const char cCharacter) const
{
    HWND hWnd = (HWND)IntToPtr(m_iWindowID);
    HDC hDC = GetDC(hWnd);
    SIZE tempSize;
    GetTextExtentPoint32(hDC, &cCharacter, 1, &tempSize);
    ReleaseDC(hWnd, hDC);

    return (int)tempSize.cx;
}
//----------------------------------------------------------------------------
int WindowApplication::GetFontHeight() const
{
    HWND hWnd = (HWND)IntToPtr(m_iWindowID);
    HDC hDC = GetDC(hWnd);
    TEXTMETRIC tempMetric;
    GetTextMetrics(hDC, &tempMetric);
    ReleaseDC(hWnd, hDC);

    return (int)tempMetric.tmHeight;
}
//----------------------------------------------------------------------------
LRESULT CALLBACK MsWindowEventHandler(HWND hWnd, UINT uiMsg, WPARAM wParam,
    LPARAM lParam)
{
    WindowApplication* pTheApp =
        (WindowApplication*)Application::TheApplication;

    if( !pTheApp || !pTheApp->GetWindowID() )
    {
        return DefWindowProc(hWnd, uiMsg, wParam, lParam);
    }

    switch( uiMsg ) 
    {
        case WM_PAINT:
        {
            PAINTSTRUCT tempPS;
            BeginPaint(hWnd, &tempPS);
            pTheApp->OnDisplay();
            EndPaint(hWnd, &tempPS);

            return 0;
        }
        case WM_ERASEBKGND:
        {
            // ��windows��Ҫ�������ɫ(Ӧ�ó�����).
            return 1;
        }
        case WM_MOVE:
        {
            int iXPos = int(LOWORD(lParam));
            int iYPos = int(HIWORD(lParam));
            pTheApp->OnMove(iXPos, iYPos);

            return 0;
        }
        case WM_SIZE:
        {
            int iWidth = int(LOWORD(lParam));
            int iHeight = int(HIWORD(lParam));
            pTheApp->OnResize(iWidth, iHeight);

            return 0;
        }
        case WM_CHAR:
        {
            unsigned char ucKey = (unsigned char)(char)wParam;

            // �������KEY_TERMINATE,���˳�Ӧ�ó���.
            if( ucKey == pTheApp->KEY_TERMINATE )
            {
                PostQuitMessage(0);

                return 0;
            }

            // ��ȡcursor��client coordinate�µ�λ��.
            POINT tempPoint;
            GetCursorPos(&tempPoint);
            ScreenToClient(hWnd, &tempPoint);
            int iXPos = (int)tempPoint.x;
            int iYPos = (int)tempPoint.y;

            pTheApp->OnKeyDown(ucKey, iXPos, iYPos);

            return 0;
        }
        case WM_KEYDOWN:
        {
            int iVirtKey = int(wParam);

            // ��ȡcursor��client coordinate�µ�λ��.
            POINT tempPoint;
            GetCursorPos(&tempPoint);
            ScreenToClient(hWnd, &tempPoint);
            int iXPos = (int)tempPoint.x;
            int iYPos = (int)tempPoint.y;

            if( (VK_F1 <= iVirtKey && iVirtKey <= VK_F12)
            ||  (VK_PRIOR <= iVirtKey && iVirtKey <= VK_DOWN)
            ||  (iVirtKey == VK_INSERT) )
            {
                pTheApp->OnSpecialKeyDown(iVirtKey, iXPos, iYPos);
            }

            return 0;
        }
        case WM_KEYUP:
        {
            int iVirtKey = int(wParam);

            // ��ȡcursor��client coordinate�µ�λ��.
            POINT tempPoint;
            GetCursorPos(&tempPoint);
            ScreenToClient(hWnd, &tempPoint);
            int iXPos = (int)tempPoint.x;
            int iYPos = (int)tempPoint.y;

            if( (VK_F1 <= iVirtKey && iVirtKey <= VK_F12)
            ||  (VK_PRIOR <= iVirtKey && iVirtKey <= VK_DOWN)
            ||  (iVirtKey == VK_INSERT) )
            {
                pTheApp->OnSpecialKeyUp(iVirtKey, iXPos, iYPos);
            }
            else
            {
                pTheApp->OnKeyUp((unsigned char)iVirtKey, iXPos, iYPos);
            }

            return 0;
        }
        case WM_LBUTTONDOWN:
        {
            int iXPos = int(LOWORD(lParam));
            int iYPos = int(HIWORD(lParam));
            pTheApp->OnMouseClick(WindowApplication::MOUSE_LEFT_BUTTON,
                WindowApplication::MOUSE_DOWN, iXPos, iYPos, PtrToUint(wParam));

            return 0;
        }
        case WM_LBUTTONUP:
        {
            int iXPos = int(LOWORD(lParam));
            int iYPos = int(HIWORD(lParam));
            pTheApp->OnMouseClick(WindowApplication::MOUSE_LEFT_BUTTON,
                WindowApplication::MOUSE_UP, iXPos, iYPos, PtrToUint(wParam));

            return 0;
        }
        case WM_MBUTTONDOWN:
        {
            int iXPos = int(LOWORD(lParam));
            int iYPos = int(HIWORD(lParam));
            pTheApp->OnMouseClick(WindowApplication::MOUSE_MIDDLE_BUTTON,
                WindowApplication::MOUSE_DOWN, iXPos, iYPos, PtrToUint(wParam));

            return 0;
        }
        case WM_MBUTTONUP:
        {
            int iXPos = int(LOWORD(lParam));
            int iYPos = int(HIWORD(lParam));
            pTheApp->OnMouseClick(WindowApplication::MOUSE_MIDDLE_BUTTON,
                WindowApplication::MOUSE_UP, iXPos, iYPos, PtrToUint(wParam));

            return 0;
        }
        case WM_RBUTTONDOWN:
        {
            int iXPos = int(LOWORD(lParam));
            int iYPos = int(HIWORD(lParam));
            pTheApp->OnMouseClick(WindowApplication::MOUSE_RIGHT_BUTTON,
                WindowApplication::MOUSE_DOWN, iXPos, iYPos, PtrToUint(wParam));

            return 0;
        }
        case WM_RBUTTONUP:
        {
            int iXPos = int(LOWORD(lParam));
            int iYPos = int(HIWORD(lParam));
            pTheApp->OnMouseClick(WindowApplication::MOUSE_RIGHT_BUTTON,
                WindowApplication::MOUSE_UP, iXPos, iYPos, PtrToUint(wParam));

            return 0;
        }
        case WM_MOUSEMOVE:
        {
            int iXPos = int(LOWORD(lParam));
            int iYPos = int(HIWORD(lParam));

            int iButton = -1;
            if( wParam & MK_LBUTTON )
            {
                iButton = WindowApplication::MOUSE_LEFT_BUTTON;
            }
            else if( wParam & MK_MBUTTON )
            {
                iButton = WindowApplication::MOUSE_MIDDLE_BUTTON;
            }
            else if( wParam & MK_RBUTTON )
            {
                iButton = WindowApplication::MOUSE_RIGHT_BUTTON;
            }

            if( iButton >= 0 )
            {
                pTheApp->OnMotion(iButton, iXPos, iYPos, PtrToUint(wParam));
            }
            else
            {
                pTheApp->OnPassiveMotion(iXPos, iYPos);
            }

            return 0;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);

            return 0;
        }
    }

    return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}
//----------------------------------------------------------------------------
int WindowApplication::Main(int, char**)
{
    WindowApplication* pTheApp = (WindowApplication*)TheApplication;
    pTheApp->KEY_TERMINATE = WindowApplication::KEY_ESCAPE;

    // �����ڴ��崴��֮ǰ��һЩ����.
    if( !pTheApp->OnPrecreate() )
    {
        return -1;
    }

    // ע�ᴰ����.
    static char s_acWindowClass[] = "Swing Application";
    WNDCLASS wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc   = MsWindowEventHandler;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = 0;
    wc.hIcon         = LoadIcon(0,IDI_APPLICATION);
    wc.hCursor       = LoadCursor(0,IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszClassName = s_acWindowClass;
    wc.lpszMenuName  = 0;
    RegisterClass(&wc);

    // ���������С�Ĵ���.
    RECT tempRect = { 0, 0, pTheApp->GetWidth()-1, pTheApp->GetHeight()-1 };
    AdjustWindowRect(&tempRect, WS_OVERLAPPEDWINDOW, false);

    // ��������.
    HWND hWnd = CreateWindow(s_acWindowClass, pTheApp->GetWindowTitle(),
        WS_OVERLAPPEDWINDOW, pTheApp->GetXPosition(), pTheApp->GetYPosition(),
        tempRect.right-tempRect.left+1, tempRect.bottom-tempRect.top+1, 0, 0, 0, 0);

    pTheApp->SetWindowID(PtrToInt(hWnd));

    // ������Ⱦ��.
    pTheApp->SetRenderer(SE_NEW DX9Renderer(hWnd, m_eFormat, m_eDepth,
        m_eStencil, m_eBuffering, m_eMultisampling, m_iWidth, m_iHeight));

    // ����������Ⱦ��.
    pTheApp->SetAudioRenderer(SE_NEW WalRenderer);

    if( pTheApp->OnInitialize() )
    {
        // ��ʾ����.
        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);

        // ��ʼ��Ϣѭ��.
        MSG tempMsg;
        while( true )
        {
            if( PeekMessage(&tempMsg, 0, 0, 0, PM_REMOVE) )
            {
                if( tempMsg.message == WM_QUIT )
                {
                    break;
                }

                HACCEL hAccel = 0;
                if( !TranslateAccelerator(hWnd, hAccel, &tempMsg) )
                {
                    TranslateMessage(&tempMsg);
                    DispatchMessage(&tempMsg);
                }
            }
            else
            {
                pTheApp->OnIdle();
            }
        }
    }

    pTheApp->OnTerminate();

    return 0;
}
//----------------------------------------------------------------------------
void Application::LaunchTreeControl(Spatial* pScene, int iXPos, int iYPos,
    int iXSize, int iYSize)
{
    TreeControl* pControl;
    GetExtraData(0, sizeof(TreeControl*), &pControl);
    if( !pControl )
    {
        pControl = SE_NEW TreeControl(this, 0, GetModuleHandle(0), 0, pScene,
            iXPos, iYPos, iXSize, iYSize);
        SetExtraData(0, sizeof(TreeControl*), &pControl);
    }
}
//----------------------------------------------------------------------------
void Application::ShutdownTreeControl()
{
    TreeControl* pControl;
    GetExtraData(0, sizeof(TreeControl*), &pControl);
    if( pControl )
    {
        SE_DELETE pControl;
        pControl = 0;
        SetExtraData(0, sizeof(TreeControl*), &pControl);
    }
}
//----------------------------------------------------------------------------