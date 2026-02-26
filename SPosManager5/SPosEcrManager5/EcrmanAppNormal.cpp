/**********************************************************************/
#include "..\CommonEcrStockTray\AutoBackupOptions.h"
#include "..\CommonEcrStockTray\AutoBackupProcessor.h"
/**********************************************************************/
#include "..\CommonEcrStock\AutoExpireHistory.h"
#include "..\CommonEcrStock\BackgroundLayoutInfo.h"
#include "..\CommonEcrStock\BarcodeAutoImporter.h"
#include "..\CommonEcrStock\ButtonActionHandler.h"
#include "..\CommonEcrStock\ButtonManager.h"
#include "..\CommonEcrStock\ButtonOptions.h"
#include "..\CommonEcrStock\CashRSPClearBufferThreadManager.h"
#include "..\CommonEcrStock\CommonDoc.h"
#include "..\CommonEcrStock\CommsBatchDlg.h"
#include "..\CommonEcrStock\CustomerAutoDeleter.h"
#include "..\CommonEcrStock\CustomerAutoImporterEOD.h"
#include "..\CommonEcrStock\DatabaseValidator.h"
#include "..\CommonEcrStock\IdleTimeManager.h"
#include "..\CommonEcrStock\IDraughtExportManager.h"
#include "..\CommonEcrStock\IDraughtOptions.h"
#include "..\CommonEcrStock\KbdListNameArray.h"
#include "..\CommonEcrStock\PluAutoImporterCSV.h"
#include "..\CommonEcrStock\PluAutoImporterEOD.h"
#include "..\CommonEcrStock\PluAutoImporterEODPriceSet.h"
#include "..\CommonEcrStock\PresetReportManager.h"
#include "..\CommonEcrStock\PriceSetItemManager.h"
#include "..\CommonEcrStock\PropertySheetPosTrayOptions.h"
#include "..\CommonEcrStock\SageExportManager.h"
#include "..\CommonEcrStock\SageOptions.h"
#include "..\CommonEcrStockTray\ServerDatabaseHeader.h"
#include "..\CommonEcrStock\SmartPaySemaphore.h"
#include "..\CommonEcrStock\SpecialChangesHelper.h"
#include "..\CommonEcrStock\SpecialKeys.h"
#include "..\CommonEcrStock\SPOSKeyboardManager.h"
#include "..\CommonEcrStock\TooManyUsersDlg.h"
#include "..\CommonEcrStockTray\TaxImportMap.h"
#include "..\CommonEcrStockTray\WMNumbers.h"
/**********************************************************************/
#include "..\CommonEcrStockTray\ChartwellsOptions.h"
#include "..\CommonEcrStockTray\PluTypeHelpers.h"
/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "BatchSalesExporter.h"
#include "CashRSPImporter.h"
#include "EcrmanView.h"
#include "CommsHandshaker.h"
#include "DatabaseCSVArray.h"
#include "DemoWarning.h"
#include "EmailOptions.h"
#include "ExportDate.h"
#include "FileRemover.h"
#include "NetworkResources.h"
#include "FontTable.h"
#include "FtpConnect.h"
#include "KeyboardSetCSVArray.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "LocationSetCSVArray.h"
#include "MainFrmEcrman.h"
#include "MessageLogger.h"
#include "NetworkCSVArray.h"
#include "NetworkTerminalMap.h"
#include "NetworkTester.h"
#include "Password.h"
#include "PCOptions.h"
#include "PluDatabaseHeader.h"
#include "PMSCalendarTable.h"
#include "PMSCheckoutDateArray.h"
#include "PMSRoomStatusTable.h"
#include "PMSOptions.h"
#include "PMSZeroNightStayArray.h"
#include "PosTrayOptions.h"
#include "PosTrayTokenFiles.h"
#include "ReportHelpers.h"
#include "ReportPointCSVArray.h"
#include "StatusProgress.h"
#include "SysInfo.h"
#include "SystemMap.h"
#include "WMNumbers.h"
#include "ZSalesImporterEcrman.h"
/**********************************************************************/
#include "EcrmanApp.h"
/**********************************************************************/
extern const char* szVERSION_ABOUT;
extern const char* szUPDATE_TOKEN_DATE;
/**********************************************************************/

