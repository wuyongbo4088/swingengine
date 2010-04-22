#ifndef Swing_iPhoneOpenAL_Renderer_H
#define Swing_iPhoneOpenAL_Renderer_H

#include "SEOpenALRendererLIB.h"
#include "SEOpenALRenderer.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ˵��:
// ����:Sun Che
// ʱ��:20091125
//----------------------------------------------------------------------------
class SE_AUDIO_RENDERER_API SEiPhoneOpenALRenderer : public SEOpenALRenderer
{
public:
    SEiPhoneOpenALRenderer(void);
    virtual ~SEiPhoneOpenALRenderer(void);

    // ��������.
    ALCdevice* GetALDevice(void){ return m_pALDevice; }
    ALCcontext* GetALContext(void){ return m_pALContext; }

protected:
    ALCdevice* m_pALDevice;
    ALCcontext* m_pALContext;
};

}

#endif
