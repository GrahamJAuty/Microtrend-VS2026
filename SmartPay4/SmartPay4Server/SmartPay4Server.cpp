//****************************************
#include <Windows.h>
#include <WinSvc.h>
//*******************************************************************
#include "..\SmartPay4Shared\FileRemover.h"
#include "..\SmartPay4Shared\GlobalState.h"
#include "..\SmartPay4Shared\GroupShiftLogger.h"
#include "..\SmartPay4Shared\MessageLogger.h"
#include "..\SmartPay4Shared\Prompter.h"
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\ServiceOptions.h"
#include "..\SmartPay4Shared\SQLConnectionOptions.h"
#include "..\SmartPay4Shared\SQLConnectionPool.h"
#include "..\SmartPay4Shared\SQLGroupShiftChecker.h"
#include "..\SmartPay4Shared\SQLPurchaseControlBuffer.h"
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
//*******************************************************************
#include "ServerRegistration.h"
#include "ServerWrapper.h"
#include "TransactionCache.h"
#include "WinService.h"
//*******************************************************************
#include <CkSettings.h>
//*******************************************************************
#include "SmartPay4Server.h"
//*******************************************************************
CSmartPay4ServerApp	theApp;
//*******************************************************************
CFilenameHandler Filenames;
CGlobalState g_GlobalState;
CGroupShiftLogger GroupShiftLogger;
CMessageLogger MessageLogger;
CPrompter Prompter;
CServiceOptions ServerServiceOptions(FALSE);
CSysset Sysset;
CSystemData System;
CServerData Server;
CServerRegistration ServerRegistration;
CSQLConnectionOptions SQLConnectionOptions;
CSQLGroupShiftChecker SQLGroupShiftChecker;
CTransactionCache TransactionCache;
//*******************************************************************
CSQLConnectionPool SQLConnectionPool;
CSQLRowSetAuditPeriod SQLRowSetAuditPeriod;
CSQLPurchaseControlBuffer SQLPurchaseControlBuffer;
//*******************************************************************
CWinService ServerService;
//*******************************************************************
extern const char* szVERSION_SMARTPAY_ABOUT;
extern const char* szUPDATE_TOKEN_DATE;
//*******************************************************************
int RegistrationCheck(CSmartPayCommandLineInfo& cmdInfo, CString& strError, int nRegRetryTime);
//*******************************************************************
#define RETCODE_OK 0
#define RETCODE_BADPARAMCOUNT 1
#define RETCODE_REGISTRATIONFAIL 2
#define RETCODE_UNKNOWNACTION 3
#define RETCODE_WININITFAIL 4
#define RETCODE_BADDIRECTORY 5
#define RETCODE_UNREGISTERED 6
#define RETCODE_DBACCESS_FAIL 7
//*******************************************************************

CSmartPay4ServerApp::CSmartPay4ServerApp()
{
	m_nRetCode = 0;
}

//*******************************************************************
BEGIN_MESSAGE_MAP(CSmartPay4ServerApp, CWinApp)
END_MESSAGE_MAP()
//*******************************************************************

BOOL CSmartPay4ServerApp::InitInstance()
{
	CKUnlock();

	ServerService.Init("SmartPayManagerV4Server", "SmartPay Manager V4 Server", "SmartPay Manager V4 Server", SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN);

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

void CSmartPay4ServerApp::RunService(CSmartPayCommandLineInfo& cmdInfo)
{
	g_GlobalState.SetServerDesktopAppFlag(FALSE);

	if (cmdInfo.GetSize() != 2)
	{
		m_nRetCode = RETCODE_BADPARAMCOUNT;
	}
	else
	{
		m_nRetCode = RETCODE_OK;
		ServerRegistration.SetDatapath(cmdInfo.GetAt(1));
		ServerService.ServiceRun();
	}
}

//*******************************************************************

void CSmartPay4ServerApp::RunDesktop(CSmartPayCommandLineInfo& cmdInfo)
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
			m_nRetCode = CheckDbAccess();

			if (RETCODE_OK == m_nRetCode)
			{
				CServerWrapper ServerWrapper;
				ServerWrapper.InitSystem(TRUE);

				while (TRUE)
				{
					ServerWrapper.Pulse();

					if (FileExists(Filenames.GetServerServiceStopFilename()) == TRUE)
					{
						MessageLogger.LogSystemMessage("Shutdown request from SmartPay");
						ServerWrapper.ShutdownThreads();
						break;
					}

					if (FileExists(Filenames.GetServerUpdatePluReqFilename()) == TRUE)
					{
						ServerWrapper.ProcessUpdatePluRequest();
					}

					Sleep(50);
				}
			}
		}
		else
		{
			CString strIndent = "==> ";
			MessageLogger.LogSystemMessage("SmartPay Server registration failure");
			MessageLogger.LogSystemMessage(strIndent + strError);
		}
	}
}

//*******************************************************************

void CSmartPay4ServerApp::RunCommand(CSmartPayCommandLineInfo& cmdInfo)
{
	ServerService.ParseStandardArgs(cmdInfo);

	CSSFile fileResult;
	if (fileResult.Open(".\\SmpSrvCmd.dat", "wb") == TRUE)
	{
		fileResult.WriteLine(ServerService.GetError());
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

int CSmartPay4ServerApp::CheckDbAccess()
{
	SQLConnectionOptions.Read();
	g_GlobalState.DetermineODBCDriverVersion(1);

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

int CSmartPay4ServerApp::ExitInstance()
{
	CkSettings::cleanupMemory();
	CWinApp::ExitInstance();
	return m_nRetCode;
}

//*******************************************************************

