/**********************************************************************/
#include "CashRSPFlusher.h"
#include "CommsHandshaker.h"
//#include "DataManager.h"
#include "DateRangeHelpers.h"
#include "DepartmentCSVArray.h"
//#include "EcrmanOptionsIni.h"
#include "FileRemover.h"
#include "FTPConnect.h"
#include "FTPConnectDlg.h"
//#include "globalfunctions.h"
#include "IDraughtExportCreator.h"
#include "IDraughtOptions.h"
#include "LocationCSVArray.h"
#include "LockManager.h"
#include "MessageLogger.h"
#include "NetworkCSVArray.h"
#include "PosTrayTokenFiles.h"
 
#include "PropertySheetIDraughtOptions.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "IDraughtExportManager.h"
/**********************************************************************/

CIDraughtExportManager::CIDraughtExportManager(void)
{
	m_bReadyToGo = FALSE;
	m_nNetworkMapIdx = 0;
	SetInitialExportDate( m_dateExportFrom );
	SetInitialExportDate( m_dateExportTo );
	SetInitialExportDate( m_dateNextConnect );
	
	CDateRangeHelpers DateRangeHelpers;
	DateRangeHelpers.AddDaysToTime( m_dateExportFrom, 1 );

	m_strExtraErrorInfo = "";
}

/**********************************************************************/

void CIDraughtExportManager::SetInitialExportDate( COleDateTime& date )
{
	date = COleDateTime( 2011, 11, 23, 0, 0, 0 );
}

/**********************************************************************/

void CIDraughtExportManager::SetDummyFutureDate( COleDateTime& date )
{
	date = COleDateTime( 3000, 1, 1, 0, 0, 0 );
}

/**********************************************************************/

void CIDraughtExportManager::BuildNetworkMap()
{
	m_bReadyToGo = FALSE;

	m_NetworkMap.RemoveAll();
	m_nNetworkMapIdx = 0;

	for ( int nNetworkIdx = 0; nNetworkIdx < dbNetwork.GetSize(); nNetworkIdx++ )
	{
		int nStartIdx, nEndIdx;
		dbLocation.GetNetworkLocIdxRange( nNetworkIdx, nStartIdx, nEndIdx );

		//NO IDRAUGHT EXPORT FOR NETWORKS WITH NO LOCATIONS
		if ( nEndIdx < nStartIdx )
			continue;

		//NO IDRAUGHT EXPORT FOR NETWORKS WHICH DO NOT HAVE REAL TIME COMMS
		if ( dbNetwork.GetConnectionType( nNetworkIdx ) != CONNECTION_TYPE_STANDARD_REALTIME )
			continue;

		CReportConsolidationArray<CSortedStringItem> arrayLocations;

		for ( int nLocIdx = nStartIdx; nLocIdx <= nEndIdx; nLocIdx++ )
		{
			int nDbNo = dbLocation.GetDbNo( nLocIdx );
		
			int nDbIdx;
			if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
			{
				arrayLocations.RemoveAll();
				break;
			}

			CSortedStringItem item;
			item.m_strItem.Format( "%5.5d%5.5d", nDbIdx, nLocIdx );
			arrayLocations.Consolidate( item );
		}

		if ( arrayLocations.GetSize() > 0 )
		{
			CCSV csv;

			for ( int n = 0; n < arrayLocations.GetSize(); n++ )
			{
				CSortedStringItem item;
				arrayLocations.GetAt( n, item );
				CString strLabel = item.m_strItem;
				csv.Add ( atoi ( strLabel.Left(5) ) );
				csv.Add ( atoi ( strLabel.Right(5) ) );
			}
	
			CIDraughtExportInfo info;
			info.m_nNetworkIdx = nNetworkIdx;
			info.m_strLocationList = csv.GetLine();
			info.m_bGotDate = FALSE;
			info.m_bBadDate = FALSE;
			info.m_lLastNetworkError = 0;
			info.m_nLastNetworkErrorCount = 0;
			SetInitialExportDate( info.m_timeRetryExport );
			SetDummyFutureDate( info.m_dateLastExport );
			m_NetworkMap.Add( info );
		}
	}

#ifdef _DEBUG

	CSSFile fileIDraughtMap;

	CString strFilename = "IDraughtMap.txt";
	GetDataProgramPath( strFilename );

	if ( fileIDraughtMap.Open( strFilename, "wb" ) == TRUE )
	{
		for ( int n = 0; n < m_NetworkMap.GetSize(); n++ )
		{
			CString str;
			str.Format( "%d   ", m_NetworkMap[n].m_nNetworkIdx );
			str += m_NetworkMap[n].m_strLocationList;
			fileIDraughtMap.WriteLine( str );
		}
	}

#endif

	m_bReadyToGo = TRUE;
}

