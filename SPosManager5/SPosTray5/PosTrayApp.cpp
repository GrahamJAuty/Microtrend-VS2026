/**********************************************************************/
#include "..\CommonEcrStockTray\AutoBackupOptions.h"
#include "..\CommonEcrStockTray\AutoBackupProcessor.h"
#include "..\CommonEcrStockTray\PluTypeHelpers.h"
/**********************************************************************/
#include "..\SmartPaySQL\FilenameHandler.h"
#include "..\SmartPaySQL\SQLConnectionOptions.h"
#include "..\SmartPaySQL\SQLConnectionPool.h"
/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "CashRSPImporter.h"
#include "CommsHandshaker.h"
#include "DatabaseCSVArray.h"
#include "EmailOptions.h"
#include "FontTable.h"
#include "NetworkResources.h"
#include "KeyboardSetCSVArray.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "LocationSelectorEntity.h"
#include "LocationSetCSVArray.h"
#include "MainFrmPosTray.h"
#include "MainFrmPosTrayPrint.h"
#include "MessageLogger.h"
#include "NetworkTerminalMap.h"
#include "MyCommandLineInfo.h"
#include "MyCSSLockFile.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "Password.h"
#include "PCOptions.h"
#include "PluDatabaseHeader.h"
#include "PMSCalendarTable.h"
#include "PMSCheckoutDateArray.h"
#include "PMSOptions.h"
#include "PMSRoomStatusTable.h"
#include "PMSZeroNightStayArray.h"
#include "PosTrayChartwellsExportCreator.h"
#include "PosTrayChartwellsSender.h"
#include "PosTrayFNBSender.h"
#include "PosTrayFTPSender.h"
#include "PosTrayLock.h"
#include "PosTrayOptions.h"
#include "PosTrayS4LabourSender.h"
#include "PosTrayStartDlg.h"
#include "PosTrayEmailSender.h"
#include "PosTrayExportSender.h"
#include "ReportHelpers.h"
#include "ReportPointCSVArray.h"
#include "ReportTypes.h"
#include "ServerDatabaseHeader.h"
#include "StatusProgress.h"
#include "SysInfo.h"
#include "SystemMap.h"
#include "TaxImportMap.h"
/**********************************************************************/
#include <winsock2.h>
/**********************************************************************/
#include "PosTrayApp.h"
/**********************************************************************/
extern const char* szUPDATE_TOKEN_DATE;
/**********************************************************************/

BEGIN_MESSAGE_MAP(CPosTrayApp, CWinApp)
	//{{AFX_MSG_MAP(CPosTrayApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CPosTrayApp theApp;

CAutoBackupOptions		AutoBackupOptions;
CAutoBackupProcessor	AutoBackupProcessor;
CBarcodePluNoTable		BarcodePluNoTable;
CDataManagerPosTray		DataManager;
CDataManagerNonDb		DataManagerNonDb;
CDealerFlags			DealerFlags;
CEcrmanOptions			EcrmanOptions;
CEmailOptions			EmailOptions;
CFontTable				FontTable;
CNetworkResources		NetworkResources;
CMessageLogger			MessageLogger;
CNetworkTerminalMap		NetworkTerminalMap;
CPasswordCSVArray		PasswordArray;
CPCOptionsClient		PCOptionsClient;
CPCOptionsHost			PCOptionsHost;
CPluDatabaseHeader		PluDatabaseHeader;
CPluTypeHelpers			PluTypeHelpers;
CPosTrayLock			PosTrayLock;
CPosTrayOptions			PosTrayOptions;
CPrompter				Prompter;
CServerDatabaseHeader	ServerDatabaseHeader;
CStatusProgress			StatusProgress;
CReportHelpers			ReportHelpers;
CSuper					Super;
CSysInfo				SysInfo;
CSysset					Sysset;
CTaxImportMap			TaxImportMap;

