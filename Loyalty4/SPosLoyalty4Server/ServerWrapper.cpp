//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
#include "..\SPosLoyalty4Shared\SQLTable_DeptInfo\SQLRepositoryDeptInfo.h"
#include "..\SPosLoyalty4Shared\SQLTable_EcrmanDatabase\SQLRepositoryEcrmanDatabase.h"
#include "..\SPosLoyalty4Shared\SQLTable_Scheme\SQLRepositoryScheme.h"
#include "..\SPosLoyalty4Shared\SQLTable_PluInfo\SQLRepositoryPluInfo.h"
#include "..\SPosLoyalty4Shared\SQLTable_StampOfferInfo\SQLRepositoryStampOfferInfo.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\FileRemover.h"
#include "..\SPosLoyalty4Shared\GlobalState.h"
#include "..\SPosLoyalty4Shared\MessageLogger.h"
#include "..\SPosLoyalty4Shared\ServiceOptions.h"
//*******************************************************************
#include "ServerWrapper.h"
//*******************************************************************
extern const char* szVERSION_LOYALTY_ABOUT;
//*******************************************************************

CServerWrapper::CServerWrapper()
{ 
	m_bVersionMatch = FALSE;

	m_TrackedFileSystem.SetFilename( Filenames.GetSystemFilenameNew() );
	m_TrackedFileServer.SetFilename( Filenames.GetServerDataFilename() );
	m_TrackedFilePeriod.SetFilename( Filenames.GetAuditPeriodTokenFilename() );
	m_TrackedFileServiceOptions.SetFilename( Filenames.GetServerServiceOptionsFilename() );
	m_TrackedFileLoyaltyVersion.SetFilename( Filenames.GetServicesVersionFilename() );
	
	m_oleNextActionLogTime = COleDateTime::GetCurrentTime();

	m_dBonusCash = 0.0;
	m_bGroupExists = FALSE;
	m_nImportCounter = 0;

	m_strVersionError = "";
	m_nLastPurgeTime = 0;

	m_bServerInnerLockOK = FALSE;
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
	if ( m_TrackedFileServer.CheckFile( bForce ) == TRUE )
	{
		Server.Read( Filenames.GetServerDataFilename() );
		MessageLogger.LogSystemMessage( "Read Server Options File" );
		
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
		SQLRowSetAuditPeriod.LoadPeriods(NULL);
		MessageLogger.LogSystemMessage( "Read Group File" );
	}

	if ( m_TrackedFileServiceOptions.CheckFile( bForce ) == TRUE )
	{
		ServerServiceOptions.Read();
		MessageLogger.LogSystemMessage( "Read Service Options File" );

		{
			CString strMsg;
			strMsg.Format( "PCId = %10.10I64d", ServerServiceOptions.GetPCId() );
			MessageLogger.LogSystemMessage( strMsg );
		}
	}

	if ( m_TrackedFileLoyaltyVersion.CheckFile( bForce ) == TRUE )
	{
		CString strManagerVersion = "";
		CString strServerVersion = szVERSION_LOYALTY_ABOUT;

		{
			CSSFile fileVersion;
			if ( fileVersion.Open( Filenames.GetServicesVersionFilename(), "rb" ) == TRUE )
			{
				fileVersion.ReadString( strManagerVersion );
			}
		}

		MessageLogger.LogSystemMessage( "Read Loyalty Version File" );

		m_bVersionMatch = ( strManagerVersion == strServerVersion );

		if ( FALSE == m_bVersionMatch )
		{
			MessageLogger.LogSystemMessage( "Incompatible Loyalty Manager Version" );

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

CString CServerWrapper::GetTimeNowString()
{
	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	CString strTime;
	strTime.Format("%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d",
		timeNow.GetYear(),
		timeNow.GetMonth(),
		timeNow.GetDay(),
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond());

	return strTime;
}

//*****************************************************************

void CServerWrapper::Pulse() 
{
	InitSystem(FALSE);
	
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
				csv.Add(GetTimeNowString());

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
				Sysset.GetPCIdHost(), 
				ServerServiceOptions.GetPCId());

			CSSFile fileInfo;
			if (fileInfo.Open(Filenames.GetServerServiceErrorTimeFilename(), "wb") == TRUE)
			{
				CCSV csv;
				csv.Add(strError);
				csv.Add(GetTimeNowString());
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
				csv.Add(GetTimeNowString());
				fileInfo.WriteLine(csv.GetLine());
			}
		}

		m_oleNextActionLogTime = COleDateTime::GetCurrentTime() + COleDateTimeSpan(0, 0, 0, 5);
	}

	if ( (ServerServiceOptions.GetPCId() == Sysset.GetPCIdHost()) && ( TRUE == System.GetGotPCIdFlag() ) && ( TRUE == bDesktopModeMatch) )
	{
		InitSystem(FALSE);

		if (TRUE == m_bVersionMatch)
		{
			CLoyaltyLockRequest LockRequest;
			LockRequest.m_bWantLockServerOuter = TRUE;

			//LOCK MANAGER DESTRUCTOR WILL FREE LOCK IF ACQUIRED
			CLoyaltyLockManager LockManager;
			int nLockResult = LockManager.AcquireLocks(LockRequest);
			
			if ( LOCKMANAGER_OK == nLockResult )
			{
				RequestCheckBalance();											// check for balance requests
				RequestCheckLog("$$1");											// check for remaining multiple logs
				RequestCheckLog("LOG");											// new logs
				CheckImports();													// look for data if not paused
			}
			
			LockManager.LogError(nLockResult);
		}
		else if (TRUE == bCanLogError)
		{
			CSSFile fileInfo;
			if (fileInfo.Open(Filenames.GetServerServiceErrorTimeFilename(), "wb") == TRUE)
			{
				CCSV csv;
				csv.Add(m_strVersionError);
				csv.Add(GetTimeNowString());
				fileInfo.WriteLine(csv.GetLine());
			}
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
		for (int nDbNo = 1; nDbNo < PLU_DBNO_COUNT; nDbNo++)
		{
			bool bProcessRequest = FALSE;

			for (int nSetNo = 1; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
			{
				CServerDataFolderSetIndex FolderSetIndex(nDbNo, nSetNo);

				if ((Server.GetEposRequestPath(FolderSetIndex) != "") || (Server.GetEposLogPath(FolderSetIndex) != ""))
				{
					bProcessRequest = TRUE;
					break;
				}
			}

			if (TRUE == bProcessRequest)
			{
				UpdatePluInfo(nDbNo);
				UpdateDeptInfo(nDbNo);
			}
		}

		UpdateStampOfferInfo();
		UpdateLoyaltySchemeInfo();
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

//**********************************************************************

void CServerWrapper::UpdatePluInfo(int nDbNo)
{
	CString strImportFile = Server.GetEcrManagerImportPluFilename(nDbNo);

	if (strImportFile != "")
	{
		if (::FileExists(strImportFile) == TRUE)	// see if an update exists
		{
			CSQLRepositoryPluInfo repoInfo;
			repoInfo.ImportPluInfo(nDbNo, strImportFile);
			::remove(strImportFile);											// lose original
		}
	}
}

//******************************************************************

void CServerWrapper::UpdateDeptInfo(int nDbNo)
{
	CString strImportFile = Server.GetEcrManagerImportDeptFilename(nDbNo);

	if (strImportFile != "")
	{
		if (::FileExists(strImportFile) == TRUE)	// see if an update exists
		{
			CSQLRepositoryDeptInfo repoInfo;
			repoInfo.ImportDeptInfo(nDbNo, strImportFile);
			::remove(strImportFile);											// lose original
		}
	}
}

//******************************************************************

void CServerWrapper::UpdateStampOfferInfo()
{
	CString strImportFile = Server.GetEcrManagerImportStampOfferFilename();

	if (strImportFile != "")
	{
		if (::FileExists(strImportFile) == TRUE)	// see if an update exists
		{
			CSQLRepositoryStampOfferInfo repInfo;
			repInfo.ImportStampOfferInfo(strImportFile);
			::remove(strImportFile);											// lose original
		}
	}
}

//**********************************************************************

void CServerWrapper::UpdateLoyaltySchemeInfo()
{
	CString strImportFile = Server.GetEcrManagerImportLoyaltySchemeFilename();

	if (strImportFile != "")
	{
		if (::FileExists(strImportFile) == TRUE)	// see if an update exists
		{
			CSQLRepositoryScheme PrepStatScheme;
			PrepStatScheme.ImportScheme(strImportFile);
			::remove(strImportFile);											// lose original
		}
	}
}

//**********************************************************************

void CServerWrapper::ShutdownThreads()
{
	m_LoyReqThreadManager.ShutdownThreads();
	m_LoyRspThreadManager.ShutdownThreads();
	m_LoyLogThreadManager.ShutdownThreads();
}

//*******************************************************************