/**********************************************************************/

void CIDraughtExportManager::NextNetworkMapIdx()
{
	m_nNetworkMapIdx++;
	if ( m_nNetworkMapIdx >= m_NetworkMap.GetSize() )
		m_nNetworkMapIdx = 0;
}

/**********************************************************************/

int CIDraughtExportManager::GetNetworkIdx()
{
	if ( ( m_nNetworkMapIdx >= 0 ) && ( m_nNetworkMapIdx < m_NetworkMap.GetSize() ) )
		return m_NetworkMap[ m_nNetworkMapIdx ].m_nNetworkIdx;
	else
		return 0;
}

/**********************************************************************/

const char* CIDraughtExportManager::GetLocationList()
{
	if ( ( m_nNetworkMapIdx >= 0 ) && ( m_nNetworkMapIdx < m_NetworkMap.GetSize() ) )
		m_strLocationList = m_NetworkMap[ m_nNetworkMapIdx ].m_strLocationList;
	else
		m_strLocationList = "";

	return m_strLocationList;
}

/**********************************************************************/

bool CIDraughtExportManager::GetBadDateFlag()
{
	if ( ( m_nNetworkMapIdx >= 0 ) && ( m_nNetworkMapIdx < m_NetworkMap.GetSize() ) )
		return m_NetworkMap[ m_nNetworkMapIdx ].m_bBadDate;
	else
		return TRUE;
}

/**********************************************************************/

void CIDraughtExportManager::GetExportRetryTime( COleDateTime& time )
{
	if ( ( m_nNetworkMapIdx >= 0 ) && ( m_nNetworkMapIdx < m_NetworkMap.GetSize() ) )
		time =  m_NetworkMap[ m_nNetworkMapIdx ].m_timeRetryExport;
	else
		SetDummyFutureDate( time );
}

/**********************************************************************/

void CIDraughtExportManager::SetExportRetryTime()
{
	if ( ( m_nNetworkMapIdx >= 0 ) && ( m_nNetworkMapIdx < m_NetworkMap.GetSize() ) )
		m_NetworkMap[ m_nNetworkMapIdx ].m_timeRetryExport = COleDateTime::GetCurrentTime() + COleDateTimeSpan ( 0, 0, 2, 0 );
}

/**********************************************************************/

bool CIDraughtExportManager::SetNetworkError( long lError )
{
	bool bResult = FALSE;

	if ( ( m_nNetworkMapIdx >= 0 ) && ( m_nNetworkMapIdx < m_NetworkMap.GetSize() ) )
	{
		if ( 0 == lError )
		{
			m_NetworkMap[ m_nNetworkMapIdx ].m_lLastNetworkError = 0;
			m_NetworkMap[ m_nNetworkMapIdx ].m_nLastNetworkErrorCount = 0;
		}
		else if ( lError == m_NetworkMap[ m_nNetworkMapIdx ].m_lLastNetworkError )
		{
			m_NetworkMap[ m_nNetworkMapIdx ].m_nLastNetworkErrorCount++;
			bResult = ( ( m_NetworkMap[ m_nNetworkMapIdx ].m_nLastNetworkErrorCount % 10 ) == 0 );
		}
		else
		{
			m_NetworkMap[ m_nNetworkMapIdx ].m_lLastNetworkError = lError;
			m_NetworkMap[ m_nNetworkMapIdx ].m_nLastNetworkErrorCount = 1;
			bResult = TRUE;
		}
	}

	return bResult;
}

/**********************************************************************/

