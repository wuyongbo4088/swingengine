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

#include "SEFoundationPCH.h"
#include "SEObject.h"
#include "SEController.h"
#include "SEStringTree.h"

using namespace Swing;

const RTTI Object::TYPE("Swing.Object", 0);
unsigned int Object::ms_uiNextID = 0;
HashTable<unsigned int, Object*>* Object::InUse = 0;
char Object::NameAppend = '#';
StringHashTable<FactoryFunction>* Object::ms_pFactory = 0;
bool Object::ms_bStreamRegistered = false;

//SE_REGISTER_STREAM(Object);

//----------------------------------------------------------------------------
Object::Object()
{
    m_iReferences = 0;
    m_uiID = ms_uiNextID++;

    if( !InUse )
    {
        InUse = SE_NEW HashTable<unsigned int, Object*>(FACTORY_MAP_SIZE);
    }

    InUse->Insert(m_uiID, this);
}
//----------------------------------------------------------------------------
Object::~Object()
{
    DetachAllControllers();
    SE_ASSERT( InUse );
    bool bFound = InUse->Remove(m_uiID);
    SE_ASSERT( bFound );
    (void)bFound;  // �������������
}
//----------------------------------------------------------------------------
void Object::DecrementReferences()
{
    if( --m_iReferences == 0 )
    {
        SE_DELETE this;
    }
}
//----------------------------------------------------------------------------
void Object::PrintInUse(const char* pFileName, const char* pMessage)
{
    FILE* pFile = System::SE_Fopen(pFileName, "at");
    SE_ASSERT( pFile );

    System::SE_Fprintf(pFile, "%s\n", pMessage);

    SE_ASSERT( InUse );
    unsigned int uiID = 0;
    // ����������е�ǰobject
    Object** ppObject = InUse->GetFirst(&uiID);
    while( ppObject )
    {
        System::SE_Fprintf(pFile, "id = %6d , type = %s\n", uiID,
            (const char*)(*ppObject)->GetType().GetName());
        ppObject = InUse->GetNext(&uiID);
    }

    System::SE_Fprintf(pFile, "\n\n");
    System::SE_Fclose(pFile);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// controllers
//----------------------------------------------------------------------------
int Object::GetControllerCount() const
{
    return (int)m_Controllers.size();
}
//----------------------------------------------------------------------------
Controller* Object::GetController(int i) const
{
    SE_ASSERT( 0 <= i && i < (int)m_Controllers.size() );

    return StaticCast<Controller>(m_Controllers[i]);
}
//----------------------------------------------------------------------------
void Object::AttachController(Controller* pController)
{
    // controller������������ʩ��controller,�Ӷ����⸴�ӵĶ���ͼ��ϵ,
    // һ������controller����һ��controller����,����ܴ���Ǳ�ڷ���...
    if( IsDerived(Controller::TYPE) )
    {
        SE_ASSERT( false );

        return;
    }

    if( !pController )
    {
        SE_ASSERT( pController );

        return;
    }

    // �Ƿ��Ѿ�����
    for( int i = 0; i < (int)m_Controllers.size(); i++ )
    {
        if( pController == m_Controllers[i] )
        {
            return;
        }
    }

    // ˫���,ע�ⲻҪ����ָ�뻥ָ
    pController->SetObject(this);     // ������ָ��
    m_Controllers.push_back(pController);  // ����ָ��
}
//----------------------------------------------------------------------------
void Object::DetachController(Controller* pController)
{
    std::vector<ObjectPtr>::iterator pIter = m_Controllers.begin();
    for( /**/; pIter != m_Controllers.end(); pIter++ )
    {
        if( pController == *pIter )
        {
            pController->SetObject(0);
            m_Controllers.erase(pIter);

            return;
        }
    }
}
//----------------------------------------------------------------------------
void Object::DetachAllControllers()
{
    for( int i = 0; i < (int)m_Controllers.size(); i++ )
    {
        Controller* pController = StaticCast<Controller>(m_Controllers[i]);
        pController->SetObject(0);
    }
    m_Controllers.clear();
}
//----------------------------------------------------------------------------
bool Object::UpdateControllers(double dAppTime)
{
    bool bSomeoneUpdated = false;
    for( int i = 0; i < (int)m_Controllers.size(); i++ )
    {
        Controller* pController = StaticCast<Controller>(m_Controllers[i]);

        if( pController->Update(dAppTime) )
        {
            bSomeoneUpdated = true;
        }
    }

    return bSomeoneUpdated;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// copying
//----------------------------------------------------------------------------
ObjectPtr Object::Copy(bool bUniqueNames) const
{
    // save the object to a memory buffer
    Stream kSaveStream;
    kSaveStream.Insert((Object*)this);
    char* pBuffer = 0;
    int iBufferSize = 0;
    bool bSuccessful = kSaveStream.Save(pBuffer,iBufferSize);
    SE_ASSERT( bSuccessful );
    if( !bSuccessful )
    {
        return 0;
    }

    // load the object from the memory buffer
    Stream kLoadStream;
    bSuccessful = kLoadStream.Load(pBuffer,iBufferSize);
    SE_ASSERT( bSuccessful );

    if( !bSuccessful )
    {
        return 0;
    }
    SE_DELETE[] pBuffer;

    if( bUniqueNames )
    {
        // generate unique names
        for( int i = 0; i < kLoadStream.GetOrderedCount(); i++ )
        {
            Object* pObject = kLoadStream.GetOrderedObject(i);
            SE_ASSERT( pObject );

            const String& rName = pObject->GetName();
            int iLength = (int)rName.GetLength();
            if( iLength > 0 )
            {
                // object has a name, append a character to make it unique
                const char* pName = (const char*)rName;
                char* acNewName = SE_NEW char[iLength + 2];
                const size_t uiSize = (size_t)(iLength + 2);
                System::SE_Strcpy(acNewName,uiSize,pName);
                acNewName[iLength] = NameAppend;
                acNewName[iLength+1] = 0;
                pObject->SetName(String(acNewName));
                SE_DELETE[] acNewName;
            }
        }
    }

    return kLoadStream.GetObjectAt(0);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// name and unique id
//----------------------------------------------------------------------------
Object* Object::GetObjectByName(const String& rName)
{
    if( rName == m_Name )
    {
        return this;
    }

    for( int i = 0; i < (int)m_Controllers.size(); i++ )
    {
        if( m_Controllers[i] )
        {
            Object* pFound = m_Controllers[i]->GetObjectByName(rName);
            if( pFound )
            {
                return pFound;
            }
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
void Object::GetAllObjectsByName(const String& rName, std::vector<Object*>& rObjects)
{
    if( rName == m_Name )
    {
        rObjects.push_back(this);
    }

    for( int i = 0; i < (int)m_Controllers.size(); i++ )
    {
        if( m_Controllers[i] )
        {
            m_Controllers[i]->GetAllObjectsByName(rName, rObjects);
        }
    }
}
//----------------------------------------------------------------------------
Object* Object::GetObjectByID(unsigned int uiID)
{
    if( uiID == m_uiID )
    {
        return this;
    }

    for( int i = 0; i < (int)m_Controllers.size(); i++ )
    {
        if( m_Controllers[i] )
        {
            Object* pFound = m_Controllers[i]->GetObjectByID(uiID);
            if( pFound )
            {
                return pFound;
            }
        }
    }

    return 0;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// streaming
//----------------------------------------------------------------------------
bool Object::RegisterFactory()
{
    if( !ms_bStreamRegistered )
    {
        Main::AddInitializer(Object::InitializeFactory);
        Main::AddTerminator(Object::TerminateFactory);
        ms_bStreamRegistered = true;
    }
    return ms_bStreamRegistered;
}
//----------------------------------------------------------------------------
void Object::InitializeFactory()
{
    if( !ms_pFactory )
    {
        ms_pFactory = SE_NEW StringHashTable<FactoryFunction>(FACTORY_MAP_SIZE);
    }
    ms_pFactory->Insert(TYPE.GetName(), (FactoryFunction)Factory);
}
//----------------------------------------------------------------------------
void Object::TerminateFactory()
{
    SE_DELETE ms_pFactory;
    ms_pFactory = 0;
}
//----------------------------------------------------------------------------
Object* Object::Factory(Stream&)
{
    SE_ASSERT( false );

    return 0;
}
//----------------------------------------------------------------------------
void Object::Load(Stream& rStream, Stream::Link* pLink)
{
    SE_BEGIN_DEBUG_STREAM_LOAD;

    // ��ȡsaveʱ�����object�ڴ�ָ��,�Ժ�����link�׶�
    Object* pLinkID;
    rStream.Read(pLinkID);
    rStream.InsertInMap(pLinkID, pLink);

    // ��ȡname
    rStream.Read(m_Name);

    // ��ȡ�������controller��link data
    int iCount;
    rStream.Read(iCount);
    m_Controllers.resize(iCount); // �ȴ������б�,�Ժ�link�׶����
    for( int i = 0; i < iCount; i++ )
    {
        Object* pObject;
        rStream.Read(pObject);
        pLink->Add(pObject);
    }

    SE_END_DEBUG_STREAM_LOAD(Object);
}
//----------------------------------------------------------------------------
void Object::Link(Stream& rStream, Stream::Link* pLink)
{
    for( int i = 0; i < (int)m_Controllers.size(); i++ )
    {
        Object* pLinkID = pLink->GetLinkID();
        m_Controllers[i] = (Controller*)rStream.GetFromMap(pLinkID);
    }
}
//----------------------------------------------------------------------------
bool Object::Register(Stream& rStream) const
{
    Object* pThis = (Object*)this;
    // ��object�Ƿ��Ѳ��뵽hash��,ȷ��Ψһ��
    if( rStream.InsertInMap(pThis, 0) )
    {
        // ����object����������ȵ�˳��,�ڱ���������ʱ���ռ�,
        // ��object�����ظ����ռ�
        rStream.InsertInOrdered(pThis);

        for( int i = 0; i < (int)m_Controllers.size(); i++ )
        {
            if( m_Controllers[i] )
            {
                m_Controllers[i]->Register(rStream);
            }
        }

        return true;
    }

    return false;
}
//----------------------------------------------------------------------------
void Object::Save(Stream& rStream) const
{
    SE_BEGIN_DEBUG_STREAM_SAVE;

    // RTTI name����loadʱ�Ĺ�������������
    rStream.Write(String(GetType().GetName()));

    // ���ڴ��ַ����load/linkʱ��object unique ID
    rStream.Write((Object*)this);

    // д��name
    rStream.Write(m_Name);

    // д���objectȫ��controller���ڴ�ָ������link data
    int iCount = (int)m_Controllers.size();
    rStream.Write(iCount);
    for( int i = 0; i < iCount; i++ )
    {
        rStream.Write(m_Controllers[i]);
    }

    SE_END_DEBUG_STREAM_SAVE(Object);
}
//----------------------------------------------------------------------------
int Object::GetDiskUsed(const StreamVersion&) const
{
    int iUsed = GetType().GetDiskUsed();

    iUsed += sizeof(this);

    iUsed += sizeof(int) + (int)m_Name.GetLength();

    iUsed += sizeof(int) + ((int)m_Controllers.size())*sizeof(ControllerPtr);

    return iUsed;
}
//----------------------------------------------------------------------------
StringTree* Object::SaveStrings(const char*)
{
    StringTree* pTree = SE_NEW StringTree;

    // strings
    pTree->Append(Format(&TYPE, (const char*)GetName()));
    pTree->Append(Format("this =", this));
    pTree->Append(Format("ID   =", m_uiID));
    pTree->Append(Format("refs =", m_iReferences));

    // children
    if( m_Controllers.size() > 0 )
    {
        StringTree* pCTree = SE_NEW StringTree;
        pCTree->Append(Format("controllers"));
        for( int i = 0; i < (int)m_Controllers.size(); i++ )
        {
            if( m_Controllers[i] )
            {
                pCTree->Append(m_Controllers[i]->SaveStrings());
            }
        }
        pTree->Append(pCTree);
    }

    return pTree;
}
//----------------------------------------------------------------------------