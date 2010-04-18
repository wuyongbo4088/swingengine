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

#include "SEMax8SceneBuilder.h"
#include "SEMax8SceneExport.h"
#include "SEMax8Exporter.h"
#include "resource.h"

using namespace Swing;

//----------------------------------------------------------------------------
Max8SceneExport::Max8SceneExport()
{
}
//----------------------------------------------------------------------------
Max8SceneExport::~Max8SceneExport()
{
}
//----------------------------------------------------------------------------
int Max8SceneExport::ExtCount()
{
    // ���֧�ֵĵ����ļ���չ��.
    // ֻ֧��һ��(*.seof).
    return 1;
}
//----------------------------------------------------------------------------
const TCHAR* Max8SceneExport::Ext(int)
{       
    // �����ļ���չ��.
    return _T("seof");
}
//----------------------------------------------------------------------------
const TCHAR* Max8SceneExport::LongDesc()
{
    // �����ļ��ĳ�����.
    return Max8Exporter::ThePlugin.GetResourceString(IDS_LONGDESCRIPTION);
}
//----------------------------------------------------------------------------
const TCHAR* Max8SceneExport::ShortDesc() 
{           
    // �����ļ��Ķ�����.
    return Max8Exporter::ThePlugin.GetResourceString(IDS_SHORTDESCRIPTION);
}
//----------------------------------------------------------------------------
const TCHAR* Max8SceneExport::AuthorName()
{           
    // �������.
    return Max8Exporter::ThePlugin.GetResourceString(IDS_AUTHORNAME);
}
//----------------------------------------------------------------------------
const TCHAR* Max8SceneExport::CopyrightMessage() 
{   
    // ��Ȩ��Ϣ.
    return
    _T("Swing Tech. Copyright (c) 2007-2008. All Rights Reserved");
}
//----------------------------------------------------------------------------
const TCHAR* Max8SceneExport::OtherMessage1() 
{       
    // ����������Ϣ1.
    return _T("");
}
//----------------------------------------------------------------------------
const TCHAR* Max8SceneExport::OtherMessage2() 
{       
    // ����������Ϣ2.
    return _T("");
}
//----------------------------------------------------------------------------
unsigned int Max8SceneExport::Version()
{               
    // ���ص������汾��.
    return EXPORTER_VERSION;
}
//----------------------------------------------------------------------------
void Max8SceneExport::ShowAbout(HWND)
{           
    // ��ʵ��.
    // ��ʾ�����About�Ի���.
}
//----------------------------------------------------------------------------
BOOL Max8SceneExport::SupportsOptions(int, DWORD)
{
    // Max�������������ȡ���֧�ָ����ļ���չ������Щѡ��.
    // ��ǰ֧������ѡ��.
    return TRUE;
}
//----------------------------------------------------------------------------
int Max8SceneExport::DoExport(const TCHAR* acFilename, ExpInterface* pExport,
    Interface* pMax, BOOL bSuppressPrompts, DWORD dwOptions)
{
    // �Ӵ����ļ���ȡ�������ĵ�ǰ����,
    // ��ʾUI����,���ݵ�ǰ����,�������ұ���Swing Engine scene graph,
    // ֮�󱣴浱ǰ���õ������ļ�.
    //    acFilename:
    //        �û�ָ����Swing Engine scene graph�ļ���.
    //    pExport:
    //        ����Max�ĵ�������ع��ܵĽӿ�ָ��.
    //    pMax:
    //        ����Max��ع��ܵ����ӿ�ָ��.
    //    bSuppressPrompts:
    //        ����,���ֹ�û�����.
    //    dwOptions:
    //        ����������������,�򳡾���ѡ�в���.

    m_pMax = pMax;
    INode* pRootNode = NULL;
    bool bExportSelected;

    if( dwOptions == SCENE_EXPORT_SELECTED )
    {
        // �û�ѡ�񵼳����ֳ���,
        // ���ȷ��ֻ��ѡ�г����е�һ���ӽڵ�,��Ϊ�����ӳ����ĸ��ڵ�.
        if( m_pMax->GetSelNodeCount() != 1 )
        {
            assert( false );

            return 0;
        }

        pRootNode = m_pMax->GetSelNode(0);
        bExportSelected = true;
    }
    else
    {
        // ������������,
        // ��Max�����ĸ��ڵ㿪ʼ.
        pRootNode = m_pMax->GetRootNode();
        bExportSelected = false;
    }

    // ��ȡ��������������ļ���·��.
    m_StrConfigFile = m_pMax->GetDir(APP_PLUGCFG_DIR);
    m_StrConfigFile += "\\Max8Exporter.cfg";

    // �ָ���ǰ������������.
    ReadConfiguration();

    // ��ʾ������UI.
    if( !bSuppressPrompts )
    {
        INT_PTR piResult = DialogBoxParam(Max8Exporter::ThePlugin.GetInstance(),
            MAKEINTRESOURCE(IDD_PANEL), GetActiveWindow(), DialogProcedure,
            (LPARAM)this);

        if( !piResult )
            return 1;
    }

    // ʵ�ʵ�������,��Max����ת��ΪSwing Engine���������ɴ����ļ�.
    Max8SceneBuilder(acFilename, bExportSelected, &m_EffectiveSettings, pExport,
        pMax, pRootNode);

    // ���浼�����ı�������.
    WriteConfiguration();
    
    return 1;
}
//----------------------------------------------------------------------------
void Max8SceneExport::ReadConfiguration()
{
    // �Ӵ����ļ��ָ����������������.
    // ������������û��ϴβ��������������ý��.

    // ��������ļ�������(�����һ�����е�����),
    // ��ʹ��Ĭ������.
    FILE* pConfig = SESystem::SE_Fopen(m_StrConfigFile, "rb");
    if( pConfig  == NULL )
    {
        return;
    }

    // ��ȡ�����ļ��汾.
    int iVersion;
    if( fread(&iVersion, sizeof(int), 1, pConfig) != 1 )
    {
        // �ļ�����ȷ
        assert( false );
        fclose(pConfig);

        return;
    }

    // ��֤�������汾�������ļ��汾�Ƿ�ƥ��.
    if( iVersion != EXPORTER_VERSION )
    {
        // ��ƥ��
        assert( false );
        fclose(pConfig);

        return;
    }

    // ��ȡ������Ϣ.
    Max8ExportSettings tempSettings;
    if( fread(&tempSettings, sizeof(Max8ExportSettings), 1, pConfig) != 1 )
    {
        // �ļ�����ȷ
        assert( false );
        fclose(pConfig);

        return;
    }
    
    fclose(pConfig);

    // ����Ĭ��������Ϣ.
    memcpy(&m_Settings, &tempSettings, sizeof(Max8ExportSettings));
}
//----------------------------------------------------------------------------
void Max8SceneExport::WriteConfiguration()
{
    FILE* pConfig = SESystem::SE_Fopen(m_StrConfigFile, "wb");
    if( pConfig == NULL )
    {
        // ���������ļ�ʧ��
        assert( false );

        return;
    }

    // д�������ļ��汾.
    int iVersion = EXPORTER_VERSION;
    if( fwrite(&iVersion, sizeof(int), 1, pConfig) != 1 )
    {
        // д�����ļ�ʧ��
        assert( false );
        fclose(pConfig);

        return;
    }

    // д�뵼����������Ϣ.
    if( fwrite(&m_Settings, sizeof(Max8ExportSettings), 1, pConfig) != 1 )
    {
        // д�����ļ�ʧ��
        assert( false );
        fclose(pConfig);

        return;
    }
    
    fclose(pConfig);
}
//----------------------------------------------------------------------------
BOOL CALLBACK Max8SceneExport::DialogProcedure(HWND hWnd, UINT uiMsg,
    WPARAM wParam, LPARAM lParam) 
{
    if( uiMsg == WM_INITDIALOG )
    {
        // ���Ի����ʼ��ʱ,Maxͨ��LPARAM�������ݵ�ǰ������ʵ����"this"ָ�����.
        // �������ָ�빩�Ժ�ʹ��.
        SetWindowLongPtr(hWnd, GWLP_USERDATA, lParam);

        // ���öԻ���ʵ���ĳ�ʼ������.
        return ((Max8SceneExport*)lParam)->OnInitDialog(hWnd);
    }

    Max8SceneExport* pExporter = (Max8SceneExport*)GetWindowLongPtr(hWnd,
        GWLP_USERDATA); 

    switch( uiMsg )
    {
    case WM_COMMAND:
        // �û�����UI������UI�¼�,
        // �ж��¼�,������Ӧ��Ӧ.
        switch( LOWORD(wParam) ) 
        {
        case IDC_CHECK_MESHES:
            return pExporter->OnMeshChecked(hWnd,
                IsDlgButtonChecked(hWnd, IDC_CHECK_MESHES));
        case IDC_CHECK_OBJECTS:
            return pExporter->OnObjectsChecked(hWnd,
                IsDlgButtonChecked(hWnd, IDC_CHECK_OBJECTS));
        case IDC_CHECK_MODIFIERS:
            return pExporter->OnModifiersChecked(hWnd,
                IsDlgButtonChecked(hWnd, IDC_CHECK_MODIFIERS));
        case IDC_RADIO_ALLFRAMES:
        case IDC_RADIO_KEYFRAMES:
            return pExporter->OnEnableFrameSpinners(hWnd, TRUE);
        case IDC_RADIO_CURRENTFRAME:
            return pExporter->OnEnableFrameSpinners(hWnd, FALSE);
        case IDOK:
            pExporter->OnOK(hWnd);
            EndDialog(hWnd, 1);

            break;
        case IDCANCEL:
            EndDialog(hWnd, 0);

            break;
        default:
            return FALSE;
        }
        break;

    case CC_SPINNER_CHANGE:
        // spinner�ؼ�����Ϣ����.
        switch( LOWORD(wParam) )
        {
        case IDC_SPINNER_ENDFRAME:
            return pExporter->OnEndFrameSpinnerChanged(hWnd);
        case IDC_SPINNER_STARTFRAME:
            return pExporter->OnStartFrameSpinnerChanged(hWnd);
        }
        break;

    case WM_CLOSE:
        EndDialog(hWnd, 0);

        return TRUE;
    }

    return FALSE;
}
//----------------------------------------------------------------------------
BOOL Max8SceneExport::OnEnableFrameSpinners(HWND hWnd, BOOL bEnabled)
{
    // ������ر�frame spinner�ؼ�.
    ISpinnerControl* pSpinner = GetISpinner(GetDlgItem(hWnd,
        IDC_SPINNER_STARTFRAME));
    pSpinner->Enable(bEnabled);
    ReleaseISpinner(pSpinner); 
    pSpinner = GetISpinner(GetDlgItem(hWnd, IDC_SPINNER_ENDFRAME)); 
    pSpinner->Enable(bEnabled);

    return TRUE;
}
//----------------------------------------------------------------------------
BOOL Max8SceneExport::OnEndFrameSpinnerChanged(HWND hWnd)
{
    // ���û��޸ı�ʾ���������������spinner�ؼ�ʱ����.

    // ��spinner�ؼ���ȡ����������ʼ��.
    ISpinnerControl* pSpinner = GetISpinner(GetDlgItem(hWnd,
        IDC_SPINNER_STARTFRAME)); 
    int iStartFrame = pSpinner->GetIVal();
    ReleaseISpinner(pSpinner);

    // ȷ������������������ڵ��ڵ���������ʼ��.
    pSpinner = GetISpinner(GetDlgItem(hWnd, IDC_SPINNER_ENDFRAME)); 
    int iEndFrame = pSpinner->GetIVal();
    if( iStartFrame > iEndFrame )
    {
        pSpinner->SetValue(iStartFrame, FALSE);
    }

    ReleaseISpinner(pSpinner);

    return TRUE;
}
//----------------------------------------------------------------------------
BOOL Max8SceneExport::OnInitDialog(HWND hWnd)
{
    // ���Ի���ӵ�WM_INITDIALOG��Ϣʱ,���е�����UI��ʼ������.

    CenterWindow(hWnd, GetParent(hWnd)); 

    // ����UI״̬.
    // ��������ļ�����,�������ϴ�ʹ�õ�������������ļ�������.
    // ����ʹ��Ĭ������.
    CheckDlgButton(hWnd,IDC_CHECK_OBJECTS, m_Settings.IncludeObjects);
    CheckDlgButton(hWnd,IDC_CHECK_LIGHTS, m_Settings.IncludeLights);
    CheckDlgButton(hWnd,IDC_CHECK_MESHES, m_Settings.IncludeMeshes);
    CheckDlgButton(hWnd,IDC_CHECK_CAMERAS, m_Settings.IncludeCameras);
    CheckDlgButton(hWnd,IDC_CHECK_NORMALS, m_Settings.IncludeNormals);       
    CheckDlgButton(hWnd,IDC_CHECK_MATERIALS, m_Settings.IncludeMaterials);
    CheckDlgButton(hWnd,IDC_CHECK_VERTEXCOLS, m_Settings.IncludeVertexColors);
    CheckDlgButton(hWnd,IDC_CHECK_TEXCOORDS, m_Settings.IncludeTexCoords);
    CheckDlgButton(hWnd,IDC_CHECK_GENERATEMAPS, m_Settings.GenerateMaps);
    CheckDlgButton(hWnd,IDC_CHECK_MODIFIERS, m_Settings.IncludeModifiers);
    CheckDlgButton(hWnd,IDC_CHECK_SKINS, m_Settings.IncludeSkins);
    CheckDlgButton(hWnd,IDC_CHECK_GPU, m_Settings.UseGPUSkin);
    CheckDlgButton(hWnd,IDC_RADIO_CURRENTFRAME, m_Settings.IncludeCurrentFrame);
    CheckDlgButton(hWnd,IDC_RADIO_KEYFRAMES, m_Settings.IncludeKeyFrames);
    CheckDlgButton(hWnd,IDC_RADIO_ALLFRAMES, m_Settings.IncludeAllFrames);
    CheckDlgButton(hWnd,IDC_CHECK_USELOCALTIME, m_Settings.UseLocalTime);

    if( !m_Settings.IncludeObjects )
    {
        // ���δѡ��"Object",��ر���������ѡ��.
        OnObjectsChecked(hWnd, FALSE);
    }
    else if( !m_Settings.IncludeMeshes )
    {
        // ���δѡ��"Mesh",��ر���������ѡ��.
        OnMeshChecked(hWnd, FALSE);
    }

    // ���δѡ��"Modifiers",��ر���������ѡ��.
    if( !m_Settings.IncludeModifiers )
    {
        OnModifiersChecked(hWnd, FALSE);
    }

    // ��ȡMax�����еĶ�������Ŀ.
    int iNumFrames = m_pMax->GetAnimRange().Duration() / GetTicksPerFrame();

    // ����spinner�ؼ�.
    ISpinnerControl* pSpinner = GetISpinner(GetDlgItem(hWnd,
        IDC_SPINNER_STARTFRAME));
    pSpinner->LinkToEdit(GetDlgItem(hWnd, IDC_EDIT_STARTFRAME), EDITTYPE_INT);
    pSpinner->SetLimits(0, iNumFrames, TRUE);
    pSpinner->SetScale(1.0f);
    pSpinner->SetValue(0, FALSE);
    if( m_Settings.IncludeCurrentFrame )
    {
        pSpinner->Enable(FALSE);
    }
    ReleaseISpinner(pSpinner);

    pSpinner = GetISpinner(GetDlgItem(hWnd, IDC_SPINNER_ENDFRAME));
    pSpinner->LinkToEdit(GetDlgItem(hWnd, IDC_EDIT_ENDFRAME), EDITTYPE_INT);
    pSpinner->SetLimits(0, iNumFrames, TRUE);
    pSpinner->SetScale(1.0f);
    pSpinner->SetValue(iNumFrames, FALSE);
    if( m_Settings.IncludeCurrentFrame )
    {
        pSpinner->Enable(FALSE);
    }
    ReleaseISpinner(pSpinner);

    return TRUE;
}
//----------------------------------------------------------------------------
BOOL Max8SceneExport::OnMeshChecked(HWND hWnd, BOOL bEnabled)
{
    // ���"Mesh"ʱ�Ķ���.
    EnableWindow(GetDlgItem(hWnd, IDC_CHECK_NORMALS), bEnabled);
    EnableWindow(GetDlgItem(hWnd, IDC_CHECK_VERTEXCOLS), bEnabled);
    EnableWindow(GetDlgItem(hWnd, IDC_CHECK_TEXCOORDS), bEnabled);
    EnableWindow(GetDlgItem(hWnd, IDC_CHECK_MATERIALS), bEnabled);
    EnableWindow(GetDlgItem(hWnd, IDC_CHECK_GENERATEMAPS), bEnabled);

    return TRUE;
}
//----------------------------------------------------------------------------
BOOL Max8SceneExport::OnModifiersChecked(HWND hWnd, BOOL bEnabled)
{
    // ���"Modifiers"ʱ�Ķ���.
    EnableWindow(GetDlgItem(hWnd, IDC_CHECK_SKINS), bEnabled);
    EnableWindow(GetDlgItem(hWnd, IDC_CHECK_GPU), bEnabled);

    return TRUE;
}
//----------------------------------------------------------------------------
BOOL Max8SceneExport::OnObjectsChecked(HWND hWnd, BOOL bEnabled)
{
    // ���"Objects"ʱ�Ķ���.
    EnableWindow(GetDlgItem(hWnd, IDC_CHECK_MESHES), bEnabled);
    EnableWindow(GetDlgItem(hWnd, IDC_CHECK_CAMERAS), bEnabled);
    EnableWindow(GetDlgItem(hWnd, IDC_CHECK_LIGHTS), bEnabled);   

    // mesh������ѡ�����object��ѡ������,��˴��ݸ�mesh��.
    OnMeshChecked(hWnd, bEnabled && IsDlgButtonChecked(hWnd, IDC_CHECK_MESHES));

    return TRUE;
}
//----------------------------------------------------------------------------
void Max8SceneExport::OnOK(HWND hWnd)
{
    // �û����OK��ť.
    // ��ʵ�ʵ���������ʼǰ,��ȡ������UI�ϵ����ѡ���������ݲ�������֤.

    m_Settings.IncludeObjects =
        IsDlgButtonChecked(hWnd, IDC_CHECK_OBJECTS);
    m_Settings.IncludeLights =
        IsDlgButtonChecked(hWnd, IDC_CHECK_LIGHTS);
    m_Settings.IncludeCameras =
        IsDlgButtonChecked(hWnd, IDC_CHECK_CAMERAS);
    m_Settings.IncludeMeshes =
        IsDlgButtonChecked(hWnd, IDC_CHECK_MESHES);
    m_Settings.IncludeNormals =
        IsDlgButtonChecked(hWnd, IDC_CHECK_NORMALS);
    m_Settings.IncludeVertexColors =
        IsDlgButtonChecked(hWnd, IDC_CHECK_VERTEXCOLS);
    m_Settings.IncludeTexCoords =
        IsDlgButtonChecked(hWnd, IDC_CHECK_TEXCOORDS);
    m_Settings.IncludeMaterials =
        IsDlgButtonChecked(hWnd, IDC_CHECK_MATERIALS);
    m_Settings.GenerateMaps =
        IsDlgButtonChecked(hWnd, IDC_CHECK_GENERATEMAPS);
    m_Settings.IncludeModifiers =
        IsDlgButtonChecked(hWnd, IDC_CHECK_MODIFIERS);
    m_Settings.IncludeSkins =
        IsDlgButtonChecked(hWnd, IDC_CHECK_SKINS);
    m_Settings.UseGPUSkin = 
        IsDlgButtonChecked(hWnd, IDC_CHECK_GPU);
    m_Settings.IncludeAllFrames =
        IsDlgButtonChecked(hWnd, IDC_RADIO_ALLFRAMES);
    m_Settings.IncludeKeyFrames =
        IsDlgButtonChecked(hWnd, IDC_RADIO_KEYFRAMES);
    m_Settings.IncludeCurrentFrame =
        IsDlgButtonChecked(hWnd, IDC_RADIO_CURRENTFRAME);
    m_Settings.UseLocalTime =
        IsDlgButtonChecked(hWnd, IDC_CHECK_USELOCALTIME);

    // �����spinner�ؼ���ȡ����������ʼ��ͽ�����.
    if( m_Settings.IncludeAllFrames || m_Settings.IncludeKeyFrames )
    {
        ISpinnerControl* pSpinner = GetISpinner(GetDlgItem(hWnd,
            IDC_SPINNER_STARTFRAME)); 
        m_Settings.StartFrame = pSpinner->GetIVal();
        ReleaseISpinner(pSpinner);

        pSpinner = GetISpinner(GetDlgItem(hWnd, IDC_SPINNER_ENDFRAME)); 
        m_Settings.EndFrame = pSpinner->GetIVal();
        ReleaseISpinner(pSpinner);
    }


    // ����UIѡ�����ú����Ծ���.
    m_EffectiveSettings = m_Settings;
    if( !m_Settings.IncludeObjects )
    {
        m_EffectiveSettings.IncludeCameras = FALSE;
        m_EffectiveSettings.IncludeLights = FALSE;
        m_EffectiveSettings.IncludeMeshes = FALSE;
    }
    if( !m_EffectiveSettings.IncludeMeshes )
    {
        m_EffectiveSettings.IncludeNormals = FALSE;
        m_EffectiveSettings.IncludeMaterials = FALSE;
        m_EffectiveSettings.IncludeVertexColors = FALSE;
        m_EffectiveSettings.IncludeTexCoords = FALSE;
        m_EffectiveSettings.GenerateMaps = FALSE;
    }
    if( !m_Settings.IncludeModifiers )
    {
        m_Settings.IncludeSkins = FALSE;
    }
}
//----------------------------------------------------------------------------
BOOL Max8SceneExport::OnStartFrameSpinnerChanged(HWND hWnd)
{
    // ���û��޸���ʼ��spinner�ؼ�������ʱ����.

    // ��ȡ��ʼ������.
    ISpinnerControl* pSpinner = GetISpinner(GetDlgItem(hWnd,
        IDC_SPINNER_STARTFRAME)); 
    int iStartFrame = pSpinner->GetIVal();
    ReleaseISpinner(pSpinner);

    // ��ȡ����������,��ȷ����ֵ���ڵ�����ʼ��ֵ.
    pSpinner = GetISpinner(GetDlgItem(hWnd,IDC_SPINNER_ENDFRAME)); 
    int iEndFrame = pSpinner->GetIVal();
    if( iEndFrame < iStartFrame )
        pSpinner->SetValue(iStartFrame, FALSE);
    ReleaseISpinner(pSpinner); 

    return TRUE;
}
//----------------------------------------------------------------------------



