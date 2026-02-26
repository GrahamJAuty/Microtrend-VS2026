/**********************************************************************/
#include "PosTrayDateRange.h"
#include "PosTrayOptions.h"
/**********************************************************************/
#include "PosTrayExportCreatorBase.h"
/**********************************************************************/

CPosTrayExportCreatorBase::CPosTrayExportCreatorBase( CPosTrayTask& Task, bool& bQuitNow, void* pHostDialog, int nManualExportNo) : m_Task( Task ), m_bQuitNow( bQuitNow )
{
	m_pHostDialog = pHostDialog;
	m_nManualExportNo = nManualExportNo;
	m_bQuitNow = FALSE;

	m_dateTask = COleDateTime( 2019, 1, 1, 0, 0, 0 );

	m_strFixedDateSuffix = "";
	m_strExportFileType = "";	
	m_strFixedDateSuffix = "";
	m_strTempFilepath = "";
	m_strExportFolderTarget = "";
	m_strExportFolderPending = "";
	m_strInternalFolderName = "";
	m_strEmailFolder = "";
	m_strEmailSubjectType = "";

	m_bWantCustomerFile = FALSE;
}

/**********************************************************************/

__int64 CPosTrayExportCreatorBase::ProcessTask( COleDateTime& dateTask )
{
	m_dateTask = dateTask;

	{
		CDataManagerInfo info;
		DataManagerNonDb.OpenReportSession( DB_READONLY, info );
		DataManagerNonDb.OpenTimeSliceList( DB_READONLY, info );
		DataManagerNonDb.OpenSystemPluFilterNames( DB_READONLY, info );
		DataManagerNonDb.OpenSystemPluFilterItems( DB_READONLY, info );
		DataManagerNonDb.OpenSystemDepartment( DB_READONLY, info );
		DataManagerNonDb.OpenSystemDepartmentSet( DB_READONLY, info );
	}

	DataManagerNonDb.ServerNameTable.BuildList();
	if ( TRUE == m_bWantCustomerFile ) DataManagerNonDb.CustomerNameTable.BuildList();
	DataManagerNonDb.EODDateTimeFilterArray.PrepareForUse();
	DataManagerNonDb.SessionDateTimeFilter.Reset();

	__int64 nLineCount = ProcessTaskInternal();

	{
		CDataManagerInfo info;
		DataManagerNonDb.CloseReportSession( info );
		DataManagerNonDb.CloseTimeSliceList( info );
		DataManagerNonDb.CloseSystemPluFilterNames( info );
		DataManagerNonDb.CloseSystemPluFilterItems( info );
		DataManagerNonDb.CloseSystemDepartment( info );
		DataManagerNonDb.CloseSystemDepartmentSet( info );
	}

	DataManagerNonDb.ServerNameTable.ClearList();
	if ( TRUE == m_bWantCustomerFile ) DataManagerNonDb.CustomerNameTable.ClearList();
	DataManagerNonDb.EODDateTimeFilterArray.ClearAfterUse();
	DataManagerNonDb.SessionDateTimeFilter.Reset();
	
	return nLineCount;
}

/**********************************************************************/

void CPosTrayExportCreatorBase::FixDates( COleDateTime& dateFrom, COleDateTime& dateTo )
{
	m_oleDateFrom = dateFrom;
	m_oleDateTo = dateTo;
	m_strFixedDateSuffix.Format( "_%4.4d%2.2d%2.2d", dateFrom.GetYear(), dateFrom.GetMonth(), dateFrom.GetDay() );
}

/**********************************************************************/

