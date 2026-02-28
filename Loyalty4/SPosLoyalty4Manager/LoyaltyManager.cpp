//***************************************************************************
#include "..\SPosLoyalty4Shared\FileRemover.h"
#include "..\SPosLoyalty4Shared\GlobalState.h"
#include "..\SPosLoyalty4Shared\GroupHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\LoyaltySubs.h"
#include "..\SPosLoyalty4Shared\MessageLogger.h"
#include "..\SPosLoyalty4Shared\PresentationOptions.h"
#include "..\SPosLoyalty4Shared\ServerData.h"
#include "..\SPosLoyalty4Shared\ServiceOptions.h"
#include "..\SPosLoyalty4Shared\SQLConnectionOptions.h"
#include "..\SPosLoyalty4Shared\SQLConnectionPool.h"
#include "..\SPosLoyalty4Shared\SQLDb.h"
#include "..\SPosLoyalty4Shared\SQLStampOfferBuffer.h"
#include "..\SPosLoyalty4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
#include "..\SPosLoyalty4Shared\TrackedFile.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//**************************************************************************
#include "AuditFix.h"
#include "AutoBackupOptions.h"
#include "AutoBackupProcessor.h"
#include "DbExportHelpers.h"
#include "DefTextEmailAddress.h"
#include "EmailOptions.h"
#include "FontTable.h"
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
#include "LoyaltyManagerDlg.h"
#include "PasswordData.h"
#include "PasswordLogger.h"
#include "PCOptions.h"
#include "SQLScriptExport.h"
#include "SQLTranScriptImport.h"
#include "TooManyUsersDlg.h"
//**************************************************************************
#include <CkSettings.h>
//**************************************************************************
extern const char* szVERSION_LOYALTY_ABOUT;
extern const char* szUPDATE_TOKEN_DATE;
//**************************************************************************

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
	virtual void ParseParam ( const char* szParam, BOOL bFlag, BOOL bLast );
	int GetSize() { return m_arrayParam.GetSize(); }
	const char* GetAt ( int n ) { return m_arrayParam.GetAt(n); }

private:
	CStringArray m_arrayParam;
};

void CMyCommandLineInfo::ParseParam ( const char* szParam, BOOL bFlag, BOOL bLast )
{
	if ( bFlag == FALSE )			// discard /p for printing etc
		m_arrayParam.Add ( szParam );

	CCommandLineInfo::ParseParam ( szParam, bFlag, bLast );
}

//******************************************************************
// CLoyaltyManagerApp

