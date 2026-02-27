//$$******************************************************************
#include "..\SmartPay4Shared\EmailOptions.h"
#include "..\SmartPay4Shared\FileRemover.h"
#include "..\SmartPay4Shared\GlobalState.h"
#include "..\SmartPay4Shared\GroupHeaderHelpers.h"
#include "..\SmartPay4Shared\GroupShiftLogger.h"
#include "..\SmartPay4Shared\MessageLogger.h"
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\PresentationOptions.h"
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\ServiceOptions.h"
#include "..\SmartPay4Shared\SmartPayHeaderHelpers.h"
#include "..\SmartPay4Shared\SQLConnectionOptions.h"
#include "..\SmartPay4Shared\SQLConnectionPool.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLGroupShiftChecker.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLPurchaseControlBuffer.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLDb.h"
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\DbExportHelpers.h"
#include "..\SmartPay4ManagerBgnd\DbReportFields.h"
#include "..\SmartPay4ManagerBgnd\PebbleHelpers.h"
//$$******************************************************************
#include "..\SmartPay4ManagerEmail\DefTextEmailAddress.h"
//$$******************************************************************
#include "AboutDlg.h"
#include "AuditFileFilter.h"
#include "AutoBackupOptions.h"
#include "AutoBackupProcessor.h"
#include "FontTable.h"
#include "PasswordLogger.h"
#include "PCOptions.h"
#include "PurchaseControlStampDeleteDlg.h"
#include "SmartPayManagerDlg.h"
#include "TooManyUsersDlg.h"
//$$******************************************************************
#include <CkGlobal.h>
#include <CkJsonArray.h>
#include <CkJsonObject.h>
#include <CkSettings.h>
#include <CkStringBuilder.h>
//$$******************************************************************
#include "SmartPay4Manager.h"
//$$******************************************************************
extern const char* szVERSION_SMARTPAY_ABOUT;
extern const char* szUPDATE_TOKEN_DATE;
//$$******************************************************************

/********************************************************************/
/*	Parameters														*/
/********************************************************************/
/*																	*/
/*	smartman {nDatapathIndex}										*/
/*																	*/
/*	{nDatapathIndex}	index to pass onto Sysset testregistration	*/
/*																	*/
/********************************************************************/

class CMyCommandLineInfo : public CCommandLineInfo
{
public:
	CMyCommandLineInfo();

public:
	virtual void ParseParam ( const char* szParam, BOOL bFlag, BOOL bLast );
	int GetSize() { return m_arrayParam.GetSize(); }
	const char* GetAt ( int n ) { return m_arrayParam.GetAt(n); }

private:
	CStringArray m_arrayParam;
};

//$$******************************************************************

CMyCommandLineInfo::CMyCommandLineInfo() : CCommandLineInfo()
{
}

//$$******************************************************************

void CMyCommandLineInfo::ParseParam ( const char* szParam, BOOL bFlag, BOOL bLast )
{
	if ( bFlag == TRUE )					// see if a flag ( strips of  '/' character )
	{
		CString strFlag = szParam;
	}
	else
	{
		m_arrayParam.Add ( szParam );
	}

	CCommandLineInfo::ParseParam ( szParam, bFlag, bLast );
}

//$$******************************************************************
BEGIN_MESSAGE_MAP(CSmartPayManagerApp, CWinApp)
	ON_COMMAND(ID_HELP, OnHelp)
END_MESSAGE_MAP()
//$$******************************************************************

CSmartPayManagerApp::CSmartPayManagerApp()
{
}

