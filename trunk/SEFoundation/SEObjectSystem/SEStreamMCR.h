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

#ifndef Swing_StreamMCR_H
#define Swing_StreamMCR_H

//----------------------------------------------------------------------------
#define SE_DECLARE_STREAM \
public: \
    static bool RegisterFactory(void); \
    static void InitializeFactory(void); \
    static Object* Factory(Stream& rStream); \
    virtual void Load(Stream& rStream, Stream::Link* pLink); \
    virtual void Link(Stream& rStream, Stream::Link* pLink); \
    virtual bool Register(Stream& rStream) const; \
    virtual void Save(Stream& rStream) const; \
    virtual int GetDiskUsed(const StreamVersion& rVersion) const; \
    virtual StringTree* SaveStrings(const char* pTitle = 0); \
private: \
    static bool ms_bStreamRegistered
//----------------------------------------------------------------------------
#define SE_DECLARE_TEMPLATE_STREAM \
public: \
    static bool RegisterFactory(void); \
    static void InitializeFactory(void); \
    static Object* Factory(Stream& rStream); \
    virtual void Load(Stream& rStream, Stream::Link* pLink); \
    virtual void Link(Stream& rStream, Stream::Link* pLink); \
    virtual bool Register(Stream& rStream) const; \
    virtual void Save(Stream& rStream) const; \
    virtual int GetDiskUsed(const StreamVersion& rVersion) const; \
    virtual StringTree* SaveStrings(const char* pTitle = 0); \
private: \
    SE_FOUNDATION_API static bool ms_bStreamRegistered
//----------------------------------------------------------------------------
#define SE_IMPLEMENT_STREAM(classname) \
bool classname::ms_bStreamRegistered = false; \
\
bool classname::RegisterFactory() \
{ \
    if( !ms_bStreamRegistered ) \
    { \
        Main::AddInitializer(classname::InitializeFactory); \
        ms_bStreamRegistered = true; \
    } \
    return ms_bStreamRegistered; \
} \
\
void classname::InitializeFactory() \
{ \
    if( !ms_pFactory ) \
    { \
        ms_pFactory = SE_NEW StringHashTable<FactoryFunction>( \
            FACTORY_MAP_SIZE); \
    } \
    ms_pFactory->Insert(TYPE.GetName(), (FactoryFunction)Factory); \
} \
\
Object* classname::Factory(Stream& rStream) \
{ \
    classname* pObject = SE_NEW classname; \
    Stream::Link* pLink = SE_NEW Stream::Link(pObject); \
    pObject->Load(rStream, pLink); \
    return pObject; \
}
//----------------------------------------------------------------------------
#define SE_IMPLEMENT_ABSTRACT_STREAM(classname) \
bool classname::ms_bStreamRegistered = false; \
\
bool classname::RegisterFactory() \
{ \
    if( !ms_bStreamRegistered ) \
    { \
        Main::AddInitializer(classname::InitializeFactory); \
        ms_bStreamRegistered = true; \
    } \
    return ms_bStreamRegistered; \
} \
\
void classname::InitializeFactory() \
{ \
    if( !ms_pFactory ) \
    { \
        ms_pFactory = SE_NEW StringHashTable<FactoryFunction>( \
            FACTORY_MAP_SIZE); \
    } \
    ms_pFactory->Insert(TYPE.GetName(), (FactoryFunction)Factory); \
} \
\
Object* classname::Factory(Stream&) \
{ \
    SE_ASSERT(false); \
    return 0; \
}
//----------------------------------------------------------------------------
#define SE_IMPLEMENT_TEMPLATE_STREAM(classname) \
template <> \
bool classname::ms_bStreamRegistered = false;
//----------------------------------------------------------------------------
#define SE_IMPLEMENT_DEFAULT_STREAM(classname, baseclassname) \
void classname::Load(Stream& rStream, Stream::Link* pLink) \
{ \
    SE_BEGIN_DEBUG_STREAM_LOAD; \
    baseclassname::Load(rStream, pLink); \
    SE_END_DEBUG_STREAM_LOAD(classname); \
} \
\
void classname::Link(Stream& rStream, Stream::Link* pLink) \
{ \
    baseclassname::Link(rStream, pLink); \
} \
\
bool classname::Register(Stream& rStream) const \
{ \
    return baseclassname::Register(rStream); \
} \
\
void classname::Save(Stream& rStream) const \
{ \
    SE_BEGIN_DEBUG_STREAM_SAVE; \
    baseclassname::Save(rStream); \
    SE_END_DEBUG_STREAM_SAVE(classname); \
} \
\
int classname::GetDiskUsed(const StreamVersion& rVersion) const \
{ \
    return baseclassname::GetDiskUsed(rVersion); \
} \
\
StringTree* classname::SaveStrings(const char*) \
{ \
    StringTree* pTree = SE_NEW StringTree; \
    pTree->Append(Format(&TYPE, (const char*)GetName())); \
    pTree->Append(baseclassname::SaveStrings()); \
    return pTree; \
} \
//----------------------------------------------------------------------------
#define SE_REGISTER_STREAM(classname) \
static bool gs_bStreamRegistered_##classname = \
    classname::RegisterFactory()
//----------------------------------------------------------------------------
#define SE_INVOKE_STREAM(classname) \
(void)gs_bStreamRegistered_##classname
//----------------------------------------------------------------------------
#ifdef _DEBUG
#define SE_BEGIN_DEBUG_STREAM_LOAD \
    int iBeginLoad_ = rStream.GetBufferNext()
#else
#define SE_BEGIN_DEBUG_STREAM_LOAD
#endif
//----------------------------------------------------------------------------
#ifdef _DEBUG
#define SE_END_DEBUG_STREAM_LOAD(classname) \
    int iEndLoad_ = rStream.GetBufferNext(); \
    int iUsedLoad_ = iEndLoad_ - iBeginLoad_ + GetType().GetDiskUsed(); \
    int iUsedReported_ = classname::GetDiskUsed(rStream.GetVersion()); \
    SE_ASSERT( iUsedLoad_ == iUsedReported_ )
#else
#define SE_END_DEBUG_STREAM_LOAD(classname)
#endif
//----------------------------------------------------------------------------
#ifdef _DEBUG
#define SE_BEGIN_DEBUG_STREAM_SAVE \
    int iBeginSave_ = rStream.GetBufferNext()
#else
#define SE_BEGIN_DEBUG_STREAM_SAVE
#endif
//----------------------------------------------------------------------------
#ifdef _DEBUG
#define SE_END_DEBUG_STREAM_SAVE(classname) \
    int iEndSave_ = rStream.GetBufferNext(); \
    int iUsedSave_ = iEndSave_ - iBeginSave_; \
    int iUsedReported_ = classname::GetDiskUsed( \
        StreamVersion(StreamVersion::MAJOR, StreamVersion::MINOR)); \
    SE_ASSERT( iUsedSave_ == iUsedReported_ )
#else
#define SE_END_DEBUG_STREAM_SAVE(classname)
#endif
//----------------------------------------------------------------------------

#endif