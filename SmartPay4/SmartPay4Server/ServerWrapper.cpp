//*******************************************************************
#include "..\SmartPay4Shared\FileRemover.h"
#include "..\SmartPay4Shared\GlobalState.h"
#include "..\SmartPay4Shared\MessageLogger.h"
#include "..\SmartPay4Shared\ServiceOptions.h"
#include "..\SmartPay4Shared\SmartPayLockManager.h"
#include "..\SmartPay4Shared\SQLGroupShiftChecker.h"
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
#include "..\SmartPay4Shared\SQLTable_OperatorName\SQLRepositoryOperatorName.h"
#include "..\SmartPay4Shared\SQLTable_PluInfo\SQLRepositoryPluInfo.h"
#include "..\SmartPay4Shared\SQLTable_PurchaseControlRule\SQLRepositoryPurchaseControlRule.h"
//*******************************************************************
#include "SQLTranImportEposTerminal.h"
//*******************************************************************
#include "ServerWrapper.h"
//*******************************************************************
extern const char* szVERSION_SMARTPAY_ABOUT;
//*******************************************************************

CServerWrapper::CServerWrapper()
{ 
	m_bIsEposRequest = FALSE;
	m_bIsKioskRequest = FALSE;
	m_bIsVendingRequest = FALSE;	
	m_bVersionMatch = FALSE;
	m_strVersionError = "";
	
	m_TrackedFileSystem.SetFilename( Filenames.GetSystemFilenameNew() );
	m_TrackedFileServer.SetFilename( Filenames.GetServerDataFilename() );
	m_TrackedFilePeriod.SetFilename( Filenames.GetAuditPeriodTokenFilename() );
	m_TrackedFileServiceOptions.SetFilename( Filenames.GetServerServiceOptionsFilename() );
	m_TrackedFileSmartPayVersion.SetFilename( Filenames.GetServicesVersionFilename() );

	m_oleNextActionLogTime = COleDateTime::GetCurrentTime();
	m_nLastPurgeTime = 0;

	m_bServerInnerLockOK = FALSE;
	m_bBackgroundLockOK = FALSE;
	m_dwLastOuterLockOKTime = 0;
	m_dwLastOuterLockTryTime = 0;
	m_dwNextOuterLockOffsetSeconds = 1;
	m_strOuterLockStuckTime = "";
}

//*****************************************************************

CServerWrapper::~CServerWrapper()
{
}

//*****************************************************************

void CServerWrapper::InitSystem( bool bForce )
{
	if (m_TrackedFileServer.CheckFile(bForce) == TRUE)
	{
		Server.Read(Filenames.GetServerDataFilename());
		MessageLogger.LogSystemMessage("Read Server Options File");

		if (TRUE == Server.GetCopyLogFilesFlag())
		{
			CreateSubdirectory(Filenames.GetLogCopyFolder());
		}
	}
	
	if ( m_TrackedFileSystem.CheckFile( bForce ) == TRUE )
	{
		System.Read();
		MessageLogger.LogSystemMessage( "Read System Options File" );
	}

	if ( m_TrackedFilePeriod.CheckFile( bForce ) == TRUE )
	{
		SQLRowSetAuditPeriod.LoadPeriods();
		MessageLogger.LogSystemMessage( "Read Audit Period Settings" );
	}

	if ( m_TrackedFileServiceOptions.CheckFile( bForce ) == TRUE )
	{
		ServerServiceOptions.Read();
		MessageLogger.LogSystemMessage( "Read Service Options File" );

		{
			CString strMsg = "";
			strMsg.Format( "PCId = %10.10I64d", ServerServiceOptions.GetPCId() );
			MessageLogger.LogSystemMessage( strMsg );
		}
	}

	if ( m_TrackedFileSmartPayVersion.CheckFile( bForce ) == TRUE )
	{
		CString strManagerVersion = "";
		CString strServerVersion = szVERSION_SMARTPAY_ABOUT;

		{
			CSSFile fileVersion;
			if ( fileVersion.Open( Filenames.GetServicesVersionFilename(), "rb" ) == TRUE )
			{
				fileVersion.ReadString( strManagerVersion );
			}
		}

		MessageLogger.LogSystemMessage( "Read SmartPay Version File" );

		m_bVersionMatch = ( strManagerVersion == strServerVersion );

		if ( FALSE == m_bVersionMatch )
		{
			MessageLogger.LogSystemMessage( "Incompatible SmartPay Manager Version" );

			CString strLine;
			strLine = "==> Manager : ";
			strLine += strManagerVersion;
			MessageLogger.LogSystemMessage( strLine );

			strLine = "==> Service : ";
			strLine += strServerVersion;
			MessageLogger.LogSystemMessage( strLine );

			m_strVersionError.Format("Server version %s (%s)",
				(const char*)strServerVersion,
				(const char*)strManagerVersion);

			if (TRUE == Server.GetBypassVersionSafetyCheckFlag())
			{
				MessageLogger.LogSystemMessage("Bypass Version Safety Check");
				m_bVersionMatch = TRUE;
			}
		}
	}

	//HOLDING FOLDER FOR LOG FILES
	Filenames.CreateLogBufferFolder();
}

