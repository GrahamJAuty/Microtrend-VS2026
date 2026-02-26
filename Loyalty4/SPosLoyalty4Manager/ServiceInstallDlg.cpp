//$$******************************************************************
#include "..\SPosLoyalty4Shared\FileRemover.h"
#include "..\SPosLoyalty4Shared\ServiceOptions.h"
//$$******************************************************************
#include "ServiceLogonDlg.h"
//$$******************************************************************
#include "ServiceInstallDlg.h"
//$$******************************************************************

CControlDisplayStatus::CControlDisplayStatus(CWnd* pControl)
{
	m_pControl = pControl;
	m_bCurrentEnable = TRUE;
	m_bCurrentShow = TRUE;
	m_bWantEnable = TRUE;
	m_bWantShow = TRUE;
	m_strCurrentText = "";
	m_strWantText = "";
}

//$$******************************************************************

void CControlDisplayStatus::UpdateDisplay(CSSDialog* pParent)
{
	if (m_pControl != NULL)
	{
		if ((m_bCurrentEnable != m_bWantEnable) || (m_bCurrentShow != m_bWantShow))
		{
			pParent->ShowAndEnableWindow(m_pControl, m_bWantShow, m_bWantEnable);
			m_bCurrentEnable = m_bWantEnable;
			m_bCurrentShow = m_bWantShow;
		}

		if (m_strCurrentText != m_strWantText)
		{
			m_pControl->SetWindowText(m_strWantText);
			m_strCurrentText = m_strWantText;
		}
	}
}

//$$******************************************************************

void CControlDisplayStatus::SetShowAndEnable(bool bShow, bool bEnable)
{
	if (FALSE == bShow)
	{
		m_bWantShow = FALSE;
		m_bWantEnable = FALSE;
	}
	else
	{
		m_bWantShow = TRUE;
		m_bWantEnable = bEnable;
	}
}

//$$******************************************************************

CServiceInstallDlg::CServiceInstallDlg( CWnd* pParent)
	: CSSDialog( CServiceInstallDlg::IDD, pParent),
	m_DisplayStaticInstall(&m_staticInstall),
	m_DisplayButtonInstall(&m_buttonInstall),
	m_DisplayStaticRunning(&m_staticRunning),
	m_DisplayButtonRunning(&m_buttonRunning)
{
	m_bServiceInstalled = FALSE;
	m_bServiceRunning = FALSE;
	m_hTimer = NULL;
	m_bInTimer = FALSE;

	m_bShowError = FALSE;
	m_bFirstCheckActive = FALSE;
	m_bFirstCheckError = FALSE;

	m_pServiceOptions = &ServerServiceOptions;
	m_Service.Init("LoyaltyManagerV4Server", "Loyalty Manager V4 Server", "Loyalty Manager V4 Server", SERVICE_ACCEPT_STOP);
	m_strServiceStopFilename = Filenames.GetServerServiceStopFilename();
	m_TrackedFileActive.SetFilename(Filenames.GetServerServiceActionTimeFilename());
	m_TrackedFileError.SetFilename(Filenames.GetServerServiceErrorTimeFilename());

	m_TrackedFileActive.CheckFile(TRUE);
	m_TrackedFileError.CheckFile(TRUE);
	m_strTimeActive = "";
	m_strInfoActive = "";
	m_strTimeError = "";
	m_strInfoError = "";

	m_nPreviousPCId = m_pServiceOptions->GetPCId();
}

//$$******************************************************************

void CServiceInstallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_THISPC, m_checkThisPC);
	DDX_Control(pDX, IDC_EDIT_PCID, m_editPCId);
	DDX_Control(pDX, IDC_CHECK_STOP, m_checkStop);
	DDX_Control(pDX, IDC_STATIC_INSTALL, m_staticInstall);
	DDX_Control(pDX, IDC_BUTTON_INSTALL, m_buttonInstall);
	DDX_Control(pDX, IDC_STATIC_RUN, m_staticRunning);
	DDX_Control(pDX, IDC_BUTTON_RUN, m_buttonRunning);
	DDX_Control(pDX, IDC_EDIT_ACTIVE_ID, m_editActiveID);
	DDX_Control(pDX, IDC_EDIT_ACTIVE_TIME, m_editActiveTime);
	DDX_Control(pDX, IDC_STATIC_ACTIVITY, m_staticActivity);
}

//$$******************************************************************
BEGIN_MESSAGE_MAP(CServiceInstallDlg, CSSDialog)
	ON_BN_CLICKED( IDC_CHECK_THISPC, OnToggleThisPC )
	ON_BN_CLICKED( IDC_CHECK_STOP, OnToggleStop )
	ON_BN_CLICKED( IDC_BUTTON_INSTALL, OnButtonInstall )
	ON_BN_CLICKED( IDC_BUTTON_RUN, OnButtonRunning )
	ON_BN_CLICKED( IDC_STATIC_INFO, OnToggleActivity)
	ON_BN_CLICKED( IDC_STATIC_TIME, OnToggleTime)
	ON_WM_TIMER()
