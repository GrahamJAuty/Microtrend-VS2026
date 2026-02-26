/**********************************************************************/
#include "CashRSPFlusher.h"
#include "CommsHandshaker.h"
#include "Consolidation.h"
#include "EODFileMover.h"
#include "FileRemover.h"
#include "..\CommonEcrStock\IDraughtExportCreator.h"
#include "..\CommonEcrStock\IDraughtExportManager.h"
#include "LocationCSVArray.h"
#include "MessageLogger.h"
#include "NetworkCSVArray.h"
#include "..\SPosTray5\PosTrayLock.h"
#include "PosTrayOptions.h"
#include "PMSOptions.h"
#include "..\CommonEcrStock\SageExportManager.h"
#include "SPOSCommsConnect.h"
#include "SysInfo.h"
/**********************************************************************/
#include "5000Server.h"
/**********************************************************************/

static const char* szRemoteDestPath = "remote\\";

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

static void FindTermFiles ( const char* szPath, bool bDeposit, bool bSend, CSortStringArray& FilenameArray )
{
	CString strFileSpec = szPath;
	
	int nFileNameLen = 0;

	if ( FALSE == bDeposit )
	{
		nFileNameLen = 8;
		strFileSpec += ( bSend ) ? "send.*" : "term.*";
	}
	else
	{
		nFileNameLen = 12;
		strFileSpec += ( bSend ) ? "senddpst.*" : "termdpst.*";
	}

	CFileFind finder;

	BOOL bWorking = finder.FindFile ( strFileSpec );

	while ( bWorking == TRUE )
	{
		bWorking = finder.FindNextFile();

		if ( finder.IsDirectory() == FALSE )
		{
			CString strFilename = finder.GetFileName();		// term.nnn or send.nnn or dpstterm.nnn or dpstsend.nnn

			if ( strFilename.GetLength() == nFileNameLen )	// filter out duff filenames
			{
				CString strTerminal = strFilename.Right(3);	// nnn

				if ( ::TestNumeric ( strTerminal ) == TRUE )
				{
					FilenameArray.Add ( strFilename );
				}
			}
		}
	}
}

/**********************************************************************/

