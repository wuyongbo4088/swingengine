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

#ifndef Swing_SoundBuffer_H
#define Swing_SoundBuffer_H

#include "SEAudioLIB.h"
#include "SEObject.h"
#include "SEAudioBindable.h"
#include "SEWave.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:wave���ݿ�����Ϊ��Ƶ��Դ�󶨸���Ƶ�豸.
// Author:Sun Che
// Date:20090618
//----------------------------------------------------------------------------
class SE_AUDIO_API SESoundBuffer : public SEObject, public SEAudioBindable
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    SESoundBuffer(int iWCount, SEWave** apWaves);
    SESoundBuffer(const SESoundBuffer* pSBuffer);
    virtual ~SESoundBuffer(void);

    // ��Ա����.
    inline int GetWaveCount(void) const;
    inline SEWave* GetWave(int i);
    inline const SEWave* GetWave(int i) const;

protected:
    // streaming support
    SESoundBuffer(void);

    void* m_pID;

    // wave����.
    std::vector<SEWavePtr> m_Waves;
};

typedef SESmartPointer<SESoundBuffer> SESoundBufferPtr;

#include "SESoundBuffer.inl"

}

#endif