//SYSTEM STRUCTURE
CDatabaseCSVArray dbDatabase;
CDataManagerEcrman DataManager;
CDataManagerNonDb DataManagerNonDb;
CKeyboardSetCSVArray dbKeyboardSet;
CLocationCSVArray dbLocation;
CLocationMasterMap LocationMasterMap;
CLocationSetCSVArray dbLocationSet;
CNetworkCSVArray dbNetwork;
CNetworkTerminalMap NetworkTerminalMap;
CPresetReportManager PresetReportManager;
CReportpointCSVArray dbReportpoint;
CSysset Sysset;
CSystemMap SystemMapFull;

//SYSTEM OPTIONS
CEcrmanOptions EcrmanOptions;
CEmailOptions EmailOptions;
CNetworkResources NetworkResources;
CPCOptionsClient PCOptionsClient;
CPCOptionsHost PCOptionsHost;
CPosTrayOptions PosTrayOptions;
CSpecialKeys SpecialKeys;

//MISCELLANEOUS
CAutoBackupOptions AutoBackupOptions;
CAutoBackupProcessor AutoBackupProcessor;
CAutoExpireHistory AutoExpireHistory;
CBarcodePluNoTable BarcodePluNoTable;
CCashRSPClearBufferThreadManager CashRSPClearBufferThreadManager;
CCashRSPImporter CashRSPImporter;
CFontTable FontTable;
CIDraughtExportManager IDraughtExportManager;
CIDraughtOptions IDraughtOptions;
CPluTypeHelpers PluTypeHelpers;
CSageExportManager SageExportManager;
CSageOptions SageOptions;
CTaxImportMap TaxImportMap;

#ifdef COMPILE_PMS
CPMSCheckoutDateArray PMSCheckoutDateArray;
CPMSCalendarTable PMSCalendarTable;
CPMSOptions PMSOptions;
CPMSRoomStatusTable PMSRoomStatusTable;
CPMSZeroNightStayArray PMSZeroNightStayArray;
#endif

CDatabaseValidator DatabaseValidator;
CDealerFlags DealerFlags;

CCommsHandshaker CommsHandshaker;
CChartwellsOptions ChartwellsOptions;
CExportDate ExportDate;
CFtpConnect FtpConnect;
CIdleTimeManager IdleTimeManager;
CKbdListNameArray KbdListNameArray;
CMessageLogger MessageLogger;
CPasswordCSVArray PasswordArray;
CSpecialChangesHelper SpecialChangesHelper;
CStatusProgress StatusProgress;
CPluDatabaseHeader PluDatabaseHeader;
CPriceSetItemManager PriceSetItemManager;
CPrompter Prompter;
CReportHelpers ReportHelpers;
CServerDatabaseHeader ServerDatabaseHeader;
CSuper Super;
CSysInfo SysInfo;

CBarcodeAutoImporter BarcodeAutoImporter;
CBatchSalesExporter BatchSalesExporter;
CCustomerAutoDeleterAccount CustomerAutoDeleterAccount;
CCustomerAutoDeleterOneOff CustomerAutoDeleterOneOff;
CCustomerAutoImporterEOD CustomerAutoImporterEOD;
CPluAutoImporterCSV PluAutoImporterCSV;
CPluAutoImporterEOD PluAutoImporterEOD;
CPluAutoImporterEODPriceSet PluAutoImporterEODPriceSet;

CSPOSKeyboardManager SPOSKeyboardManager;

CSmartPaySemaphore LoyaltySemaphore;
CSmartPaySemaphore SmartPaySemaphore;

CBackgroundLayoutInfo BackgroundLayoutInfo;
CButtonActionHandler ButtonActionHandler;
CButtonManager ButtonManager;
CButtonOptions ButtonOptions;
CBitmap GlobalBitmap;
CBitmap GlobalMask;

/**********************************************************************/

CEcrmanApp::CEcrmanApp() : CWinApp ( "Ecr Manager" )
{
	{
		char c = '0';
		m_dwStackBase = DWORD(&c);
	}

	CKUnlock();

	m_bLogPassword = FALSE;
	m_bLogEntry = FALSE;
	m_strStartTime = "";

	m_timeNextCheck = COleDateTime::GetCurrentTime();
	m_nNextIdleTask = 0;
	m_pMainWnd = NULL;

	//FIDDLE
	m_oleNextPosTrayTime = COleDateTime::GetCurrentTime();
}

/**********************************************************************/