bool CIDraughtExportManager::CheckExportTime()
{
	if ( FALSE == m_bReadyToGo )
		return FALSE;

	if ( GetBadDateFlag() == TRUE )
		return FALSE;

	//WAIT FOR NETWORK EXPORT RETRY TIME
	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	
	COleDateTime timeRetry;
	GetExportRetryTime( timeRetry );
	
	if ( timeNow < timeRetry )
		return FALSE;

	//FIND THE LATEST DATE FOR WHICH AN EXPORT IS DUE
	m_dateExportTo = COleDateTime( timeNow.GetYear(), timeNow.GetMonth(), timeNow.GetDay(), 0, 0, 0 );

	int nHourNow = timeNow.GetHour();
	int nHourDelay = EcrmanOptions.GetReportsDayStartHour() + IDraughtOptions.GetExportDelayTime();

	int nDaysBehind;
	if ( nHourNow < nHourDelay - 24 )
		nDaysBehind = 3;
	else if ( nHourNow < nHourDelay )
		nDaysBehind = 2;
	else
		nDaysBehind = 1;
	
	CDateRangeHelpers DateRangeHelpers;
	DateRangeHelpers.AddDaysToTime( m_dateExportTo, -nDaysBehind );

	//FIND THE FIRST DATE FOR WHICH AN EXPORT HAS NOT BEEN DONE
	GetLastExportDate( m_dateExportFrom );

	if ( GetBadDateFlag() == TRUE )
		return FALSE;

	DateRangeHelpers.AddDaysToTime( m_dateExportFrom, 1 );

	return ( m_dateExportTo >= m_dateExportFrom );
}

/**********************************************************************/

void CIDraughtExportManager::GetLastExportDate( COleDateTime& date )
{
	//NETWORK MAP INDEX OUT OF RANGE
	if ( ( m_nNetworkMapIdx < 0 ) || ( m_nNetworkMapIdx >= m_NetworkMap.GetSize() ) )
	{
		SetDummyFutureDate( date );
		return;
	}

	//DATE ALREADY RETREIVED FROM FILE
	if ( TRUE == m_NetworkMap[ m_nNetworkMapIdx ].m_bGotDate )
	{
		date = m_NetworkMap[ m_nNetworkMapIdx ].m_dateLastExport;

		if ( date.GetStatus() != COleDateTime::valid )
			m_NetworkMap[ m_nNetworkMapIdx ].m_bBadDate = TRUE;

		return;
	}

	//FILE DOES NOT EXIST, SO USE DEFAULT START DATE
	int nNetworkIdx = m_NetworkMap[ m_nNetworkMapIdx ].m_nNetworkIdx;
	CFilenameUpdater FnUp( SysFiles::IDraughtDate, nNetworkIdx  );
	CString strDateFile = FnUp.GetFilenameToUse();
	
	if ( ::FileExists( strDateFile ) == FALSE )
	{
		SetInitialExportDate( date );
		m_NetworkMap[ m_nNetworkMapIdx ].m_bGotDate = TRUE;
		m_NetworkMap[ m_nNetworkMapIdx ].m_bBadDate = FALSE;
		m_NetworkMap[ m_nNetworkMapIdx ].m_dateLastExport = date; 
		return;
	}

	//ASSUME INVALID DATE UNLESS PROVED OTHERWISE
	SetDummyFutureDate( date );
	m_NetworkMap[ m_nNetworkMapIdx ].m_bGotDate = TRUE;
	m_NetworkMap[ m_nNetworkMapIdx ].m_bBadDate = TRUE;
	m_NetworkMap[ m_nNetworkMapIdx ].m_dateLastExport = date; 
			
	//READ LAST EXPORT DATE FROM FILE
	CSSFile fileDate;
	if ( fileDate.Open( strDateFile, "rb" ) == TRUE )
	{
		CString strDate;
		fileDate.ReadString( strDate );

		if ( strDate.GetLength() == 8 )
		{
			if ( ::TestNumeric( strDate ) == TRUE )
			{
				COleDateTime dateFile = COleDateTime ( 
					atoi ( strDate.Left(4) ), 
					atoi ( strDate.Mid(4,2) ), 
					atoi ( strDate.Right(2) ),
					0, 0, 0 );
							
				if ( dateFile.GetStatus() == COleDateTime::valid )
				{			
					m_NetworkMap[ m_nNetworkMapIdx ].m_bBadDate = FALSE;
					m_NetworkMap[ m_nNetworkMapIdx ].m_dateLastExport = dateFile;
					date = dateFile;
				}
			}
		}
	}
}

/**********************************************************************/

