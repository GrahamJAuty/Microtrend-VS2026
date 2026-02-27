//****************************************************************************
#include "AboutDlg.h"
#include "AdminDatabase.h"
#include "CommandFile.h"
#include "GetAdminDlg.h"
#include "GetEnrolmentDetails.h"
#include "GetUserDlg.h"
#include "GetUserDualDlg.h"
#include "GlobalFunctions.h"
#include "Imagehandler.h"
#include "MyCommandLineInfo.h"
#include "PasswordData.h"
#include "PayLinkAuditRecord.h"
#include "PhotoIDEnquiryDlg.h"
#include "PostTransaction.h"
#include "ScannerDlg.h"
#include "SelectionDlg.h"
#include "SelectPurseDlg.h"
#include "SetupDlg.h"
#include "SetupReaderDlg.h"
#include "status.h"
#include "SystemData.h"
#include "TerminalData.h"
#include "WaitDlg.h"
#include "WaitGetUserDlg.h"
#include "WaitPostTransactionDlg.h"
#include "WaitStatusDlg.h"
#include "YesNoDlg.h"
//****************************************************************************
#include "CardLink.h"
//****************************************************************************
const char* szMSG_ERROR1	= "Account not accessible via manual entry";
//****************************************************************************
BEGIN_MESSAGE_MAP(CCardLinkApp, CWinApp)
END_MESSAGE_MAP()
//****************************************************************************
extern const char* szVERSION_CARDLINK_ABOUT;
extern const char* szUPDATE_TOKEN_DATE;
//****************************************************************************

CCardLinkApp::CCardLinkApp()
{
}

//***************************************************************************
CCardLinkApp theApp;
CSysset	Sysset;
CSystemData	System;
//***************************************************************************

BOOL CCardLinkApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	//INITCOMMONCONTROLSEX InitCtrls;
	//InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	//InitCtrls.dwICC = ICC_WIN95_CLASSES;
	//InitCommonControlsEx(&InitCtrls);

	// Initialize Common Controls early to prevent WIL exceptions
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_WIN95_CLASSES     // Standard controls
		| ICC_TAB_CLASSES       // Tab controls
		| ICC_LISTVIEW_CLASSES  // List view controls
		| ICC_TREEVIEW_CLASSES  // Tree view controls
		| ICC_BAR_CLASSES       // Toolbar, status bar
		| ICC_PROGRESS_CLASS    // Progress bars
		| ICC_HOTKEY_CLASS      // Hot key controls
		| ICC_ANIMATE_CLASS     // Animation controls
		| ICC_USEREX_CLASSES;   // Extended controls

	if (!InitCommonControlsEx(&icex))
	{
		TRACE("Failed to initialize Common Controls\n");
		// Continue anyway - fallback to older initialization
		InitCommonControls();
	}

	CWinApp::InitInstance();

	CString strStartupCommand = "";

	{
		CMyCommandLineInfo cmdInfo;
		ParseCommandLine(cmdInfo);
		if (cmdInfo.GetSize() != 0)
		{
			strStartupCommand = cmdInfo.GetAt(0);
			strStartupCommand.MakeUpper();
		}
	}

	if (strStartupCommand == "VERSIONNO")
	{
		exit(Sysset.ConvertStringVersionToInt(szVERSION_CARDLINK_ABOUT));
		return FALSE;
	}

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Sysset test

	bool bCanCreatePCID = TRUE;
	if (Sysset.TestRegistrationMainExe(SYSSET_KIOSKBIOLINK, NULL, bCanCreatePCID, szVERSION_CARDLINK_ABOUT, szUPDATE_TOKEN_DATE) == FALSE)
	{	
		CString strMess = "";
		strMess.Format("%s\n\nPlease contact your software supplier %s",
			(const char*)Sysset.GetError(),
			(const char*)Sysset.GetField(4));

		AfxMessageBox(strMess, MB_ICONSTOP | MB_OK);
		return FALSE;
	}

	//delay to allow other programs to catch up if running from startup
	int nDelay = 0;

	{
		int nLength = strStartupCommand.GetLength();
		if ((nLength >= 7) && (nLength <= 9))
		{
			if (strStartupCommand.Left(6) == "DELAY=")
			{
				nDelay = atoi(strStartupCommand.Mid(6));
			}
		}
	}

	if ((nDelay < 0) || (nDelay > 120))
	{
		nDelay = 0;
	}
		
	if (nDelay != 0)
	{
		LONGLONG dwStartTime = GetTickCount64();
		LONGLONG dwTimeNow = dwStartTime;
		LONGLONG dwEndTime = dwStartTime + (1000 * nDelay);

		while ((dwTimeNow >= dwStartTime) && (dwTimeNow < dwEndTime))
		{
			Sleep(500);
			dwTimeNow = GetTickCount64();
		}
	}

	// begin
	// must do read to setup filenames etc
	bool bSetupReqd = (System.Read() == FALSE);		
	
	CCommandFile command;
	if (command.Found() == TRUE)							// set if a command file exists on start
	{
		bSetupReqd = TRUE;
	}