BOOL CEcrmanApp::InitInstance()
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

	SysInfo.SetStackBase(m_dwStackBase);

	InitCommonControls();
	
	LoadStdProfileSettings(0);  
	CMyCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (cmdInfo.GetSize() == 1)
	{
		CString strCommand = cmdInfo.GetAt(0);
		strCommand.MakeUpper();
		if (strCommand == "VERSIONNO")
		{
			exit(Sysset.ConvertStringVersionToInt(szVERSION_ABOUT));
			return FALSE;
		}
	}
	
	int nDataPathNo = 0;
	if (cmdInfo.GetSize() > 0)
	{
		nDataPathNo = atoi(cmdInfo.GetAt(0));
	}

	SysInfo.SetDataPathNo( nDataPathNo );

	//CHECK FOR COMMAND LINE FLAGS
	bool bScriptMode = FALSE;
	CString strTaskString = "";

	for ( int n = 1; ( n < cmdInfo.GetSize() ) && ( n <= 2 ); n++ )
	{
		CString str = cmdInfo.GetAt(n);
		str.MakeUpper();

		if (str == "SCRIPT")
		{
			bScriptMode = TRUE;
		}

		if ( str.Left(7) == "RUNTASK" )	
		{
			strTaskString = str;
			bScriptMode = TRUE;
		}
	}

	SysInfo.SetScriptModeFlag( bScriptMode );
	SysInfo.SetTaskModeString( strTaskString );

	CTime tm = CTime::GetCurrentTime();
	m_strStartTime = tm.Format ( "(started %a %b %d %Y %H:%M:%S)");

	//CHECK FOR ILLEGAL REGISTRATION
	bool bCanCreatePCId = TRUE;
	bool bResult = Sysset.TestRegistrationMainExe ( SYSSET_ECRMANSPOSV4, NULL, bCanCreatePCId, szVERSION_ABOUT, szUPDATE_TOKEN_DATE, FALSE, SysInfo.GetDataPathNo() );
	Super.Initialise();
	MessageLogger.Initialise();

	if ( bResult == FALSE )
	{
		CString strError = Sysset.GetError();

		CMessageLoggerInfo info;
		info.m_strMessage = strError;
		info.m_nHideLevel = 2;

		MessageLogger.LogSchedulerMessage( info, FALSE, TRUE, FALSE );

		if ( strError == "Too many users" )
		{	
			CTooManyUsersDlg dlgTooMany(NULL);
			dlgTooMany.DoModal();
		}
		else
		{
			Prompter.FatalError ( strError );
		}

		return FALSE;
	}

	//MAKE SURE THAT COMMS LOG IS NOT READ ONLY
	if ( ::FileExists ( Super.SchedLogAll() ) == TRUE )
	{
		CSSFile fileLog;
		if ( fileLog.Open ( Super.SchedLogAll(), "ab" ) == FALSE )
		{
			Prompter.FatalError ( "Error 1 - Unable to continue" );
			return FALSE;
		}
	}

	//IF REGISTERED FOR PMS THEN CHECK WE HAVE CORRECT SOFTWARE
#ifndef COMPILE_PMS
	if ( SysInfo.GetMaxPMSRooms() > 0 )
	{
		CString strPMSError = "";
		strPMSError += "Your software licence specifies that PMS (Guest Account)\n";
		strPMSError += "features are required, but this version of Ecr Manager\n";
		strPMSError += "does not include the PMS features.\n\n";
		strPMSError += "Please ask your dealer to replace this software with a\n";
		strPMSError += "version of Ecr Manager which has PMS support.";
		Prompter.FatalError( strPMSError );
		return FALSE;
	}