void CIDraughtExportManager::SetLastExportDate( COleDateTime& date )
{
	if ( ( m_nNetworkMapIdx >= 0 ) && ( m_nNetworkMapIdx < m_NetworkMap.GetSize() ) )
	{
		if ( date.GetStatus() == COleDateTime::valid )
		{
			int nNetworkIdx = m_NetworkMap[ m_nNetworkMapIdx ].m_nNetworkIdx;
			CFilenameUpdater FnUp( SysFiles::IDraughtDate, nNetworkIdx  );
			CString strDateFile = FnUp.GetFilenameToUse();
			
			CSSFile fileDate;
			if ( fileDate.Open( strDateFile, "wb" ) == TRUE )
			{
				CString strDate;
				strDate.Format( "%4.4d%2.2d%2.2d",
					date.GetYear(),
					date.GetMonth(),
					date.GetDay() );

				fileDate.WriteLine( strDate );

				m_NetworkMap[ m_nNetworkMapIdx ].m_bGotDate = TRUE;
				m_NetworkMap[ m_nNetworkMapIdx ].m_dateLastExport = COleDateTime( date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0 );
			}
		}
	}
}

/**********************************************************************/

bool CIDraughtExportManager::CheckConnectTime()
{
	return ( COleDateTime::GetCurrentTime() >= m_dateNextConnect );
}

/**********************************************************************/

void CIDraughtExportManager::SetConnectTime()
{
	m_dateNextConnect = COleDateTime::GetCurrentTime();
	m_dateNextConnect += COleDateTimeSpan ( 0, 0, IDraughtOptions.GetConnectRetryTime(), 0 );
}

/**********************************************************************/

void CIDraughtExportManager::BuildNetworkStatusArray( CArray<CIDraughtExportStatus,CIDraughtExportStatus>& arrayStatus )
{
	arrayStatus.RemoveAll();

	for ( int nNetworkIdx = 0; nNetworkIdx < dbNetwork.GetSize(); nNetworkIdx++ )
	{
		CIDraughtExportStatus info;

		if ( dbNetwork.GetConnectionType( nNetworkIdx ) != CONNECTION_TYPE_STANDARD_REALTIME )
			info.m_nStatus = IDRAUGHT_EXPORT_STATUS_NOT_REALTIME;
		else
		{
			int nStartIdx, nEndIdx;
			dbLocation.GetNetworkLocIdxRange( nNetworkIdx, nStartIdx, nEndIdx );

			if ( nStartIdx <= nEndIdx )
				info.m_nStatus = IDRAUGHT_EXPORT_STATUS_NO_DBLINK;
			else
				info.m_nStatus = IDRAUGHT_EXPORT_STATUS_NO_LOCATIONS;
		}

		arrayStatus.Add( info );
	}

	for ( int nMapIdx = 0; nMapIdx < m_NetworkMap.GetSize(); nMapIdx++ )
	{
		int nNetworkIdx = m_NetworkMap[ nMapIdx ].m_nNetworkIdx;

		if ( ( nNetworkIdx >= 0 ) && ( nNetworkIdx <= arrayStatus.GetSize() ) )
		{
			if ( FALSE == m_NetworkMap[ nMapIdx ].m_bGotDate )
				arrayStatus[ nNetworkIdx ].m_nStatus = IDRAUGHT_EXPORT_STATUS_WAIT_DATE;
			else if ( TRUE == m_NetworkMap[ nMapIdx ].m_bBadDate )
				arrayStatus[ nNetworkIdx ].m_nStatus = IDRAUGHT_EXPORT_STATUS_BAD_DATE;
			else
			{
				COleDateTime date = m_NetworkMap[ nMapIdx ].m_dateLastExport;

				if ( date.GetStatus() != COleDateTime::valid )
					arrayStatus[ nNetworkIdx ].m_nStatus = IDRAUGHT_EXPORT_STATUS_BAD_DATE;
				else
				{
					arrayStatus[ nNetworkIdx ].m_nStatus = IDRAUGHT_EXPORT_STATUS_OK;
					arrayStatus[ nNetworkIdx ].m_dateLastExport = date;
				}
			}
		}
	}
}

/**********************************************************************/