#ifdef _DEBUG
	bSetupReqd = TRUE;
#endif

	// set main screen

	bool bExitProgram = FALSE;

	CString strFilename = "";
	CImageHandler image(System.m_strSysFilesImagePath, "MainScreen");
	if (image.GetFirst() == TRUE)							// get first filename name
	{
		strFilename = image.GetFilepath();
	}

	CSSFullScreenWnd* pScreen = new CSSFullScreenWnd();	// dont call delete (called automatically)
	if (pScreen->Open("CardLink", strFilename, BLACK) == TRUE)
	{
		if (bSetupReqd == TRUE)
		{
			CSetupDlg dlg(&System, pScreen);
			if (dlg.DoModal() == IDOK)
			{
				System.Write();
			}
			else
			{
				bExitProgram = TRUE;
			}
		}

		// see if Reader \ server online

		if (FALSE == bExitProgram)
		{
			switch (System.m_nInterfaceType)
			{
			case nINTERFACE_BIOMETRIC:
			case nINTERFACE_PINPAD:
				break;

			default:
			{
				CTerminalData terminal;
				terminal.Read();

				if (terminal.m_strTerminal == "NONE")
				{
					CSetupReaderDlg dlg(pScreen);
					if (dlg.DoModal() == IDCANCEL)
					{
						bExitProgram = TRUE;
					}
				}
			}
			break;
			}

			if (FALSE == bExitProgram)
			{
				CStatus status(System.m_nTerminalNo);
				bool bIDReaderOffline = FALSE;

				switch (System.m_nInterfaceType)
				{
				case nINTERFACE_BIOMETRIC:
				case nINTERFACE_DUAL:
				{
					if (System.AutoRefreshBiometrics(10, pScreen) == TRUE)				// see if reader is on line
					{
						CWaitDlg waitDlg("Verifing Kiosk Server", "Please wait", pScreen);
						waitDlg.PumpMessages();
						Sleep(1000);											// slow things down
					}
					else
					{
						::DisplayMessage(pScreen, "Biometric reader is offline", "Hardware Failure", MB_ICONSTOP);

						if (nINTERFACE_BIOMETRIC == System.m_nInterfaceType)	// could still use MiFare if dual system
						{
							bIDReaderOffline = TRUE;
						}
					}
				}
				break;
				}

				MainProgram(pScreen, bIDReaderOffline);

				/*
				CWaitDlg waitDlg("Closing down", "Please wait", pScreen);
				waitDlg.PumpMessages();
				Sleep(1000);
				*/
			}
		}

		pScreen->Close();
	}

	return FALSE;
}

//******************************************************************
// Mian Program loop

