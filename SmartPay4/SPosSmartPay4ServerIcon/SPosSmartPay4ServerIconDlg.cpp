//$$******************************************************************
#include "pch.h"
//$$******************************************************************
#include "AboutDlg.h"
#include "Framework.h"
#include "Resource.h"
#include "MessageLogger.h"
#include "RepmanHandler.h"
#include "ServiceInstallDlg.h"
#include "SPosSmartPay4ServerIcon.h"
#include "SPosSmartPay4ServerIconDlg.h"
//$$******************************************************************
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//$$******************************************************************

CSPosSmartPay4ServerIconDlg::CSPosSmartPay4ServerIconDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SPOSSMARTPAY4SERVERICON_DIALOG, pParent),
	m_trayIcon(IDR_MAINFRAME)
{
#ifndef BGNDICON
	m_TrackedFileActive.SetFilename(Filenames.GetServerServiceActionTimeFilename());
#else
	m_TrackedFileActive.SetFilename(Filenames.GetBgndServiceActionTimeFilename());
	m_TrackedFileBusy.SetFilename(Filenames.GetBgndServiceBusyTimeFilename());
#endif
}

//$$******************************************************************

void CSPosSmartPay4ServerIconDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CSPosSmartPay4ServerIconDlg, CDialogEx)
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

BOOL CSPosSmartPay4ServerIconDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (TRUE == m_bGotPCId)
	{
		m_TrackedFileRunOnce.SetFilename(GetRunOnceFilename());
	}

	if (m_trayIcon.CreateIconCache() == FALSE)
	{
		EndDialog(IDOK);
		return FALSE;
	}

	m_trayIcon.SetNotificationWnd(this, WM_MY_TRAY_NOTIFICATION);
	m_trayIcon.SetYellowIcon();
	
	ShowWindow(SW_HIDE);
	MoveWindow(0, 0, 1, 1, TRUE);

	m_timeActiveFileModified = CTime::GetCurrentTime() - CTimeSpan(0, 0, 0, 5);

#ifdef BGNDICON
	m_timeBusyFileModified = m_timeActiveFileModified;
#endif

	m_hTimer = SetTimer(1, 2000, 0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

//$$******************************************************************

void CSPosSmartPay4ServerIconDlg::OnPaint()
{
	ShowWindow(SW_HIDE);
	return;
}

//$$******************************************************************

LRESULT CSPosSmartPay4ServerIconDlg::OnTrayNotification(WPARAM uID, LPARAM lEvent)
{
	return m_trayIcon.OnTrayNotification(uID, lEvent);
}

//$$******************************************************************

void CSPosSmartPay4ServerIconDlg::OnTimer(UINT nIDEvent)
{
	if (FALSE == m_bInTimer)
	{
		if (1 == nIDEvent)
		{
			m_bInTimer = TRUE;

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

			if ( (m_strRunOnceTimeStamp != "") && ( FALSE == m_bWantRunOnceQuit ) )
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
					m_bRedGreenFlash = FALSE;

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

				bool bCheckBusy = FALSE;
				if (m_timeActiveFileModified + CTimeSpan(0, 0, 0, 20) < timeNow)
				{
					m_trayIcon.SetRedIcon();
					m_bYellowGreenFlash = FALSE;
					m_bRedGreenFlash = FALSE;
					bCheckBusy = TRUE;
				}
				else if (m_timeActiveFileModified + CTimeSpan(0, 0, 0, 10) < timeNow)
				{
					m_trayIcon.SetYellowIcon();
					m_bYellowGreenFlash = FALSE;
					m_bRedGreenFlash = FALSE;
					bCheckBusy = TRUE;
				}

#ifdef BGNDICON

				if (TRUE == bCheckBusy)
				{
					if (m_TrackedFileBusy.CheckFile(FALSE) == TRUE)
					{
						if (TRUE == m_bGotBusyFileTime)
						{
							m_timeBusyFileModified = CTime::GetCurrentTime();
						}

						m_bGotBusyFileTime = TRUE;
					}

					if (m_timeBusyFileModified + CTimeSpan(0, 0, 0, 10) >= timeNow)
					{
						m_bRedGreenFlash = TRUE;
					}
				}
#endif
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
			else if ( TRUE == m_bRedGreenFlash)
			{
				if (TRUE == m_bGreenTime)
				{
					m_trayIcon.SetGreenIcon();
				}
				else
				{
					m_trayIcon.SetRedIcon();
				}

				m_bGreenTime = (FALSE == m_bGreenTime);
			}

			m_bInTimer = FALSE;
		}
	}

	if ((TRUE == m_bWantRunOnceQuit) && (FALSE == m_bModalBusy))
	{
		EndDialog(IDOK);
	}

	if ((TRUE == theApp.m_bFailIconChange) && (GetTickCount64() > theApp.m_AppStartTime + 30000) && (FALSE == m_bModalBusy))
	{
#ifdef BGNDICON
		MessageLogger.LogSystemMessage("Restarting Background Tray Icon");
#else
		MessageLogger.LogSystemMessage("Restarting Server Tray Icon");
#endif

		CCommandLineBuilder CommandLineBuilder;
		
#ifdef BGNDICON
		CommandLineBuilder.AddToken(".\\SPosSmartPay4BgndIcon.EXE");
#else
		CommandLineBuilder.AddToken(".\\SPosSmartPay4ServerIcon.EXE");
#endif
		
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

void CSPosSmartPay4ServerIconDlg::OnStatus()
{
	if (FALSE == m_bModalBusy)
	{
		m_bModalBusy = TRUE;

#ifndef BGNDICON

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

#else

		CSSFile fileLock;
		if (fileLock.Open(Filenames.GetBgndServiceOptionsLockFilename(), "wb") == FALSE)
		{
			Prompter.Error("These options are being modified by another user.\n\nPlease try again later.");
		}
		else
		{
			BgndServiceOptions.Read();
			CServiceInstallDlg dlg(this);
			dlg.DoModal();
			BgndServiceOptions.Write();
		}

#endif

		m_bModalBusy = FALSE;
	}
}

//$$******************************************************************

void CSPosSmartPay4ServerIconDlg::OnViewLog()
{
#ifndef BGNDICON
	CRepmanHandler RepmanHandler("Server Service Log", this);
	RepmanHandler.DisplayReport(Filenames.GetServerLogFilename(), Filenames.GetServerLogParamsFilename(), "");
#else
	CRepmanHandler RepmanHandler("Background Service Log", this);
	RepmanHandler.DisplayReport(Filenames.GetBackgroundLogFilename(), Filenames.GetBackgroundLogParamsFilename(), "");
#endif
}

//$$******************************************************************

void CSPosSmartPay4ServerIconDlg::OnAbout()
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

void CSPosSmartPay4ServerIconDlg::OnClose()
{
	if (FALSE == m_bModalBusy)
	{
		EndDialog(IDCANCEL);
	}
}

//$$******************************************************************

void CSPosSmartPay4ServerIconDlg::OnDestroy()
{
	if (m_hTimer != NULL)
	{
		KillTimer(m_hTimer);
	}

	CDialogEx::OnDestroy();
}

//$$******************************************************************