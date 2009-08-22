#ifndef Swing_iPhoneOES1_Renderer_H
#define Swing_iPhoneOES1_Renderer_H

#include "SEOGLES1RendererLIB.h"
#include "SEOGLES1Renderer.h"
#include "SEEAGLView.h"

// ע��:
// ����ļ������˺���cocoa�����ͷ�ļ�,���ֻ�ܱ�.mm�ļ�������.

namespace Swing
{

//----------------------------------------------------------------------------
// ����:iPhone OpenGL ES1 Renderer��
// ˵��:
// ����:Sun Che
// ʱ��:20090607
//----------------------------------------------------------------------------
class SE_RENDERER_API iPhoneOES1Renderer : public OGLES1Renderer
{
public:
    iPhoneOES1Renderer(UIWindow* pWindow, FrameBuffer::FormatType eFormat,
        FrameBuffer::DepthType eDepth, FrameBuffer::StencilType eStencil,
        FrameBuffer::BufferingType eBuffering,
        FrameBuffer::MultisamplingType eMultisampling, int iX, int iY,
        int iWidth, int iHeight);

    virtual ~iPhoneOES1Renderer(void);

    virtual void Resize(int iWidth, int iHeight);
    virtual void ToggleFullscreen(void);

    virtual void DisplayBackBuffer(void);

    virtual int LoadFont(const char* acFace, int iSize, bool bBold,
        bool bItalic);

    // �����view����.
    EAGLView* GetView(void);
    UIWindow* GetWindow(void);

private:
    // �����view����.
    EAGLView* m_pEAGLView;
    UIWindow* m_pWindow;
    Boolean m_bFullScreen;

    // ��ʵ��.
    // �������.

    // ����֧��fullscreen/window.
    int m_iX, m_iY;
    int m_iSaveWidth, m_iSaveHeight;
};

}

#endif