CSQLConnectionOptions 	SQLConnectionOptions;
CSQLConnectionPool		SQLConnectionPool;
CFilenameHandler		SQLFilenames;
CChartwellsOptions		ChartwellsOptions;

#ifdef COMPILE_PMS

CPMSCalendarTable		PMSCalendarTable;
CPMSCheckoutDateArray	PMSCheckoutDateArray;
CPMSOptions				PMSOptions;
CPMSRoomStatusTable		PMSRoomStatusTable;
CPMSZeroNightStayArray	PMSZeroNightStayArray;

#endif

CCashRSPImporter		CashRSPImporter;
CCommsHandshaker		CommsHandshaker;

CDatabaseCSVArray		dbDatabase;
CKeyboardSetCSVArray	dbKeyboardSet;
CLocationCSVArray		dbLocation;
CLocationMasterMap		LocationMasterMap;
CLocationSetCSVArray	dbLocationSet;
CNetworkCSVArray		dbNetwork;
CReportpointCSVArray	dbReportpoint;
CSystemMap				SystemMapFull;

/**********************************************************************/

CPosTrayApp::CPosTrayApp() : CWinApp ( "PosTray" )
{
	m_nPosTrayExitCode = 0;
	CKUnlock();
}

/**********************************************************************/

BOOL CPosTrayApp::InitInstance()
{
	CWinApp::InitInstance();

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

	LoadStdProfileSettings(0);  
	CMyCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	bool bResult = TRUE;

	switch( cmdInfo.GetSize() )
	{
	case 0:
		//RUN POSTRAY DIRECTLY
		SysInfo.SetDataPathNo(0);
		ListenForTasks( TRUE, 0 );
		break;

	case 1:
		//RUN POSTRAY DIRECTLY WITH DATA PATH
		{
			int nDataPath = atoi( cmdInfo.GetAt(0) );
			SysInfo.SetDataPathNo( nDataPath );
			ListenForTasks( TRUE, 0 );
		}
		break;

	default:
		switch (atoi(cmdInfo.GetAt(0)))
		{
			//RUN SCHEDULER FROM ECR MAN OR STOCK MAN
		case 1:
			if (cmdInfo.GetSize() == 3)
			{
				CString strType = cmdInfo.GetAt(1);
				int nType = (strType == "1" ? 2 : 1);

				int nDataPathNo = atoi(cmdInfo.GetAt(2));
				SysInfo.SetDataPathNo(nDataPathNo);
				ListenForTasks(FALSE, nType);
			}
			break;

			//CHECK FOR SAME VERSION AS MAIN PROGRAM
		case 201:
			if (cmdInfo.GetSize() == 2)
			{
				CString strType = cmdInfo.GetAt(0);
				CString strVersionMain = cmdInfo.GetAt(1);
				CString strVersionScheduler = szVERSION_ABOUT;

				if (strVersionMain == strVersionScheduler)
					m_nPosTrayExitCode = 201;
				else
					m_nPosTrayExitCode = 0;

				bResult = FALSE;
			}
			break;

			//AUTOMATIC EMAIL
		case 302:
			if (SysInfo.IsPosTrayNoTaskSystem() == FALSE)
			{
				if (cmdInfo.GetSize() == 4)
				{
					int nDataPathNo = atoi(cmdInfo.GetAt(3));
					SysInfo.SetDataPathNo(nDataPathNo);
					EmailReports(cmdInfo);
					bResult = FALSE;
				}
			}
			break;

			//FNB EXPORT
		case 401:
			if (SysInfo.IsPosTrayNoTaskSystem() == FALSE)
			{
				if (cmdInfo.GetSize() == 3)
				{
					int nDataPathNo = atoi(cmdInfo.GetAt(2));
					SysInfo.SetDataPathNo(nDataPathNo);
					FNBExport(cmdInfo);
					bResult = FALSE;
				}
			}
			break;

			//CHARTWELLS EXPORT
		case 902:
			if (SysInfo.IsPosTrayNoTaskSystem() == FALSE)
			{
				if (cmdInfo.GetSize() == 3)
				{
					CKUnlock();
					int nDataPathNo = atoi(cmdInfo.GetAt(2));
					SysInfo.SetDataPathNo(nDataPathNo);
					ChartwellsExport(cmdInfo);
					bResult = FALSE;
				}
			}
			break;

			//CHARTWELLS EXPORT
		case 903:
			if (SysInfo.IsPosTrayNoTaskSystem() == FALSE)
			{
				if (cmdInfo.GetSize() == 3)
				{
					CKUnlock();
					int nDataPathNo = atoi(cmdInfo.GetAt(2));
					SysInfo.SetDataPathNo(nDataPathNo);
					ChartwellsDbCheck(cmdInfo);
					bResult = FALSE;
				}
			}
			break;

			//AUTO PRINT
		case 501:
			if (SysInfo.IsPosTrayNoTaskSystem() == FALSE)
			{
				if (cmdInfo.GetSize() == 3)
				{
					int nDataPathNo = atoi(cmdInfo.GetAt(2));
					SysInfo.SetDataPathNo(nDataPathNo);
					PrintReports(cmdInfo);
					bResult = FALSE;
				}
			}
			break;

			//FTP EXPORT
		case 601:
			if (SysInfo.IsPosTrayNoTaskSystem() == FALSE)
			{
				if (cmdInfo.GetSize() == 3)
				{
					int nDataPathNo = atoi(cmdInfo.GetAt(2));
					SysInfo.SetDataPathNo(nDataPathNo);
					FTPExport(cmdInfo);
					bResult = FALSE;
				}
			}
			break;

			//S4 LABOUR EXPORT
		case 701:
			if (SysInfo.IsPosTrayNoTaskSystem() == FALSE)
			{
				if (cmdInfo.GetSize() == 3)
				{
					int nDataPathNo = atoi(cmdInfo.GetAt(2));
					SysInfo.SetDataPathNo(nDataPathNo);
					S4LabourExport(cmdInfo);
					bResult = FALSE;
				}
			}
			break;

			//AUTO EXPORT
		case 801:
			if (SysInfo.IsPosTrayNoTaskSystem() == FALSE)
			{
				if (cmdInfo.GetSize() == 3)
				{
					int nDataPathNo = atoi(cmdInfo.GetAt(2));
					SysInfo.SetDataPathNo(nDataPathNo);
					ExportReports(cmdInfo);
					bResult = FALSE;
				}
			}
			break;

#ifdef POSTRAY_SOCKET
			//CREATE EPOS REPORT FOR SOCKET
		case 901:
			if (SysInfo.IsPosTrayNoTaskSystem() == FALSE)
			{
				if (cmdInfo.GetSize() == 4)
				{
					int nDataPathNo = atoi(cmdInfo.GetAt(2));
					SysInfo.SetDataPathNo(nDataPathNo);
					CreateSocketReportEpos(cmdInfo);
					bResult = FALSE;
				}
			}
			break;
#endif

		case 1001:
			if (SysInfo.IsPosTrayNoTaskSystem() == FALSE)
			{
				if (cmdInfo.GetSize() == 3)
				{
					int nDataPathNo = atoi(cmdInfo.GetAt(2));
					SysInfo.SetDataPathNo(nDataPathNo);
					CheckSystemBackup(cmdInfo);
					bResult = FALSE;
				}
			}
			break;
		}
	}

	return bResult;
}

