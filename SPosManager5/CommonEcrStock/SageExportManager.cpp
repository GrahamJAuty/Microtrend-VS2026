/**********************************************************************/
#include "CashRSPFlusher.h"
#include "CommsHandshaker.h"
#include "DatabaseCSVArray.h"
#include "DateRangeHelpers.h"
#include "LocationCSVArray.h"
#include "LockManager.h"
#include "MessageLogger.h"
#include "NetworkCSVArray.h"
#include "SageExportCreator.h"
#include "SageExportOptionsDlg.h"
#include "SageOptions.h"
/**********************************************************************/
#include "SageExportManager.h"
/**********************************************************************/

CSageExportManager::CSageExportManager(void)
{
	m_bReadyToGo = FALSE;
	m_nDatabaseMapIdx = 0;
	SetInitialExportDate( m_dateExportFrom );
	SetInitialExportDate( m_dateExportTo );
	
	CDateRangeHelpers DateRangeHelpers;
	DateRangeHelpers.AddDaysToTime( m_dateExportFrom, 1 );

	m_strExtraErrorInfo = "";
}

/**********************************************************************/

void CSageExportManager::SetInitialExportDate( COleDateTime& date )
{
	date = COleDateTime( 2013, 1, 1, 0, 0, 0 );
}

/**********************************************************************/

void CSageExportManager::SetDummyFutureDate( COleDateTime& date )
{
	date = COleDateTime( 3000, 1, 1, 0, 0, 0 );
}

/**********************************************************************/

void CSageExportManager::BuildDatabaseMap()
{
	m_bReadyToGo = FALSE;

	m_DatabaseMap.RemoveAll();
	m_arrayStatus.RemoveAll();
	m_nDatabaseMapIdx = 0;

	for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
	{
		CArray<int,int> arrayLocIdx;
		dbLocation.GetDatabaseLocIdxSet( nDbIdx, arrayLocIdx );

		int nCurrentNwkNo = -1;
		bool bSkipThisDb = FALSE;
		CCSV csvNwkNo;

		for ( int l = 0; ( l < arrayLocIdx.GetSize() ) && ( FALSE == bSkipThisDb ); l++ )
		{
			int nLocIdx = arrayLocIdx.GetAt(l);
			int nNwkNo = dbLocation.GetNetworkNo( nLocIdx );

			if ( nNwkNo != nCurrentNwkNo )
			{
				int nNwkIdx;
				if ( dbNetwork.FindNetworkByNumber( nNwkNo, nNwkIdx ) == FALSE )
					bSkipThisDb = TRUE;
				else if ( dbNetwork.GetConnectionType( nNwkIdx ) != CONNECTION_TYPE_STANDARD_REALTIME )
					bSkipThisDb = TRUE;
				else
					csvNwkNo.Add( nNwkNo );
			}

			nCurrentNwkNo = nNwkNo;
		}

		if ( TRUE == bSkipThisDb )
		{
			CSageExportStatus infoStatus;
			infoStatus.m_nStatus = SAGE_EXPORT_STATUS_NOT_REALTIME;
			m_arrayStatus.Add( infoStatus );
		}
		else
		{
			if ( csvNwkNo.GetSize() == 0 )
			{
				CSageExportStatus infoStatus;
				infoStatus.m_nStatus = SAGE_EXPORT_STATUS_NO_LOCATIONS;
				m_arrayStatus.Add( infoStatus );
			}
			else
			{
				CSageExportInfo info;
				info.m_nDbIdx = nDbIdx;
				info.m_strNetworkList = csvNwkNo.GetLine();
				info.m_bGotDate = FALSE;
				info.m_bBadDate = FALSE;
				info.m_lLastDatabaseError = 0;
				info.m_nLastDatabaseErrorCount = 0;
				SetInitialExportDate( info.m_timeRetryExport );
				SetDummyFutureDate( info.m_dateLastExport );
				m_DatabaseMap.Add( info );

				CSageExportStatus infoStatus;
				infoStatus.m_nStatus = SAGE_EXPORT_STATUS_OK;
				m_arrayStatus.Add( infoStatus );
			}
		}
	}

#ifdef _DEBUG

	CSSFile fileSageMap;

	CString strFilename = "SageMap.txt";
	GetDataProgramPath( strFilename );

	if ( fileSageMap.Open( strFilename, "wb" ) == TRUE )
	{
		for ( int n = 0; n < m_DatabaseMap.GetSize(); n++ )
		{
			CString str;
			str.Format( "%d   ", m_DatabaseMap[n].m_nDbIdx );
			str += m_DatabaseMap[n].m_strNetworkList;
			fileSageMap.WriteLine( str );
		}
	}

#endif

	m_bReadyToGo = TRUE;
}