#endif

	//LOG ENTRY
	m_bLogEntry = TRUE;
	LogEntry ( FALSE );

	SmartPaySemaphore.Init( FALSE );
	LoyaltySemaphore.Init( TRUE );

	//READ THE SYSTEM DATABASES AND VALIDATE THEM
	if (InitialiseSystem() == FALSE)
	{
		return FALSE;
	}

	if (SysInfo.GetTaskModeFlag() == TRUE)
	{
		//NETWORK TESTER
		CNetworkTester NetworkTester;
		if (NetworkTester.TestNetwork() != NETWORK_ERROR_NONE)
		{
			return FALSE;
		}

		PasswordArray.CheckPassword(PasswordArray.GetMasterPassword());

		//CHECK FOR TOO MANY SYSTEM DATABASES
		if (dbDatabase.GetSize() > SysInfo.GetMaxDatabases())
		{
			return FALSE;
		}

		if (dbLocation.GetSharpPosLocationCount() > SysInfo.GetMaxLocations())
		{
			return FALSE;
		}

		//CREATE THE FOLDER FOR MISC EPOS REPORTS
		CreateSyssetSubdirectory("SchedLog");
		CreateSyssetSubdirectory("SchedOld");

		//LOG PASSWORD ENTRY IF REQUIRED
		if (m_bLogPassword == TRUE)
		{
			LogPassword(FALSE);
		}

		DatabaseValidator.Run(TRUE, NULL);

		if ((DatabaseValidator.ValidAtStartup() == FALSE) || (DatabaseValidator.CancelAtStartup() == TRUE))
		{
			DatabaseValidator.WriteQuitToken();
		}
		else
		{
			DatabaseValidator.DeleteQuitToken();

			CCSV csv(SysInfo.GetTaskModeString());

			int nTask = csv.GetInt(0);
			if ((nTask < 1) || (nTask > SysInfo.GetMaxPresetTasks()))
			{
				nTask = 1;
			}

			int nAutoMode = csv.GetInt(1);
			int nCloseDelay = csv.GetInt(2);

			CEposTaskDatabase TaskDatabase;
			TaskDatabase.Read();

			CEposTaskDatabase EditableTask;
			TaskDatabase.LoadEditableTask(EditableTask, 0x80 + nTask);

			CEposTaskHeader TaskHeader;
			EditableTask.GetEditableTaskHeader(TaskHeader);
			TaskHeader.SetImportType(IMPORT_IMMEDIATE);
			EditableTask.SetEditableTaskHeader(TaskHeader);

			CString strDate = "";
			CKeyboardMap KeyboardMap;
			CCommsBatchDlg BatchDlg(EditableTask, strDate, FALSE, KeyboardMap, -1, nAutoMode, nCloseDelay, NULL);
			BatchDlg.DoModal();
		}

		return FALSE;
	}
	else
	{
		// Register document templates
		CSingleDocTemplate* pDocTemplate;
		pDocTemplate = new CSingleDocTemplate(
			IDR_MAINFRAME,
			RUNTIME_CLASS(CCommonDoc),
			RUNTIME_CLASS(CMainFrameEcrman),
			RUNTIME_CLASS(CEcrmanView));
		AddDocTemplate(pDocTemplate);

		// Dispatch commands specified on the command line
		if (!ProcessShellCommand(cmdInfo))
		{
			return FALSE;
		}

		if (BackgroundLayoutInfo.GotValidLayout() == FALSE)
		{
			Sysset.RestoreWindowSize(m_pMainWnd);
		}
		
		m_pMainWnd->ShowWindow(SW_SHOW);
		m_pMainWnd->UpdateWindow();
	
		( (CMainFrameEcrman*) m_pMainWnd )-> DisableMenus();

		//NETWORK TESTER
		CNetworkTester NetworkTester;
		if ( NetworkTester.TestNetwork() != NETWORK_ERROR_NONE )
		{
			NetworkTester.ShowError();
			m_pMainWnd -> SendMessage ( WM_CLOSE, 0, 0 );
			return FALSE;
		}

		//PROMPT FOR PASSWORD AND FLAG ILLEGAL SOFTWARE
		CString strPassword = "";

		CString strMaster = PasswordArray.GetMasterPassword();
			
		if (strMaster == "")
		{
			strPassword = Sysset.DisplayLicenceDetails("", "", 0, NULL);
		}
		else
		{
			strPassword = Sysset.DisplayLicenceDetails ( "Password", "Please enter your assigned password", Super.MaxPasswordLen(), NULL );
			strPassword.MakeUpper();	
		}
		
		PasswordArray.CheckPassword( strPassword );
		
		{
			CString strUsername = "None";

			if (strMaster != "")
			{
				if (0 == PasswordArray.GetPasswordIdx())
				{
					strUsername = "Master";
				}
				else
				{
					strUsername = PasswordArray.GetUserName();
				}
			}

			Sysset.CreateProgUserFile(strUsername);
		}

		MessageLogger.LogSystem( 12, 1, TRUE, PasswordArray.GetPasswordIdx() );
		MessageLogger.LogSystem( 12, 2, FALSE, DealerFlags.GetLogFileWritesFlag() ? 1 : 0 );
		
		//CHECK FOR TOO MANY SYSTEM DATABASES
		CStringArray arrayTooMany;
		if ( dbDatabase.GetSize() > SysInfo.GetMaxDatabases() )
		{
			CString strLine;
			strLine.Format( "%d databases defined (max %d)",
				dbDatabase.GetSize(),
				SysInfo.GetMaxDatabases() );

			arrayTooMany.Add( strLine );
		}

		if ( dbLocation.GetSharpPosLocationCount() > SysInfo.GetMaxLocations() )
		{
			CString strLine;
			strLine.Format( "%d locations defined (max %d)",
				dbLocation.GetSharpPosLocationCount(),
				SysInfo.GetMaxLocations() );

			arrayTooMany.Add( strLine );
		}

		if (arrayTooMany.GetSize() > 0)
		{
			CString strMsg = "Ecr Manager cannot run from this folder because too many\n";
			strMsg += "databases or locations have been defined.\n\n";

			for (int n = 0; n < arrayTooMany.GetSize(); n++)
			{
				strMsg += arrayTooMany.GetAt(n);

				if (n < arrayTooMany.GetSize() - 1)
				{
					strMsg += "\n\n";
				}
			}

			Prompter.FatalError(strMsg);
			m_pMainWnd->SendMessage(WM_CLOSE, 0, 0);
			return FALSE;
		}

		//POSUTILITY TESTER
		if ( SysInfo.IsPosTraySystem() == TRUE )
		{
			CString strExe = ".\\spostray.exe";

			if ( ::FileExists( strExe ) == TRUE )
			{
				CFilenameUpdater FnUp ( SysFiles::SchedulerQuit );
				CString strQuitToken = FnUp.GetFilenameToUse();

				CString strVersion = szVERSION_ABOUT;
				if ( SSspawnl( m_pMainWnd, P_WAIT, strExe, strExe, "201", strVersion, NULL ) == 201 )
				{
					CFileRemover FileRemover( strQuitToken );

					CString strLegacyFolder = "Email";
					GetSyssetProgramPath( strLegacyFolder );

					CString strActiveFolder = "PosTray";
					GetSyssetProgramPath( strActiveFolder );

					if ( ::ExistSubdirectory( strLegacyFolder ) == TRUE )
					{
						if ( ::ExistSubdirectory( strActiveFolder ) == FALSE )
						{
							CPropertySheetPosTrayOptions propSheet;
							propSheet.BuildTaskIndex();
							propSheet.SaveTaskChanges( TRUE );
						}
					}

					rename( strLegacyFolder, strActiveFolder + "_OLD" );
				}
				else
				{	
					if ( ::FileExists( strQuitToken ) == FALSE )
					{
						CSSFile fileQuitToken;
						if ( fileQuitToken.Open( strQuitToken, "wb" ) == TRUE )
						{
							COleDateTime timeNow = COleDateTime::GetCurrentTime();

							CString strMsg;
							strMsg.Format ( "%2.2d/%2.2d/%4.4d, %2.2d:%2.2d:%2.2d",
								timeNow.GetDay(),
								timeNow.GetMonth(),
								timeNow.GetYear(),
								timeNow.GetHour(),
								timeNow.GetMinute(),
								timeNow.GetSecond() );

							fileQuitToken.WriteLine( strMsg );
						}
					}

					CString strMsg = "";
					strMsg += "The PosTray utility that is installed on this PC does not have the same\n";
					strMsg += "version number as the Ecr Manager back office.\n\n";
					strMsg += "PosTray cannot be used on this PC unless its version number matches\n";
					strMsg += "that of Ecr Manager.\n\n";
					strMsg += "Please ask your dealer to install the current release versions of PosTray\n";
					strMsg += "and Ecr Manager if you wish to use PosTray on this PC."; 
					Prompter.Error( strMsg );
				}
			}
		}
		
		//CREATE THE FOLDER FOR MISC EPOS REPORTS
		CreateSyssetSubdirectory ( "SchedLog" );
		CreateSyssetSubdirectory ( "SchedOld" );
		
		//LOG PASSWORD ENTRY IF REQUIRED
		if ( m_bLogPassword == TRUE ) LogPassword ( FALSE );
			
		if ( FALSE == SysInfo.GetScriptModeFlag() )
		{
			//PROMPT TO CREATE A NEW ECR SYSTEM IF NO EXISTING SYSTEM
			//WAS FOUND, OTHERWISE CHECK FOR UNRECOGNISED ECR MODELS 
			if ( ( (CMainFrameEcrman*) m_pMainWnd )-> CheckSystem() == FALSE )
			{
				m_pMainWnd -> SendMessage ( WM_CLOSE, 0, 0 );
				return FALSE;
			}

			//PROMPT TO UPGRADE FROM V3 IF THE UPGRADE MODULE IS PRESENT AND
			//THERE IS NO SIGNIFICANT DATA IN THE V4 SYSTEM
			if ( ( (CMainFrameEcrman*) m_pMainWnd )->CheckForLegacySystem() == FALSE )
			{
				m_pMainWnd -> SendMessage ( WM_CLOSE, 0, 0 );
				return FALSE;
			}
		}

		//ADD DEFAULT PMS ITEMS IF REQUIRED
		( (CMainFrameEcrman*) m_pMainWnd ) -> CheckPMSSystem();
		
		//VALIDATE MAIN SYSTEM DATABASE
		DatabaseValidator.SetBusyFlag( TRUE );
		DatabaseValidator.Run( TRUE, NULL );
		DatabaseValidator.SetBusyFlag( FALSE );

		if ( DatabaseValidator.CancelAtStartup() == TRUE )
		{
			m_pMainWnd -> SendMessage ( WM_CLOSE, 0, 0 );
			return FALSE;
		}
		
		//DISPLAY THE APPROPRIATE OPTIONS FOR THIS USER
		( (CMainFrameEcrman*) m_pMainWnd )->SetPasswordMenuAccess();
		m_pMainWnd->DrawMenuBar();

		if ( DatabaseValidator.ValidAtStartup() == FALSE )
		{
			DatabaseValidator.WriteQuitToken();
			DatabaseValidator.ShowErrorMessage();
		}
		else
		{
			DatabaseValidator.DeleteQuitToken();
			CommsHandshaker.InitialiseComms();
			CommsHandshaker.BeginComms();
			CommsHandshaker.ResumeComms();

			if (SysInfo.IsPosTraySystem() == TRUE)
			{
				CPosTrayTokenFiles::CreateRetryEmailFile();
			}

			CustomerAutoImporterEOD.SetReadyToGoFlag( TRUE );
			CustomerAutoDeleterAccount.SetReadyToGoFlag( TRUE );
			CustomerAutoDeleterOneOff.SetReadyToGoFlag( TRUE );
			PluAutoImporterCSV.SetReadyToGoFlag( TRUE );
			PluAutoImporterEOD.SetReadyToGoFlag( TRUE );
			PluAutoImporterEODPriceSet.SetReadyToGoFlag( TRUE );
			BarcodeAutoImporter.SetReadyToGoFlag( TRUE );
			AutoBackupProcessor.ResetNextAttemptTime();
			AutoBackupProcessor.SetReadyToGoFlag( TRUE );
			BatchSalesExporter.SetReadyToGoFlag( TRUE );

			if (SysInfo.IsIDraughtExportSystem() == TRUE)
			{
				IDraughtExportManager.BuildNetworkMap();
			}

			if (SysInfo.IsSageExportSystem() == TRUE)
			{
				SageExportManager.BuildDatabaseMap();
			}
		}

		AutoExpireHistory.Read();
		
		CDemoWarning DemoWarning;
		
		if (EcrmanOptions.GetStartupMessageBoxFlag() == TRUE)
		{
			m_pMainWnd->PostMessage(WM_STARTUPMSG, 0, 0);
		}

		return TRUE;
	}
}

