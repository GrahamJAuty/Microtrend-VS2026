//$$******************************************************************
#include "pch.h"
//$$******************************************************************
#include "MessageLogger.h"
//$$******************************************************************
#include "SPosLoyalty4ServerIcon.h"
//$$******************************************************************
#include "..\SPosLoyalty4ServerIcon\Resource.h"
//$$******************************************************************
#include "AboutDlg.h"
#include "Framework.h"
#include "RepmanHandler.h"
#include "ServiceInstallDlg.h"
#include "SPosLoyalty4ServerIcon.h"
#include "SPosLoyalty4ServerIconDlg.h"
//$$******************************************************************
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//$$******************************************************************

CSPosLoyalty4ServerIconDlg::CSPosLoyalty4ServerIconDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SPOSLOYALTY3SERVERICON_DIALOG, pParent),
	m_trayIcon(IDR_MAINFRAME)
{
	m_bModalBusy = FALSE;
	m_bInTimer = FALSE;
	m_hTimer = NULL;
	m_bGotActiveFileTime = FALSE;
	m_bGotPCId = FALSE;
	m_strRunOnceTimeStamp = "";
	m_bWantRunOnceQuit = FALSE;
	m_TrackedFileActive.SetFilename(Filenames.GetServerServiceActionTimeFilename());
	m_bYellowGreenFlash = FALSE;
	m_bGreenTime = FALSE;
}

//$$******************************************************************