static void AppendFile ( const char* szSource, const char* szDest )
{
	CSSFile fileSource;

	if ( fileSource.Open ( szSource, "rb" ) == TRUE )
	{
		CSSFile fileDest;

		if ( fileDest.Open ( szDest, "ab", _SH_DENYWR ) == TRUE )
		{
			CString strBuf;

			while ( fileSource.ReadLine ( strBuf ) == TRUE )
				fileDest.WriteString ( strBuf );

			fileDest.Close();
		}

		fileSource.Close();
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

C5000Server::C5000Server()
{
	m_pRealTimeCommsThread = NULL;
	
	m_nCommsEnable = 1;
	m_nEnableCount = 0;
	m_nDisableCount = 0;

	m_bPaused = TRUE;
	m_bWaitPause = FALSE;
	m_bConfirmStopped = FALSE;
	m_bPMSPulse = FALSE;
}

/**********************************************************************/

C5000Server::~C5000Server()
{
	TRACE( "C5000Server Destructor\n" );
}

/**********************************************************************/

void C5000Server::LogMessageDateTime ( const char* szText, bool bHide )
{
	CString strText = "Server5000 ";
	strText += szText;

	CMessageLoggerInfo info;
	info.m_strMessage = szText;
	info.m_strStatus = "";
	info.m_nHideLevel = ( bHide == TRUE ) ? 1 : 0;
	info.m_nNetworkIdx = 0;
	info.m_bAddDate = TRUE;
	info.m_bAddTimeOfDay = TRUE;
	info.m_bAddNetworkName = FALSE;

	MessageLogger.LogSchedulerMessage( info, FALSE, TRUE, FALSE );
}

/**********************************************************************/

void C5000Server::LogTerminate ( const char* szText )
{
	for ( int i = 0 ; i < m_SiteArraySales.GetSize() ; i++ )
	{
		CString strMsg;
		strMsg.Format ( "Stopped %d (%u processed) (%d:%d)", 
			i+1, 
			m_SiteArraySales.GetTransactionCount(i),
			m_nEnableCount,
			m_nDisableCount );

		CRealTimeCommsThread::LogRealTimeCommsMessage( &m_RealTimeCommsThreadInfo, strMsg );
	}

	m_nEnableCount = 0;
	m_nDisableCount = 0;

	if ( strlen ( szText ) != 0 )
		CRealTimeCommsThread::LogRealTimeCommsMessage ( &m_RealTimeCommsThreadInfo, szText );
}

/**********************************************************************/

bool C5000Server::BeginComms ( CStringArray& strSiteArraySales, CStringArray& strSiteArrayProducts, CStringArray& strSiteArrayCustomer, bool bEnableLocal )
{
	m_RealTimeCommsThreadInfo.m_strSchedLogAllFilename = Super.SchedLogAll();
	m_RealTimeCommsThreadInfo.m_strSchedLogOldFilename = Super.SchedLogOld();
	m_RealTimeCommsThreadInfo.m_lMaxSchedLogFileSize = Super.MaxLogSysFilesize();

	m_RealTimeCommsThreadInfo.m_strNetworkLogAllFilename = dbNetwork.GetFilePathSyssetSchedLogAll( 0 );
	m_RealTimeCommsThreadInfo.m_strNetworkLogOldFilename = dbNetwork.GetFilePathSyssetSchedLogOld( 0 );
	m_RealTimeCommsThreadInfo.m_lMaxNetworkLogFileSize = Super.MaxLogLocFilesize();

	CFilenameUpdater FnUp ( SysFiles::PosTrayStopFile );
	m_RealTimeCommsThreadInfo.m_strPosUtilityStopFile = FnUp.GetFilenameToUse();
	m_RealTimeCommsThreadInfo.m_bPosUtilityStopFlag = FALSE;
	m_RealTimeCommsThreadInfo.m_nMaxUsers = Sysset.GetMaxUsers();

	m_nCommsEnable = 1;
	
	m_bEnableLocal = bEnableLocal;

	m_strRemoteDestPath = m_strNetworkPath;
	m_strRemoteDestPath += szRemoteDestPath;

	m_SiteArraySales.RemoveAll();
	m_SiteArrayProducts.RemoveAll();
	m_SiteArrayCustomer.RemoveAll();

	for ( int n = 0 ; n < strSiteArraySales.GetSize() ; n++ )
	{
		CCSV csv( strSiteArraySales.GetAt(n) );

		C5000SiteInfo info;
		info.m_strEcrDataPath = csv.GetString(0);
		info.m_strLocalDestPath = csv.GetString(1);
		info.m_strNetworkLockFilename = csv.GetString(2);
		info.m_strExportControlLocs = csv.GetString(3);
		m_SiteArraySales.AddSite ( info );
	}

	for ( int n = 0 ; n < strSiteArrayProducts.GetSize() ; n++ )
	{
		CCSV csv( strSiteArrayProducts.GetAt(n) );

		C5000SiteInfo info;
		info.m_strEcrDataPath = csv.GetString(0);
		info.m_strLocalDestPath = csv.GetString(1);
		info.m_strNetworkLockFilename = csv.GetString(2);
		info.m_strExportControlLocs = "";
		m_SiteArrayProducts.AddSite ( info );
	}

	for ( int n = 0 ; n < strSiteArrayCustomer.GetSize() ; n++ )
	{
		CCSV csv( strSiteArrayCustomer.GetAt(n) );

		C5000SiteInfo info;
		info.m_strEcrDataPath = csv.GetString(0);
		info.m_strLocalDestPath = csv.GetString(1);
		info.m_strNetworkLockFilename = csv.GetString(2);
		info.m_strExportControlLocs = "";
		m_SiteArrayCustomer.AddSite ( info );
	}

	remove ( m_strNetworkPath+szEnqFilename );
	remove ( m_strNetworkPath+szAckFilename );

	m_SiteArraySales.SelectFirstSite();
	m_SiteArrayProducts.SelectFirstSite();
	m_SiteArrayCustomer.SelectFirstSite();

	for ( int i = 0 ; i < m_SiteArraySales.GetSize() ; i++ )
	{
		C5000SiteInfo info;
		m_SiteArraySales.GetSiteInfo( info );
		m_SiteArraySales.SelectNextSite();

		if ( m_bEnableLocal == TRUE )
			_mkdir ( info.m_strLocalDestPath );		// make local folder

		CString strMsg;
		strMsg.Format ( "Started %d (%s) (%s)", i+1, info.m_strEcrDataPath, info.m_strLocalDestPath );

		if ( m_bEnableLocal == TRUE )
			strMsg += " local";

		LogMessageDateTime ( strMsg );
	}

	for ( int i = 0 ; i < m_SiteArrayProducts.GetSize() ; i++ )
	{
		C5000SiteInfo info;
		m_SiteArrayProducts.GetSiteInfo( info );
		m_SiteArrayProducts.SelectNextSite();
		_mkdir ( info.m_strLocalDestPath );		// make local folder
	}

	for ( int i = 0 ; i < m_SiteArrayCustomer.GetSize() ; i++ )
	{
		C5000SiteInfo info;
		m_SiteArrayCustomer.GetSiteInfo( info );
		m_SiteArrayCustomer.SelectNextSite();
		_mkdir ( info.m_strLocalDestPath );		// make local folder
	}

	m_SiteArraySales.SelectFirstSite();
	m_SiteArrayProducts.SelectFirstSite();
	m_SiteArrayCustomer.SelectFirstSite();
	
	return TRUE;
}

/**********************************************************************/

void C5000Server::PauseComms()
{
	m_bConfirmStopped = FALSE;
	m_bPaused = TRUE;
	m_bWaitPause = TRUE;
	m_RealTimeCommsThreadInfo.m_bForceQuit = TRUE;
	CNetCommsServer::DisableComms();
}

/**********************************************************************/

void C5000Server::ResumeComms()
{
	m_bPaused = FALSE;

	if ( m_nCommsEnable >= 1 )
		CNetCommsServer::EnableComms();

}

/**********************************************************************/

void C5000Server::EnableComms()
{
	m_nCommsEnable++;
	m_nEnableCount++;

	if ( m_nCommsEnable >= 1 )
		CNetCommsServer::EnableComms();
}

/**********************************************************************/

void C5000Server::DisableComms()
{
	m_bConfirmStopped = FALSE;
	m_nCommsEnable--;
	m_nDisableCount++;

	if ( m_nCommsEnable <= 0 )
	{
		m_RealTimeCommsThreadInfo.m_bForceQuit = TRUE;
		CNetCommsServer::DisableComms();
	}
}

/**********************************************************************/

void C5000Server::PMSPulse()
{
	m_bConfirmStopped = FALSE;
	m_bPMSPulse = TRUE;
}

/**********************************************************************/

void C5000Server::Timer() 
{
	bool bAlreadyRunning = FALSE;

	if ( m_pRealTimeCommsThread != NULL )
	{
		DWORD dwExitCode = 0;
		GetExitCodeThread( m_pRealTimeCommsThread -> m_hThread, &dwExitCode );

		if ( STILL_ACTIVE == dwExitCode )
			bAlreadyRunning = TRUE;
		else
		{
			m_SiteArraySales.SyncZeroFileCount( m_RealTimeCommsThreadInfo.m_nZeroFileCount );

			if ( m_RealTimeCommsThreadInfo.m_nTransactionCount != 0 )
			{
				m_SiteArraySales.IncTransactionCount( m_RealTimeCommsThreadInfo.m_nTransactionCount );
				
#ifdef POSTRAY_UTILITY

				C5000SiteInfo infoSite;
				m_SiteArraySales.GetSiteInfo( infoSite );

				CSortedStringCounter infoString;
				infoString.m_strItem = infoSite.m_strEcrDataPath;
				infoString.m_nCount = m_RealTimeCommsThreadInfo.m_nTransactionCount;
				m_arrayTranCountForPosTray.Consolidate( infoString );
#endif
			}
			else
				m_SiteArraySales.SelectNextSite();

			if ( TRUE == m_RealTimeCommsThreadInfo.m_bProducts )
				m_SiteArrayProducts.SelectNextSite();

			if ( TRUE == m_RealTimeCommsThreadInfo.m_bCustomer )
				m_SiteArrayCustomer.SelectNextSite();

			delete m_pRealTimeCommsThread;
			m_pRealTimeCommsThread = NULL;
		}
	}

	bool bCommsToDo = ( m_SiteArraySales.GetSize() > 0 ) || ( m_SiteArrayProducts.GetSize() > 0 ) || ( m_SiteArrayCustomer.GetSize() > 0 ) || ( SysInfo.IsPMSSystem() == TRUE );
	bCommsToDo &= CommsHandshaker.GetEnableRealTimeSalesFlag();

	if ( FALSE == bAlreadyRunning )
	{
		if ( FALSE == bCommsToDo )
		{
			m_bWaitPause = FALSE;
			m_bConfirmStopped = TRUE;
		}
		else
		{
			bool bRestartThread = FALSE;

			if ( TRUE == m_bPMSPulse )
				bRestartThread = TRUE;
			else if ( m_nCommsEnable <= 0 )
				bRestartThread = FALSE;
			else if ( TRUE == m_bPaused )
				bRestartThread = FALSE;
			else
				bRestartThread = TRUE;

			if ( FALSE == bRestartThread )
			{
				if ( TRUE == m_bWaitPause )
				{
					LogTerminate( "");
					m_bWaitPause = FALSE;
				}

				m_bConfirmStopped = TRUE;
			}
			else
			{
				m_RealTimeCommsThreadInfo.m_bCASHRSP = FALSE;
				m_RealTimeCommsThreadInfo.m_bLogAllCASHRSPFiles = FALSE;
				m_RealTimeCommsThreadInfo.m_bPMS = FALSE;
				m_RealTimeCommsThreadInfo.m_bProducts = FALSE;
				m_RealTimeCommsThreadInfo.m_bCustomer = FALSE;

				if ( ( m_SiteArraySales.GetSize() > 0 ) && ( FALSE == m_bPMSPulse ) )
				{
					C5000SiteInfo infoSite;
					m_SiteArraySales.GetSiteInfo( infoSite );

					m_RealTimeCommsThreadInfo.m_siteInfoSales = infoSite;
					m_RealTimeCommsThreadInfo.m_bEnableLocal = m_bEnableLocal;
					m_RealTimeCommsThreadInfo.m_nTransactionCount = 0;
					m_RealTimeCommsThreadInfo.m_nZeroFileCount = infoSite.m_nZeroFileCount;

					m_RealTimeCommsThreadInfo.m_bCASHRSP = TRUE;
					m_RealTimeCommsThreadInfo.m_bLogAllCASHRSPFiles = DealerFlags.GetLogAllCashRSPFilesFlag();
				}

				if ( ( m_SiteArrayProducts.GetSize() > 0 ) && ( FALSE == m_bPMSPulse ) )
				{
					C5000SiteInfo infoSite;
					m_SiteArrayProducts.GetSiteInfo( infoSite );

					m_RealTimeCommsThreadInfo.m_siteInfoProducts = infoSite;
					m_RealTimeCommsThreadInfo.m_bProducts = TRUE;
				}

				if ( ( m_SiteArrayCustomer.GetSize() > 0 ) && ( FALSE == m_bPMSPulse ) )
				{
					C5000SiteInfo infoSite;
					m_SiteArrayCustomer.GetSiteInfo( infoSite );

					m_RealTimeCommsThreadInfo.m_siteInfoCustomer = infoSite;
					m_RealTimeCommsThreadInfo.m_bCustomer = TRUE;
				}

#ifdef COMPILE_PMS
				if ( SysInfo.IsPMSSystem() == TRUE )
				{
					CFilenameUpdater FnUp( SysFiles::PMSLockFile );
					m_RealTimeCommsThreadInfo.m_strPMSDownloadFolder = PMSOptions.GetPMSFolderDownload();
					m_RealTimeCommsThreadInfo.m_strPMSRoomFolder = PMSOptions.GetEposLinkRoomPath();
					m_RealTimeCommsThreadInfo.m_strPMSLockFile = FnUp.GetFilenameToUse();
					m_RealTimeCommsThreadInfo.m_strPMSPostPath = PMSOptions.GetEposLinkPostPath();
					m_RealTimeCommsThreadInfo.m_strPMSFolderPending = PMSOptions.GetPMSFolderPending();
					m_RealTimeCommsThreadInfo.m_strPMSFolderSales = PMSOptions.GetPMSFolderSales();

					m_RealTimeCommsThreadInfo.m_bAllPMS = FALSE;
					m_RealTimeCommsThreadInfo.m_bPMS = TRUE;
				}
#endif

#ifndef POSTRAY_UTILITY
				m_RealTimeCommsThreadInfo.m_nSpeed = SysInfo.GetCommsThreadSpeed();
#else
				m_RealTimeCommsThreadInfo.m_nSpeed = PosTrayOptions.GetRealTimeCommsSpeed();
#endif

				if ( TRUE == m_bPMSPulse )
				{
					m_RealTimeCommsThreadInfo.m_bAllPMS = TRUE;
					m_bPMSPulse = FALSE;
					m_bConfirmStopped = FALSE;
				}

				m_RealTimeCommsThreadInfo.m_bForceQuit = FALSE;
				m_pRealTimeCommsThread = AfxBeginThread( CRealTimeCommsThread::DoComms, &m_RealTimeCommsThreadInfo, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL );
		
				if ( m_pRealTimeCommsThread != NULL )
				{
					m_pRealTimeCommsThread -> m_bAutoDelete = FALSE;
					m_pRealTimeCommsThread -> ResumeThread();
				}
			}	
		}
	}
}

/**********************************************************************/

void C5000Server::TerminateThread()
{
	if ( m_pRealTimeCommsThread != NULL )
	{
		DWORD dwExitCode = 0;
		m_RealTimeCommsThreadInfo.m_bForceQuit = TRUE;

		do
		{
			GetExitCodeThread( m_pRealTimeCommsThread -> m_hThread, &dwExitCode );
		}
		while ( STILL_ACTIVE == dwExitCode );
		
		delete m_pRealTimeCommsThread;

		LogTerminate( "" );
	}
	else if ( TRUE == m_bWaitPause )
		LogTerminate( "" );
}

/**********************************************************************/

bool C5000Server::QuickCheckNoComms()
{
	if ( CommsHandshaker.GetEnableRealTimeSalesFlag() == FALSE )
		return TRUE;

	if ( m_SiteArraySales.GetSize() != 0 )
		return FALSE;

	if ( m_SiteArrayProducts.GetSize() != 0 )
		return FALSE;

	if ( m_SiteArrayCustomer.GetSize() != 0 )
		return FALSE;

	if ( SysInfo.IsPMSSystem() == TRUE )
		return FALSE;

	return ( NULL == m_pRealTimeCommsThread );
}

/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

long C5000Server::FetchAllRealTimeSales( int nNetworkIdx, int nType )
{
	CString strDataPath;
	dbNetwork.GetSafeFileNetworkPath ( nNetworkIdx, strDataPath );

	if ( strDataPath == "" )
	{
		switch( nType )
		{
		case CASHRSP_FLUSH_IDRAUGHT:
			{
				CString str;
				str.Format( "%3.3d", dbNetwork.GetNetworkNo( nNetworkIdx ) );
				IDraughtExportManager.SetExtraErrorInfo( str );
			}
			break;

		case CASHRSP_FLUSH_SAGE:
			{
				CString str;
				str.Format( "( Network %3.3d)", dbNetwork.GetNetworkNo( nNetworkIdx ) );
				SageExportManager.SetExtraErrorInfo( str );
			}
			break;
		}
		
		return CCashRSPFlusher::GetErrorCode( nType, CASHRSP_FLUSH_ERROR_NETWORK_NOPATH );
	}

	CString strLocalPath = dbNetwork.GetFolderPathSyssetCashRSP( nNetworkIdx );
	strLocalPath += "\\";

	CFilenameUpdater FnUp( SysFiles::NetworkCommsLock, nNetworkIdx );
	CString strLockFile = FnUp.GetFilenameToUse();

	C5000SiteInfo infoSite;
	infoSite.m_nTransactionCount = 0;
	infoSite.m_strEcrDataPath = strDataPath;
	infoSite.m_strLocalDestPath = strLocalPath;
	infoSite.m_strNetworkLockFilename = strLockFile;

	m_RealTimeCommsThreadInfo.m_siteInfoSales = infoSite;
	m_RealTimeCommsThreadInfo.m_bEnableLocal = m_bEnableLocal;
	m_RealTimeCommsThreadInfo.m_nTransactionCount = 0;
	m_RealTimeCommsThreadInfo.m_bLogAllCASHRSPFiles = DealerFlags.GetLogAllCashRSPFilesFlag();
	
	return CCashRSPFlusher::DoComms( &m_RealTimeCommsThreadInfo, nType );
}

/**********************************************************************/
#else
/**********************************************************************/

int C5000Server::GetTranCountForPosTray( const char* sz )
{
	int nResult = 0;

	CSortedStringCounter infoString;
	infoString.m_strItem = sz;

	int nPos;
	if ( m_arrayTranCountForPosTray.Find( infoString, nPos ) == TRUE )
	{
		m_arrayTranCountForPosTray.GetAt( nPos, infoString );
		nResult = infoString.m_nCount;
	}

	return nResult;
}

/**********************************************************************/
#endif
/**********************************************************************/