/**********************************************************************/

void CSageExportManager::NextDatabaseMapIdx()
{
	m_nDatabaseMapIdx++;
	if ( m_nDatabaseMapIdx >= m_DatabaseMap.GetSize() )
		m_nDatabaseMapIdx = 0;
}

/**********************************************************************/

int CSageExportManager::GetDatabaseIdx()
{
	if ( ( m_nDatabaseMapIdx >= 0 ) && ( m_nDatabaseMapIdx < m_DatabaseMap.GetSize() ) )
		return m_DatabaseMap[ m_nDatabaseMapIdx ].m_nDbIdx;
	else
		return 0;
}

/**********************************************************************/

const char* CSageExportManager::GetNetworkList()
{
	if ( ( m_nDatabaseMapIdx >= 0 ) && ( m_nDatabaseMapIdx < m_DatabaseMap.GetSize() ) )
		m_strNetworkList = m_DatabaseMap[ m_nDatabaseMapIdx ].m_strNetworkList;
	else
		m_strNetworkList = "";

	return m_strNetworkList;
}

/**********************************************************************/

bool CSageExportManager::GetBadDateFlag()
{
	if ( ( m_nDatabaseMapIdx >= 0 ) && ( m_nDatabaseMapIdx < m_DatabaseMap.GetSize() ) )
		return m_DatabaseMap[ m_nDatabaseMapIdx ].m_bBadDate;
	else
		return TRUE;
}

/**********************************************************************/

void CSageExportManager::GetExportRetryTime( COleDateTime& time )
{
	if ( ( m_nDatabaseMapIdx >= 0 ) && ( m_nDatabaseMapIdx < m_DatabaseMap.GetSize() ) )
		time =  m_DatabaseMap[ m_nDatabaseMapIdx ].m_timeRetryExport;
	else
		SetDummyFutureDate( time );
}

/**********************************************************************/

void CSageExportManager::SetExportRetryTime()
{
	if ( ( m_nDatabaseMapIdx >= 0 ) && ( m_nDatabaseMapIdx < m_DatabaseMap.GetSize() ) )
		m_DatabaseMap[ m_nDatabaseMapIdx ].m_timeRetryExport = COleDateTime::GetCurrentTime() + COleDateTimeSpan ( 0, 0, 2, 0 );
}

/**********************************************************************/

void CSageExportManager::ResetExportRetryTime()
{
	for ( int nDbMapIdx = 0; nDbMapIdx < m_DatabaseMap.GetSize(); nDbMapIdx++ )
	{
		m_DatabaseMap[ nDbMapIdx ].m_bGotDate = FALSE;
		m_DatabaseMap[ nDbMapIdx ].m_timeRetryExport = COleDateTime::GetCurrentTime();
		m_DatabaseMap[ nDbMapIdx ].m_lLastDatabaseError = 999999;
	}
}

/**********************************************************************/