void CPosTrayExportCreatorBase::RoundDates()
{
	if ( m_strFixedDateSuffix == "" )
	{
		switch ( m_Task.GetBatchDateRoundTo() )
		{
		case 1:	//ROUND TO WEEK
			{
				int nWeekStartDay = EcrmanOptions.GetReportsWeekStartDay();
					
				int nWeekDayFrom = m_oleDateFrom.GetDayOfWeek() - 1;
				int nOffsetFrom = nWeekDayFrom - nWeekStartDay;
				if ( nOffsetFrom < 0 ) nOffsetFrom += 7;
				m_oleDateFrom -= COleDateTimeSpan ( nOffsetFrom, 0, 0, 0 );

				int nWeekDayTo = m_oleDateTo.GetDayOfWeek() - 1;
				int nOffsetTo = ( nWeekStartDay - nWeekDayTo ) - 1;
				if ( nOffsetTo < 0 ) nOffsetTo += 7;
				m_oleDateTo += COleDateTimeSpan ( nOffsetTo, 0, 0, 0 );
			}
			break;

		case 2:	//ROUND TO MONTH
			{
				int nYearFrom = m_oleDateFrom.GetYear();
				int nMonthFrom = m_oleDateFrom.GetMonth();
				m_oleDateFrom = COleDateTime( nYearFrom, nMonthFrom, 1, 0, 0, 0 );
			
				int nYearTo = m_oleDateTo.GetYear();
				int nMonthTo = m_oleDateTo.GetMonth();
					
				nMonthTo++;
				if ( nMonthTo > 12 )
				{
					nYearTo++;
					nMonthTo = 1;
				}

				m_oleDateTo = COleDateTime( nYearTo, nMonthTo, 1, 0, 0, 0 );
				m_oleDateTo -= COleDateTimeSpan ( 1, 0, 0, 0 );
			}

		default:
			break;
		}
	}
}

/**********************************************************************/

void CPosTrayExportCreatorBase::PrepareDatesAndFolders()
{
	m_strEmailFolder = "PosTray\\Email";
	::GetSyssetProgramPath( m_strEmailFolder );
	::CreateSubdirectory( m_strEmailFolder );
	m_strEmailFolder += "\\";
	m_strEmailFolder += m_strInternalFolderName;
	::CreateSubdirectory( m_strEmailFolder );

	//EXPORT FOLDER
	{
		m_strExportFolderTarget = m_Task.GetTaskExportFolder();
		::TrimSpaces( m_strExportFolderTarget, FALSE );
		
		if ( m_strExportFolderTarget == "" )
		{
			m_strExportFolderTarget = "PosTray\\Exports";
			::GetSyssetProgramPath( m_strExportFolderTarget );
		}

		::CreateSubdirectory( m_strExportFolderTarget );

		m_strExportFolderPending = "PosTray\\Pending";
		::GetSyssetProgramPath( m_strExportFolderPending );
		::CreateSubdirectory( m_strExportFolderPending );
		m_strExportFolderPending += "\\";
		m_strExportFolderPending += m_strInternalFolderName;
		::CreateSubdirectory( m_strExportFolderPending );
	}

	if ( m_strFixedDateSuffix == "" )
	{
		COleDateTime oleDummy;
		CPosTrayDateRange dateRange;
		
		if ( PosTrayOptions.GetBusinessDateFlag() == TRUE )
			dateRange.SetDateToday( m_dateTask, EcrmanOptions.GetReportsDayStartHour(), EcrmanOptions.GetReportsDayStartMinute() );
		else
			dateRange.SetDateToday( m_dateTask );
		
		dateRange.GetRange( m_Task.GetBatchDateType(), m_oleDateFrom, m_oleDateTo, oleDummy );
		RoundDates();
	}
	else
	{
		CString strExtra;
		strExtra.Format( "\\Manual%4.4d", m_nManualExportNo );
		m_strExportFolderTarget += strExtra;
		::CreateSubdirectory( m_strExportFolderTarget );
		m_strExportFolderPending = m_strExportFolderTarget;
	}
}

/**********************************************************************/

void CPosTrayExportCreatorBase::PrepareSessionAndTimeSlice( CTimeSliceMapCSVRecord& TimeSliceRecord )
{
	CString strSession = m_Task.GetBatchSessionEntity();
	if ( strSession == "" )
		strSession = m_Task.GetBatchSessionLegacy();

	m_ReportSession.ClearRecord();

	if ( strSession == "EOD" )
	{
		m_ReportSession.SetEODFlag( TRUE );
	}
	else
	{
		for ( int n = 0; n < DataManagerNonDb.ReportSession.GetSize(); n++ )
		{
			CSessionCSVRecord arrayRecord;
			DataManagerNonDb.ReportSession.GetAt( n, arrayRecord );

			if ( strSession == arrayRecord.GetEntityID() )
			{
				DataManagerNonDb.ReportSession.GetAt( n, m_ReportSession );				
				break;
			}
		}
	}
	
	{
		CString strTimeSlice = m_Task.GetBatchTimeSlice();

		for ( int n = 0; n < DataManagerNonDb.TimeSliceList.GetSize(); n++ )
		{
			CTimeSliceMapCSVRecord arrayRecord;
			DataManagerNonDb.TimeSliceList.GetAt( n, arrayRecord );

			if ( strTimeSlice == arrayRecord.GetEntityID() )
			{
				DataManagerNonDb.TimeSliceList.GetAt( n, TimeSliceRecord );				
				break;
			}
		}
	}
}