void CSPosLoyalty4ServerIconDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CSPosLoyalty4ServerIconDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_MY_TRAY_NOTIFICATION, OnTrayNotification)
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_COMMAND(IDM_STATUS, OnStatus)
	ON_COMMAND(IDM_VIEWLOG, OnViewLog)
	ON_COMMAND(ID_CLOSE, OnClose)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CSPosLoyalty4ServerIconDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (TRUE == m_bGotPCId)
	{
		m_TrackedFileRunOnce.SetFilename(GetRunOnceFilename());
	}

	m_trayIcon.CreateIconCache();
	m_trayIcon.SetNotificationWnd(this, WM_MY_TRAY_NOTIFICATION);
	m_trayIcon.SetYellowIcon();

	ShowWindow(SW_HIDE);
	MoveWindow(0, 0, 1, 1, TRUE);

	m_timeActiveFileModified = CTime::GetCurrentTime() - CTimeSpan(0, 0, 0, 5);

	m_hTimer = SetTimer(1, 2000, 0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

//$$******************************************************************

void CSPosLoyalty4ServerIconDlg::OnPaint()
{
	ShowWindow(SW_HIDE);
	return;
}

//$$******************************************************************

LRESULT CSPosLoyalty4ServerIconDlg::OnTrayNotification(WPARAM uID, LPARAM lEvent)
{
	return m_trayIcon.OnTrayNotification(uID, lEvent);
}

//$$******************************************************************

void CSPosLoyalty4ServerIconDlg::OnTimer(UINT nIDEvent)
{
	if (FALSE == m_bInTimer)
	{
		if (1 == nIDEvent)
		{
			m_bInTimer = TRUE;
			HandleTimer1();
			m_bInTimer = FALSE;
		}
	}
}

//$$******************************************************************

void CSPosLoyalty4ServerIconDlg::HandleTimer1()
{
	if (FALSE == m_bGotPCId)
	{
		bool bCreatePCId = FALSE;
		Sysset.SetPCId(bCreatePCId);
		m_bGotPCId = bCreatePCId;

		if (TRUE == m_bGotPCId)
		{
			m_strRunOnceTimeStamp = ::CreateRunOnceTimeStamp();
			m_TrackedFileRunOnce.SetFilename(GetRunOnceFilename());
		}
	}

	if ((m_strRunOnceTimeStamp != "") && (FALSE == m_bWantRunOnceQuit))
	{
		if (m_TrackedFileRunOnce.CheckFile(FALSE) == TRUE)
		{
			CString strBuffer = "";

			{
				CSSFile fileCheck;
				if (fileCheck.Open(GetRunOnceFilename(), "rb") == TRUE)
				{
					fileCheck.ReadString(strBuffer);
				}
			}

			if ((strBuffer.GetLength() == m_strRunOnceTimeStamp.GetLength()) && (strBuffer != m_strRunOnceTimeStamp))
			{
				m_bWantRunOnceQuit = TRUE;
			}
		}
	}

	if (m_TrackedFileActive.CheckFile(FALSE) == TRUE)
	{
		if (TRUE == m_bGotActiveFileTime)
		{
			m_bYellowGreenFlash = FALSE;

			CSSFile fileInfo;
			if (fileInfo.Open(m_TrackedFileActive.GetFilename(), "rb") == TRUE)
			{
				CString strBuffer = "";
				fileInfo.ReadString(strBuffer);
				fileInfo.Close();

				CCSV csv(strBuffer);
				m_bYellowGreenFlash = csv.GetBool(2);
			}

			m_trayIcon.SetGreenIcon();

			m_timeActiveFileModified = CTime::GetCurrentTime();
		}

		m_bGotActiveFileTime = TRUE;
	}
	else
	{
		CTime timeNow = CTime::GetCurrentTime();

		if (m_timeActiveFileModified + CTimeSpan(0, 0, 0, 20) < timeNow)
		{
			m_trayIcon.SetRedIcon();
			m_bYellowGreenFlash = FALSE;
		}
		else if (m_timeActiveFileModified + CTimeSpan(0, 0, 0, 10) < timeNow)
		{
			m_trayIcon.SetYellowIcon();
			m_bYellowGreenFlash = FALSE;
		}
	}

	if (TRUE == m_bYellowGreenFlash)
	{
		if (TRUE == m_bGreenTime)
		{
			m_trayIcon.SetGreenIcon();
		}
		else
		{
			m_trayIcon.SetYellowIcon();
		}

		m_bGreenTime = (FALSE == m_bGreenTime);
	}

	if ((TRUE == m_bWantRunOnceQuit) && (FALSE == m_bModalBusy))
	{
		EndDialog(IDOK);
	}

	if ((TRUE == theApp.m_bFailIconChange) && (GetTickCount64() > theApp.m_AppStartTime + 30000) && (FALSE == m_bModalBusy) )
	{
		MessageLogger.LogSystemMessage("Restarting Tray Icon");

		CCommandLineBuilder CommandLineBuilder;
		CommandLineBuilder.AddToken(".\\SPosLoyalty4ServerIcon.EXE");
		CString strCommandLine = CommandLineBuilder.GetLine();

		LPTSTR cmd = strCommandLine.GetBuffer(strCommandLine.GetLength() + 1);

		STARTUPINFO startup;
		PROCESS_INFORMATION procinfo;

		::ZeroMemory(&startup, sizeof(startup));
		::ZeroMemory(&procinfo, sizeof(procinfo));

		if (CreateProcess(NULL, cmd, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startup, &procinfo) == TRUE)
		{
			CloseHandle(procinfo.hProcess);
			CloseHandle(procinfo.hThread);
		}
		else
		{
			errno = ENOENT;
		}

		EndDialog(IDOK);
	}
}

//$$******************************************************************

void CSPosLoyalty4ServerIconDlg::OnStatus()
{
	if (FALSE == m_bModalBusy)
	{
		m_bModalBusy = TRUE;
		
		CSSFile fileLock;
		if (fileLock.Open(Filenames.GetServerServiceOptionsLockFilename(), "wb") == FALSE)
		{
			Prompter.Error("These options are being modified by another user.\n\nPlease try again later.");
		}
		else
		{
			ServerServiceOptions.Read();
			CServiceInstallDlg dlg(this);
			dlg.DoModal();
			ServerServiceOptions.Write();
		}

		m_bModalBusy = FALSE;
	}
}

//$$******************************************************************

void CSPosLoyalty4ServerIconDlg::OnViewLog()
{
	CRepmanHandler RepmanHandler("Server Service Log", this);
	RepmanHandler.DisplayReport(Filenames.GetServerLogFilename(), Filenames.GetServerLogParamsFilename(), "");
}

//$$******************************************************************

void CSPosLoyalty4ServerIconDlg::OnAbout()
{
	if (FALSE == m_bModalBusy)
	{
		m_bModalBusy = TRUE;
		CAboutDlg dlg;
		dlg.DoModal();
		m_bModalBusy = FALSE;
	}
}

//$$******************************************************************

void CSPosLoyalty4ServerIconDlg::OnClose()
{
	if (FALSE == m_bModalBusy)
	{
		EndDialog(IDCANCEL);
	}
}

//$$******************************************************************

void CSPosLoyalty4ServerIconDlg::OnDestroy()
{
	if (m_hTimer != NULL)
	{
		KillTimer(m_hTimer);
	}

	CDialogEx::OnDestroy();
}

//$$******************************************************************