/**********************************************************************/

void CPosTrayApp::ListenForTasks( bool bShowErrors, int nSpawnType )
{
	//CHECK FOR ILLEGAL REGISTRATION
	bool bRegistered = FALSE;

	while ( FALSE == bRegistered )
	{
		bool bCanCreatePCId = TRUE;

		switch( nSpawnType )
		{
		case 1:
			if ( Sysset.TestRegistrationModule ( SYSSET_ECRMANSPOSV4, NULL, bCanCreatePCId, TRUE, SysInfo.GetDataPathNo() ) == TRUE )
			{
				bRegistered = TRUE;
				SysInfo.SetPosTrayStockSystemFlag( FALSE );
			}
			break;

		case 2:
			if ( Sysset.TestRegistrationModule ( SYSSET_STKMANSPOSV4, NULL, bCanCreatePCId, TRUE, SysInfo.GetDataPathNo() ) == TRUE )
			{
				bRegistered = TRUE;
				SysInfo.SetPosTrayStockSystemFlag( TRUE );
			}
			break;

		case 0:
		default:
			if ( Sysset.TestRegistrationModule ( SYSSET_STKMANSPOSV4, NULL, bCanCreatePCId, TRUE, SysInfo.GetDataPathNo() ) == TRUE )
			{
				bRegistered = TRUE;
				SysInfo.SetPosTrayStockSystemFlag( TRUE );
			}
			else if ( Sysset.TestRegistrationModule ( SYSSET_ECRMANSPOSV4, NULL, bCanCreatePCId, TRUE, SysInfo.GetDataPathNo() ) == TRUE )
			{
				bRegistered = TRUE;
				SysInfo.SetPosTrayStockSystemFlag( FALSE );
			}
			break;
		}

		if ( FALSE == bRegistered )
		{
			CString strTest = Sysset.GetError();
			strTest.MakeLower();

			if ( strTest.Find( "data path does not exist" ) == -1 )
				break;

			CPosTrayStartDlg dlg ( NULL );
			dlg.DoModal();

			if ( dlg.GetRetryFlag() == FALSE )
				break;
		}
	}

	Super.Initialise();
	MessageLogger.Initialise();

	CMessageLoggerInfo info;
	info.m_strMessage = "PosTray : ";
	info.m_nHideLevel = 2;

	CString strError = "Unable to start PosTray.\n\n";
			
	if ( bRegistered == FALSE )
	{
		info.m_strMessage += Sysset.GetError();
		MessageLogger.LogSchedulerSystem( info );

		if ( bShowErrors == TRUE ) 
			Prompter.FatalError ( strError + info.m_strMessage );

		return;
	}

	if ( SysInfo.IsPosTraySystem() == FALSE )
	{
		info.m_strMessage += "System not registered to use PosTray.";
		MessageLogger.LogSchedulerSystem( info );
		
		if ( bShowErrors == TRUE ) 
			Prompter.FatalError ( strError + info.m_strMessage );

		return;
	}

	//IF REGISTERED FOR PMS THEN CHECK WE HAVE CORRECT SOFTWARE
#ifndef COMPILE_PMS
	if ( SysInfo.GetMaxPMSRooms() > 0 )
	{
		CString strPMSError = "";
		strPMSError += "Your software licence specifies that PMS (Guest Account)\n";
		strPMSError += "features are required, but this version of PosTray\n";
		strPMSError += "does not include the PMS features.\n\n";
		strPMSError += "Please ask your dealer to replace this software with a\n";
		strPMSError += "version of PosTray which has PMS support.";
		
		if ( bShowErrors == TRUE ) 
			Prompter.FatalError( strPMSError );
		
		return;
	}
#endif

	PosTrayLock.SetFilename();

	if (SysInfo.IsPosTraySocketSystem() == TRUE)
	{
		Super.SetPosTraySocketFlag(AfxSocketInit() != 0);
	}
	else
	{
		Super.SetPosTraySocketFlag(FALSE);
	}

	CMainFramePosTray* pMainFrame = new CMainFramePosTray();

	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		return;
	}

	pMainFrame->ShowWindow(SW_HIDE);
	pMainFrame->UpdateWindow();
	
	m_pMainWnd = pMainFrame;
	CommsHandshaker.InitialiseComms();

	return;
}

