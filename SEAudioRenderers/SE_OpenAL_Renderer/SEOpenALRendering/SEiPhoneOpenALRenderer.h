#ifndef Swing_iPhoneOpenAL_Renderer_H
#define Swing_iPhoneOpenAL_Renderer_H

#include "SEOpenALRendererLIB.h"
#include "SEOpenALRenderer.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:iPhone OpenAL1.0 Renderer��
// ˵��:
// ����:Sun Che
// ʱ��:20091125
//----------------------------------------------------------------------------
class SE_AUDIO_RENDERER_API iPhoneOpenALRenderer : public OpenALRenderer
{
public:
    iPhoneOpenALRenderer(void);
    virtual ~iPhoneOpenALRenderer(void);

    // ��������.
    ALCdevice* GetALDevice(void){ return m_pALDevice; }
    ALCcontext* GetALContext(void){ return m_pALContext; }

protected:
    ALCdevice* m_pALDevice;
    ALCcontext* m_pALContext;
};

}

#endif