//$$******************************************************************
CSmartPayManagerApp		theApp;
CAutoBackupOptions		AutoBackupOptions;
CAutoBackupProcessor	AutoBackupProcessor;
CDbExportHelpers		DbExportHelpers;
CDbReportFields			DbReportFields;
CDefTextEmailAddress	EmailAddresses;
CEmailOptions			EmailOptions;
CGlobalState			g_GlobalState;
CGroupShiftLogger		GroupShiftLogger;
CMessageLogger			MessageLogger;
CPasswordData			Passwords;
CPasswordLogger			PasswordLogger;
CPCOptionsClient		PCOptionsClient;
CPCOptionsHost			PCOptionsHost;
CServiceOptions			BgndServiceOptions( TRUE );
CServiceOptions			ServerServiceOptions( FALSE );
CSysset					Sysset;
CServerData				Server;
CServerData				ServerCopy;
CSQLConnectionOptions	SQLConnectionOptions;
CSQLGroupShiftChecker	SQLGroupShiftChecker;
CSystemData				System;
CFilenameHandler		Filenames;
CFontTable				FontTable;
CPebbleHelpers*			pPebbleHelpers;
CPresentationOptions	PresentationOptions;
CPrompter				Prompter;
//$$******************************************************************
CSQLRowSetAuditPeriod				SQLRowSetAuditPeriod;
//$$******************************************************************
CSQLConnectionPool					SQLConnectionPool;
//$$******************************************************************
CSSAutoShutdownHelper				AutoShutdownHelper;
CSmartPayHeaderHelpers				SmartPayHeaderHelpers;
CGroupHeaderHelpers					GroupHeaderHelpers;
//$$******************************************************************
CSQLPurchaseControlBuffer SQLPurchaseControlBuffer;
//$$******************************************************************

BOOL CSmartPayManagerApp::InitInstance()
{
	CWinApp::InitInstance();
	CKUnlock();

	pPebbleHelpers = new CPebbleHelpers;
	if ( pPebbleHelpers == nullptr )
	{
		Prompter.FatalError ( "Unable to allocate memory for Pebble Helpers" );
		return FALSE;
	}
	
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

	CMyCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	{
		CString strCommand = "";
		CString strPath = "";

		if (cmdInfo.GetSize() >= 1)
		{
			strCommand = cmdInfo.GetAt(0);
		}

		strCommand.MakeUpper();

		if (strCommand == "VERSIONNO")
		{
			exit(Sysset.ConvertStringVersionToInt(szVERSION_SMARTPAY_ABOUT));
			return FALSE;
		}

		if (strCommand == "SERIALNO")
		{
			if (cmdInfo.GetSize() >= 2)
			{
				strPath = cmdInfo.GetAt(1);
			}
		}

		int nSerialNo = Sysset.GetRegTokenOrLegacyEmbeddedSerialNo(SYSSET_SMARTPAY4, strPath, szVERSION_SMARTPAY_ABOUT);

		if (strCommand == "SERIALNO")
		{
			exit(987000000 + nSerialNo);
			return FALSE;
		}

		if ((nSerialNo >= 9000) && (nSerialNo <= 999999))
		{
			CString strFilename = ".\\SERIALNO.DAT";

			if (::FileExists(strFilename) == FALSE)
			{
				CSSFile fileSerialNo;
				if (fileSerialNo.Open(strFilename, "wb") == TRUE)
				{
					CString strNo;
					strNo.Format("%d", nSerialNo);
					fileSerialNo.WriteLine(strNo);
				}
			}
		}
	}

	bool bCreatePCId = TRUE;
	int nDummyDatapathIndex = 0;
	bool bTestSysset = Sysset.TestRegistrationMainExe(SYSSET_SMARTPAY4, NULL, bCreatePCId, szVERSION_SMARTPAY_ABOUT, szUPDATE_TOKEN_DATE, FALSE, nDummyDatapathIndex);

	if (FALSE == bTestSysset)
	{
		CString strError = Sysset.GetError();

		if (strError.Left(5) == "Too m")
		{
			CTooManyUsersDlg dlgTooMany(NULL);
			dlgTooMany.DoModal();
		}
		else
		{
			CString strText = CSysset::DecodeMessage(",FK9XK7r-iC9rC7x-`S7X-ICz9SK7X`22FnKS"); //"Please contact your software supplier"

			CString strMess;
			strMess.Format("%s\n\n%s:\n\n ==> %s",
				(const char*)strError,
				(const char*)strText,
				(const char*)Sysset.GetField(4));

			Prompter.FatalError(strMess);
		}

		return FALSE;
	}

	{
		CString strPath = "";
		GetPCPathHost(strPath);
		CreateSubdirectory(strPath);
	}

	{
		CString strPath = "";
		GetPCPathClient(strPath);
		CreateSubdirectory(strPath);
	}

	System.Read();											// read system data
	PresentationOptions.Read();
	EmailOptions.Read();
	PCOptionsClient.Read();
	PCOptionsHost.Read();
	BgndServiceOptions.Read();
	ServerServiceOptions.Read();
	Passwords.Read ( Filenames.GetPasswordFilename() );
	SQLConnectionOptions.Read();
	g_GlobalState.DetermineODBCDriverVersion(0);

	//ACCESS OR CREATE SQL DATABASE
	{
		CSQLDb SQLDb;

		{
			int nErrorNumber = 0;
			
			SQLDb.Connect(nErrorNumber, TRUE);

			if (nErrorNumber != 0)
			{
				CString strError = "";
				strError.Format("Unable to open database\n\nError code (%d:%d)",
					SQLSRVR_ERRCONTEXT_CONNECT,
					nErrorNumber);

				Prompter.FatalError(strError);
				return 0;
			}
		}

		{
			int nErrorContext = 0;
			int nErrorNumber = 0;
			SQLDb.CreateDatabaseSchema(nErrorContext, nErrorNumber);

			if (nErrorContext != 0)
			{
				CString strError = "";
				strError.Format("Unable to create or update database schema\n\nError code (%d:%d)",
					nErrorContext,
					nErrorNumber);

				Prompter.FatalError(strError);
				return 0;
			}
		}

		CSQLRepositoryUserText repo;
		repo.CreateDefaultTexts(NULL);
	}

	if ( System.GetAuditFileFilterFlag() == TRUE )
	{
		CAuditFileFilter auditFileFilter;
		auditFileFilter.FilterAuditFile();
		return FALSE;
	}

	CPurchaseControlStampDeleteDlg::AutoDeleteExpiredStamps();

	AutoBackupOptions.Read();
	AutoBackupProcessor.ResetNextAttemptTime();
	AutoBackupProcessor.SetReadyToGoFlag(TRUE);

	return StartStandardVersion();
}