/**********************************************************************/

bool CEcrmanApp::InitialiseSystem()
{
	{
		CString strPathClient = "";
		GetPCPathClient(strPathClient);
		CreateSubdirectory(strPathClient);

		CString strPathHost = "";
		GetPCPathHost(strPathHost);

		if (strPathHost != strPathClient)
		{
			CreateSubdirectory(strPathHost);
		}
	}

#ifdef COMPILE_PMS
	if ( SysInfo.IsPMSSystem() == TRUE )
		PMSOptions.CreatePMSFolders();
#endif

	if ( SysInfo.Read() == FALSE )
	{
		if ( SysInfo.GetFatalReadErrorFlag() )
		{
			Prompter.FatalReadError ( SysFiles::System );
			return FALSE;
		}
	}

	if ( DealerFlags.ReadFromFile() == FALSE )
	{
		if ( DealerFlags.GetFatalReadErrorFlag() )
		{
			Prompter.FatalReadError ( SysFiles::DealerFlags );
			return FALSE;
		}
	}
	
	if ( BarcodePluNoTable.Read() == FALSE )
	{
		if ( BarcodePluNoTable.GetFatalReadErrorFlag() )
		{
			Prompter.FatalReadError ( SysFiles::BarcodeEntryTable );
			return FALSE;
		}
	}

	PriceSetItemManager.Reset();

	if ( PasswordArray.Read() == FALSE )
	{
		if ( PasswordArray.GetFatalReadErrorFlag() )
		{
			Prompter.FatalReadError ( SysFiles::Passwords );
			return FALSE;
		}
	}

	PasswordArray.BuildTickNameList();

	if ( EcrmanOptions.Read() == FALSE )
	{
		if ( EcrmanOptions.GetFatalReadErrorFlag() )
		{
			Prompter.FatalReadError ( SysFiles::LegacyEcrmanOptions );
			return FALSE;
		}
	}
	DataManagerNonDb.FixTaxFunction();

	{
		SpecialKeys.Read();
		CFilenameUpdater FnUp( SysFiles::SpecialKeys );
		if ( ::FileExists( FnUp.GetFilenameToUse() ) == FALSE )
			SpecialKeys.Write();
	}
	
	if ( ButtonOptions.Read() == FALSE )
	{
		if ( ButtonOptions.GetFatalReadErrorFlag() )
		{
			Prompter.FatalReadError ( SysFiles::ButtonOptions );
			return FALSE;
		}
	}

	ButtonManager.ReadButtonActions();

	if ( EmailOptions.Read() == FALSE )
	{
		if ( EmailOptions.GetFatalReadErrorFlag() )
		{
			Prompter.FatalReadError ( SysFiles::EmailOptions );
			return FALSE;
		}
	}

	if ( PCOptionsClient.Read() == FALSE )
	{
		if ( PCOptionsClient.GetFatalReadErrorFlag() )
		{
			Prompter.FatalReadError ( SysFiles::PCOptionsClient );
			return FALSE;
		}
	}

	if (PCOptionsHost.Read() == FALSE)
	{
		if (PCOptionsHost.GetFatalReadErrorFlag())
		{
			Prompter.FatalReadError(SysFiles::PCOptionsHost);
			return FALSE;
		}
	}

	if ( NetworkResources.Read() == FALSE )
	{
		if ( NetworkResources.GetFatalReadErrorFlag() )
		{
			Prompter.FatalReadError ( SysFiles::NetworkResources );
			return FALSE;
		}
	}
	NetworkResources.RunBatchFile();

	if ( PosTrayOptions.Read() == FALSE )
	{
		if ( PosTrayOptions.GetFatalReadErrorFlag() )
		{
			Prompter.FatalReadError ( SysFiles::PosTrayOptions );
			return FALSE;
		}
	}

	m_bLogPassword = ( EcrmanOptions.GetPasswordsLogFlag() != 0 );

	if ( AutoBackupOptions.Read() == FALSE )
	{
		if ( AutoBackupOptions.GetFatalReadErrorFlag() )
		{
			Prompter.FatalReadError ( SysFiles::AutoBackupOptions );
			return FALSE;
		}
	}

#ifdef COMPILE_PMS

	if ( SysInfo.IsPMSSystem() == TRUE )
	{
		if ( PMSOptions.Read() == FALSE )
		{
			if ( PMSOptions.GetFatalReadErrorFlag() )
			{
				Prompter.FatalReadError ( SysFiles::PMSOptionsNew );
				return FALSE;
			}
		}
	}

#endif

	if ( SysInfo.IsIDraughtExportSystem() == TRUE )
	{
		if ( IDraughtOptions.Read() == FALSE )
		{
			if ( IDraughtOptions.GetFatalReadErrorFlag() )
			{
				Prompter.FatalReadError ( SysFiles::IDraughtOptions );
				return FALSE;
			}
		}
	}

	if ( SysInfo.IsSageExportSystem() == TRUE )
	{
		if ( SageOptions.Read() == FALSE )
		{
			if ( SageOptions.GetFatalReadErrorFlag() )
			{
				Prompter.FatalReadError ( SysFiles::SageOptions );
				return FALSE;
			}
		}
	}

	SystemMapFull.CreateSystem();
	SystemMapFull.FixWebSalesNetworks();
	NetworkTerminalMap.BuildMap();
	CPosTrayTokenFiles::CreateSystemUpdateFile();
	CashRSPClearBufferThreadManager.Init();
	
	bool bWriteOptions = FALSE;
	bool bWriteDealerFlags = FALSE;
	
	if ( dbReportpoint.GetSize() != 0 )
	{
		EcrmanOptions.SetReportsReportpointFlag ( TRUE );
		bWriteOptions = TRUE;
	}

	if ( dbLocationSet.GetSize() != 0 )
	{
		EcrmanOptions.SetReportsLocationSetFlag ( TRUE );
		bWriteOptions = TRUE;
	}

	if ( bWriteOptions == TRUE )
		EcrmanOptions.Write();

	if ( bWriteDealerFlags == TRUE )
		DealerFlags.WriteToFile();
	  
	return TRUE;
}