void CCardLinkApp::MainProgram(CSSFullScreenWnd* pScreen, bool bIDReaderOffline)
{
	CUserIDRecord UserIDRecord;
	bool bKioskInService = TRUE;					// assume kiosk in service
	bool bHaveInputFile = FALSE;					// set if input file found during previous PhotoID screen
	int nServerStatus = nSTATUS_UNKNOWN;

	while (TRUE)
	{
		bool bAdminAllowed = FALSE;
		bool bTopupAllowed = FALSE;
		bool bUntrustedManualInput = FALSE;
		bool bCommandFile = FALSE;
		
		//** see if any pending transaction still exist - would get created if power lost before transaction finalised

		if (::FileExists(System.m_strPayLinkTTLFilename) == TRUE)
		{
			CPostTransaction trans;
			if (trans.AddToOffline() == FALSE)
			{
				CSSFile file;
				CPayLinkAuditRecord	audit;

				file.Open(System.m_strPayLinkTTLFilename, "rb");
				audit.ReadLine(&file);
				file.Close();

				CString strMsg = "";
				strMsg.Format("System Halted\n\nA Purse%d topup of £%.2lf for %s ( %s ) was never finalised",
					audit.GetTopupPurse(),
					audit.ValueAdded(),
					(const char*)audit.UserName(),
					(const char*)audit.UserID());

				AfxMessageBox(strMsg);
				return;
			}
		}

		// see if have offline file to send

		if (::FileExists(System.m_strOfflineLoyLogFilename) == TRUE)
		{
			CWaitPostTransactionDlg dlgPost(nTRANPOST_ACTION_SENDOFFLINE, pScreen);
			dlgPost.DoModal();
		}

		// Get UserID

		CString strMiFareNo = "";
		bool bAllowTimeoutCancel = FALSE;
		int nInterface = System.m_nInterfaceType;

		if (nINTERFACE_DUAL == nInterface)
		{
			CGetUserDualDlg dlg(pScreen);
			if (dlg.DoModal() == IDCANCEL)
			{
				break;
			}

			nInterface = dlg.GetInterfaceType();
			bHaveInputFile = dlg.HaveInputFile();
			bAllowTimeoutCancel = TRUE;
		}

		int nGetUserType = nGETUSER_PINPAD_ONLY;
		switch (nInterface)
		{
		case nINTERFACE_MIFAREv1:
		case nINTERFACE_MIFAREv2:
			if ( System.GetEnableKeypadEntryFlag() == TRUE )
			{
				nGetUserType = nGETUSER_MIFARE_PINPAD;
			}
			else
			{
				nGetUserType = nGETUSER_MIFARE_ONLY;
			}
			break;

		case nINTERFACE_BIOMETRIC:
			if ( System.GetEnableKeypadEntryFlag() == TRUE )
			{
				nGetUserType = nGETUSER_BIO_PINPAD;
			}
			else
			{
				nGetUserType = nGETUSER_BIO_ONLY;
			}
			break;

		case nINTERFACE_PINPAD:
		default:
			nGetUserType = nGETUSER_PINPAD_ONLY;
			break;
		}

		{
			CGetUserDlg dlg(nGetUserType, bIDReaderOffline, bHaveInputFile, pScreen);
			dlg.AllowTimeoutCancel(bAllowTimeoutCancel);
			int nReply = dlg.DoModal();							// wait for file to appear

			bCommandFile = dlg.m_bCommandFileFound;				// see if command file found
			bKioskInService = dlg.m_bKioskInService;			// see if kiosk in service
			nServerStatus = dlg.m_nServerStatus;				// get server status ( offline \ online \ Disabled )
			bUntrustedManualInput = dlg.m_bManualInput;			// see if account numver entered manually
			strMiFareNo = dlg.m_strMiFareNo;					// get MiFare number if used			

			if (IDCANCEL == nReply)
			{
				continue;
			}
		}

		//SERVER STATUS MAY BE UNKNOWN COMING OUT OF GETUSERDLG
		if ((TRUE == bKioskInService) && (nSTATUS_UNKNOWN == nServerStatus))
		{
			CStatus status(System.m_nTerminalNo, System.m_strUserIDFilename);
			CWaitStatusDlg waitDlg(status, nWAITSTATUS_ACTION_CHECKSERVERONLINE, TRUE, pScreen);
			waitDlg.DoModal();
			nServerStatus = waitDlg.GetIntResult();
		}

		bHaveInputFile = FALSE;							// clear flag
		CString strError = "";

		if (TRUE == bCommandFile)							// see if command file found
		{
			bAdminAllowed = TRUE;							// adnin menu alllowed
		}
		else
		{
			CStringArray arrayUserLines;;

			bool bServerValidate = ((TRUE == bKioskInService) && (nSTATUS_ONLINE == nServerStatus));

			if (TRUE == bUntrustedManualInput)
			{
				bServerValidate = TRUE;
			}

			CWaitGetUserDlg dlgGetUser(System.m_strUserIDFilename, bServerValidate, arrayUserLines, UserIDRecord, pScreen);
			dlgGetUser.DoModal();
			bTopupAllowed = dlgGetUser.GetBoolResult();

			if (TRUE == bTopupAllowed)
			{
				switch (arrayUserLines.GetSize())
				{
				case 0:
					bTopupAllowed = FALSE;
					break;

				case 1:
					bTopupAllowed = UserIDRecord.ExtractUserData(arrayUserLines.GetAt(0));
					strError = UserIDRecord.GetError();
					break;

				default:
				{
					CSelectionDlg dlg(&arrayUserLines, &UserIDRecord, pScreen);
					bTopupAllowed = (dlg.DoModal() == IDOK);
				}
				break;
				}
			}

			if ((FALSE == UserIDRecord.m_bRecordExists) && (strMiFareNo != ""))	// no record found
			{
				if ((nINTERFACE_MIFAREv2 == System.m_nInterfaceType) && (TRUE == System.GetEnrolmentAllowedFlag()))
				{
					if ((TRUE == bKioskInService) && (nSTATUS_ONLINE == nServerStatus))
					{
						bTopupAllowed = EnrolCard(pScreen, strMiFareNo, &UserIDRecord, strError);
					}
				}
			}

			// SmartPay Server offline / Kiosk Out of Service
			if (FALSE == bTopupAllowed)								
			{
				// check user against admin database
				// Note: Only UserID is set when SmartServer not running
				CAdminDatabase db;										
				if (db.Exists(UserIDRecord.GetUserID()) == TRUE)		
				{
					if (TRUE == bUntrustedManualInput)
					{
						if (System.m_nInterfaceType == nINTERFACE_PINPAD)
						{
							if (::GetAdminPassword(pScreen) == TRUE)
							{
								bUntrustedManualInput = FALSE;
							}
						}
					}

					if (FALSE == bUntrustedManualInput)
					{
						bAdminAllowed = TRUE;							// offline Admin functions allowed
					}
					else
					{
						::DisplayMessage(pScreen, szMSG_ERROR1, "Access Denied", MB_ICONSTOP);
					}
				}
				else													// not found - so error
				{
					if (TRUE == bKioskInService)
					{
						int nMessageNo = UserIDRecord.m_nMessageNo;
						::PlayMessage(nMessageNo);

						if (nMessageNo == nMESSAGE_NOSERVERRESPONSE)
						{
							::DisplayMessage(pScreen, "Please try later", "No response from Server", 0);
						}

						else if (strError != "")
						{
							::DisplayMessage(pScreen, strError, "Server error reported", 0);
						}
					}
				}
			}
			else					// Topup is allowed - Server must be online
			{
				if (strError != "")							// see if any warning set
				{
					::DisplayMessage(pScreen, strError, "Warning reported by Server", MB_ICONEXCLAMATION);
				}

				if (UserIDRecord.m_nGroupNo == nGROUP_ADMIN)
				{
					// update admin user database
					CAdminDatabase db;
					db.Update(UserIDRecord.GetUserID());			
				
					if (TRUE == bUntrustedManualInput)
					{
						if (nINTERFACE_PINPAD == System.m_nInterfaceType)
						{
							if (::GetAdminPassword(pScreen) == TRUE)
							{
								bUntrustedManualInput = FALSE;
							}
						}
					}

					if (TRUE == bUntrustedManualInput)					// check for Admin user
					{
						bAdminAllowed = FALSE;					// no Admin functions allowed
						bTopupAllowed = FALSE;					// no topup allowed

						if (strError == "")					// check no other error
						{
							::DisplayMessage(pScreen, szMSG_ERROR1, "Access Denied", MB_ICONSTOP);
						}
					}
					else
					{
						bAdminAllowed = TRUE;					// Admin functions allowed
					}
				}
			}
		}

		if (TRUE == bAdminAllowed)						// display admin menu option selection
		{
			bool bOnline = (nServerStatus == nSTATUS_ONLINE);

			if (TRUE == bCommandFile)
			{
				bOnline = FALSE;
			}

			bool bWantTerminate = FALSE;

			CGetAdminDlg dlg(&UserIDRecord, bOnline, pScreen);
			dlg.DoModal();

			switch (dlg.GetClosingState())
			{
			case nGETADMIN_WANTSETUP:
				{
					bTopupAllowed = FALSE;
					CAboutDlg dlgAbout(pScreen);
					dlgAbout.DoModal();
					bWantTerminate = dlgAbout.m_bTerminateReqd;
				}
				break;

			case nGETADMIN_WANTTOPUP:
				if (FALSE == bTopupAllowed)
				{
					if (strError != "")							// see if any warning set
					{
						::DisplayMessage(pScreen, strError, "Server error reported", MB_ICONEXCLAMATION);
					}
				}
				break;

			case nGETADMIN_CANCEL:
			default:
				bTopupAllowed = FALSE;
				break;
			}
			
			if (TRUE == bWantTerminate)
			{
				break;
			}
		}

		if (TRUE == bTopupAllowed)
		{
			bool bAllowMultiSelection = System.HaveMultiplePurses();

			while (TRUE)
			{
				int nTopupPurse = 0;										// 1 or 3
				if (TRUE == bAllowMultiSelection)
				{
					CSelectPurseDlg selectPurse(pScreen);
					if (selectPurse.DoModal() == IDOK)
					{
						nTopupPurse = selectPurse.m_nTopupPurse;
					}
				}
				else
				{
					nTopupPurse = (System.GetTopupPurse3Flag() == TRUE) ? 3 : 1;
				}

				if (nTopupPurse != 0)
				{
					CPhotoIDEnquiryDlg photo(&UserIDRecord, nTopupPurse, pScreen);
					photo.DoModal();
					bHaveInputFile = photo.m_bHaveInputFile;				// see if exited because of new input

					if (bHaveInputFile == TRUE)
					{
						break;
					}

					if (FALSE == bAllowMultiSelection)
					{
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
	}

	switch (nServerStatus)
	{
	case nSTATUS_OFFLINE:
	case nSTATUS_UNKNOWN:
		break;

	default:
	{
		CStatus status(System.m_nTerminalNo);
		CWaitStatusDlg dlg3(status, nWAITSTATUS_ACTION_SETSTATUSOFF, TRUE, pScreen);
		dlg3.DoModal();
	}
	break;
	}
}

//*******************************************************************

bool CCardLinkApp::EnrolCard(CSSFullScreenWnd* pScreen, const char* szMiFareNo, CUserIDRecord* pUserRecord, CString& strError)
{
	bool bTopupAllowed = FALSE;
	strError = "";

	CString strPrompt = "";
	strPrompt.Format("Unregistered %s - register now?",
		(const char*)System.m_strCardNoText);

	CYesNoDlg dlg(strPrompt, pScreen);

	if (dlg.DoModal() == IDOK)
	{
		if (IDYES == dlg.m_nReply)
		{
			CScannerDlg dlgScanner(pScreen);
			if (dlgScanner.DoModal() == IDOK)
			{
				CGetEnrolmentDetails enrolDetails(szMiFareNo, dlgScanner.m_strKbInput, pUserRecord, pScreen);

				bTopupAllowed = enrolDetails.Extract();
				strError = enrolDetails.m_strErrorMessage;		// get any error message
			}
		}
	}

	return bTopupAllowed;
}

//*******************************************************************