//*****************************************************************

void CServerWrapper::Pulse() 
{
	InitSystem(FALSE);
	SQLGroupShiftChecker.CheckGroupShifts();

	bool bCanLogError = FALSE;

	if (FALSE == System.GetGotPCIdFlag())
	{
		bool bCanCreatePCId = FALSE;
		Sysset.SetPCId(bCanCreatePCId);

		if (TRUE == bCanCreatePCId)
		{
			System.SetGotPCIdFlag(TRUE);
			MessageLogger.LogSystemMessage("Got PCId after retry");
		}
	}

	bool bDesktopModeMatch = (ServerServiceOptions.GetAllowDesktopAppFlag() == g_GlobalState.IsServerDesktopApp());

	if (COleDateTime::GetCurrentTime() >= m_oleNextActionLogTime)
	{
		bCanLogError = TRUE;

		{
			CSSFile fileInfo;
			if (fileInfo.Open(Filenames.GetServerServiceActionTimeFilename(), "wb") == TRUE)
			{
				CString strInfo;
				strInfo.Format("PCId = %10.10d",
					Sysset.GetPCIdHost());

				CCSV csv;
				csv.Add(strInfo);
				csv.Add(GetSimpleDateTimeNowString());

				if ((ServerServiceOptions.GetPCId() != Sysset.GetPCIdHost()) || (FALSE == m_bVersionMatch))
				{
					csv.Add(1);
				}

				fileInfo.WriteLine(csv.GetLine());
			}
		}
		
		if (ServerServiceOptions.GetPCId() != Sysset.GetPCIdHost())
		{
			CString strError;
			strError.Format("PCId = %10.10d (%10.10I64d)",
				Sysset.GetPCIdHost(), ServerServiceOptions.GetPCId());

			CSSFile fileInfo;
			if (fileInfo.Open(Filenames.GetServerServiceErrorTimeFilename(), "wb") == TRUE)
			{
				CCSV csv;
				csv.Add(strError);
				csv.Add(GetSimpleDateTimeNowString());
				fileInfo.WriteLine(csv.GetLine());
			}
		}
		else if (FALSE == bDesktopModeMatch)
		{
			CString strError = ServerServiceOptions.GetAllowDesktopAppFlag() ?
				"Service mode not enabled" :
				"Desktop App mode not enabled";

			CSSFile fileInfo;
			if (fileInfo.Open(Filenames.GetServerServiceErrorTimeFilename(), "wb") == TRUE)
			{
				CCSV csv;
				csv.Add(strError);
				csv.Add(GetSimpleDateTimeNowString());
				fileInfo.WriteLine(csv.GetLine());
			}
		}

		m_oleNextActionLogTime = COleDateTime::GetCurrentTime() + COleDateTimeSpan(0, 0, 0, 10);
	}

	if ((ServerServiceOptions.GetPCId() == Sysset.GetPCIdHost()) && (TRUE == bDesktopModeMatch))
	{
		bool bDoneCheck = FALSE;

		InitSystem(FALSE);

		if (TRUE == m_bVersionMatch)
		{
			CSmartPayLockRequest Request;
			Request.m_bWantLockServerOuter = TRUE;

			CSmartPayLockManager LockManager;
			int nLockError = LockManager.AcquireLocks(Request);

			if (LOCKMANAGER_OK == nLockError)
			{
				RequestCheckBalance();

				RequestCheckLog("$$1");											// check for remaining multiple logs
				RequestCheckLog("LOG");											// new logs

				bDoneCheck = TRUE;
			}

			LockManager.LogError(nLockError);
		}
		else if ( TRUE == bCanLogError )
		{
			CSSFile fileInfo;
			if (fileInfo.Open(Filenames.GetServerServiceErrorTimeFilename(), "wb") == TRUE)
			{
				CCSV csv;
				csv.Add(m_strVersionError);
				csv.Add(GetSimpleDateTimeNowString());
				fileInfo.WriteLine(csv.GetLine());
			}
		}

		if (TRUE == bDoneCheck)
		{
			Sleep(50);
		}
	}	
}