bool CSageExportManager::SetDatabaseError( long lError )
{
	bool bResult = FALSE;

	if ( ( m_nDatabaseMapIdx >= 0 ) && ( m_nDatabaseMapIdx < m_DatabaseMap.GetSize() ) )
	{
		if ( 0 == lError )
		{
			m_DatabaseMap[ m_nDatabaseMapIdx ].m_lLastDatabaseError = 0;
			m_DatabaseMap[ m_nDatabaseMapIdx ].m_nLastDatabaseErrorCount = 0;
		}
		else if ( lError == m_DatabaseMap[ m_nDatabaseMapIdx ].m_lLastDatabaseError )
		{
			m_DatabaseMap[ m_nDatabaseMapIdx ].m_nLastDatabaseErrorCount++;
			bResult = ( ( m_DatabaseMap[ m_nDatabaseMapIdx ].m_nLastDatabaseErrorCount % 10 ) == 0 );
		}
		else
		{
			m_DatabaseMap[ m_nDatabaseMapIdx ].m_lLastDatabaseError = lError;
			m_DatabaseMap[ m_nDatabaseMapIdx ].m_nLastDatabaseErrorCount = 1;
			bResult = TRUE;
		}
	}

	return bResult;
}

/**********************************************************************/

bool CSageExportManager::CheckExportTime()
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
	int nHourDelay = EcrmanOptions.GetReportsDayStartHour() + SageOptions.GetExportDelayTime();

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

void CSageExportManager::GetLastExportDate( COleDateTime& date )
{
	//NETWORK MAP INDEX OUT OF RANGE
	if ( ( m_nDatabaseMapIdx < 0 ) || ( m_nDatabaseMapIdx >= m_DatabaseMap.GetSize() ) )
	{
		SetDummyFutureDate( date );
		return;
	}

	//DATE ALREADY RETREIVED FROM FILE
	if ( TRUE == m_DatabaseMap[ m_nDatabaseMapIdx ].m_bGotDate )
	{
		date = m_DatabaseMap[ m_nDatabaseMapIdx ].m_dateLastExport;

		if ( date.GetStatus() != COleDateTime::valid )
			m_DatabaseMap[ m_nDatabaseMapIdx ].m_bBadDate = TRUE;

		return;
	}

	//FILE DOES NOT EXIST, SO USE DEFAULT START DATE
	int nDatabaseIdx = m_DatabaseMap[ m_nDatabaseMapIdx ].m_nDbIdx;
	CFilenameUpdater FnUp( SysFiles::SageDate, nDatabaseIdx  );
	CString strDateFile = FnUp.GetFilenameToUse();
	
	if ( ::FileExists( strDateFile ) == FALSE )
	{
		SetInitialExportDate( date );
		m_DatabaseMap[ m_nDatabaseMapIdx ].m_bGotDate = TRUE;
		m_DatabaseMap[ m_nDatabaseMapIdx ].m_bBadDate = FALSE;
		m_DatabaseMap[ m_nDatabaseMapIdx ].m_dateLastExport = date; 
		return;
	}

	//ASSUME INVALID DATE UNLESS PROVED OTHERWISE
	SetDummyFutureDate( date );
	m_DatabaseMap[ m_nDatabaseMapIdx ].m_bGotDate = TRUE;
	m_DatabaseMap[ m_nDatabaseMapIdx ].m_bBadDate = TRUE;
	m_DatabaseMap[ m_nDatabaseMapIdx ].m_dateLastExport = date; 
			
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
					m_DatabaseMap[ m_nDatabaseMapIdx ].m_bBadDate = FALSE;
					m_DatabaseMap[ m_nDatabaseMapIdx ].m_dateLastExport = dateFile;
					date = dateFile;
				}
			}
		}
	}
}

/**********************************************************************/

void CSageExportManager::SetLastExportDate( COleDateTime& date )
{
	if ( ( m_nDatabaseMapIdx >= 0 ) && ( m_nDatabaseMapIdx < m_DatabaseMap.GetSize() ) )
	{
		if ( date.GetStatus() == COleDateTime::valid )
		{
			int nDatabaseIdx = m_DatabaseMap[ m_nDatabaseMapIdx ].m_nDbIdx;
			CFilenameUpdater FnUp( SysFiles::SageDate, nDatabaseIdx  );
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

				m_DatabaseMap[ m_nDatabaseMapIdx ].m_bGotDate = TRUE;
				m_DatabaseMap[ m_nDatabaseMapIdx ].m_dateLastExport = COleDateTime( date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0 );
			}
		}
	}
}