/**********************************************************************/

void CEcrmanApp::LogEntry ( bool bLogOut )
{
	MEMORYSTATUSEX MemoryStatus{};
	MemoryStatus.dwLength = sizeof( MemoryStatus );
	GlobalMemoryStatusEx( &MemoryStatus );
	
	CString strMemory = "";
	strMemory.Format ( "(%s:%I64d,%d)",
		(const char*) Sysset.GetComputerName(),
		MemoryStatus.ullTotalPhys / 1048576, 
		MemoryStatus.dwMemoryLoad );
	
	int nMessageType = 1;

	CString strMessage;
	if ( bLogOut == FALSE )
	{
		if ( SysInfo.IsEcrLiteSystem() == FALSE )
		{
			strMessage.Format ( "Start Ecrman %s %s", 
				(const char*) Sysset.GetVersion( szVERSION_ABOUT ), 
				(const char*) strMemory );
		}
		else
		{
			strMessage.Format ( "Start Ecrman Lite %s %s", 
				(const char*) Sysset.GetVersion( szVERSION_ABOUT ), 
				(const char*) strMemory );
		}
		
		nMessageType = 1;
	}
	else
	{
		if ( SysInfo.IsEcrLiteSystem() == FALSE )
		{
			strMessage.Format ( "Stop Ecrman %s %s %s", 
				(const char*) Sysset.GetVersion( szVERSION_ABOUT ),
				(const char*) strMemory,
				(const char*) m_strStartTime );
		}
		else
		{
			strMessage.Format ( "Stop Ecrman Lite %s %s %s", 
				(const char*) Sysset.GetVersion( szVERSION_ABOUT ),
				(const char*) strMemory,
				(const char*) m_strStartTime );
		}

		nMessageType = 2;
	}

	CMessageLoggerInfo info;
	info.m_strMessage = strMessage;
	info.m_nHideLevel = 2;
	info.m_bAddNetworkName = FALSE;
	info.m_bLogSystemStartStop = TRUE;

	MessageLogger.LogSchedulerMessage ( info, FALSE, TRUE, FALSE );
	MessageLogger.LogSystem ( 7, nMessageType, TRUE, strMemory );
}