/**********************************************************************/

bool CPosTrayApp::TestRegistration(CMyCommandLineInfo& cmdInfo)
{
	CString strStockmanFlag = cmdInfo.GetAt(1);
	bool bStockman = ( strStockmanFlag == "1" );
	
	SysInfo.SetPosTrayStockSystemFlag( bStockman );

	bool bRegistered = FALSE;
	{
		bool bCreatePCId = TRUE;
		
		if (FALSE == bStockman)
		{
			bRegistered = Sysset.TestRegistrationModule(SYSSET_ECRMANSPOSV4, NULL, bCreatePCId, TRUE, SysInfo.GetDataPathNo());
		}
		else
		{
			bRegistered = Sysset.TestRegistrationModule(SYSSET_STKMANSPOSV4, NULL, bCreatePCId, TRUE, SysInfo.GetDataPathNo());
		}
	}
	return bRegistered;
}

/**********************************************************************/

void CPosTrayApp::EmailReports( CMyCommandLineInfo& cmdInfo )
{
	if (TestRegistration(cmdInfo) == TRUE)
	{
		CString strSMTPFlag = cmdInfo.GetAt(2);
		bool bSMTP = (strSMTPFlag == "1");

		CMyCSSLockFile fileLock;
		CFilenameUpdater FnUp(SysFiles::PosTrayEmailLock);
		if (fileLock.OpenLock(FnUp.GetFilenameToUse()) == TRUE)
		{
			Super.Initialise();
			MessageLogger.Initialise();

			if (SysInfo.IsPosTraySystem() == FALSE)
			{
				return;
			}

			CreateSyssetSubdirectory("SchedLog");
			CreateSyssetSubdirectory("SchedOld");

			CPosTrayEmailSender LocalPosTrayEmailSender;
			LocalPosTrayEmailSender.LookForReports(bSMTP);
		}
	}
}