END_MESSAGE_MAP()
//$$******************************************************************

BOOL CServiceInstallDlg::OnInitDialog()
{
	CSSDialog::OnInitDialog();

	SetWindowText("Loyalty Server Service");
	
	if (m_nPreviousPCId == Sysset.GetPCIdHost())
	{
		m_checkThisPC.SetCheck(TRUE);
		DisplayPCId(m_nPreviousPCId);
		m_nPreviousPCId = 0;
	}
	else
	{
		m_checkThisPC.SetCheck(FALSE);
		DisplayPCId(m_nPreviousPCId);
	}

	UpdateInstallOptions();
	UpdateActiveTime();
	m_hTimer = SetTimer(1, 5000, 0);

	return TRUE;
}

//$$******************************************************************

void CServiceInstallDlg::DisplayPCId(__int64 nId)
{
	if (0 == nId)
	{
		m_editPCId.SetWindowText("None");
	}
	else
	{
		CString strPCId = "";
		strPCId.Format("%10.10I64d", nId);
		m_editPCId.SetWindowText(strPCId);
	}
}

//$$******************************************************************

void CServiceInstallDlg::OnToggleThisPC()
{
	__int64 nId = 0;

	if (IsTicked(m_checkThisPC))
	{
		nId = Sysset.GetPCIdHost();
	}
	else
	{
		nId = m_nPreviousPCId;
	}

	DisplayPCId(nId);
	m_pServiceOptions->SetPCId(nId);
	m_pServiceOptions->Write();
}

//$$******************************************************************

void CServiceInstallDlg::OnToggleStop()
{
	if (IsTicked(m_checkStop) == TRUE)
	{
		CSSFile fileStop;
		fileStop.Open(m_strServiceStopFilename, "wb");
		fileStop.WriteLine("Stop");
		fileStop.Close();
	}
	else
	{
		CFileRemover FileRemover;
		FileRemover.RemoveFile(m_strServiceStopFilename);
	}

	UpdateInstallOptions();
}

//$$******************************************************************

void CServiceInstallDlg::UpdateInstallOptions()
{
	m_bServiceRunning = FALSE;
	
	bool bStopRequest = FileExists( m_strServiceStopFilename );
	m_checkStop.SetCheck( bStopRequest );
	
	bool bInstallStatusKnown;
	m_Service.IsInstalled( bInstallStatusKnown, m_bServiceInstalled );

	if ( FALSE == bInstallStatusKnown )
	{
		m_DisplayStaticInstall.SetText( "Unable to check service installation status" );
		m_DisplayButtonInstall.SetShowAndEnable( FALSE );
		m_DisplayStaticRunning.SetShowAndEnable( FALSE );
		m_DisplayButtonRunning.SetShowAndEnable( FALSE );	
	}
	else if ( TRUE == m_bServiceInstalled )
	{
		m_DisplayStaticInstall.SetText( "Service is installed on this PC" );
		m_DisplayButtonInstall.SetText( "Uninstall Service" );
		m_DisplayStaticRunning.SetShowAndEnable( TRUE );
		m_DisplayButtonRunning.SetShowAndEnable( TRUE, FALSE == bStopRequest );
		UpdateRunningOptions( bStopRequest );
	}
	else
	{
		m_DisplayStaticInstall.SetText( "Service is NOT installed on this PC" );
		m_DisplayButtonInstall.SetText( "Install Service" );
		m_DisplayButtonInstall.SetShowAndEnable( TRUE, FALSE == bStopRequest );
		m_DisplayStaticRunning.SetShowAndEnable( FALSE );
		m_DisplayButtonRunning.SetShowAndEnable( FALSE );
	}

	m_DisplayStaticInstall.UpdateDisplay( this );
	m_DisplayButtonInstall.UpdateDisplay( this );
	m_DisplayStaticRunning.UpdateDisplay( this );
	m_DisplayButtonRunning.UpdateDisplay( this );
}

//$$******************************************************************

void CServiceInstallDlg::UpdateRunningOptions( bool bStopRequest )
{
	bool bRunningStatusKnown;
	m_Service.IsRunning( bRunningStatusKnown, m_bServiceRunning );

	if ( FALSE == bRunningStatusKnown )
	{
		m_DisplayStaticRunning.SetText( "Unable to check service running status" );
		m_DisplayButtonRunning.SetShowAndEnable( FALSE );
		m_DisplayButtonInstall.SetShowAndEnable( TRUE, FALSE );
	}
	else if ( TRUE == m_bServiceRunning )
	{
		m_DisplayStaticRunning.SetText( "Service is running on this PC" );
		m_DisplayButtonRunning.SetText( "Stop Service" );
		m_DisplayButtonInstall.SetShowAndEnable( TRUE, FALSE );
	}
	else
	{
		m_DisplayStaticRunning.SetText( "Service is NOT running on this PC" );
		m_DisplayButtonRunning.SetText( "Start Service" );
		m_DisplayButtonInstall.SetShowAndEnable( TRUE, FALSE == bStopRequest );
	}
}