void CIDraughtExportManager::HandleSetupOptions( CWnd* pParent )
{
	CPropertySheetIDraughtOptions propSheet ( pParent );
		
	CIDraughtOptions oldOptions;
	oldOptions.CopyFrom( IDraughtOptions );
	IDraughtOptions.SetSiteIDBuffer( "" );
	
	if ( propSheet.DoModal() != IDOK )
		IDraughtOptions.CopyFrom( oldOptions );
	else
	{
		IDraughtOptions.Write();

		int nEndDbIdx = ( EcrmanOptions.GetGlobalDepartmentFlag() ? 0 : dbDatabase.GetSize() - 1 );
		__int64 nTarget = ( Dept::DeptNo.Max + 1 ) * ( nEndDbIdx + 1 );
		__int64 nProgress = 0;

		StatusProgress.Lock( "Updating Departments" );

		for ( int nDbIdx = 0; nDbIdx <= nEndDbIdx; nDbIdx++ )
		{
			CDataManagerInfo info;
			DataManager.OpenDatabaseQuickNoFiles( nDbIdx, FALSE );
			
			if ( DataManager.OpenDepartment( DB_READWRITE, info ) == TRUE )
			{
				for ( int nDeptIdx = 0; nDeptIdx < DataManager.Department.GetSize(); nDeptIdx++ )
				{
					StatusProgress.SetPos( nProgress + nDeptIdx, nTarget );

					CDepartmentCSVRecord Department;
					DataManager.Department.GetAt( nDeptIdx, Department );
					Department.SetIDraughtExportFlag( IDraughtOptions.GetDatabaseDepartmentFlag( nDbIdx, Department.GetDeptNo() ) );
					DataManager.Department.SetAt( nDeptIdx, Department );
				}

				DataManager.WriteDepartment( info );
			}

			nProgress += Dept::DeptNo.Max + 1;
		}

		StatusProgress.Unlock();

		CCSV csv( IDraughtOptions.GetSiteIDBuffer() );
		if ( csv.GetSize() == dbLocation.GetSize() )
		{
			for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
			{
				dbLocation.SetIDraughtID( nLocIdx, csv.GetString( nLocIdx ) );
				dbLocation.WriteLocation( nLocIdx );
			}

			dbLocation.UpdateIDraughtStatus();
		}
	}
}

/**********************************************************************/

long CIDraughtExportManager::HandleIDraughtExport( CWnd* pParent )
{
	if ( dbLocation.GetIDraughtStatus() == IDRAUGHT_STATUS_UNKNOWN )
		dbLocation.UpdateIDraughtStatus();

	if ( dbLocation.GetIDraughtStatus() != IDRAUGHT_STATUS_VALID )
	{
		m_strExtraErrorInfo = "";
		return IDRAUGHT_EXPORT_ERROR_CONFIG;
	}

	long lResult = 0;

	CSSFile fileLock;
	CFilenameUpdater FnUp( SysFiles::IDraughtLockFile );

	if ( fileLock.Open( FnUp.GetFilenameToUse(), "wb" ) == FALSE )
	{
		m_strExtraErrorInfo = "";
		lResult = IDRAUGHT_EXPORT_ERROR_LOCK;
	}
	else
	{
		CString strAction;
		int nNetworkIdx = GetNetworkIdx();
		strAction.Format( "i Draught Export (%s)", dbNetwork.GetName( nNetworkIdx ) );

		CLockManagerInfo infoLock;
		infoLock.m_bLockAllUsers = FALSE;
		infoLock.m_nCommsAction = COMMS_ACTION_PAUSE;
		infoLock.m_bStopComms = TRUE;
		infoLock.m_bStopCommsPMSExemption = FALSE;
		infoLock.m_strStopCommsPrompt = strAction;
		infoLock.m_bStopPosTray = TRUE;
		infoLock.m_pParentWnd = pParent;

		lResult = IDRAUGHT_EXPORT_ERROR_PAUSE;
		m_strExtraErrorInfo = "";

		CLockManager LockManager( infoLock );

		if ( LockManager.GetLock() == TRUE )
		{
			lResult = CommsHandshaker.FetchAllRealTimeSales( nNetworkIdx, CASHRSP_FLUSH_IDRAUGHT );
				
			if ( IDRAUGHT_EXPORT_ERROR_NONE != lResult )
			{
				SetExportRetryTime();
			}
			else
			{
				CommsHandshaker.ProcessSales();

				COleDateTime dateExport = GetDateExportFrom();

				while( dateExport <= GetDateExportTo() )
				{
					CIDraughtExportCreator ExportCreator( dateExport );
						
					lResult = ExportCreator.CreateExport();

					if ( IDRAUGHT_EXPORT_ERROR_ALREADY_SENT == lResult )
					{
						SetLastExportDate( dateExport );
						break;
					}

					if ( IDRAUGHT_EXPORT_ERROR_NONE != lResult )
					{
						SetExportRetryTime();
						break;
					}
							
					SetLastExportDate( dateExport );
					CDateRangeHelpers DateRangeHelpers;
					DateRangeHelpers.AddDaysToTime( dateExport, 1 );		
				}
			}
		}
	}

	return lResult;
}