/**********************************************************************/

void CPosTrayApp::ExportReports(CMyCommandLineInfo& cmdInfo)
{
	if (TestRegistration(cmdInfo) == TRUE)
	{
		CMyCSSLockFile fileLock;
		CFilenameUpdater FnUp(SysFiles::PosTrayExportLock);
		if (fileLock.OpenLock(FnUp.GetFilenameToUse()) == TRUE)
		{
			Super.Initialise();
			MessageLogger.Initialise();

			if (SysInfo.IsPosTraySystem() == FALSE)
			{
				return;
			}

			CreateSyssetSubdirectory("SchedLog");
			CreateSyssetSubdirectory("SchedOld");

			CPosTrayExportSender LocalPosTrayExportSender;
			LocalPosTrayExportSender.LookForReports();
		}
	}
}

/**********************************************************************/

void CPosTrayApp::PrintReports(CMyCommandLineInfo& cmdInfo)
{
	if (TestRegistration(cmdInfo) == TRUE)
	{
		CMyCSSLockFile fileLock;
		CFilenameUpdater FnUp(SysFiles::PosTrayPrintLock);
		if (fileLock.OpenLock(FnUp.GetFilenameToUse()) == TRUE)
		{
			Super.Initialise();
			MessageLogger.Initialise();

			if (SysInfo.IsPosTraySystem() == FALSE)
			{
				return;
			}

			CreateSyssetSubdirectory("SchedLog");
			CreateSyssetSubdirectory("SchedOld");

			if (DealerFlags.ReadFromFile() == TRUE)
			{
				CMainFramePosTrayPrint* pMainFrame = new CMainFramePosTrayPrint();

				if (pMainFrame->LoadFrame(IDR_MAINFRAME_POSTRAY_PRINT) == TRUE)
				{
					pMainFrame->ShowWindow(SW_HIDE);
					pMainFrame->UpdateWindow();
					m_pMainWnd = pMainFrame;
					pMainFrame->PrintReports();
					pMainFrame->SendMessage(WM_CLOSE);
				}
			}
		}
	}
}

