#include "SEOpenALRendererPCH.h"
#include "SEAalRenderer.h"

using namespace Swing;

//----------------------------------------------------------------------------
AalRenderer::AalRenderer()
{
    m_pALDevice = 0;
    m_pALContext = 0;

    // ����device��context.
    m_pALDevice = alcOpenDevice(NULL);
    SE_ASSERT( m_pALDevice );

    if( m_pALDevice )
    {
        m_pALContext = alcCreateContext(m_pALDevice, NULL);
        SE_ASSERT( m_pALContext );
        alcMakeContextCurrent(m_pALContext);
    }

    InitializeState();
}
//----------------------------------------------------------------------------
AalRenderer::~AalRenderer()
{
    // �ͷ�context��device.
    alcMakeContextCurrent(NULL);
    alcDestroyContext(m_pALContext);
    alcCloseDevice(m_pALDevice);
}
//----------------------------------------------------------------------------