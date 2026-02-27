//****************************************
#include <Windows.h>
#include <WinSvc.h>
//****************************************
#include "..\SmartPay4Shared\EmailOptions.h"
#include "..\SmartPay4Shared\GlobalState.h"
#include "..\SmartPay4Shared\GroupShiftLogger.h"
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\ServiceOptions.h"
#include "..\SmartPay4Shared\SmartPayHeaderHelpers.h"
#include "..\SmartPay4Shared\SQLConnectionOptions.h"
#include "..\SmartPay4Shared\SQLConnectionPool.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLGroupShiftChecker.h"
#include "..\SmartPay4Shared\SQLPurchaseControlBuffer.h"
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
#include "..\SmartPay4Shared\SQLDb.h"
//****************************************
#include "..\SmartPay4ManagerBgnd\DbEXportHelpers.h"
#include "..\SmartPay4ManagerBgnd\DbReportFields.h"
#include "..\SmartPay4ManagerBgnd\PebbleHelpers.h"
//****************************************
#include "..\SmartPay4Bgnd\MessageLogger.h"
//****************************************
#include "BackgroundRegistration.h"
#include "BackgroundWrapper.h"
#include "BusyFileUpdateThreadManager.h"
#include "WinService.h"
//****************************************
#include <CkSettings.h>
//****************************************
#include "SmartPay4Bgnd.h"
//****************************************
CSmartPay4BackgroundApp theApp;
CBackgroundRegistration BackgroundRegistration;
CDbExportHelpers DbExportHelpers;
CDbReportFields DbReportFields;
CEmailOptions EmailOptions;
CGlobalState g_GlobalState;
CGroupShiftLogger GroupShiftLogger;
CFilenameHandler Filenames;
CMessageLogger MessageLogger;
CPasswordData Passwords;
CPebbleHelpers* pPebbleHelpers;
CPrompter Prompter;
CServerData Server;
CServiceOptions BgndServiceOptions(TRUE);
CSQLConnectionOptions SQLConnectionOptions;
CSQLGroupShiftChecker SQLGroupShiftChecker;
CSysset Sysset;
CSystemData System;
//****************************************
CWinService BackgroundService;
//****************************************
CSmartPayHeaderHelpers SmartPayHeaderHelpers;
//****************************************
CSQLConnectionPool SQLConnectionPool;
CSQLRowSetAuditPeriod SQLRowSetAuditPeriod;
CSQLPurchaseControlBuffer SQLPurchaseControlBuffer;
//****************************************
#define RETCODE_OK 0
#define RETCODE_BADPARAMCOUNT 1
#define RETCODE_REGISTRATIONFAIL 2
#define RETCODE_UNKNOWNACTION 3
#define RETCODE_WININITFAIL 4
#define RETCODE_BADDIRECTORY 5
#define RETCODE_UNREGISTERED 6
#define RETCODE_DBACCESS_FAIL 7
//****************************************
extern const char* szVERSION_SMARTPAY_ABOUT;
extern const char* szUPDATE_TOKEN_DATE;
//****************************************
int RegistrationCheck(CSmartPayCommandLineInfo& cmdInfo, CString& strError, int nRegRetryTime);
//*******************************************************************

CSmartPay4BackgroundApp::CSmartPay4BackgroundApp()
{
	m_nRetCode = 0;
}

//*******************************************************************
BEGIN_MESSAGE_MAP(CSmartPay4BackgroundApp, CWinApp)
END_MESSAGE_MAP()
//*******************************************************************

BOOL CSmartPay4BackgroundApp::InitInstance()
{
	CKUnlock();

	pPebbleHelpers = new CPebbleHelpers;
	if (pPebbleHelpers == nullptr)
	{
		Prompter.Error("Unable to allocate memory for Pebble Helpers");
		return FALSE;
	}

	BackgroundService.Init("SmartPayManagerV4BackgroundProcessor",
		"SmartPay Manager V4 Background Processor",
		"SmartPay Manager V4 Background Processor",
		SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PRESHUTDOWN);

	m_nRetCode = RETCODE_OK;

	CSmartPayCommandLineInfo cmdInfo(FALSE);
	ParseCommandLine(cmdInfo);

	if (cmdInfo.GetSize() == 0)
	{
		m_nRetCode = RETCODE_BADPARAMCOUNT;
	}
	else
	{
		CString strCommand = cmdInfo.GetAt(0);
		strCommand.MakeUpper();

		if (strCommand == "<SERVICE>")
		{
			RunService(cmdInfo);
		}
		else if (strCommand.Left(10) == "DESKTOPAPP")
		{
			RunDesktop(cmdInfo);
		}
		else if (strCommand == "COMMAND")
		{
			RunCommand(cmdInfo);
		}
		else
		{
			m_nRetCode = RETCODE_UNKNOWNACTION;

			CString strIndent = "==> ";
			MessageLogger.LogSystemMessage("Unknown SmartPay Service command");
			MessageLogger.LogSystemMessage(strIndent + strCommand);
		}
	}

	return FALSE;
}

//*******************************************************************

void CSmartPay4BackgroundApp::RunService(CSmartPayCommandLineInfo& cmdInfo)
{
	g_GlobalState.SetServerDesktopAppFlag(FALSE);

	if (cmdInfo.GetSize() != 2)
	{
		m_nRetCode = RETCODE_BADPARAMCOUNT;
	}
	else
	{
		m_nRetCode = RETCODE_OK;
		BackgroundRegistration.SetDatapath(cmdInfo.GetAt(1));
		BackgroundService.ServiceRun();
	}
}