/**********************************************************************/

void CPosTrayApp::FNBExport( CMyCommandLineInfo& cmdInfo )
{
	if (TestRegistration(cmdInfo) == TRUE)
	{
		CMyCSSLockFile fileLock;
		CFilenameUpdater FnUp(SysFiles::FNBSenderFTPLock);
		if (fileLock.OpenLock(FnUp.GetFilenameToUse()) == TRUE)
		{
			Super.Initialise();
			MessageLogger.Initialise();

			if (SysInfo.IsPosTraySystem() == FALSE)
			{
				return;
			}

			SystemMapFull.CreateSystem();
			SystemMapFull.FixWebSalesNetworks();

			CreateSyssetSubdirectory("SchedLog");
			CreateSyssetSubdirectory("SchedOld");

			CPosTrayFNBSender FNBSender;
			FNBSender.SendExports();
		}
	}
}

/**********************************************************************/

void CPosTrayApp::ChartwellsExport(CMyCommandLineInfo& cmdInfo)
{
	if (TestRegistration(cmdInfo) == TRUE)
	{
		CMyCSSLockFile fileLock;
		CFilenameUpdater FnUp(SysFiles::ChartwellsSenderFTPLock);
		if (fileLock.OpenLock(FnUp.GetFilenameToUse()) == TRUE)
		{
			Super.Initialise();
			MessageLogger.Initialise();

			if (SysInfo.IsPosTraySystem() == FALSE)
			{
				return;
			}

			SystemMapFull.CreateSystem();
			SystemMapFull.FixWebSalesNetworks();

			CreateSyssetSubdirectory("SchedLog");
			CreateSyssetSubdirectory("SchedOld");

			CPosTrayChartwellsSender ChartwellsSender;
			ChartwellsSender.SendExports();
		}
	}
}

/**********************************************************************/

void CPosTrayApp::ChartwellsDbCheck(CMyCommandLineInfo& cmdInfo)
{
	if (TestRegistration(cmdInfo) == TRUE)
	{
		CPosTrayChartwellsScheduleInfo infoExport;
		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		ChartwellsOptions.Read(FALSE);
		SQLConnectionOptions.Read();

		CSQLDb SQLDb;
		CPosTrayChartwellsExportCreator ExportCreator(infoExport);
		int nErrorNo = 0;
		int nVersionNo = 0;
		int nExternalVersionNo = 0;
		ExportCreator.CheckDbAccess(SQLDb, nErrorNo, nVersionNo, nExternalVersionNo);

		CSSFile fileInfo;
		CFilenameUpdater FnUp(SysFiles::ChartwellsDbCheck);
		if (fileInfo.Open(FnUp.GetFilenameToUse(), "wb") == TRUE)
		{
			CCSV csv;
			csv.Add(infoExport.m_strLastDbCheckTime);
			csv.Add(infoExport.m_nDatabaseError);
			fileInfo.WriteLine(csv.GetLine());
		}
	}
}

/**********************************************************************/

void CPosTrayApp::S4LabourExport( CMyCommandLineInfo& cmdInfo )
{
	if (TestRegistration(cmdInfo) == TRUE)
	{
		if (PosTrayOptions.Read() == FALSE)
		{
			return;
		}

		CMyCSSLockFile fileLock;
		CFilenameUpdater FnUp(SysFiles::S4LabourSenderFTPLock);
		if (fileLock.OpenLock(FnUp.GetFilenameToUse()) == TRUE)
		{
			Super.Initialise();
			MessageLogger.Initialise();

			if (SysInfo.IsPosTraySystem() == FALSE)
			{
				return;
			}

			SystemMapFull.CreateSystem();
			SystemMapFull.FixWebSalesNetworks();

			CreateSyssetSubdirectory("SchedLog");
			CreateSyssetSubdirectory("SchedOld");

			CPosTrayS4LabourSender S4LabourSender;
			S4LabourSender.SendExports();
		}
	}
}