/**********************************************************************/

void CSageExportManager::BuildDatabaseStatusArray( CArray<CSageExportStatus,CSageExportStatus>& arrayStatus )
{
	arrayStatus.RemoveAll();

	if ( m_arrayStatus.GetSize() == dbDatabase.GetSize() )
	{
		for ( int n = 0; n < m_arrayStatus.GetSize(); n++ )
			arrayStatus.Add( m_arrayStatus.GetAt(n) );

		for ( int nMapIdx = 0; nMapIdx < m_DatabaseMap.GetSize(); nMapIdx++ )
		{
			int nDatabaseIdx = m_DatabaseMap[ nMapIdx ].m_nDbIdx;

			if ( ( nDatabaseIdx >= 0 ) && ( nDatabaseIdx <= arrayStatus.GetSize() ) )
			{
				if ( FALSE == m_DatabaseMap[ nMapIdx ].m_bGotDate )
					arrayStatus[ nDatabaseIdx ].m_nStatus = SAGE_EXPORT_STATUS_WAIT_DATE;
				else if ( TRUE == m_DatabaseMap[ nMapIdx ].m_bBadDate )
					arrayStatus[ nDatabaseIdx ].m_nStatus = SAGE_EXPORT_STATUS_BAD_DATE;
				else
				{
					COleDateTime date = m_DatabaseMap[ nMapIdx ].m_dateLastExport;

					if ( date.GetStatus() != COleDateTime::valid )
						arrayStatus[ nDatabaseIdx ].m_nStatus = SAGE_EXPORT_STATUS_BAD_DATE;
					else
					{
						arrayStatus[ nDatabaseIdx ].m_nStatus = SAGE_EXPORT_STATUS_OK;
						arrayStatus[ nDatabaseIdx ].m_dateLastExport = date;
					}
				}
			}
		}
	}
}

/**********************************************************************/

void CSageExportManager::HandleSetupOptions( CWnd* pParent )
{
	CSageExportOptionsDlg dlgOptions ( pParent );
		
	CSageOptions oldOptions;
	oldOptions.CopyFrom( SageOptions );
	
	if ( dlgOptions.DoModal() != IDOK )
		SageOptions.CopyFrom( oldOptions );
	else
		SageOptions.Write();
}

/**********************************************************************/