//$$******************************************************************

void CServiceInstallDlg::OnButtonInstall()
{
	if (TRUE == m_bServiceInstalled)
	{
		if (m_Service.ServiceUninstall() == FALSE)
		{
			DisplayError("Uninstall service", m_Service.GetError());
		}

		UpdateInstallOptions();
	}
	else
	{
		CServiceLogonDlg dlgLogon(this);

		if (dlgLogon.DoModal() == IDOK)
		{
			m_pServiceOptions->Write();
			if (m_Service.ServiceInstall() == FALSE)
			{
				DisplayError("Install service", m_Service.GetError());
			}

			UpdateInstallOptions();
		}
	}
}

//$$******************************************************************

void CServiceInstallDlg::OnButtonRunning()
{
	if (TRUE == m_bServiceRunning)
	{
		if (m_Service.ServiceStop() == FALSE)
		{
			DisplayError("Stop service", m_Service.GetError());
		}

		UpdateInstallOptions();
	}
	else
	{
		if (m_Service.ServiceStart() == FALSE)
		{
			DisplayError("Start service", m_Service.GetError());
		}

		UpdateInstallOptions();
	}
}

//$$******************************************************************

void CServiceInstallDlg::DisplayError(CString strAction, CString strError)
{
	CString strMsg = "";
	strMsg += strAction;
	strMsg += " failed.\n\n";
	strMsg += strError;
	Prompter.Error(strMsg);
}

//$$******************************************************************

void CServiceInstallDlg::UpdateActiveTime(bool bForce)
{
	CString strInfo = "";
	CString strTime = "";

	if (FALSE == m_bShowError)
	{
		if ( m_TrackedFileActive.CheckFile(m_bFirstCheckActive) == TRUE )
		{
			CSSFile fileInfo;
			if (fileInfo.Open(m_TrackedFileActive.GetFilename(), "rb") == TRUE)
			{
				CString strBuffer;
				fileInfo.ReadString(strBuffer);
				fileInfo.Close();

				CCSV csv(strBuffer);
				m_strInfoActive = csv.GetString(0);
				m_strTimeActive = csv.GetString(1);
			}
		}

		strInfo = m_strInfoActive;
		strTime = m_strTimeActive;
	}
	else
	{
		if ( m_TrackedFileError.CheckFile(m_bFirstCheckError) == TRUE )
		{
			CSSFile fileInfo;
			if (fileInfo.Open(m_TrackedFileError.GetFilename(), "rb") == TRUE)
			{
				CString strBuffer;
				fileInfo.ReadString(strBuffer);
				fileInfo.Close();

				CCSV csv(strBuffer);
				m_strInfoError = csv.GetString(0);
				m_strTimeError = csv.GetString(1);
			}
		}

		strInfo = m_strInfoError;
		strTime = m_strTimeError;
	}

	if ((strInfo.GetLength() >= 10) && (strTime.GetLength() == 14))
	{
		m_editActiveID.SetWindowText(strInfo);

		CString strDisplayTime;
		strDisplayTime.Format("%s/%s/%s  %s:%s:%s",
			(const char*)strTime.Mid(6, 2),
			(const char*)strTime.Mid(4, 2),
			(const char*)strTime.Left(4),
			(const char*)strTime.Mid(8, 2),
			(const char*)strTime.Mid(10, 2),
			(const char*)strTime.Right(2));

		m_editActiveTime.SetWindowText(strDisplayTime);
	}
	else if (TRUE == bForce)
	{
		m_editActiveID.SetWindowText("");
		m_editActiveTime.SetWindowText("");
	}
}

//$$******************************************************************

void CServiceInstallDlg::OnToggleActivity()
{
	if (FALSE == m_bShowError)
	{
		m_bShowError = TRUE;
		m_staticActivity.SetWindowText("Last Error");
	}
	else
	{
		m_bShowError = FALSE;
		m_staticActivity.SetWindowText("Last Activity");
	}

	UpdateActiveTime(TRUE);
}

//$$******************************************************************

void CServiceInstallDlg::OnToggleTime()
{
	if (FALSE == m_bShowError)
	{
		m_bFirstCheckActive = TRUE;
	}
	else
	{
		m_bFirstCheckError = TRUE;
	}

	UpdateActiveTime(FALSE);
}

//$$******************************************************************

void CServiceInstallDlg::OnTimer(UINT nIDEvent)
{
	if (FALSE == m_bInTimer)
	{
		if (1 == nIDEvent)
		{
			m_bInTimer = TRUE;
			UpdateInstallOptions();
			UpdateActiveTime();
			m_bInTimer = FALSE;
		}
	}
}

//$$******************************************************************

void CServiceInstallDlg::OnDestroy()
{
	if (m_hTimer != NULL)
		KillTimer(m_hTimer);

	CSSDialog::OnDestroy();
}

//$$******************************************************************