//**********************************************************************

void CServerWrapper::ProcessUpdatePluRequest()
{
	CString strFilenameReq = Filenames.GetServerUpdatePluReqFilename();
	CString strFilenameRsp = Filenames.GetServerUpdatePluRspFilename();

	CString strToken = "";

	{
		CSSFile fileReq;
		if (fileReq.Open(strFilenameReq, "rb") == TRUE)
		{
			fileReq.ReadString(strToken);
			fileReq.Close();
		}
	}

	{
		CFileRemover FileRemover;
		FileRemover.RemoveFile(strFilenameReq);
		FileRemover.RemoveFile(strFilenameRsp);
	}

	if (strToken != "")
	{
		UpdatePluInfo();
		//UpdateOperatorInfo();
		UpdateEposTerminalInfo();
		UpdatePurchaseControlInfo();
	}

	{
		CSSFile fileRsp;
		if (fileRsp.Open(strFilenameRsp, "wb") == TRUE)
		{
			fileRsp.WriteLine(strToken);
			fileRsp.Close();
		}
	}
}

//******************************************************************

void CServerWrapper::UpdateOperatorInfo()
{
	/*
	CString strFilename = Server.GetEcrManagerImportOperatorFilename();
	if (::FileExists(strFilename) == TRUE)	// see if an update exists
	{
		SQLRepositoryOperatorName.ImportOperatorInfo(strFilename);
		::remove(strFilename);											// lose original
	}
	*/
}

//**********************************************************************

void CServerWrapper::UpdateEposTerminalInfo()
{
	CSQLTranImportEposTerminal Tran;
	Tran.BeginTrans();
	Tran.DoWork();
	Tran.EndTrans();
}

//**********************************************************************

void CServerWrapper::UpdatePluInfo()
{
	CString strImportFile = Server.GetEcrManagerImportPluFilename();

	if (strImportFile != "")
	{
		if (::FileExists(strImportFile) == TRUE)	// see if an update exists
		{
			CSQLRepositoryPluInfo RepoPlu;
			RepoPlu.ImportPluInfo(strImportFile);
			::remove(strImportFile);											// lose original
		}
	}
}

//******************************************************************

void CServerWrapper::UpdatePurchaseControlInfo()
{
	CString strImportFile = Server.GetEcrManagerImportPurchaseControlFilename();

	if (strImportFile != "")
	{
		if (::FileExists(strImportFile) == TRUE)	// see if an update exists
		{
			CSQLRepositoryPurchaseControlRule RepoRule;
			RepoRule.ImportPurchaseControlInfo(strImportFile);
			::remove(strImportFile);											// lose original
		}
	}
}

//**********************************************************************

void CServerWrapper::LogAction2 ( const char* szAction )
{
	if (Server.GetLogAction2Flag() == TRUE)
	{
		MessageLogger.LogSystemMessage(szAction);

		CString strFilename;	
		CTime tm = CTime::GetCurrentTime();
		strFilename.Format("sfilelog-%d%2.2d.dat", tm.GetYear(), tm.GetMonth());
		::LogSysFileError(szAction, strFilename, "");
	}
}

//*******************************************************************
// log when cannot open account database

void CServerWrapper::LogAccountError ( const char* szError )
{
	MessageLogger.LogSystemMessage(szError);
}

//*******************************************************************

void CServerWrapper::ShutdownThreads()
{
	m_LoyReqThreadManager.ShutdownThreads();
	m_LoyRspThreadManager.ShutdownThreads();
	m_LoyLogThreadManager.ShutdownThreads();
}

//*******************************************************************
