#include "SEiPhoneOES2Renderer.h"

using namespace Swing;

//----------------------------------------------------------------------------
iPhoneOES2Renderer::iPhoneOES2Renderer(UIWindow* pWindow,
    FrameBuffer::FormatType eFormat, FrameBuffer::DepthType eDepth,
    FrameBuffer::StencilType eStencil, FrameBuffer::BufferingType eBuffering,
    FrameBuffer::MultisamplingType eMultisampling, int iX, int iY, int iWidth,
    int iHeight)
    :
    OGLES2Renderer(eFormat, eDepth, eStencil, eBuffering, eMultisampling, 
        iWidth, iHeight)
{
    SE_ASSERT( m_iWidth > 0 && m_iHeight > 0 );
    SE_ASSERT( pWindow );
    
    m_iSaveWidth = m_iWidth;
    m_iSaveHeight = m_iHeight;

    m_iX = iX;
    m_iY = iY;
    // �Ժ󴴽�EAGLView.
    m_pEAGLView = 0;
    m_pWindow = pWindow;
    m_bFullScreen = false;

    // Request a depth buffer.
    if( m_eDepth != FrameBuffer::DT_DEPTH_NONE )
    {
        // ��ʵ��.
        // Ŀǰֻ֧��GL_DEPTH_COMPONENT16_OES.
    }

    // Request a stencil buffer.
    if( m_eStencil == FrameBuffer::ST_STENCIL_8 )
    {
        // ��ʵ��.
    }

    // Request buffering.
    if( m_eBuffering == FrameBuffer::BT_BUFFERED_DOUBLE )
    {
        // ��ʵ��.
    }

    // Request sampling.
    if( m_eMultisampling != FrameBuffer::MT_SAMPLING_NONE )
    {
        // ��ʵ��.
    }

    // ����EAGLView����EAGLContext.
    CGRect tempRect = [[UIScreen mainScreen] bounds];
    m_pEAGLView = [[EAGLView alloc] initWithFrame:tempRect 
        pixelFormat:GL_RGB565 depthFormat:GL_DEPTH_COMPONENT16 
        preserveBackbuffer:NO];

    // ��EAGLView�󶨸�UIWindow.
    [m_pWindow addSubview:m_pEAGLView];

    // ��ʼ��OpenGL����,��ȡ�豸����.
    InitializeState();
}
//----------------------------------------------------------------------------
iPhoneOES2Renderer::~iPhoneOES2Renderer()
{
    [m_pEAGLView release];
}
//----------------------------------------------------------------------------
void iPhoneOES2Renderer::Resize(int iWidth, int iHeight)
{
    // ��ʵ��.
}
//----------------------------------------------------------------------------
void iPhoneOES2Renderer::ToggleFullscreen()
{
    // ��ʵ��.
}
//----------------------------------------------------------------------------
void iPhoneOES2Renderer::DisplayBackBuffer()
{
    [m_pEAGLView swapBuffers];
}
//----------------------------------------------------------------------------
int iPhoneOES2Renderer::LoadFont(const char*, int, bool, bool)
{
    // ��ʵ��.
    return 0;
}
//----------------------------------------------------------------------------
EAGLView* iPhoneOES2Renderer::GetView()
{
    return m_pEAGLView;
}
//----------------------------------------------------------------------------
UIWindow* iPhoneOES2Renderer::GetWindow()
{
    return m_pWindow;
}
//----------------------------------------------------------------------------