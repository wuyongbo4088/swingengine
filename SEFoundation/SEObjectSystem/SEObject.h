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

#ifndef Swing_Object_H
#define Swing_Object_H

#include "SEFoundationLIB.h"
#include "SEPlatforms.h"
#include "SEMain.h"
#include "SERTTI.h"
#include "SENameIdMCR.h"
#include "SESmartPointer.h"
#include "SEStream.h"
#include "SEStringTree.h"
#include "SEStringHashTable.h"

namespace Swing
{

class Controller;

//----------------------------------------------------------------------------
// ����:�������
// ˵��:
// ����:Sun Che
// ʱ��:20080329
//----------------------------------------------------------------------------
class SE_FOUNDATION_API Object
{
public:
    virtual ~Object(void);
protected:
    // ��ֹ��ʵ����,�����
    Object(void);

// RTTIϵͳ
public:
    static const RTTI TYPE;
    virtual const RTTI& GetType(void) const;
    inline bool IsExactly(const RTTI& rType) const;
    inline bool IsDerived(const RTTI& rType) const;
    inline bool IsExactlyTypeOf(const Object* pObject) const;
    inline bool IsDerivedTypeOf(const Object* pObject) const;

// Controllerϵͳ
public:
    int GetControllerCount(void) const;
    Controller* GetController(int i) const;
    void AttachController(Controller* pController);
    void DetachController(Controller* pController);
    void DetachAllControllers(void);
    bool UpdateControllers(double dAppTime);
private:
    std::vector< SmartPointer<Object> > m_Controllers;

// Copyingϵͳ
public:
    SmartPointer<Object> Copy(bool bUniqueNames = true) const;
    static char NameAppend;

// Name-IDϵͳ
public:
    inline void SetName(const String& rName);
    inline const String& GetName(void) const;
    inline unsigned int GetID(void) const;
    inline static unsigned int GetNextID(void);
    virtual Object* GetObjectByName(const String& rName);
    virtual void GetAllObjectsByName(const String& rName, std::vector<Object*>& rObjects);
    virtual Object* GetObjectByID(unsigned int uiID);
private:
    String m_Name;
    unsigned int m_uiID;
    static unsigned int ms_uiNextID;

// Smart pointerϵͳ
public:
    inline void IncrementReferences(void);
    void DecrementReferences(void);
    inline int GetReferences(void) const;
    static HashTable<unsigned int, Object*>* InUse;   // Object�����ڴ�й©����ʹ�ô�hash��
    static void PrintInUse(const char* pFileName, const char* pMessage);
private:
    int m_iReferences;

// Streamingϵͳ
public:
    enum { FACTORY_MAP_SIZE = 256 };
    static StringHashTable<FactoryFunction>* ms_pFactory; // ��Ȼ�ǹ��г�Ա,������ϵͳ�ڲ�ʹ�õ�
    static bool RegisterFactory(void);
    static void InitializeFactory(void);
    static void TerminateFactory(void);
    static Object* Factory(Stream& rStream);
    virtual void Load(Stream& rStream, Stream::Link* pLink);
    virtual void Link(Stream& rStream, Stream::Link* pLink);
    virtual bool Register(Stream& rStream) const;
    virtual void Save(Stream& rStream) const;
    virtual int GetDiskUsed(const StreamVersion& rVersion) const;
    virtual StringTree* SaveStrings(const char* pTitle = 0);
private:
    static bool ms_bStreamRegistered;
};

// ��̬,��̬����ת��
template <class T> T* StaticCast(Object* pObject);
template <class T> const T* StaticCast(const Object* pObject);
template <class T> T* DynamicCast(Object* pObject);
template <class T> const T* DynamicCast(const Object* pObject);

typedef SmartPointer<Object> ObjectPtr;

#include "SEObject.inl"

}

#endif
