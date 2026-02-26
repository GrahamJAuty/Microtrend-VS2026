#include <Windows.h>
#include <WinSvc.h>
//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_AuditPeriod\SQLRepositoryAuditPeriod.h"
#include "..\SPosLoyalty4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
#include "..\SPosLoyalty4Shared\SQLTable_CCNo\SQLRepositoryCCNo.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\FileRemover.h"
#include "..\SPosLoyalty4Shared\GlobalFunctions.h"
#include "..\SPosLoyalty4Shared\GlobalState.h"
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\LoyaltyLockManager.h"
#include "..\SPosLoyalty4Shared\LoyaltySubs.h"
#include "..\SPosLoyalty4Shared\MessageLogger.h"
#include "..\SPosLoyalty4Shared\ServerData.h"
#include "..\SPosLoyalty4Shared\ServiceOptions.h"
#include "..\SPosLoyalty4Shared\SQLConnectionOptions.h"
#include "..\SPosLoyalty4Shared\SQLConnectionPool.h"
#include "..\SPosLoyalty4Shared\SQLDb.h"
#include "..\SPosLoyalty4Shared\SQLStampOfferBuffer.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_DeptInfo\SQLRepositoryDeptInfo.h"
#include "..\SPosLoyalty4Shared\SQLTable_EcrmanDatabase\SQLRepositoryEcrmanDatabase.h"
#include "..\SPosLoyalty4Shared\SQLTable_ExternalAccount\SQLRepositoryExternalAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SPosLoyalty4Shared\SQLTable_GroupTopupBonus\SQLRepositoryGroupTopupBonus.h"
#include "..\SPosLoyalty4Shared\SQLTable_OfferStamp\SQLRepositoryOfferStamp.h"
#include "..\SPosLoyalty4Shared\SQLTable_PluInfo\SQLRepositoryPluInfo.h"
#include "..\SPosLoyalty4Shared\SQLTable_StaffCard\SQLRepositoryStaffCard.h"
#include "..\SPosLoyalty4Shared\SQLTable_StaffGiftException\SQLRepositoryStaffGiftException.h"
#include "..\SPosLoyalty4Shared\SQLTable_StampOfferInfo\SQLRepositoryStampOfferInfo.h"
//*******************************************************************
#include "ServerRegistration.h"
#include "ServerWrapper.h"
#include "SQLStaffAccountBuffer.h"
#include "TransactionCache.h"
#include "WinService.h"
//*******************************************************************
#include <CkSettings.h>
//*******************************************************************
#include "SPosLoyalty4Server.h"
//*******************************************************************
CLoyalty4ServerApp theApp;
CFilenameHandler Filenames;
CGlobalState g_GlobalState;
CLoyaltyHeaderHelpers LoyaltyHeaderHelpers;
CLoyaltySubs LoyaltySubs;
CMessageLogger MessageLogger;
CPrompter Prompter;
CServerRegistration ServerRegistration;
CServiceOptions ServerServiceOptions;
CSQLConnectionOptions SQLConnectionOptions;
CSysset Sysset;
CSystemData System;
CServerData Server;
CServerData ServerCopy;
CTransactionCache TransactionCache;
//*******************************************************************
CSQLRepositoryAccount SQLRepositoryAccount;
CSQLRowSetAuditPeriod SQLRowSetAuditPeriod;
CSQLRepositoryAuditPeriod	SQLRepositoryAuditPeriod;
CSQLRepositoryCCNo SQLRepositoryCCNo;
CSQLRepositoryDeptInfo SQLRepositoryDeptInfo;
CSQLRepositoryEcrmanDatabase SQLRepositoryEcrmanDatabase;
CSQLRepositoryExternalAccount SQLRepositoryExternalAccount;
CSQLRepositoryGroup SQLRepositoryGroup;
CSQLRepositoryGroupTopupBonus SQLRepositoryGroupTopupBonus;
CSQLRepositoryOfferStamp SQLRepositoryOfferStamp;
CSQLRepositoryPluInfo SQLRepositoryPluInfo;
CSQLRepositoryStaffCard SQLRepositoryStaffCard;
CSQLRepositoryStaffGiftException SQLRepositoryStaffGiftException;
CSQLStaffAccountBuffer SQLStaffAccountBuffer;
CSQLStampOfferBuffer SQLStampOfferBuffer;
CSQLRepositoryStampOfferInfo SQLRepositoryStampOfferInfo;
//*******************************************************************
CSQLConnectionPool SQLConnectionPool;
//*******************************************************************
CWinService ServerService;
//*******************************************************************
int RegistrationCheck( CLoyaltyCommandLineInfo& cmdInfo, CString& strError, int nRegRetryTime );
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