/**********************************************************************/

long CIDraughtExportManager::HandleIDraughtTransfer( CWnd* pParent )
{
	{
		CString strFTPServer = IDraughtOptions.GetFTPServer();
		CString strFTPUserName = IDraughtOptions.GetFTPUserName();
		CString strFTPPassword = IDraughtOptions.GetFTPPassword();

		if ( ( strFTPServer == "" ) || ( strFTPUserName == "" ) || ( strFTPPassword == "" ) )
			return IDRAUGHT_EXPORT_ERROR_FTP_CONFIG;
	}

	CStringArray arrayFilesToDelete, arrayFilesToTransfer;

	CString strSourcePath = "IDraught\\Pending\\";
	GetDataProgramPath( strSourcePath );

	CString strSentPath = "IDraught\\Sent\\";
	GetDataProgramPath( strSentPath );

	CFileFind finder;
	BOOL bWorking = finder.FindFile ( strSourcePath + "*.csv" );

	long lResult = IDRAUGHT_EXPORT_ERROR_NONE;

	while ( bWorking == TRUE )
	{
		bWorking = finder.FindNextFile();

		if ( finder.IsDirectory() == TRUE )
			continue;

		if ( finder.IsDots() == TRUE )
			continue;

		CString strFilename = finder.GetFileName();
		if ( strFilename.GetLength() != 17 )
			continue;

		if ( ::TestNumeric( strFilename.Left(8) ) == FALSE )
			continue;

		if ( ::TestNumeric( strFilename.Mid(10,3) ) == FALSE )
			continue;

		if ( ::FileExists( strSentPath + strFilename ) == TRUE )
			arrayFilesToDelete.Add( strFilename );
		else
			arrayFilesToTransfer.Add( strFilename );
	}

	for ( int n = 0; n < arrayFilesToDelete.GetSize(); n++ )
		CFileRemover FileRemover( strSourcePath + arrayFilesToDelete.GetAt(n) );

	if ( arrayFilesToTransfer.GetSize() != 0 )
	{
		CSSFile fileLock;
		CFilenameUpdater FnUp( SysFiles::IDraughtLockFile );

		if ( fileLock.Open( FnUp.GetFilenameToUse(), "wb" ) == FALSE )
		{
			m_strExtraErrorInfo = "";
			lResult = IDRAUGHT_EXPORT_ERROR_LOCK;
		}
		else
		{
			bool bDoFTP = ( IDraughtOptions.GetFiddleLevel() != 2 );

			if ( TRUE == bDoFTP )
			{
				CFtpConnectDlg dlgConnect( 0, FALSE, pParent );
				dlgConnect.DoModal();
				pParent -> RedrawWindow();

				if ( dlgConnect.GetStatus() != CONNECT_STATUS_FINISHED )
				{
					m_strExtraErrorInfo = "";
					lResult = IDRAUGHT_EXPORT_ERROR_FTP_CONNECT;
				}
			}

			CString strDestPath = "";
			
			if ( IDRAUGHT_EXPORT_ERROR_NONE == lResult )
			{
				StatusProgress.Lock( TRUE, "Transferring IDraught Files" );

				int nSize = arrayFilesToTransfer.GetSize();
				for ( int nIdx = 0; nIdx < nSize; nIdx++ )
				{
					CString strFilename = arrayFilesToTransfer.GetAt( nIdx );

					StatusProgress.SetPos( nIdx, nSize );
					StatusProgress.SetDialogText( strFilename );

					if ( ( FALSE == bDoFTP ) || ( FtpConnect.PutFile( strSourcePath + strFilename, strDestPath + strFilename ) == TRUE ) )
					{
						rename( strSourcePath + strFilename, strSentPath + strFilename );

						CMessageLoggerInfo info;
						info.m_bAddDate = TRUE;
						info.m_bAddNetworkName = FALSE;
						info.m_bAddTimeOfDay = TRUE;
						info.m_bFollowingLineNetwork = FALSE;
						info.m_bFollowingLineSystem = FALSE;
						info.m_bLeadingLineNetwork = FALSE;
						info.m_bLeadingLineSystem = FALSE;
						info.m_bLogSystemStartStop = FALSE;
						info.m_nHideLevel = 0;
						info.m_strMessage = "Sent File : ";
						info.m_strMessage += strFilename;
						info.m_strStatus = "OK";

						MessageLogger.LogIDraughtSystem( info );
					}
					else
					{
						m_strExtraErrorInfo = strFilename;
						lResult = IDRAUGHT_EXPORT_ERROR_FTP_PUTFILE;
						break;
					}
				}

				StatusProgress.Unlock();
				
				if ( TRUE == bDoFTP )
					FtpConnect.Disconnect();
			}
		}
	}
	
	return lResult;
}