/**********************************************************************/

bool CPosTrayExportCreatorBase::ProcessExport()
{
	if ( ::ExistSubdirectory( m_strExportFolderPending ) == FALSE )
		return FALSE;

	if ( ( 0 == m_Task.GetTaskFormatEmail() ) && ( m_strFixedDateSuffix == "" ) )
	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		CString strEmailFilename;
		strEmailFilename.Format( "\\%s_%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d.CSV",
			(const char*) m_strExportFileType,
			timeNow.GetYear(),
			timeNow.GetMonth(),
			timeNow.GetDay(),
			timeNow.GetHour(),
			timeNow.GetMinute(),
			timeNow.GetSecond() );

		CString strNKNFilename;
		strNKNFilename = strEmailFilename.Left( strEmailFilename.GetLength() - 3 );
		strNKNFilename += "nkn";

		::CopyFile( m_strTempFilepath, m_strEmailFolder + strEmailFilename, FALSE );

		CSSFile fileNKN;
		if ( fileNKN.Open( m_strEmailFolder + strNKNFilename, "wb" ) == TRUE )
		{
			CString strSubject = "";
			strSubject += "CSV Export of ";
			strSubject += m_strEmailSubjectType;

			fileNKN.WriteLine( m_Task.GetTaskEmailNickname(0) );
			fileNKN.WriteLine( strSubject );
			fileNKN.Close();
		}
	}

	if ( ( 0 == m_Task.GetTaskFormatExport() ) || ( m_strFixedDateSuffix != "" ) )
		if ( false == m_bQuitNow )
			CreateExportFile();

	if ( ( m_Task.GetTaskFTPFlag() == TRUE ) && ( PosTrayOptions.GotFTPSettings() == TRUE ) )
	{
		if ( FALSE == m_bQuitNow )
		{
			CString strDestFolder = m_Task.GetLiveTaskFolder();
			strDestFolder += "\\FTP";
			::CreateSubdirectory( strDestFolder );
			::CopyFile( m_strTempFilepath, strDestFolder + m_Task.GetExportFilename( m_strExportFileType, m_strFixedDateSuffix, m_dateTask ), FALSE );
		}
	}

	return TRUE;
}

/**********************************************************************/

void CPosTrayExportCreatorBase::CreateExportFile()
{
	CString strSlash = "\\";

	CString strExportFilenameTarget = m_Task.GetExportFilename( m_strExportFileType, m_strFixedDateSuffix, m_dateTask );	
	strExportFilenameTarget.TrimLeft(strSlash);

	if ( m_strFixedDateSuffix != "" )
	{
		CopyFile( m_strTempFilepath, m_strExportFolderTarget + strSlash + strExportFilenameTarget, false );
	}
	else
	{
		CString strExportFilenamePending = "";
		strExportFilenamePending += CSSMD5::CalcMD5(m_strExportFolderTarget + strSlash + strExportFilenameTarget);
		strExportFilenamePending += ".CSV";

		CString strInfoFilename = strExportFilenamePending.Left( strExportFilenamePending.GetLength() - 3 );
		strInfoFilename += "001";

		CSSFile file001;
		if ( file001.Open( m_strExportFolderPending + strSlash + strInfoFilename, "wb" ) == TRUE )
		{
			file001.WriteLine( strExportFilenameTarget );
			file001.WriteLine( m_Task.GetFilename( FALSE ) );
			file001.Close();
		}

		::CopyFile( m_strTempFilepath, m_strExportFolderPending + strSlash + strExportFilenamePending, FALSE );
	}
}

/**********************************************************************/