long CSageExportManager::HandleSageExport( CWnd* pParent )
{
	CSSFile fileLock;
	CFilenameUpdater FnUp( SysFiles::SageLockFile );

	if ( fileLock.Open( FnUp.GetFilenameToUse(), "wb" ) == FALSE )
	{
		m_strExtraErrorInfo = "";
		return SAGE_EXPORT_ERROR_LOCK;
	}
	else
	{
		CString strAction;
		int nDatabaseIdx = GetDatabaseIdx();
		strAction.Format( "Sage Export (%s)", dbDatabase.GetName( nDatabaseIdx ) );

		CLockManagerInfo infoLock;
		infoLock.m_bLockAllUsers = FALSE;
		infoLock.m_nCommsAction = COMMS_ACTION_PAUSE;
		infoLock.m_bStopComms = TRUE;
		infoLock.m_bStopCommsPMSExemption = FALSE;
		infoLock.m_strStopCommsPrompt = strAction;
		infoLock.m_bStopPosTray = TRUE;
		infoLock.m_pParentWnd = pParent;

		long lResult = SAGE_EXPORT_ERROR_PAUSE;
		m_strExtraErrorInfo = "";

		CLockManager LockManager( infoLock );

		if ( LockManager.GetLock() == FALSE )
		{
			SetExportRetryTime();
		}
		else
		{
			bool bSalesImportFail = FALSE;
			CCSV csvNwkNo ( GetNetworkList() );

			for ( int n = 0; n < csvNwkNo.GetSize(); n++ )
			{
				int nNwkIdx;
				int nNwkNo = csvNwkNo.GetInt(n);
				if ( dbNetwork.FindNetworkByNumber( nNwkNo, nNwkIdx ) == FALSE )
				{
					m_strExtraErrorInfo.Format( "(Network %d)", nNwkNo );
					lResult = SAGE_EXPORT_ERROR_CONFIG;
					bSalesImportFail = TRUE;
					break;
				}

				lResult = CommsHandshaker.FetchAllRealTimeSales( nNwkIdx, CASHRSP_FLUSH_SAGE );
			
				if ( SAGE_EXPORT_ERROR_NONE != lResult	)
				{
					bSalesImportFail = TRUE;
					break;
				}
			}

			if ( TRUE == bSalesImportFail )
			{
				SetExportRetryTime();
			}
			else
			{
				CommsHandshaker.ProcessSales();

				COleDateTime dateExport = GetDateExportFrom();

				bool bError = FALSE;
				while ( ( dateExport <= GetDateExportTo() ) && ( FALSE == bError ) ) 
				{
					m_dateLastExportAttempt = dateExport;
					CSageExportCreator ExportCreator( dateExport );
					
					lResult = ExportCreator.CreateExport();

					switch ( lResult )
					{
					case SAGE_EXPORT_ERROR_NONE:
					case SAGE_EXPORT_ERROR_NODATA:
					case SAGE_EXPORT_ERROR_NOCODES:
						{
							LogResult( lResult );
							SetLastExportDate( dateExport );
							CDateRangeHelpers DateRangeHelpers;
							DateRangeHelpers.AddDaysToTime( dateExport, 1 );
							lResult = SAGE_EXPORT_ERROR_NONE;
						}
						break;

					default:
						SetExportRetryTime();
						bError = TRUE;
						break;
					}			
				}
			}
		}	

		return lResult;
	}
}

/**********************************************************************/