/**********************************************************************/

void CIDraughtExportManager::LogResult( long lResult )
{
	CMessageLoggerInfo info;
	info.m_bAddDate = TRUE;
	info.m_bAddTimeOfDay = TRUE;
	info.m_bFollowingLineNetwork = FALSE;
	info.m_bFollowingLineSystem = FALSE;
	info.m_bLeadingLineNetwork = FALSE;
	info.m_bLeadingLineSystem = FALSE;
	info.m_bLogSystemStartStop = FALSE;
	info.m_nHideLevel = 0;

	CString strExtra = "";
	if ( m_strExtraErrorInfo != "" )
	{
		strExtra += " (";
		strExtra += m_strExtraErrorInfo;
		strExtra += ")";
	}
	
	switch( lResult )
	{
	case IDRAUGHT_EXPORT_ERROR_NONE:
	case IDRAUGHT_EXPORT_ERROR_LOCK:
	case IDRAUGHT_EXPORT_ERROR_CONFIG:
	case IDRAUGHT_EXPORT_ERROR_PAUSE:
		break;

	case IDRAUGHT_EXPORT_ERROR_NETWORK_NOPATH:
	case IDRAUGHT_EXPORT_ERROR_NETWORK_LOCK:
	case IDRAUGHT_EXPORT_ERROR_ALREADY_SENT:
	case IDRAUGHT_EXPORT_ERROR_READ_SALES:
	case IDRAUGHT_EXPORT_ERROR_COPY_SALES:
	case IDRAUGHT_EXPORT_ERROR_DATABASE_OPEN:
	case IDRAUGHT_EXPORT_ERROR_DEPT_ENABLE:
	case IDRAUGHT_EXPORT_ERROR_SOURCEFILE_OPEN:
	case IDRAUGHT_EXPORT_ERROR_DESTFILE_DELETE:
	case IDRAUGHT_EXPORT_ERROR_DESTFILE_APPEND:
	case IDRAUGHT_EXPORT_ERROR_DESTFILE_RENAME:
		info.m_nNetworkIdx = IDraughtExportManager.GetNetworkIdx();
		info.m_bAddNetworkName = TRUE;
		info.m_strMessage.Format( "Export error %d", lResult );
		info.m_strMessage += strExtra;
		MessageLogger.LogIDraughtSystem( info );
		break;

	case IDRAUGHT_EXPORT_ERROR_FTP_CONNECT:
	case IDRAUGHT_EXPORT_ERROR_FTP_PUTFILE:
	case IDRAUGHT_EXPORT_ERROR_FTP_CONFIG:
		info.m_nNetworkIdx = 0;
		info.m_bAddNetworkName = FALSE;
		info.m_strMessage.Format( "FTP error %d", lResult );
		info.m_strMessage += strExtra;
		MessageLogger.LogIDraughtSystem( info );
		break;
	}

	m_strExtraErrorInfo = "";
}

/**********************************************************************/

void CIDraughtExportManager::ResetExportDatesForTesting()
{
	COleDateTime date;
	SetInitialExportDate( date );

	for ( int n = 0; n < m_NetworkMap.GetSize(); n++ )
	{
		m_NetworkMap[n].m_dateLastExport = date;
		m_NetworkMap[n].m_timeRetryExport = date;
	}
}

/**********************************************************************/