/**********************************************************************/

void CPosTrayApp::FTPExport(CMyCommandLineInfo& cmdInfo)
{
	if (TestRegistration(cmdInfo) == TRUE)
	{
		if (PosTrayOptions.Read() == FALSE)
		{
			return;
		}

		CMyCSSLockFile fileLock;
		CFilenameUpdater FnUp(SysFiles::PosTrayFTPLock);
		if (fileLock.OpenLock(FnUp.GetFilenameToUse()) == TRUE)
		{
			Super.Initialise();
			MessageLogger.Initialise();

			if (SysInfo.IsPosTraySystem() == FALSE)
			{
				return;
			}

			SystemMapFull.CreateSystem();
			SystemMapFull.FixWebSalesNetworks();

			CreateSyssetSubdirectory("SchedLog");
			CreateSyssetSubdirectory("SchedOld");

			for (int n = 0; n < 4; n++)
			{
				int nMode = 0;
				switch (n)
				{
				case 0:	nMode = POSTRAY_TASKFILE_EXPORT_SALES_TASK;		break;
				case 1:	nMode = POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK;	break;
				case 2:	nMode = POSTRAY_TASKFILE_EXPORT_VOID_TASK;		break;
				case 3:	nMode = POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK;	break;
				}

				for (int nTaskNo = 1; nTaskNo <= POSTRAY_MAX_TASK; nTaskNo++)
				{
					CPosTrayTask Task(nMode);
					Task.SetTaskNo(nTaskNo);
					Task.Read(FALSE);

					CPosTrayFTPSender FTPSender;
					FTPSender.SendExports(Task);
				}
			}
		}
	}
}

/**********************************************************************/

void CPosTrayApp::CheckSystemBackup(CMyCommandLineInfo& cmdInfo)
{
	if (TestRegistration(cmdInfo) == TRUE)
	{
		Super.Initialise();
		MessageLogger.Initialise();

		if (PCOptionsHost.Read() == FALSE)
		{
			AutoBackupProcessor.LogPosTrayBackupMessage("Unable to check PC host options(2)");
			return;
		}

		if (PCOptionsHost.GetBackupAccessType() != 0)
		{
			AutoBackupProcessor.LogPosTrayBackupMessage("Auto backup not enabled at this PC(2)");
			return;
		}

		if (AutoBackupOptions.Read() == FALSE)
		{
			AutoBackupProcessor.LogPosTrayBackupMessage("Unable to check backup schedule(2)");
			return;
		}

		AutoBackupProcessor.ResetNextAttemptTime();
		AutoBackupProcessor.SetReadyToGoFlag(TRUE);

		CFilenameUpdater FnUp(SysFiles::PosTrayBackupToken);
		CSharedStringArray arrayDummy;

		//USE DUMMY ARRAY TO PREVENT MULTIPLE BACKUPS STARTING
		if (arrayDummy.Open(FnUp.GetFilenameToUse(), DB_READWRITE) == DB_ERR_NONE)
		{
			AutoBackupProcessor.CheckAutoBackup();
			arrayDummy.Close();
		}
	}
}

/**********************************************************************/
#ifdef POSTRAY_SOCKET
/**********************************************************************/

