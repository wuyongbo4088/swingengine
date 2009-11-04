#ifndef Swing_iPhoneOES2_Renderer_H
#define Swing_iPhoneOES2_Renderer_H

#include "SEOGLES2RendererLIB.h"
#include "SEOGLES2Renderer.h"
#include "SEEAGLView.h"

// ע��:
// ����ļ������˺���cocoa�����ͷ�ļ�,���ֻ�ܱ�.mm�ļ�������.

namespace Swing
{

//----------------------------------------------------------------------------
// ����:iPhone OpenGL ES2 Renderer��
// ˵��:
// ����:Sun Che
// ʱ��:20091104
//----------------------------------------------------------------------------
class SE_RENDERER_API iPhoneOES2Renderer : public OGLES2Renderer
{
public:
    iPhoneOES2Renderer(UIWindow* pWindow, FrameBuffer::FormatType eFormat,
        FrameBuffer::DepthType eDepth, FrameBuffer::StencilType eStencil,
        FrameBuffer::BufferingType eBuffering,
        FrameBuffer::MultisamplingType eMultisampling, int iX, int iY,
        int iWidth, int iHeight);

    virtual ~iPhoneOES2Renderer(void);

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