/**********************************************************************/

void CEcrmanApp::LogPassword ( bool bLogOut )
{
	CTime tm = CTime::GetCurrentTime();
	CString strTime = tm.Format ( "%H:%M:%S" );
	CString strDate = tm.Format ( "%a %b %d %Y" );

	CCSV csv ( '\t' );

	csv.Add ( PasswordArray.GetUserNo() );
	csv.Add ( PasswordArray.GetUserName() );
	csv.Add ( strDate );
	csv.Add ( strTime );
	csv.Add ( ( bLogOut == TRUE ) ? "Logout" : "Login" );
	
	if ( ::FileExists ( Super.UserLog() ) == FALSE )
	{
		CReportFile ReportFile;
		ReportFile.Open ( Super.UserLog() );
		ReportFile.SetStyle1 ( "User Log" );
		ReportFile.AddColumn ( "No", TA_RIGHT, 100 );
		ReportFile.AddColumn ( "User Name", TA_LEFT, 400 );
		ReportFile.AddColumn ( "Date", TA_LEFT, 400 );
		ReportFile.AddColumn ( "Time", TA_LEFT, 200 );
		ReportFile.AddColumn ( "Action", TA_LEFT, 200 );
		ReportFile.WriteLine ( csv.GetLine() );
		ReportFile.Close();
	}
	else
	{
		CSSFile fileLog;
		if ( fileLog.Open ( Super.UserLog(), "ab" ) == TRUE )
			fileLog.WriteLine ( csv.GetLine() );
	}
}

/**********************************************************************/

BOOL CEcrmanApp::OnIdle(LONG lCount) 
{
	BOOL bResult = TRUE;

	if ( CWinApp::OnIdle(lCount) == FALSE )
	{
		if ( TRUE == SysInfo.GetScriptModeFlag() )
			bResult = FALSE;
		else
		{
			if ( lCount >= 2 )
			{
				IdleTimeManager.DoNextTask( m_pMainWnd );	
				bResult = FALSE;
			}
		}
	}

	return bResult;
}

/**********************************************************************/

int CEcrmanApp::ExitInstance() 
{
	if ( m_bLogEntry == TRUE )
		LogEntry ( TRUE );
	
	if ( m_bLogPassword == TRUE )
		LogPassword ( TRUE );

	CommsHandshaker.TerminateThread();
	CashRSPClearBufferThreadManager.TerminateThread();
	return CWinApp::ExitInstance();
}

/**********************************************************************/