BEGIN_MESSAGE_MAP(CLoyaltyManagerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


//******************************************************************
// CLoyaltyManagerApp construction

CLoyaltyManagerApp::CLoyaltyManagerApp()
{
	CKUnlock();
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

//******************************************************************
// The one and only CLoyaltyManagerApp object

CAutoBackupOptions AutoBackupOptions;
CAutoBackupProcessor AutoBackupProcessor;
CDbExportHelpers DbExportHelpers;
CDefTextEmailAddress EmailAddresses;
CEmailOptions EmailOptions;
CFontTable FontTable;
CGlobalState g_GlobalState;
CGroupHeaderHelpers GroupHeaderHelpers;
CLoyaltyHeaderHelpers LoyaltyHeaderHelpers;
CLoyaltyManagerApp theApp;
CLoyaltySubs LoyaltySubs;
CMessageLogger MessageLogger;
CPasswordData Passwords;
CPasswordLogger PasswordLogger;
CPCOptionsClient PCOptionsClient;
CPCOptionsHost PCOptionsHost;
CServerData Server;
CServerData ServerCopy;
CServiceOptions	ServerServiceOptions;
CSQLConnectionOptions SQLConnectionOptions;
CSysset Sysset;
CSystemData System;
CFilenameHandler Filenames;
CPresentationOptions PresentationOptions;
CPrompter Prompter;
CSQLStampOfferBuffer SQLStampOfferBuffer;
//******************************************************************
CSQLRowSetAuditPeriod SQLRowSetAuditPeriod;
//******************************************************************
CSQLConnectionPool SQLConnectionPool;
//******************************************************************
// CLoyaltyManagerApp initialization

BOOL CLoyaltyManagerApp::InitInstance()
{
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
			exit(Sysset.ConvertStringVersionToInt(szVERSION_LOYALTY_ABOUT));
			return FALSE;
		}

		if (strCommand == "SERIALNO")
		{
			if (cmdInfo.GetSize() >= 2)
			{
				strPath = cmdInfo.GetAt(1);
			}
		}

		int nSerialNo = Sysset.GetRegTokenOrLegacyEmbeddedSerialNo(SYSSET_LOYALTY4, strPath, szVERSION_LOYALTY_ABOUT);

		if (strCommand == "SERIALNO")
		{
			exit(987000000 + nSerialNo);
			return FALSE;
		}

		if ((nSerialNo >= 9000) && (nSerialNo <= 999999))
		{
			CString strFilename = ".\\SERIALNO.DAT";

			if (SolutionGlobalFunctions::FileExists(strFilename) == FALSE)
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

	bool bSpawned = FALSE;
	bool bCreatePCId = TRUE;
	int nDummyDatapathIndex = 0;
	if ( Sysset.TestRegistrationMainExe ( SYSSET_LOYALTY4, NULL, bCreatePCId, szVERSION_LOYALTY_ABOUT, szUPDATE_TOKEN_DATE, bSpawned, nDummyDatapathIndex ) == FALSE )
	{
		CString strError = Sysset.GetError();

		if ( strError.Left(5) == "Too m" )					
		{
			CTooManyUsersDlg dlgTooMany(NULL);
			dlgTooMany.DoModal();
		}
		else
		{
			CString strText = CSysset::DecodeMessage ( ",FK9XK7r-iC9rC7x-`S7X-ICz9SK7X`22FnKS" ); //"Please contact your software supplier"

			CString strMess;
			strMess.Format ( "%s\n\n%s:\n\n ==> %s", 
				(const char*) strError, 
				(const char*) strText, 
				(const char*) Sysset.GetField(4) );

			Prompter.FatalError ( strMess );
		}

		return FALSE;
	}

	{
		CString strPathClient = "";
		GetPCPathClient(strPathClient);

		CString strPathHost = "";
		GetPCPathHost(strPathHost);

		CreateSubdirectory(strPathClient);

		if (strPathClient != strPathHost)
		{
			CreateSubdirectory(strPathHost);
		}
	}

	System.Read();										// read system data 
	PresentationOptions.Read();
	EmailOptions.Read();
	PCOptionsHost.Read();
	PCOptionsClient.Read();
	Passwords.Read ( Filenames.GetPasswordFilename() );
	SQLConnectionOptions.Read();
	g_GlobalState.DetermineODBCDriverVersion(FALSE);

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
	}

	CAuditFix AuditFix;
	AuditFix.DoAuditFix();

// validate plu database

	Server.Read ( Filenames.GetServerDataFilename() );

	{
		CreateSubdirectory(Filenames.GetServicesFolder());

		CSSFile fileVersion;
		if (fileVersion.Open(Filenames.GetServicesVersionFilename(), "wb") == TRUE)
		{
			fileVersion.WriteLine(szVERSION_LOYALTY_ABOUT);
		}
	}

	AutoBackupOptions.Read();
	AutoBackupProcessor.ResetNextAttemptTime();
	AutoBackupProcessor.SetReadyToGoFlag(TRUE);

	CLoyaltyManagerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	
	if (Passwords.GetPasswordInUse() >= 0)
	{
		PasswordLogger.LogEvent(Passwords.GetPasswordInUse(), PASSWORD_EVENT_LOGOFF_SYSTEM);
	}

	return FALSE;
}

//*******************************************************************

int CLoyaltyManagerApp::ExitInstance()
{
	CkSettings::cleanupMemory();
	return CWinApp::ExitInstance();
}

//*******************************************************************