CLoyalty4ServerApp::CLoyalty4ServerApp() : CWinApp("")
{
	CKUnlock();
	m_nRetCode = 0;
}

//*******************************************************************
BEGIN_MESSAGE_MAP(CLoyalty4ServerApp, CWinApp)
END_MESSAGE_MAP()
//*******************************************************************
extern const char* szVERSION_LOYALTY_ABOUT;
extern const char* szUPDATE_TOKEN_DATE;
//*******************************************************************

BOOL CLoyalty4ServerApp::InitInstance()
{
	ServerService.Init("LoyaltyManagerV4Server", "Loyalty Manager V4 Server", "Loyalty Manager V4 Server", SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN);

	m_nRetCode = RETCODE_OK;

	CLoyaltyCommandLineInfo cmdInfo;
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
			MessageLogger.LogSystemMessage("Unknown Loyalty Service command");
			MessageLogger.LogSystemMessage(strIndent + strCommand);
		}
	}

	return FALSE;
}

//*******************************************************************

void CLoyalty4ServerApp::RunService(CLoyaltyCommandLineInfo& cmdInfo)
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

void CLoyalty4ServerApp::RunDesktop(CLoyaltyCommandLineInfo& cmdInfo)
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
						MessageLogger.LogSystemMessage("Shutdown request from Loyalty");
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
			MessageLogger.LogSystemMessage("Loyalty Server registration failure");
			MessageLogger.LogSystemMessage(strIndent + strError);
		}
	}
}

//*******************************************************************

void CLoyalty4ServerApp::RunCommand(CLoyaltyCommandLineInfo& cmdInfo)
{
	ServerService.ParseStandardArgs(cmdInfo);

	CSSFile fileResult;
	if (fileResult.Open(".\\LoySrvCmd.dat", "wb") == TRUE)
	{
		fileResult.WriteLine(ServerService.GetError());
	}
}

//*******************************************************************

int RegistrationCheck(CLoyaltyCommandLineInfo& cmdInfo, CString& strError, int nRegRetryTime)
{
	strError = "";

	if (cmdInfo.GetSize() != 2)
	{
		strError = "Unknown Loyalty service command line format";
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
		bRegistered = Sysset.TestRegistrationModule(SYSSET_LOYALTY4, NULL, bCreatePCId, TRUE, 0);

		if (FALSE == bRegistered)
		{
			Sleep(200);
		}
	} while ((FALSE == bRegistered) && (GetTickCount64() < timeGiveUp));

	if (FALSE == bRegistered)
	{
		strError = "Unable to find registered installation of Loyalty Manager in specified directory";
		return RETCODE_UNREGISTERED;
	}
	System.SetGotPCIdFlag(bCreatePCId);

	return RETCODE_OK;
}

//*******************************************************************

int CLoyalty4ServerApp::CheckDbAccess()
{
	SQLConnectionOptions.Read();
	g_GlobalState.DetermineODBCDriverVersion(TRUE);

	//ACCESS LOYALTY DATABASE
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
		MessageLogger.LogSystemMessage("Unable to access Loyalty database");
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
		MessageLogger.LogSystemMessage("Incompatible Loyalty database version");
		MessageLogger.LogSystemMessage(strIndent + strError);

		return RETCODE_DBACCESS_FAIL;
	}

	return RETCODE_OK;
}

//*******************************************************************

int CLoyalty4ServerApp::ExitInstance()
{
	CkSettings::cleanupMemory();
	CWinApp::ExitInstance();
	return m_nRetCode;
}

//*******************************************************************

