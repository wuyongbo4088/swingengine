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
#include "SECommand.h"

using namespace Swing;

char Command::ms_acOptionNotFound[] = "option not found";
char Command::ms_acArgumentRequired[] = "option requires an argument";
char Command::ms_acArgumentOutOfRange[] = "argument out of range";
char Command::ms_acFilenameNotFound[] = "filename not found";

//----------------------------------------------------------------------------
Command::Command(int iCount, char** apcArgument)
{
    m_iCount = iCount;
    m_acCmdline = 0;
    m_abUsed = 0;

    if( m_iCount > 0 )
    {
        m_apcArgument = SE_NEW char*[m_iCount];
        for( int i = 0; i < m_iCount; i++ )
        {
            m_apcArgument[i] = apcArgument[i];
        }
    }
    else
    {
        m_apcArgument = 0;
    }

    Initialize();
}
//----------------------------------------------------------------------------
Command::Command(char* acCmdline)
{
    class Argument
    {
    public:
        char* m_pcItem;
        Argument* m_pNext;
    };

    m_iCount = 0;
    m_apcArgument = 0;
    m_acCmdline = 0;
    m_abUsed = 0;

    if( acCmdline == 0 || strlen(acCmdline) == 0 )
    {
        return;
    }

    size_t uiSize = strlen(acCmdline) + 1;
    m_acCmdline = SE_NEW char[uiSize];
    System::SE_Strcpy(m_acCmdline, uiSize, acCmdline);

    // 拆分包含空格符或tab符的原始字符串.
    // 被以上两者分割的每个子串都是一个传入参数.
    char* pcNextToken;
    char* pcToken = System::SE_Strtok(m_acCmdline, " \t"/*空格符或tab符*/, pcNextToken);
    Argument* pList = 0;

    while( pcToken )
    {
        m_iCount++;

        Argument* pCurrent = SE_NEW Argument;
        pCurrent->m_pcItem = pcToken;
        pCurrent->m_pNext = pList;
        pList = pCurrent;

        pcToken = System::SE_Strtok(0, " \t", pcNextToken);
    }

    m_iCount++;
    m_apcArgument = SE_NEW char*[m_iCount];
    m_apcArgument[0] = m_acCmdline;
    int i = m_iCount - 1;
    while( pList )
    {
        m_apcArgument[i--] = pList->m_pcItem;
        
        Argument* pSave = pList->m_pNext;
        SE_DELETE pList;
        pList = pSave;
    }

    // 注意我们分配的m_acCmdline依然存在,只是被strtok函数把若干位置替换成了'\0'.
    // 使m_acCmdline成为了若干独立子字符串,但其内存使用情况不变,
    // 稍后在析构函数中将释放m_acCmdline.
    Initialize();
}
//----------------------------------------------------------------------------
Command::~Command()
{
    SE_DELETE[] m_abUsed;
    SE_DELETE[] m_apcArgument;
    SE_DELETE[] m_acCmdline;
}
//----------------------------------------------------------------------------
void Command::Initialize()
{
    m_abUsed = SE_NEW bool[m_iCount];
    memset(m_abUsed, false, m_iCount*sizeof(bool));

    m_dSmall = 0.0;
    m_dLarge = 0.0;
    m_bMinSet = false;
    m_bMaxSet = false;
    m_bInfSet = false;
    m_bSupSet = false;

    m_acLastError = 0;
}
//----------------------------------------------------------------------------
int Command::ExcessArguments()
{
    // 检查是否有尚未处理的命令行参数.
    for( int i = 1; i < m_iCount; i++ )
    {
        if( !m_abUsed[i] )
        {
            return i;
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
Command& Command::Min(double dValue)
{
    m_dSmall = dValue;
    m_bMinSet = true;

    return *this;
}
//----------------------------------------------------------------------------
Command& Command::Max(double dValue)
{
    m_dLarge = dValue;
    m_bMaxSet = true;

    return *this;
}
//----------------------------------------------------------------------------
Command& Command::Inf(double dValue)
{
    m_dSmall = dValue;
    m_bInfSet = true;

    return *this;
}
//----------------------------------------------------------------------------
Command& Command::Sup(double dValue)
{
    m_dLarge = dValue;
    m_bSupSet = true;

    return *this;
}
//----------------------------------------------------------------------------
int Command::Boolean(char* acName)
{
    bool bValue = false;

    return Boolean(acName, bValue);
}
//----------------------------------------------------------------------------
int Command::Boolean(char* acName, bool& rbValue)
{
    int iMatchFound = 0;
    rbValue = false;
    for( int i = 1; i < m_iCount; i++ )
    {
        char* pcTmp = m_apcArgument[i];
        if( !m_abUsed[i] && pcTmp[0] == '-' && strcmp(acName, ++pcTmp) == 0 )
        {
            m_abUsed[i] = true;
            iMatchFound = i;
            rbValue = true;

            break;
        }
    }

    if( iMatchFound == 0 )
    {
        m_acLastError = ms_acOptionNotFound;
    }

    return iMatchFound;
}
//----------------------------------------------------------------------------
int Command::Integer(char* acName, int& riValue)
{
    int iMatchFound = 0;
    for( int i = 1; i < m_iCount; i++ )
    {
        char* pcTmp = m_apcArgument[i];
        if( !m_abUsed[i] && pcTmp[0] == '-' && strcmp(acName, ++pcTmp) == 0 )
        {
            // 获取指定参数名的具体参数值.
            pcTmp = m_apcArgument[i+1];
            if( m_abUsed[i+1] || (pcTmp[0] == '-' && !isdigit(pcTmp[1])) )
            {
                // 不是数字
                m_acLastError = ms_acArgumentRequired;

                return 0;
            }
            riValue = atoi(pcTmp);
            if( (m_bMinSet && riValue < m_dSmall)
            ||  (m_bMaxSet && riValue > m_dLarge)
            ||  (m_bInfSet && riValue <= m_dSmall)
            ||  (m_bSupSet && riValue >= m_dLarge) )
            {
                // 数字超过限制范围
                m_acLastError = ms_acArgumentOutOfRange;

                return 0;
            }
            m_abUsed[i] = true;
            m_abUsed[i+1] = true;
            iMatchFound = i;

            break;
        }
    }

    m_bMinSet = false;
    m_bMaxSet = false;
    m_bInfSet = false;
    m_bSupSet = false;

    if( iMatchFound == 0 )
    {
        m_acLastError = ms_acOptionNotFound;
    }

    return iMatchFound;
}
//----------------------------------------------------------------------------
int Command::Float(char* acName, float& rfValue)
{
    int iMatchFound = 0;
    for( int i = 1; i < m_iCount; i++ )
    {
        char* pcTmp = m_apcArgument[i];
        if( !m_abUsed[i] && pcTmp[0] == '-' && strcmp(acName, ++pcTmp) == 0 )
        {
            // 获取指定参数名的具体参数值.
            pcTmp = m_apcArgument[i+1];
            if( m_abUsed[i+1] || (pcTmp[0] == '-' && !isdigit(pcTmp[1])) )
            {
                //  不是数字
                m_acLastError = ms_acArgumentRequired;

                return 0;
            }
            rfValue = (float)atof(pcTmp);
            if( (m_bMinSet && rfValue < m_dSmall)
            ||  (m_bMaxSet && rfValue > m_dLarge)
            ||  (m_bInfSet && rfValue <= m_dSmall)
            ||  (m_bSupSet && rfValue >= m_dLarge) )
            {
                // 数字超过限制范围
                m_acLastError = ms_acArgumentOutOfRange;

                return 0;
            }
            m_abUsed[i] = true;
            m_abUsed[i+1] = true;
            iMatchFound = i;

            break;
        }
    }

    m_bMinSet = false;
    m_bMaxSet = false;
    m_bInfSet = false;
    m_bSupSet = false;

    if( iMatchFound == 0 )
    {
        m_acLastError = ms_acOptionNotFound;
    }

    return iMatchFound;
}
//----------------------------------------------------------------------------
int Command::Double(char* acName, double& rdValue)
{
    int iMatchFound = 0;
    for( int i = 1; i < m_iCount; i++ )
    {
        char* pcTmp = m_apcArgument[i];
        if( !m_abUsed[i] && pcTmp[0] == '-' && strcmp(acName, ++pcTmp) == 0 )
        {
            // 获取指定参数名的具体参数值.
            pcTmp = m_apcArgument[i+1];
            if( m_abUsed[i+1] || (pcTmp[0] == '-' && !isdigit(pcTmp[1])) )
            {
                //  不是数字
                m_acLastError = ms_acArgumentRequired;

                return 0;
            }
            rdValue = atof(pcTmp);
            if( (m_bMinSet && rdValue < m_dSmall)
            ||  (m_bMaxSet && rdValue > m_dLarge)
            ||  (m_bInfSet && rdValue <= m_dSmall)
            ||  (m_bSupSet && rdValue >= m_dLarge) )
            {
                // 数字超过限制范围
                m_acLastError = ms_acArgumentOutOfRange;

                return 0;
            }
            m_abUsed[i] = true;
            m_abUsed[i+1] = true;
            iMatchFound = i;

            break;
        }
    }

    m_bMinSet = false;
    m_bMaxSet = false;
    m_bInfSet = false;
    m_bSupSet = false;

    if( iMatchFound == 0)
    {
        m_acLastError = ms_acOptionNotFound;
    }

    return iMatchFound;
}
//----------------------------------------------------------------------------
int Command::String(char* acName, char*& racValue)
{
    int iMatchFound = 0;
    for( int i = 1; i < m_iCount; i++ )
    {
        char* pcTmp = m_apcArgument[i];
        if( !m_abUsed[i] && pcTmp[0] == '-' && strcmp(acName, ++pcTmp) == 0 )
        {
            // 获取指定参数名的具体参数值.
            pcTmp = m_apcArgument[i+1];
            if( m_abUsed[i+1] || pcTmp[0] == '-' )
            {
                // 不是字符串
                m_acLastError = ms_acArgumentRequired;

                return 0;
            }

            // 获取该字符串参数.
            size_t uiSize = strlen(pcTmp) + 1;
            racValue = SE_NEW char[uiSize];
            System::SE_Strcpy(racValue, uiSize, pcTmp);
            m_abUsed[i] = true;
            m_abUsed[i+1] = true;
            iMatchFound = i;

            break;
        }
    }

    if( iMatchFound == 0 )
    {
        m_acLastError = ms_acOptionNotFound;
    }

    return iMatchFound;
}
//----------------------------------------------------------------------------
int Command::Filename(char*& racName)
{
    int iMatchFound = 0;
    for( int i = 1; i < m_iCount; i++ )
    {
        char* pcTmp = m_apcArgument[i];
        if( !m_abUsed[i] && pcTmp[0] != '-' )
        {
            size_t uiSize = strlen(pcTmp) + 1;
            racName = SE_NEW char[uiSize];
            System::SE_Strcpy(racName, uiSize, pcTmp);
            m_abUsed[i] = true;
            iMatchFound = i;

            break;
        }
    }

    if( iMatchFound == 0 )
    {
        m_acLastError = ms_acFilenameNotFound;
    }

    return iMatchFound;
}
//----------------------------------------------------------------------------
const char* Command::GetLastError()
{
    return m_acLastError;
}
//----------------------------------------------------------------------------
