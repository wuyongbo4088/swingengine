#ifndef Swing_Aal_Renderer_H
#define Swing_Aal_Renderer_H

#include "SEOpenALRendererLIB.h"
#include "SEOpenALRenderer.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:Mac OS X OpenAL1.0 Renderer��
// ˵��:
// ����:Sun Che
// ʱ��:20090629
//----------------------------------------------------------------------------
class SE_AUDIO_RENDERER_API AalRenderer : public OpenALRenderer
{
public:
    AalRenderer(void);
    virtual ~AalRenderer(void);

    // ��������.
    ALCdevice* GetALDevice(void){ return m_pALDevice; }
    ALCcontext* GetALContext(void){ return m_pALContext; }

protected:
    ALCdevice* m_pALDevice;
    ALCcontext* m_pALContext;
};

}

#endif