//$$****************************************************************

int CSmartPayManagerApp::ExitInstance()
{
	if ( pPebbleHelpers != nullptr )
	{
		delete pPebbleHelpers;
		pPebbleHelpers = nullptr;
	}

	CkSettings::cleanupMemory();	
	
	return CWinApp::ExitInstance();
}

//$$****************************************************************

bool CSmartPayManagerApp::StartStandardVersion()
{
	{
		CreateSubdirectory( Filenames.GetServicesFolder() );
	
		CSSFile fileVersion;
		if ( fileVersion.Open( Filenames.GetServicesVersionFilename(), "wb" ) == TRUE )
		{
			fileVersion.WriteLine( szVERSION_SMARTPAY_ABOUT );
		}
	}

	int nMinutes = 0;
	switch (System.GetAutoShutdownType())
	{
	case 1:	nMinutes = 1;	break;
	case 2:	nMinutes = 5;	break;		
	case 3:	nMinutes = 10;	break;		
	case 4:	nMinutes = 15;	break;		
	case 5:	nMinutes = 20;	break;		
	case 6:	nMinutes = 30;	break;		
	}

	AutoShutdownHelper.Init( nMinutes ); 
	
	CSmartPayManagerDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	if ( Passwords.GetPasswordInUse() >= 0 )
	{
		PasswordLogger.LogEvent(Passwords.GetPasswordInUse(), PASSWORD_EVENT_LOGOFF_SYSTEM);
	}

	return FALSE;
}

//$$******************************************************************

