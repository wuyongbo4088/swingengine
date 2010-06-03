#ifndef Swing_Agl_Renderer_H
#define Swing_Agl_Renderer_H

#include "SEOpenGLRendererLIB.h"
#include "SEOpenGLRenderer.h"
#include <AGL/agl.h>
#include <Carbon/Carbon.h>
#include <DrawSprocket/DrawSprocket.h>

namespace Swing
{

//----------------------------------------------------------------------------
// ˵��:
// ����:Sun Che
// ʱ��:20081009
//----------------------------------------------------------------------------
class SE_RENDERER_API SEAglRenderer : public SEOpenGLRenderer
{
public:
    SEAglRenderer(AGLDevice hDevice, WindowRef pWindow,
        SEFrameBuffer::FormatType eFormat, SEFrameBuffer::DepthType eDepth,
        SEFrameBuffer::StencilType eStencil,
        SEFrameBuffer::BufferingType eBuffering,
        SEFrameBuffer::MultisamplingType eMultisampling, int iX, int iY,
        int iWidth, int iHeight);

    virtual ~SEAglRenderer(void);

    virtual void Resize(int iWidth, int iHeight);
    virtual void ToggleFullscreen(void);

    virtual bool BeginScene(void);
    virtual void DisplayBackBuffer(void);

    virtual int LoadFont(const char* acFace, int iSize, bool bBold,
        bool bItalic);

    // �������.
    AGLDevice GetDevice(void);
    WindowRef GetWindow(void);
    AGLDrawable GetDrawable(void);
    AGLContext GetContext(void);

private:
    // ������ͼ����.
    void SetUpBufferRect(void);

    // ��ʾ������Ϣ����ֹ.
    void FatalErrorMessage(const char* acMessage);

    // �������.
    AGLDevice m_hAglDevice;
    WindowRef m_pAglWindow;
    AGLDrawable m_pAglDrawable;
    AGLContext m_pAglContext;
    AGLDrawable m_pFullScreenDrawable;
    DSpContextReference m_pDSpContext;
    Boolean m_bFullScreen;

    // �������.
    GWorldPtr m_pBack;

    // ����֧��fullscreen/window.
    int m_iX, m_iY;
    int m_iSaveWidth, m_iSaveHeight;

    static Boolean ms_bDSpStarted;
};

}

#endif
