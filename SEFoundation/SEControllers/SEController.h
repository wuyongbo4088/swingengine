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

#ifndef Swing_Controller_H
#define Swing_Controller_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEObject.h"

namespace Swing
{

//----------------------------------------------------------------------------
// ����:��������
// ˵��:
// ����:Sun Che
// ʱ��:20080327
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Controller : public Object
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    virtual ~Controller(void);

    // ��ȡ�����ƵĶ���
    inline Object* GetObject(void) const;

    // ��������ʵ�ִ˺���
    virtual bool Update(double dAppTime);

    enum RepeatType
    {
        RT_CLAMP,
        RT_WRAP,
        RT_CYCLE,
        RT_COUNT
    };

    RepeatType Repeat;  // default = RT_CLAMP
    double MinTime;     // default = 0.0
    double MaxTime;     // default = 0.0
    double Phase;       // default = 0.0
    double Frequency;   // default = 1.0
    bool Active;        // default = true

protected:
    // �����
    Controller(void);

    friend class Object;
    // ���غ�,������ɶ����������ı����ƶ����������ʶ��.
    virtual void SetObject(Object* pObject);

    // ��applicationʱ�䵥λ��controllerʱ�䵥λ��ת��.
    // ����������update�����п���ʹ���������.
    double GetControlTime(double dAppTime);

    // ����ָ��ָ�򱻿��Ƶ�object,����object��controller��smart pointer��ָ����
    Object* m_pObject;

    double m_dLastAppTime;

private:
    static const char* ms_pRepeatType[RT_COUNT];
};

typedef SmartPointer<Controller> ControllerPtr;

#include "SEController.inl"

}

#endif