//*******************************************************************

void CSmartPay4BackgroundApp::RunDesktop(CSmartPayCommandLineInfo& cmdInfo)
{
	g_GlobalState.SetServerDesktopAppFlag(TRUE);

	if (cmdInfo.GetSize() != 2)
	{
		m_nRetCode = RETCODE_BADPARAMCOUNT;
	}
	else
	{
		int nRegRetryTime = 0;

		{
			CString strAction = cmdInfo.GetAt(0);

			int nLen = strAction.GetLength();
			if (nLen >= 12)
			{
				if (strAction.Mid(10, 1) == "_")
				{
					nRegRetryTime = atoi(strAction.Right(nLen - 11));
					nRegRetryTime = min(nRegRetryTime, 300);
					nRegRetryTime = max(nRegRetryTime, 0);
				}
			}
		}

		CString strError = "";
		m_nRetCode = RegistrationCheck(cmdInfo, strError, nRegRetryTime);

		if (RETCODE_OK == m_nRetCode)
		{
			CBackgroundWrapper BackgroundWrapper;
			BackgroundWrapper.InitSystem(TRUE);

			CBusyFileUpdateThreadManager BFUThreadManager;
			BFUThreadManager.CheckThread();

			while (TRUE)
			{
				BackgroundWrapper.Pulse(BFUThreadManager);

				if (FileExists(Filenames.GetBgndServiceStopFilename()) == TRUE)
				{
					MessageLogger.LogSystemMessage("Shutdown request from SmartPay");
					break;
				}

				Sleep(100);
			}

			BFUThreadManager.ShutdownThread();
		}
		else
		{
			CString strIndent = "==> ";
			MessageLogger.LogSystemMessage("SmartPay Background Processor registration failure");
			MessageLogger.LogSystemMessage(strIndent + strError);
		}
	}
}

//*******************************************************************

void CSmartPay4BackgroundApp::RunCommand(CSmartPayCommandLineInfo& cmdInfo)
{
	BackgroundService.ParseStandardArgs(cmdInfo);

	CSSFile fileResult;
	if (fileResult.Open(".\\SmpBgdCmd.dat", "wb") == TRUE)
	{
		fileResult.WriteLine(BackgroundService.GetError());
	}
}

//*******************************************************************

int RegistrationCheck(CSmartPayCommandLineInfo& cmdInfo, CString& strError, int nRegRetryTime)
{
	strError = "";

	if (cmdInfo.GetSize() != 2)
	{
		strError = "Unknown SmartPay service command line format";
		return RETCODE_BADPARAMCOUNT;
	}

	CString strPath = cmdInfo.GetAt(1);

	if (_chdir(strPath) != 0)
	{
		strError = "Unable to access specified directory";
		return RETCODE_BADDIRECTORY;
	}

	bool bCreatePCId = FALSE;

	ULONGLONG timeGiveUp = GetTickCount64() + (1000 * nRegRetryTime);
	bool bRegistered = FALSE;

	do
	{
		bRegistered = Sysset.TestRegistrationModule(SYSSET_SMARTPAY4, NULL, bCreatePCId, TRUE, 0);

		if (FALSE == bRegistered)
		{
			Sleep(200);
		}
	} while ((FALSE == bRegistered) && (GetTickCount64() < timeGiveUp));

	if (FALSE == bRegistered)
	{
		strError = "Unable to find registered installation of SmartPay Manager in specified directory";
		return RETCODE_UNREGISTERED;
	}
	System.SetGotPCIdFlag(bCreatePCId);

	return RETCODE_OK;
}

//*******************************************************************

int CSmartPay4BackgroundApp::CheckDbAccess()
{
	SQLConnectionOptions.Read();
	g_GlobalState.DetermineODBCDriverVersion(2);

	int nError = 0;
	CSQLDb SQLDb;
	SQLDb.Connect(nError, FALSE);

	if (nError != 0)
	{
		CString strError = "";
		strError.Format("Error code (%d:%d)",
			SQLSRVR_ERRCONTEXT_CONNECT,
			nError);

		CString strIndent = "==> ";
		MessageLogger.LogSystemMessage("Unable to access SmartPay database");
		MessageLogger.LogSystemMessage(strIndent + strError);

		return RETCODE_DBACCESS_FAIL;
	}

	SQLDb.GetDatabaseVersion(nError);
	if (nError != 0)
	{
		CString strError = "";
		strError.Format("Error code (%d:%d)",
			SQLSRVR_ERRCONTEXT_CHECKVERSION,
			nError);

		CString strIndent = "==> ";
		MessageLogger.LogSystemMessage("Incompatible SmartPay database version");
		MessageLogger.LogSystemMessage(strIndent + strError);

		return RETCODE_DBACCESS_FAIL;
	}

	return RETCODE_OK;
}

//*******************************************************************

int CSmartPay4BackgroundApp::ExitInstance()
{
	if (pPebbleHelpers != nullptr)
	{
		delete pPebbleHelpers;
		pPebbleHelpers = nullptr;
	}

	CkSettings::cleanupMemory();
	CWinApp::ExitInstance();
	return m_nRetCode;
}

//*******************************************************************