void CSageExportManager::LogResult( long lResult )
{
	CreateSyssetSubdirectory ( "SchedLog" );
	CreateSyssetSubdirectory ( "SchedOld" );

	CString	strNew = Super.SageLogAll();
	CString	strOld = Super.SageLogOld();
	CMessageLogger::CheckMaxLogSysFileSize( strNew, strOld );
	
	if ( ::FileExists ( strNew ) == FALSE )
	{
		CReportFile ReportFile;
		ReportFile.Open ( strNew );
		ReportFile.SetStyle1 ( "Sage Export Log" );
		ReportFile.AddColumn ( "Action Date",TA_LEFT, 280 );
		ReportFile.AddColumn ( "Action Time",TA_LEFT, 280 );
		ReportFile.AddColumn ( "Database",TA_LEFT, 350 );
		ReportFile.AddColumn ( "Export Date",TA_LEFT, 280 );
		ReportFile.AddColumn ( "Result",TA_LEFT, 700 );
		ReportFile.AddColumn ( "Detail",TA_LEFT, 1000 );
		ReportFile.WriteHeader();
		ReportFile.Close();
	}

	CString strExportDate;
	strExportDate.Format( "%2.2d/%2.2d/%4.4d",
		m_dateLastExportAttempt.GetDay(),
		m_dateLastExportAttempt.GetMonth(),
		m_dateLastExportAttempt.GetYear() );

	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	CString strCurrentDate;
	strCurrentDate.Format( "%2.2d/%2.2d/%4.4d",
		timeNow.GetDay(),
		timeNow.GetMonth(),
		timeNow.GetYear() );

	CString strCurrentTime;
	strCurrentTime.Format( "%2.2d:%2.2d:%2.2d",
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond() );

	CString strDatabase = dbDatabase.GetName( GetDatabaseIdx() );

	CString strReportExportDate = "";
	CString strReportError = "";
	CString strReportDetail = "";

	switch( lResult )
	{
	case SAGE_EXPORT_ERROR_NONE:
		strReportExportDate = strExportDate;
		strReportError = "OK";
		strReportDetail = m_strExtraErrorInfo;
		break;
	
	case SAGE_EXPORT_ERROR_LOCK:
		strReportExportDate = "";
		strReportError = "Unable to lock system for Sage export";
		strReportDetail = "";
		break;

	case SAGE_EXPORT_ERROR_CONFIG:
		strReportExportDate = "";
		strReportError = "Unknown network linked to database";
		strReportDetail = m_strExtraErrorInfo;
		break;

	case SAGE_EXPORT_ERROR_PAUSE:
		strReportExportDate = "";
		strReportError = "Unable to pause real time comms";
		strReportDetail = "";
		break;

	case SAGE_EXPORT_ERROR_NETWORK_NOPATH:
		strReportExportDate = "";
		strReportError = "No path defined for real time network";
		strReportDetail = m_strExtraErrorInfo;
		break;

	case SAGE_EXPORT_ERROR_NETWORK_LOCK:
		strReportExportDate = "";
		strReportError = "Unable to lock network and process sales";
		strReportDetail = m_strExtraErrorInfo;
		break;

	case SAGE_EXPORT_ERROR_READ_SALES:
		strReportExportDate = "";
		strReportError = "Unable to access CashRSP sales path";
		strReportDetail = m_strExtraErrorInfo;
		break;

	case SAGE_EXPORT_ERROR_COPY_SALES:
		strReportExportDate = "";
		strReportError = "Unable to import CashRSP sales";
		strReportDetail = m_strExtraErrorInfo;
		break;

	case SAGE_EXPORT_ERROR_DATABASE_OPEN:
		strReportExportDate = strExportDate;
		strReportError = "Unable to open database";
		strReportDetail = "";
		break;

	case SAGE_EXPORT_ERROR_NODATA:
		strReportExportDate = strExportDate;
		strReportError = "OK";
		strReportDetail = "(No sales data)";
		break;

	case SAGE_EXPORT_ERROR_SOURCEFILE_OPEN:
		strReportExportDate = strExportDate;
		strReportError = "Unable to open daily sales file";
		strReportDetail = m_strExtraErrorInfo;
		break;

	case SAGE_EXPORT_ERROR_DESTFILE_DELETE:
		strReportExportDate = strExportDate;
		strReportError = "Unable to delete temporary file";
		strReportDetail = m_strExtraErrorInfo;
		break;

	case SAGE_EXPORT_ERROR_DESTFILE_WRITE:
		strReportExportDate = strExportDate;
		strReportError = "Unable to create export file";
		strReportDetail = m_strExtraErrorInfo;
		break;

	case SAGE_EXPORT_ERROR_DESTFILE_RENAME:
		strReportExportDate = strExportDate;
		strReportError = "Unable to rename temporary file";
		strReportDetail = m_strExtraErrorInfo;
		break;
	}
	
	CSSFile fileExportLog;
	if ( fileExportLog.Open( strNew, "ab" ) == TRUE )
	{
		CCSV csv( '\t' );
		csv.Add( strCurrentDate );
		csv.Add( strCurrentTime );
		csv.Add( strDatabase );
		csv.Add( strReportExportDate );
		csv.Add( strReportError );
		csv.Add( strReportDetail );
		fileExportLog.WriteLine( csv.GetLine() );
	}
}

/**********************************************************************/

void CSageExportManager::ResetExportDatesForTesting()
{
	COleDateTime date;
	SetInitialExportDate( date );

	for ( int n = 0; n < m_DatabaseMap.GetSize(); n++ )
	{
		m_DatabaseMap[n].m_dateLastExport = date;
		m_DatabaseMap[n].m_timeRetryExport = date;
	}
}

/**********************************************************************/