void CPosTrayApp::CreateSocketReportEpos(CMyCommandLineInfo& cmdInfo)
{
	if (TestRegistration(cmdInfo) == TRUE)
	{
		Super.Initialise();
		MessageLogger.Initialise();

		CPosTraySocketMessage SocketMessage;
		SocketMessage.SetParamsFilename(cmdInfo.GetAt(3));
		SocketMessage.DeletePendingFiles();

		{
			CString strReportFilename = "";
			strReportFilename += SocketMessage.GetParamsFilePath();
			strReportFilename += ".rep";
			Super.SetSpecialReportFilename(strReportFilename);
		}

		CString strError = "";
		if (SocketLoadSystem(strError) == FALSE)
		{
			SocketMessage.CreateHTMLResponseFile(strError);
			return;
		}

		COleDateTime dateFrom = COleDateTime(2010, 1, 1, 0, 0, 0);
		COleDateTime dateTo = COleDateTime(2020, 12, 31, 0, 0, 0);

		CEposSelectArray SelectArray;
		CLocationSelectorEntity LocSelEntity;
		LocSelEntity.SetSystemNode();
		LocSelEntity.GetEPOSSelectArray(SelectArray);

		CEposReportSelect EposReportSelect;
		EposReportSelect.LoadReportSelection();

		CEposReportSelectInfo infoReport;
		infoReport.SetReportType(REPORT_TYPE_PLUSALES);
		bool bResult = EposReportSelect.GetReportInfo(infoReport);

		CEposReport* pReport = NULL;

		if (TRUE == bResult)
		{
			CSessionCSVRecord DefaultSession;
			pReport = new CEposReportPluVipSalesSummary(SelectArray, FALSE);
			pReport->SetDates(dateFrom, dateTo, DefaultSession);
			pReport->SetConLevelTitle(LocSelEntity.GetConsolidationTitle());
			pReport->SetSessionTitle("00:00 to 23:59");
			pReport->SetConLevel(NODE_SYSTEM);
			pReport->SetPreferConsolType(EPOSREPORT_CONSOL_DEFAULT);
			pReport->SetEposReportSelectInfo(infoReport);
			pReport->m_ReportFilters.DeptFilter.SetDeptFilterNo(DEPARTMENTSET_REPORT);
			pReport->Consolidate();
			bResult = pReport->CreateReport();
		}

		if (TRUE == bResult)
		{
			CString strReportFilename = Super.ReportFilename();
			CReportFileDoc ReportFileDoc(strReportFilename);
			ReportFileDoc.SaveAsHTML(SocketMessage.GetHTMLFilePath());
		}
		else
		{
			SocketMessage.CreateHTMLResponseFile("Failed");
		}

		delete pReport;
	}
}

/**********************************************************************/

bool CPosTrayApp::SocketLoadSystem( CString& strError )
{
	SysInfo.ClearFatalReadErrorFlag();
	DealerFlags.ClearFatalReadErrorFlag();
	EcrmanOptions.ClearFatalReadErrorFlag();

	if ( SysInfo.Read() == FALSE )
	{
		if ( SysInfo.GetFatalReadErrorFlag() )
		{
			strError = "Unable to open SysInfo file";
			return FALSE;
		}
	}
		
	if ( DealerFlags.ReadFromFile() == FALSE )
	{
		if ( DealerFlags.GetFatalReadErrorFlag() )
		{
			strError = "Unable to open Dealer Options file";
			return FALSE;
		}
	}

	if ( EcrmanOptions.Read() == FALSE )
	{
		if ( EcrmanOptions.GetFatalReadErrorFlag() )
		{
			strError = "Unable to open System Options file";
			return FALSE;
		}
	}

	SystemMapFull.CreateSystem();
	SystemMapFull.FixWebSalesNetworks();

	return TRUE;
}

/**********************************************************************/
#endif
/**********************************************************************/

BOOL CPosTrayApp::OnIdle(LONG lCount) 
{
	if (CWinApp::OnIdle(lCount))
		return TRUE; 

	return TRUE;
}

/**********************************************************************/

int CPosTrayApp::ExitInstance() 
{
#ifdef POSTRAY_SOCKET
	if ( Super.GetPosTraySocketFlag() == TRUE )
	{
		WSACleanup();
	}
#endif

	CommsHandshaker.TerminateThread();
	CWinApp::ExitInstance();
	return m_nPosTrayExitCode;
}

/**********************************************************************/
