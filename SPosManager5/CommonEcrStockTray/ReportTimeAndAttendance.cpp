/**********************************************************************/
#include "CashRSPImporterDefines.h"
#include "DateRangeHelpers.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
/**********************************************************************/
#include "DateRangeHelpers.h"
/**********************************************************************/
#include "ReportTimeAndAttendance.h"
/**********************************************************************/

CEposReportConsolTimeAndAttendance::CEposReportConsolTimeAndAttendance()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolTimeAndAttendance::Reset()
{
	m_nServerNo = 0;
	m_nDbNo = 0;
	m_nNwkNo = 0;
	m_nLocNo = 0;
	m_strDate = "";
	m_strTime = "";
	m_nEventID = 0;
	m_strData = "";
	m_nHighestEdit = -1;
	m_bSingleEvent = FALSE;
}

/**********************************************************************/

void CEposReportConsolTimeAndAttendance::Add ( CEposReportConsolTimeAndAttendance& source )
{
	if ( TRUE == source.m_bSingleEvent )
	{
		if ( source.m_nHighestEdit >= m_nHighestEdit )
		{
			m_strData = source.m_strData;
			m_nHighestEdit = source.m_nHighestEdit;
		}
	}
	else if ( source.m_nHighestEdit >= m_nHighestEdit )
	{
		if (m_strData != "")
		{
			m_strData += ",";
		}

		m_strData += source.m_strData;

		m_nHighestEdit = source.m_nHighestEdit;
	}
	else
	{
		CCSV csvSource1( source.m_strData );
		CCSV csvSource2( csvSource1.GetString(0) );
		int nSourceEditCount = csvSource2.GetInt(0);
		bool bInserted = FALSE;
		
		CCSV csvThis1( m_strData );

		for ( int n = 0; ( n < csvThis1.GetSize() ) && ( FALSE == bInserted ); n++ )
		{
			CCSV csvThis2( csvThis1.GetString(n) );
			int nThisEditCount = csvThis2.GetInt(0);

			if ( nSourceEditCount < nThisEditCount )
			{
				csvThis1.InsertAt( n, csvSource2.GetLine() );
				m_strData = csvThis1.GetLine();
				bInserted = TRUE;
			}
		}
	}
}

/**********************************************************************/

int CEposReportConsolTimeAndAttendance::Compare( CEposReportConsolTimeAndAttendance& source, int nHint )
{
	switch( nHint )
	{
	case WORKING_REPORT_SYS:
		{
			if ( m_nServerNo != source.m_nServerNo )
				return ( ( m_nServerNo > source.m_nServerNo ) ? 1 : -1 );

			if ( m_nDbNo != source.m_nDbNo )
				return ( ( m_nDbNo > source.m_nDbNo ) ? 1 : -1 );

			if ( m_nNwkNo != source.m_nNwkNo )
				return ( ( m_nNwkNo > source.m_nNwkNo ) ? 1 : -1 );

			if ( m_nLocNo != source.m_nLocNo )
				return ( ( m_nLocNo > source.m_nLocNo ) ? 1 : -1 );

			if ( m_strDate != source.m_strDate )
				return ( ( m_strDate > source.m_strDate ) ? 1 : -1 );

			if ( m_strTime != source.m_strTime )
				return ( ( m_strTime > source.m_strTime ) ? 1 : -1 );

			if ( m_nEventID != source.m_nEventID )
				return ( ( m_nEventID > source.m_nEventID ) ? 1 : -1 );

			return 0;
		}
		break;

	case WORKING_REPORT_DB:
		{
			if ( m_nDbNo != source.m_nDbNo )
				return ( ( m_nDbNo > source.m_nDbNo ) ? 1 : -1 );

			if ( m_nServerNo != source.m_nServerNo )
				return ( ( m_nServerNo > source.m_nServerNo ) ? 1 : -1 );

			if ( m_nNwkNo != source.m_nNwkNo )
				return ( ( m_nNwkNo > source.m_nNwkNo ) ? 1 : -1 );

			if ( m_nLocNo != source.m_nLocNo )
				return ( ( m_nLocNo > source.m_nLocNo ) ? 1 : -1 );

			if ( m_strDate != source.m_strDate )
				return ( ( m_strDate > source.m_strDate ) ? 1 : -1 );

			if ( m_strTime != source.m_strTime )
				return ( ( m_strTime > source.m_strTime ) ? 1 : -1 );

			if ( m_nEventID != source.m_nEventID )
				return ( ( m_nEventID > source.m_nEventID ) ? 1 : -1 );

			return 0;
		}
		break;

	case WORKING_REPORT_LOC:
		{
			if ( m_nDbNo != source.m_nDbNo )
				return ( ( m_nDbNo > source.m_nDbNo ) ? 1 : -1 );

			if ( m_nNwkNo != source.m_nNwkNo )
				return ( ( m_nNwkNo > source.m_nNwkNo ) ? 1 : -1 );

			if ( m_nLocNo != source.m_nLocNo )
				return ( ( m_nLocNo > source.m_nLocNo ) ? 1 : -1 );

			if ( m_nServerNo != source.m_nServerNo )
				return ( ( m_nServerNo > source.m_nServerNo ) ? 1 : -1 );

			if ( m_strDate != source.m_strDate )
				return ( ( m_strDate > source.m_strDate ) ? 1 : -1 );

			if ( m_strTime != source.m_strTime )
				return ( ( m_strTime > source.m_strTime ) ? 1 : -1 );

			if ( m_nEventID != source.m_nEventID )
				return ( ( m_nEventID > source.m_nEventID ) ? 1 : -1 );

			return 0;
		}
		break;
	}

	return 0;
}


/**********************************************************************/

bool CEposReportConsolTimeAndAttendance::CompareServer( CEposReportConsolTimeAndAttendance& source )
{
	if ( m_nServerNo != source.m_nServerNo )
		return FALSE;

	if ( m_nDbNo != source.m_nDbNo )
		return FALSE;

	if ( m_nNwkNo != source.m_nNwkNo )
		return FALSE;

	if ( m_nLocNo != source.m_nLocNo )
		return FALSE;

	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolTAWorkHours::CEposReportConsolTAWorkHours()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolTAWorkHours::Reset()
{
	m_strDate = "";
	m_nMinutesWorked = 0;
	m_dEstPay = 0.0;
	m_dCapPay = 0.0;
}

/**********************************************************************/

void CEposReportConsolTAWorkHours::Add(CEposReportConsolTAWorkHours& source)
{
	m_nMinutesWorked += source.m_nMinutesWorked;
	m_dEstPay += source.m_dEstPay;
	m_dCapPay += source.m_dCapPay;
}

/**********************************************************************/

int CEposReportConsolTAWorkHours::Compare(CEposReportConsolTAWorkHours& source, int nHint)
{
	if (m_strDate != source.m_strDate)
		return ((m_strDate > source.m_strDate) ? 1 : -1);
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

void CTimeAndAttendanceExportDetail::LoadFromCSV(CCSV& csv)
{
	m_bBreak = csv.GetBool(0);
	m_strStartDate = csv.GetString(1);
	m_strStartTime = csv.GetString(2);
	m_strEndDate = csv.GetString(3);
	m_strEndTime = csv.GetString(4);
	m_nWeekday = csv.GetInt(5);
	m_nEventMinutes = csv.GetInt(6);
	m_strTimeSpan = csv.GetString(7);
	m_bOpenSession = csv.GetBool(8);
	m_bBadShift = csv.GetBool(9);
	m_nBreakMinutes = csv.GetInt(10);
}

/**********************************************************************/

CReportTimeAndAttendance::CReportTimeAndAttendance( CEposSelectArray& SelectArray, CServerSelectionList& list, bool bGroupByServer ) :
	m_SelectArray( SelectArray ), 
	m_ServerSelectionList( list )
{
	m_bHasData = FALSE;
	m_nServerPickType = 0;
	m_nServerStart = Server::ServerNo.Min;
	m_nServerEnd = Server::ServerNo.Max;
	m_currentServerInfo.Reset();
	m_strTAReportTitleName = "";
	m_strTAReportTitleInfo = "";
	m_strNewDateReport = "";
	m_strLastDateReport = "";
	m_nLastSelArrayIdx = -1;
	m_nTAReportType = TAREPORT_DETAIL;
	m_nServerConsolidationType = WORKING_REPORT_LOC;
	m_nShowEditType = 0;
	m_bCheckClosingTime = FALSE;
	m_bOpenShift = FALSE;
	m_bEventsBySecond = TRUE;
	m_bGotBadShift = FALSE;
	
	switch( EcrmanOptions.GetFeaturesServerSettingsType() )
	{
	case SERVER_LOC:
		m_nServerConsolidationType = WORKING_REPORT_LOC;
		break;

	case SERVER_DB:
		m_nServerConsolidationType = WORKING_REPORT_DB;
		break;

	case SERVER_SYS:
	case SERVER_NONE:
	default:
		m_nServerConsolidationType = WORKING_REPORT_SYS;
		break;
	}

	if (bGroupByServer == FALSE)
	{
		m_nServerConsolidationType = WORKING_REPORT_LOC;
	}
}

/**********************************************************************/

void CReportTimeAndAttendance::SetServerPickType( int n )
{
	if ((n >= 0) && (n <= 2))
	{
		m_nServerPickType = n;
	}
}

/**********************************************************************/

void CReportTimeAndAttendance::SetServerRange( int nStart, int nEnd )
{
	if ((nStart < 1) || (nStart > 9999))
	{
		nStart = 1;
	}

	if ((nEnd < 1) || (nEnd > 9999))
	{
		nEnd = 9999;
	}

	if ( nStart <= nEnd )
	{
		m_nServerStart = nStart;
		m_nServerEnd = nEnd;
	}
	else
	{
		m_nServerStart = nEnd;
		m_nServerEnd = nStart;
	}
}

/**********************************************************************/

void CReportTimeAndAttendance::SetDates ( COleDateTime oleDateFrom, COleDateTime oleDateTo, CSessionCSVRecord& ReportSession )
{
	m_oleDateTimeNow = COleDateTime::GetCurrentTime();
	
	if (m_oleDateTimeNow < oleDateTo)
	{
		oleDateTo = COleDateTime(m_oleDateTimeNow.GetYear(), m_oleDateTimeNow.GetMonth(), m_oleDateTimeNow.GetDay(), 0, 0, 0);
	}

	if (oleDateFrom > oleDateTo)
	{
		oleDateFrom = oleDateTo;
	}

	DataManagerNonDb.SessionDateTimeFilter.SetDates(oleDateFrom, oleDateTo, ReportSession);

	COleDateTime oleTimeLastFilter = DataManagerNonDb.SessionDateTimeFilter.GetLatestAcceptedDateTime();

	if (oleTimeLastFilter >= m_oleDateTimeNow)
	{
		m_oleDateTimeTidyUp = m_oleDateTimeNow;
	}
	else
	{
		m_oleDateTimeTidyUp = oleTimeLastFilter;
	}
}

/**********************************************************************/

void CReportTimeAndAttendance::GetFileList(CEposSelectArray& EposSelectArray, CStringArray& FileList, CWordArray& DbIdxList, CWordArray& LocIdxList, CWordArray& SelIdxList, __int64& nTotalFileSize)
{
	CWaitCursor wait;

	FileList.RemoveAll();
	DbIdxList.RemoveAll();
	LocIdxList.RemoveAll();
	SelIdxList.RemoveAll();

	nTotalFileSize = 0;

	for (int nIndex = 0; nIndex < EposSelectArray.GetListSize(); nIndex++)
	{
		CEposSelectArrayListItem ListItem;
		EposSelectArray.GetListItem(nIndex, ListItem);

		{
			int nSPOSVersion;
			int nConnectionType;
			dbLocation.GetLocSPOSVersion(ListItem.m_nLocIdx, nSPOSVersion, nConnectionType);
			if (CONNECTION_TYPE_STANDARD_NONE != nConnectionType)
				continue;
		}

		if (EPOSSELECT_TERMINAL == ListItem.m_nLineType)
		{
			int nNetWkIdx;
			if (dbNetwork.FindNetworkByNumber(dbLocation.GetNetworkNo(ListItem.m_nLocIdx), nNetWkIdx) == TRUE)
			{
				CString strPath;
				strPath = dbNetwork.GetFolderPathData(nNetWkIdx);

				for (int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount(ListItem.m_nLocIdx); nTerminalIdx++)
				{
					int nTerminalNo = dbLocation.GetTNo(ListItem.m_nLocIdx, nTerminalIdx);

					CString strFilePath = strPath;
					strFilePath += "\\";

					CString strFileName;
					::GetTermSaleFilename(CASHRSP_SALES_TIMEATTEND, nTerminalNo, strFileName);

					CSSFile fileTest;
					if (fileTest.Open(strFilePath + strFileName, "rb", _SH_DENYNO) == TRUE)
					{
						FileList.Add(strFilePath + strFileName);
						DbIdxList.Add(ListItem.m_nDbIdx + 1);
						LocIdxList.Add(ListItem.m_nLocIdx + 1);
						SelIdxList.Add(nIndex);
						nTotalFileSize += fileTest.GetLength();
					}
				}
			}
		}
	}
}

/**********************************************************************/

bool CReportTimeAndAttendance::CheckTimeStage1( CCSVEposTimeAttendData& csv, CString& strDateUTC, CString& strTimeUTC, CString& strDateReport, CString& strTimeReport )
{
	CString strCSVDate = csv.GetEventDate();
	CString strCSVTime = csv.GetEventTime();

	if (strCSVDate.GetLength() != 8)		//	31/12/99
	{
		return FALSE;
	}

	if (strCSVTime.GetLength() != 8)		//	12:59:59
	{
		return FALSE;
	}

	strDateUTC = "20";
	strDateUTC += strCSVDate.Right(2);
	strDateUTC += strCSVDate.Mid(3,2);
	strDateUTC += strCSVDate.Left(2);
	
	strTimeUTC = strCSVTime.Left(2);
	strTimeUTC += strCSVTime.Mid(3,2);
	strTimeUTC += strCSVTime.Right(2);

	COleDateTime timeUTC, timeReport;
	UTCToOleDate( strDateUTC + strTimeUTC, timeUTC, timeReport );
	
	if ((timeUTC.GetStatus() != COleDateTime::valid) || (timeReport.GetStatus() != COleDateTime::valid))
	{
		return FALSE;
	}

	if (timeReport > m_oleDateTimeNow)
	{
		return FALSE;
	}

	strDateUTC.Format( "%4.4d%2.2d%2.2d",
		timeUTC.GetYear(),
		timeUTC.GetMonth(),
		timeUTC.GetDay() );

	strTimeUTC.Format( "%2.2d%2.2d%2.2d",
		timeUTC.GetHour(),
		timeUTC.GetMinute(),
		timeUTC.GetSecond() );

	strDateReport.Format( "%4.4d%2.2d%2.2d",
		timeReport.GetYear(),
		timeReport.GetMonth(),
		timeReport.GetDay() );

	strTimeReport.Format( "%2.2d%2.2d%2.2d",
		timeReport.GetHour(),
		timeReport.GetMinute(),
		timeReport.GetSecond() );

	return TRUE;
}

/**********************************************************************/

bool CReportTimeAndAttendance::CheckTimeStage2( CString& strDateReport, CString& strTimeReport )
{
	return ( DataManagerNonDb.SessionDateTimeFilter.CheckTime( strDateReport, strTimeReport ) != 0 );
}

/**********************************************************************/

void CReportTimeAndAttendance::ClearConsolidateArray()
{
	m_arrayListedNames.RemoveAll();
	m_strNewDateReport = "";
	m_strLastDateReport = "";
	m_nLastSelArrayIdx = -1;
	m_ConsolArrayStage1.RemoveAll();
	m_ConsolArrayStage2.RemoveAll();
	m_ConsolArrayWorkHours.RemoveAll();
}

/**********************************************************************/

void CReportTimeAndAttendance::ConsolidateServerData ( CCSVEposTimeAttendData& csv, int nNwkNo, int nDbNo, int nLocNo, CEposSelectArray& SelectArray, int nSelArrayIdx )
{
	int nServerNo = csv.GetInt(5);

	bool bAccept = FALSE;
	switch( m_nServerPickType )
	{
	case 0:
		bAccept = TRUE;
		break;

	case 1:
		bAccept = ( ( nServerNo >= m_nServerStart ) && ( nServerNo <= m_nServerEnd ) );
		break;

	case 2:
		{
			CServerSelectionInfo info;
			GetServerInfo( info, nDbNo, nNwkNo, nLocNo, nServerNo );
			bAccept = info.m_bSelected;
		}
		break;
	}

	if (TRUE == bAccept )
	{
		if ( ConsolidateServerDataInternal( csv, nServerNo, nNwkNo, nDbNo, nLocNo ) == TRUE )
		{
			if ( ( m_strNewDateReport != m_strLastDateReport ) || ( nSelArrayIdx != m_nLastSelArrayIdx ) )
			{
				COleDateTime date = COleDateTime( atoi( m_strNewDateReport.Left(4) ), atoi( m_strNewDateReport.Mid(4,2) ), atoi( m_strNewDateReport.Right(2) ), 0, 0, 0 );
				if ( date.GetStatus() == COleDateTime::valid )
				{
					SelectArray.ProcessStartDate( nSelArrayIdx, date );
					SelectArray.ProcessEndDate( nSelArrayIdx, date );
					m_strLastDateReport = m_strNewDateReport;
					m_nLastSelArrayIdx = nSelArrayIdx;
				}
			}
		}
	}
}

/**********************************************************************/

bool CReportTimeAndAttendance::ConsolidateServerDataInternal( CCSVEposTimeAttendData& csv, int nServerNo, int nNwkNo, int nDbNo, int nLocNo )
{
	CString strDateUTC, strTimeUTC, strDateReport, strTimeReport;
	if (CheckTimeStage1(csv, strDateUTC, strTimeUTC, strDateReport, strTimeReport) == FALSE)
	{
		return FALSE;
	}

	m_strNewDateReport = strDateReport;

	switch( m_nServerConsolidationType )
	{
	case WORKING_REPORT_SYS:
	case WORKING_REPORT_DB:
	case WORKING_REPORT_LOC:
		{
			CEposReportConsolTimeAndAttendance ConsolInfo;
			ConsolInfo.m_nServerNo = nServerNo;
			ConsolInfo.m_nDbNo = nDbNo;
			ConsolInfo.m_nNwkNo = nNwkNo;
			ConsolInfo.m_nLocNo = nLocNo;
			ConsolInfo.m_strDate = "";
			ConsolInfo.m_strTime = "";
			ConsolInfo.m_nEventID = csv.GetEventId();

			int nEditCount = csv.GetEditCount();
			ConsolInfo.m_nHighestEdit = nEditCount;

			switch (m_nTAReportType)
			{
			case TAREPORT_EXPORT:
			case TAREPORT_EXPORT_BREAKS:
			case TAREPORT_SESSION:
				ConsolInfo.m_bSingleEvent = TRUE;
				break;

			default:
				ConsolInfo.m_bSingleEvent = (0 == m_nShowEditType);
				break;
			}

			CCSV csvBlock;
			csvBlock.Add( nEditCount );
			csvBlock.Add( csv.GetEventType() );
			csvBlock.Add( csv.GetTerminalNo() );
			csvBlock.Add( strDateUTC );
			csvBlock.Add( strTimeUTC );
			csvBlock.Add( strDateReport );
			csvBlock.Add( strTimeReport );
			csvBlock.Add( csv.GetEditServer() );
			csvBlock.Add( csv.GetEditDate() );
			csvBlock.Add( csv.GetEditTime() );

			CCSV csvReport;
			csvReport.Add( csvBlock.GetLine() );

			ConsolInfo.m_strData = csvReport.GetLine();

			m_ConsolArrayStage1.ConsolidateWithHint( ConsolInfo, m_nServerConsolidationType );
		}
		break;
	}
	
	return TRUE;
}

/**********************************************************************/

void CReportTimeAndAttendance::SortByEventDateAndTime()
{
	m_ConsolArrayStage2.RemoveAll();

	for ( int n = 0; n < m_ConsolArrayStage1.GetSize(); n++ )
	{
		CEposReportConsolTimeAndAttendance ConsolInfo;
		m_ConsolArrayStage1.GetAt( n, ConsolInfo );

		CCSV csv1( ConsolInfo.m_strData );
		int nBlockCount = csv1.GetSize();
		
		if ( nBlockCount > 0 )
		{
			CCSV csv2( csv1.GetString ( nBlockCount - 1 ) );

			if (csv2.GetSize() == 10)
			{
				ConsolInfo.m_strDate = csv2.GetString(3);
				ConsolInfo.m_strTime = csv2.GetString(4);

				if (CheckTimeStage2(ConsolInfo.m_strDate, ConsolInfo.m_strTime) == TRUE)
				{
					m_ConsolArrayStage2.ConsolidateWithHint(ConsolInfo, m_nServerConsolidationType);
				}
			}
		}
	}

	m_ConsolArrayStage1.RemoveAll();
}

/**********************************************************************/

void CReportTimeAndAttendance::GetServerInfo( CServerSelectionInfo& info, int nDbNo, int nNwkNo, int nLocNo, int nServerNo )
{
	int nNwkToCheck, nLocToCheck, nDbToCheck;
			
	switch( EcrmanOptions.GetFeaturesServerSettingsType() )
	{
	case SERVER_LOC:
		nNwkToCheck = nNwkNo;
		nLocToCheck = nLocNo;
		nDbToCheck = nDbNo;
		break;

	case SERVER_DB:
		nLocToCheck = 0;
		nNwkToCheck = 0;
		nDbToCheck = nDbNo;
		break;

	case SERVER_SYS:
	default:
		nNwkToCheck = 0;
		nLocToCheck = 0;
		nDbToCheck = 0;
		break;
	}

	info.m_nDbNo = nDbToCheck;
	info.m_nNwkNo = nNwkToCheck;
	info.m_nLocNo = nLocToCheck;
	info.m_nServerNo = nServerNo;
	info.m_bSelected = FALSE;
	m_ServerSelectionList.GetServerStatus( info );
}

/**********************************************************************/

bool CReportTimeAndAttendance::CreateReport()
{
	bool bResult = FALSE;

	m_strTAReportTitleName = "Time & Attendance ";

	switch (m_nTAReportType)
	{
	case TAREPORT_EXPORT:		
		m_strTAReportTitleName += "Export Shifts";
		bResult = CreateSimpleExport(FALSE);	
		break;

	case TAREPORT_EXPORT_BREAKS:		
		m_strTAReportTitleName += "Export Shifts + Breaks";
		bResult = CreateSimpleExport(TRUE);		
		break;

	case TAREPORT_DETAIL:
		m_strTAReportTitleName += "Detail";
		bResult = CreateFullReport();
		break;

	case TAREPORT_SESSION:
		m_strTAReportTitleName += "Session Summary";
		bResult = CreateSessionSummaryReport();
		break;

	case TAREPORT_DETAIL_ESTPAY:
		m_strTAReportTitleName += "Detail + Estimated Pay";
		bResult = CreateFullReport();
		break;

	case TAREPORT_ESTPAY:
	default:	
		m_strTAReportTitleName += "Estimated Pay Summary";
		bResult = CreateFullReport();
		break;

	}

	return bResult;
}

/**********************************************************************/

bool CReportTimeAndAttendance::CreateFullReport()
{
	m_strNewDateReport = "";
	m_strLastDateReport = "";
	m_nLastSelArrayIdx = -1;
	ResetTotals();

	if ( m_ReportFile.Open ( Super.ReportFilename() ) == TRUE )
	{
		m_ReportFile.SetStyle1 ( GetTAReportTitleFull() );

		switch (m_nTAReportType)
		{
		case TAREPORT_ESTPAY:
			m_ReportFile.AddColumn("", TA_LEFT, 500);
			m_ReportFile.AddColumn("", TA_LEFT, 400);
			m_ReportFile.AddColumn("", TA_LEFT, 400);
			break;

		case TAREPORT_DETAIL_ESTPAY:
		case TAREPORT_DETAIL:
		default:
			m_ReportFile.AddColumn("Date", TA_LEFT, 500);
			m_ReportFile.AddColumn("Action", TA_LEFT, 400);
			m_ReportFile.AddColumn("Time", TA_LEFT, 400);
			break;
		}

		switch (m_nTAReportType)
		{
		case TAREPORT_ESTPAY:
			m_ReportFile.AddColumn("", TA_LEFT, 250);
			m_ReportFile.AddColumn("", TA_RIGHT, 250);
			m_ReportFile.AddColumn("", TA_RIGHT, 250);
			break;

		case TAREPORT_DETAIL_ESTPAY:
			m_ReportFile.AddColumn("Comment", TA_LEFT, 250);
			m_ReportFile.AddColumn("", TA_RIGHT, 250);
			m_ReportFile.AddColumn("", TA_RIGHT, 250);
			break;

		case TAREPORT_DETAIL:
		default:
			m_ReportFile.AddColumn("Comment", TA_LEFT, 600);
			break;
		}
		
		bool bResult = CreateFullReportInternal();

		m_ReportFile.WriteLine ( "" );
		m_ReportFile.Close();
		return bResult;
	}

	return FALSE;
}

/**********************************************************************/

bool CReportTimeAndAttendance::CreateFullReportInternal()
{
	ResetTotals();
	m_ConsolArrayWorkTotal.RemoveAll();

	CEposReportConsolTimeAndAttendance CurrentServer;
	CurrentServer.m_nServerNo = -1;

	COleDateTime oleTimeNow = COleDateTime::GetCurrentTime();

	for ( int n = 0; n < m_ConsolArrayStage2.GetSize(); n++ )
	{
		CEposReportConsolTimeAndAttendance ConsolInfo;
		m_ConsolArrayStage2.GetAt( n, ConsolInfo );

		if ( ConsolInfo.CompareServer( CurrentServer ) == FALSE )
		{
			if ( CurrentServer.m_nServerNo != -1 )
			{
				TidyTimeSpans();

				switch (m_nTAReportType)
				{
				case TAREPORT_DETAIL_ESTPAY:
					m_ReportFile.WriteLine("");
					WriteTimeSpans(FALSE,FALSE);
					WriteWorkHours();
					break;

				case TAREPORT_ESTPAY:
					WriteWorkHours();
					break;

				case TAREPORT_DETAIL:
					m_ReportFile.WriteLine("");
					WriteTimeSpans(FALSE,FALSE);
					break;
				}

				m_ReportFile.WriteLine( "" );
				m_ReportFile.WriteLine( "" );

				m_currentServerInfo.Reset();
			}

			WriteHeader( ConsolInfo );
			ResetTotals();
			CurrentServer = ConsolInfo;
		}

		WriteReportLines( ConsolInfo, FALSE );
	}

	if ( CurrentServer.m_nServerNo != -1 )
	{
		TidyTimeSpans();
		
		switch (m_nTAReportType)
		{
		case TAREPORT_DETAIL_ESTPAY:
			m_ReportFile.WriteLine("");
			WriteTimeSpans(FALSE,FALSE);
			WriteWorkHours();
			break;

		case TAREPORT_ESTPAY:
			WriteWorkHours();
			break;

		case TAREPORT_DETAIL:
			m_ReportFile.WriteLine("");
			WriteTimeSpans(FALSE,FALSE);
			break;
		}
	}

	switch (m_nTAReportType)
	{
	case TAREPORT_DETAIL_ESTPAY:
	case TAREPORT_ESTPAY:
		m_ReportFile.WriteLine("");
		WriteWorkHoursTotal();
		break;
	}

	m_ReportFile.WriteLine( "" );
	return TRUE;
}

/**********************************************************************/

void CReportTimeAndAttendance::WriteHeader( CEposReportConsolTimeAndAttendance& ConsolInfo )
{
	CServerSelectionInfo info;
	GetServerInfo(info, ConsolInfo.m_nDbNo, ConsolInfo.m_nNwkNo, ConsolInfo.m_nLocNo, ConsolInfo.m_nServerNo);

	CString strServerNo = "";
	strServerNo.Format("Server %4.4d", info.m_nServerNo);

	int nLocIdx = 0;
	CString strLocName = "";
	strLocName.Format("Location %d.%d", ConsolInfo.m_nNwkNo, ConsolInfo.m_nLocNo);
	if (dbLocation.FindLocationByNumber(ConsolInfo.m_nNwkNo, ConsolInfo.m_nLocNo, nLocIdx) == TRUE)
	{
		strLocName = dbLocation.GetName(nLocIdx);
	}

	CString strFullLine = "<..>";

	//if (TAREPORT_ESTPAY == m_nTAReportType)
	{
		m_ReportFile.WriteLine(strFullLine + strLocName);
		m_ReportFile.WriteLine(strFullLine + strServerNo);
	}
	/*else
	{
		CCSV csv('\t');
		csv.Add(strServerNo);
		csv.Add(info.m_strServerName);
		csv.Add(strFullLine + strLocName);
		m_ReportFile.WriteLine(csv.GetLine());
		m_ReportFile.WriteLine("<LI>");
	}
	*/

	if (TAREPORT_ESTPAY != m_nTAReportType)
	{
		m_ReportFile.WriteLine(strFullLine + info.m_strServerName);
		m_ReportFile.WriteLine("<LI>");
	}
	
	m_currentServerInfo = info;
}

/**********************************************************************/

void CReportTimeAndAttendance::AssignWorkHoursToDate(COleDateTime& timeSignOutUTC)
{
	COleDateTimeSpan spanWorkShiftUTC = timeSignOutUTC - m_oleTimeSignInUTC;

	COleDateTime oleSignInTimeLocal, oleSignOutTimeLocal;
	UTCToOleDate(m_oleTimeSignInUTC, oleSignInTimeLocal);
	UTCToOleDate(timeSignOutUTC, oleSignOutTimeLocal);

	CString strDate = "";
	strDate.Format("%4.4d%2.2d%2.2d",
		oleSignInTimeLocal.GetYear(),
		oleSignInTimeLocal.GetMonth(),
		oleSignInTimeLocal.GetDay());

	//SIMPLE CASE, ENTIRE SHIFT ON SAME DAY
	if (oleSignInTimeLocal.GetDay() == oleSignOutTimeLocal.GetDay())
	{
		CEposReportConsolTAWorkHours Hours;
		Hours.m_strDate = strDate;
		Hours.m_nMinutesWorked = (spanWorkShiftUTC.GetHours() * 60) + spanWorkShiftUTC.GetMinutes();
		m_ConsolArrayWorkHours.Consolidate(Hours);
	}
	else if (TRUE == m_currentServerInfo.m_bMidnightShift)
	{
		CDateRangeHelpers DateRangeHelpers;
		COleDateTime NextDayTest = oleSignInTimeLocal;
		DateRangeHelpers.AddDaysToTime(NextDayTest, 1);

		if (NextDayTest.GetDay() == oleSignOutTimeLocal.GetDay())
		{
			COleDateTimeSpan spanNextDay = COleDateTimeSpan(0, oleSignOutTimeLocal.GetHour(), oleSignOutTimeLocal.GetMinute(), 0);
			COleDateTimeSpan spanThisDay = spanWorkShiftUTC - spanNextDay;

			CEposReportConsolTAWorkHours Hours;
			Hours.m_strDate = strDate;
			Hours.m_nMinutesWorked = (spanThisDay.GetHours() * 60) + spanThisDay.GetMinutes();
			m_ConsolArrayWorkHours.Consolidate(Hours);

			strDate.Format("%4.4d%2.2d%2.2d",
				oleSignOutTimeLocal.GetYear(),
				oleSignOutTimeLocal.GetMonth(),
				oleSignOutTimeLocal.GetDay());

			Hours.m_strDate = strDate;
			Hours.m_nMinutesWorked = (spanNextDay.GetHours() * 60) + spanNextDay.GetMinutes();
			m_ConsolArrayWorkHours.Consolidate(Hours);
		}
	}
}

/**********************************************************************/

void CReportTimeAndAttendance::WriteReportLines( CEposReportConsolTimeAndAttendance& ConsolInfo, bool bTidyUp )
{
	CString strFullLine = "<..>";

	CCSV csvData( ConsolInfo.m_strData );
	
	int nStartIdx = 0;
	int nEndIdx = csvData.GetSize() - 1;

	//FLAG TO SHOW ASTERIX FOR EDITED EVENTS, BUT HIDE ORIGINAL EVENT TIME
	bool bShowEditMarker = FALSE;
	if (0 == m_nShowEditType)
	{
		CCSVReportTimeAttendData csvIn(csvData.GetString(0));

		if (csvIn.GetEditCount() != 0)
		{
			bShowEditMarker = TRUE;
		}
	}

	for ( int nDataIdx = nStartIdx; nDataIdx <= nEndIdx; nDataIdx++ )
	{
		CCSVReportTimeAttendData csvIn( csvData.GetString( nDataIdx ) );
		CString strCSVDateUTC = csvIn.GetEventDateUTC();
		CString strCSVTimeUTC = csvIn.GetEventTimeUTC();
		CString strCSVDateReport = csvIn.GetEventDateReport();
		CString strCSVTimeReport = csvIn.GetEventTimeReport();

		COleDateTime timeLineUTC = COleDateTime(
			atoi ( strCSVDateUTC.Left(4) ),
			atoi ( strCSVDateUTC.Mid(4,2) ),
			atoi ( strCSVDateUTC.Right(2) ),
			atoi ( strCSVTimeUTC.Left(2) ),
			atoi ( strCSVTimeUTC.Mid(2,2) ),
			m_bEventsBySecond ? atoi ( strCSVTimeUTC.Right(2) ) : 0 );

		CString strDateReport;
		strDateReport.Format( "%s/%s/%s",
			(const char*) strCSVDateReport.Right(2),
			(const char*) strCSVDateReport.Mid(4,2),
			(const char*) strCSVDateReport.Left(4) );
	
		if (strDateReport == m_strLastDateReport)
		{
			strDateReport = "";
		}
		else
		{
			m_strLastDateReport = strDateReport;
		}

		CString strTimeReport;
		strTimeReport.Format( "%s:%s",
			(const char*) strCSVTimeReport.Left(2),
			(const char*) strCSVTimeReport.Mid(2,2) );

		CString strError = "";
		if (timeLineUTC.GetStatus() != COleDateTime::valid)
		{
			strError = "*** Invalid date";
		}
		else
		{
			m_oleTimeLastLineUTC = timeLineUTC;
		}

		CString strAction = "";
		switch( csvIn.GetEventType() )
		{
		case WORKING_ACTION_SIGNIN:
			strAction = "Time In";
			break;

		case WORKING_ACTION_SIGNOUT:
			strAction = "Time Out";
			break;

		case WORKING_ACTION_BREAKIN:
			strAction = "Break In";
			break;

		case WORKING_ACTION_BREAKOUT:
			strAction = "Break Out";
			break;
		}

		//VALIDATE EVENT STATUS IF WE HAVE REACHED THE LATEST
		//EDIT FOR THIS EVENT AND THERE IS NO OTHER ERROR
		if ( ( nDataIdx == nEndIdx ) && ( strError == "" ) )
		{
			switch( csvIn.GetEventType() )
			{
			case WORKING_ACTION_SIGNIN:
				switch( m_nServerStatus )
				{
				case WORKING_STATUS_SIGNOUT:
					{
						m_nServerStatus = WORKING_STATUS_SIGNIN;
						m_oleTimeSignInUTC = timeLineUTC;
						m_oleSpanCurrentShiftBreakUTC = COleDateTimeSpan( 0, 0, 0, 0 );

						if ( TRUE == m_bCheckClosingTime )
						{
							SetAutoSignOutTime(timeLineUTC);
						}
					}
					break;

				case WORKING_STATUS_SIGNIN:
				case WORKING_STATUS_BREAKIN:
					strError = "*** Already signed in";
					break;
				}
				break;

			case WORKING_ACTION_SIGNOUT:
				switch( m_nServerStatus )
				{
				case WORKING_STATUS_SIGNIN:
				case WORKING_STATUS_BREAKIN:

					strError = "";

					if (WORKING_STATUS_BREAKIN == m_nServerStatus)
					{
						m_nServerStatus = WORKING_STATUS_SIGNIN;
						COleDateTimeSpan oleSpanThisBreak = timeLineUTC - m_oleTimeBreakInUTC;
						m_oleSpanBreakUTC += oleSpanThisBreak;
						m_oleSpanCurrentShiftBreakUTC += oleSpanThisBreak;
						strError = "*** Signout during break";
					}

					{
						m_nServerStatus = WORKING_STATUS_SIGNOUT;
						COleDateTimeSpan oleSpanCurrentShiftWorkUTC = timeLineUTC - m_oleTimeSignInUTC; 
						m_oleSpanWorkUTC += oleSpanCurrentShiftWorkUTC;

						if ( TRUE == m_bCheckClosingTime )
						{
							if ( timeLineUTC >= m_oleTimeAutoSignOutUTC )
							{
								if (strError != "")
								{
									strError += " ";
								}

								strError += "*** Signout after closing time";
								m_oleSpanBadShiftBreakUTC += m_oleSpanCurrentShiftBreakUTC;
								m_oleSpanBadShiftWorkUTC += oleSpanCurrentShiftWorkUTC;
								m_bGotBadShift = TRUE;
							}
						}

						AssignWorkHoursToDate(timeLineUTC);
					}
					break;

				case WORKING_STATUS_SIGNOUT:
					strError = "***  Already signed out";
					break;
				}
				break;
		
			case WORKING_ACTION_BREAKIN:
				switch( m_nServerStatus )
				{
				case WORKING_STATUS_SIGNIN:
					m_nServerStatus = WORKING_STATUS_BREAKIN;
					m_oleTimeBreakInUTC = timeLineUTC;
					break;

				case WORKING_STATUS_BREAKIN:
					strError = "***  Already in break";
					break;
			
				case WORKING_STATUS_SIGNOUT:
					strError = "***  Not signed in";
					break;
				}
				break;

			case WORKING_ACTION_BREAKOUT:
				switch( m_nServerStatus )
				{
				case WORKING_STATUS_BREAKIN:
					{
						m_nServerStatus = WORKING_STATUS_SIGNIN;
						COleDateTimeSpan oleSpanThisBreak = timeLineUTC - m_oleTimeBreakInUTC; 
						m_oleSpanBreakUTC += oleSpanThisBreak;
						m_oleSpanCurrentShiftBreakUTC += oleSpanThisBreak;
					}
					break;
			
				case WORKING_STATUS_SIGNIN:
					strError = "***  Not in break";
					break;

				case WORKING_STATUS_SIGNOUT:
					strError = "***  Not signed in";
					break;
				}
				break;
			}
		}

		//SKIP DISPLAY OF T&A SESSION FOR PAYMENT SUMMARY
		if ( TAREPORT_ESTPAY != m_nTAReportType)
		{
			//SHOW INDENT IF THIS IS NOT THE ORIGINAL EVENT
			if (nDataIdx != nStartIdx)
			{
				CString str = "--> ";
				str += strAction;
				strAction = str;
			}

			//MARK WITH ASTERIX IF WE HAVE REACHED THE LATEST EDIT
			//FOR THIS EVENT AND ARE NOT SHOWING EDITS IN DETAIL.
			if (TRUE == bShowEditMarker)
			{
				strTimeReport += " **";
			}

			if (TRUE == bTidyUp)
			{
				switch (csvIn.GetEventType())
				{
				case WORKING_ACTION_BREAKOUT:
					strAction = "(Still in break)";
					break;

				case WORKING_ACTION_SIGNOUT:
					strAction = "(Still signed in)";
					break;
				}

				strError = "*** End of reporting period";
			}

			//SHOW FULL EDITING DETAIL IF THIS IS NOT THE ORIGINAL 
			//EVENT AND FULL DETAIL HAS BEEN REQUESTED.
			if ((nDataIdx != nStartIdx) && (2 == m_nShowEditType))
			{
				CServerSelectionInfo info;
				GetServerInfo(info, ConsolInfo.m_nDbNo, ConsolInfo.m_nNwkNo, ConsolInfo.m_nLocNo, csvIn.GetEditServer());

				CString strEditServer;
				strEditServer.Format("<..>Edited by : S%3.3d  %s",
					csvIn.GetEditServer(),
					(const char*)info.m_strServerName);

				CString strCSVDate = csvIn.GetEditDate();
				if (strCSVDate.GetLength() != 8)
				{
					strCSVDate = "00000000";
				}

				CString strCSVTime = csvIn.GetEditTime();
				if (strCSVTime.GetLength() != 8)
				{
					strCSVTime = "00000000";
				}

				CString strEditTime;
				strEditTime.Format("On : %s/%s/20%s at %s:%s",
					(const char*)strCSVDate.Left(2),
					(const char*)strCSVDate.Mid(3, 2),
					(const char*)strCSVDate.Right(2),
					(const char*)strCSVTime.Left(2),
					(const char*)strCSVTime.Mid(3, 2));

				CStringArray arrayComment;
				arrayComment.Add(strEditServer);
				arrayComment.Add(strEditTime);

				if ("" != strError)
				{
					arrayComment.Add(strError);
				}

				m_ReportFile.WriteLine("");

				for (int n = 0; n < arrayComment.GetSize(); n++)
				{
					CCSV csvOut('\t');

					if (0 == n)
					{
						csvOut.Add(strDateReport);
						csvOut.Add(strAction);
						csvOut.Add(strTimeReport);
					}
					else
					{
						csvOut.Add("");
						csvOut.Add("");
						csvOut.Add("");
					}

					csvOut.Add(strFullLine + arrayComment.GetAt(n));
					m_ReportFile.WriteLine(csvOut.GetLine());
				}

				m_ReportFile.WriteLine("");
			}
			else
			{
				CCSV csvOut('\t');
				csvOut.Add(strDateReport);
				csvOut.Add(strAction);
				csvOut.Add(strTimeReport);
				csvOut.Add(strFullLine + strError);
				m_ReportFile.WriteLine(csvOut.GetLine());
			}
		}
	}
}

/**********************************************************************/

bool CReportTimeAndAttendance::CreateSessionSummaryReport()
{
	m_strNewDateReport = "";
	m_strLastDateReport = "";
	m_nLastSelArrayIdx = -1;
	ResetTotals();

	if (m_ReportFile.Open(Super.ReportFilename()) == TRUE)
	{
		m_ReportFile.SetStyle1(GetTAReportTitleFull());
		m_ReportFile.AddColumn("", TA_LEFT, 400);
		m_ReportFile.AddColumn("Weekday", TA_LEFT, 350);
		m_ReportFile.AddColumn("Date", TA_LEFT, 300);
		m_ReportFile.AddColumn("Type", TA_LEFT, 200);
		m_ReportFile.AddColumn("Session", TA_LEFT, 350);
		m_ReportFile.AddColumn("Hours In", TA_LEFT, 300);
		m_ReportFile.AddColumn("Break", TA_LEFT, 300);
		m_ReportFile.AddColumn("Net Hours", TA_LEFT, 300);

		bool bResult = CreateSessionSummaryReportInternal();

		m_ReportFile.WriteLine("");
		m_ReportFile.Close();
		return bResult;
	}

	return FALSE;
}

/**********************************************************************/

bool CReportTimeAndAttendance::CreateSessionSummaryReportInternal()
{
	ResetTotals();

	CEposReportConsolTimeAndAttendance CurrentServer;
	CurrentServer.m_nServerNo = -1;

	//SORT AS SINGLE LINE PER SHIFT OR BREAK, ASCENDING BY START TIME
	for (int n = 0; n < m_ConsolArrayStage2.GetSize(); n++)
	{
		CEposReportConsolTimeAndAttendance ConsolInfo;
		m_ConsolArrayStage2.GetAt(n, ConsolInfo);

		if (ConsolInfo.CompareServer(CurrentServer) == FALSE)
		{
			TidyTimeSpans();
			ResetTotals();
			CurrentServer = ConsolInfo;
			m_currentServerInfo.Reset();
			m_currentServerInfo.m_nDbNo = ConsolInfo.m_nDbNo;
			m_currentServerInfo.m_nNwkNo = ConsolInfo.m_nNwkNo;
			m_currentServerInfo.m_nLocNo = ConsolInfo.m_nLocNo;
			m_currentServerInfo.m_nServerNo = ConsolInfo.m_nServerNo;
		}

		ProcessExportLine(ConsolInfo, TRUE, FALSE);
	}

	m_ConsolArrayStage2.RemoveAll();
	CurrentServer.m_nServerNo = -1;

	CString strLocationName = "";
	CString strServerName = "";
	bool bGotServer = FALSE;
	bool bGotOpenSession = FALSE;
	int nBreakMinutes = 0;
	int nWorkMinutes = 0;

	for (int n = 0; n < m_ConsolArrayStage1.GetSize(); n++)
	{
		CEposReportConsolTimeAndAttendance ConsolInfo;
		m_ConsolArrayStage1.GetAt(n, ConsolInfo);

		if (ConsolInfo.CompareServer(CurrentServer) == FALSE)
		{
			if (TRUE == bGotServer)
			{
				m_ReportFile.WriteLine("");
				WriteTimeSpans(TRUE, bGotOpenSession);
				m_ReportFile.WriteLine("");
				m_ReportFile.WriteLine("");
			}

			ResetTotals();
			WriteHeader(ConsolInfo);
			CurrentServer = ConsolInfo;
			bGotOpenSession = FALSE;
		}

		CTimeAndAttendanceExportDetail ExportDetail;
		
		{
			CCSV csvExport(ConsolInfo.m_strData);
			ExportDetail.LoadFromCSV(csvExport);
		}

		CString strWeekday = "";
		switch (ExportDetail.m_nWeekday)
		{
		case 1:
			strWeekday = "Sunday";
			break;

		case 2:
			strWeekday = "Monday";
			break;

		case 3:
			strWeekday = "Tuesday";
			break;

		case 4:
			strWeekday = "Wednesday";
			break;

		case 5:
			strWeekday = "Thursday";
			break;

		case 6:
			strWeekday = "Friday";
			break;

		case 7:
			strWeekday = "Saturday";
			break;
		}

		CString strSession = ExportDetail.m_strStartTime + CString(" - ") + ExportDetail.m_strEndTime;

		if (TRUE == ExportDetail.m_bOpenSession)
		{
			strSession += " **";
			bGotOpenSession = TRUE;
		}

		CCSV csvReport('\t');
		csvReport.Add("");
		csvReport.Add(strWeekday);
		csvReport.Add(ExportDetail.m_strStartDate);
		csvReport.Add(ExportDetail.m_bBreak ? "Break" : "Shift");
		csvReport.Add(strSession);

		if (TRUE == ExportDetail.m_bBreak)
		{
			csvReport.Add("");
			csvReport.Add(ExportDetail.m_strTimeSpan);
		}
		else
		{
			csvReport.Add(ExportDetail.m_strTimeSpan);
			csvReport.Add("");
		}

		m_ReportFile.WriteLine(csvReport.GetLine());

		bGotServer = TRUE;

		if (TRUE == ExportDetail.m_bBreak)
		{
			m_oleSpanBreakUTC += COleDateTimeSpan(0, 0, ExportDetail.m_nEventMinutes, 0);
		}
		else
		{
			m_oleSpanWorkUTC += COleDateTimeSpan(0, 0, ExportDetail.m_nEventMinutes, 0);
		}

		if ( TRUE == ExportDetail.m_bBadShift)
		{
			m_oleSpanBadShiftWorkUTC += COleDateTimeSpan(0, 0, ExportDetail.m_nEventMinutes, 0);
			m_oleSpanBadShiftBreakUTC += COleDateTimeSpan(0, 0, ExportDetail.m_nBreakMinutes, 0);
			m_bGotBadShift = TRUE;
		}
	}

	if (TRUE == bGotServer)
	{
		m_ReportFile.WriteLine("");
		WriteTimeSpans(TRUE, bGotOpenSession);
	}

	m_ReportFile.WriteLine("");
	return TRUE;
}

/**********************************************************************/

bool CReportTimeAndAttendance::CreateSimpleExport( bool bBreaks )
{
	m_strNewDateReport = "";
	m_strLastDateReport = "";
	m_nLastSelArrayIdx = -1;
	ResetTotals();

	if ( m_ReportFile.Open ( Super.ReportFilename() ) == TRUE )
	{
		m_ReportFile.SetStyle1 ( GetTAReportTitleFull() );
		m_ReportFile.AddColumn ( "Location", TA_LEFT, 400 );
		m_ReportFile.AddColumn ( "Server", TA_LEFT, 400 );

		if (TRUE == bBreaks)
		{
			m_ReportFile.AddColumn("Type", TA_LEFT, 250);
		}

		m_ReportFile.AddColumn ( "Date In", TA_LEFT, 250 );
		m_ReportFile.AddColumn ( "Time In", TA_LEFT, 200 );
		m_ReportFile.AddColumn ( "Date Out", TA_LEFT, 250 );
		m_ReportFile.AddColumn ( "Time Out", TA_LEFT, 200 );
		
		bool bResult = CreateSimpleExportInternal( bBreaks );

		m_ReportFile.WriteLine ( "" );
		m_ReportFile.Close();
		return bResult;
	}

	return FALSE;
}

/**********************************************************************/

bool CReportTimeAndAttendance::CreateSimpleExportInternal( bool bBreaks )
{
	ResetTotals();

	CEposReportConsolTimeAndAttendance CurrentServer;
	CurrentServer.m_nServerNo = -1;

	//SORT AS SINGLE LINE PER SHIFT OR BREAK, ASCENDING BY START TIME
	for ( int n = 0; n < m_ConsolArrayStage2.GetSize(); n++ )
	{
		CEposReportConsolTimeAndAttendance ConsolInfo;
		m_ConsolArrayStage2.GetAt( n, ConsolInfo );

		if ( ConsolInfo.CompareServer( CurrentServer ) == FALSE )
		{
			ResetTotals();
			CurrentServer = ConsolInfo;
		}

		ProcessExportLine( ConsolInfo, bBreaks, FALSE );
	}

	m_ConsolArrayStage2.RemoveAll();
	CurrentServer.m_nServerNo = -1;

	CString strLocationName = "";
	CString strServerName = "";
	
	for ( int n = 0; n < m_ConsolArrayStage1.GetSize(); n++ )
	{
		CEposReportConsolTimeAndAttendance ConsolInfo;
		m_ConsolArrayStage1.GetAt( n, ConsolInfo );

		if ( ConsolInfo.CompareServer( CurrentServer ) == FALSE )
		{
			ResetTotals();

			int nLocIdx = 0;
			strLocationName = "";
			if (dbLocation.FindLocationByNumber(ConsolInfo.m_nNwkNo, ConsolInfo.m_nLocNo, nLocIdx) == TRUE)
			{
				strLocationName = dbLocation.GetName(nLocIdx);
			}

			CServerSelectionInfo info;
			GetServerInfo( info, ConsolInfo.m_nDbNo, ConsolInfo.m_nNwkNo, ConsolInfo.m_nLocNo, ConsolInfo.m_nServerNo );
			strServerName = info.m_strServerName;

			if (strServerName == "")
			{
				strServerName.Format("Server %d", ConsolInfo.m_nServerNo);
			}

			CurrentServer = ConsolInfo;
		}

		CTimeAndAttendanceExportDetail ExportDetail;

		{
			CCSV csvExport(ConsolInfo.m_strData);
			ExportDetail.LoadFromCSV(csvExport);
		}

		CCSV csvReport( '\t' );
		csvReport.Add( strLocationName );
		csvReport.Add( strServerName );
		
		if ( TAREPORT_EXPORT_BREAKS == m_nTAReportType )
		{
			if (TRUE == ExportDetail.m_bBreak)
			{
				csvReport.Add("Break");
			}
			else
			{
				csvReport.Add("Shift");
			}
		}

		csvReport.Add( ExportDetail.m_strStartDate );
		csvReport.Add( ExportDetail.m_strStartTime );
		csvReport.Add( ExportDetail.m_strEndDate );
		csvReport.Add( ExportDetail.m_strEndTime );

		m_ReportFile.WriteLine( csvReport.GetLine() );
	}

	m_ReportFile.WriteLine( "" );
	return TRUE;
}

/**********************************************************************/

void CReportTimeAndAttendance::ProcessExportLine(CEposReportConsolTimeAndAttendance& ConsolInfo, bool bBreaks, bool bTidyUp)
{
	//CONSOLIDATION STAGE WILL ENSURE THAT WE ONLY HAVE ONE DATA BLOCK PER LINE

	CCSV csvData(ConsolInfo.m_strData);

	CCSVReportTimeAttendData csvIn(csvData.GetString(0));
	CString strCSVDateUTC = csvIn.GetEventDateUTC();
	CString strCSVTimeUTC = csvIn.GetEventTimeUTC();
	CString strCSVDateReport = csvIn.GetEventDateReport();
	CString strCSVTimeReport = csvIn.GetEventTimeReport();

	COleDateTime timeLineUTC = COleDateTime(
		atoi(strCSVDateUTC.Left(4)),
		atoi(strCSVDateUTC.Mid(4, 2)),
		atoi(strCSVDateUTC.Right(2)),
		atoi(strCSVTimeUTC.Left(2)),
		atoi(strCSVTimeUTC.Mid(2, 2)),
		m_bEventsBySecond ? atoi(strCSVTimeUTC.Right(2)) : 0);

	if (timeLineUTC.GetStatus() != COleDateTime::valid)
	{
		return;
	}

	CString strDateReport;
	strDateReport.Format("%s/%s/%s",
		(const char*)strCSVDateReport.Right(2),
		(const char*)strCSVDateReport.Mid(4, 2),
		(const char*)strCSVDateReport.Left(4));

	CString strTimeReport;
	strTimeReport.Format("%s:%s",
		(const char*)strCSVTimeReport.Left(2),
		(const char*)strCSVTimeReport.Mid(2, 2));

	bool bDoBreakOut = FALSE;
	bool bDoSignOut = FALSE;

	switch (csvIn.GetEventType())
	{
	case WORKING_ACTION_SIGNIN:
		switch (m_nServerStatus)
		{
		case WORKING_STATUS_SIGNOUT:
			m_nServerStatus = WORKING_STATUS_SIGNIN;
			m_strExportSignInCSVDate = strCSVDateUTC + strCSVTimeUTC;
			m_strExportSignInDate = strDateReport;
			m_strExportSignInTime = strTimeReport;
			m_nExportSignInWeekday = timeLineUTC.GetDayOfWeek();
			m_oleTimeSignInUTC = timeLineUTC;
			m_oleSpanCurrentShiftBreakUTC = COleDateTimeSpan(0, 0, 0, 0);

			if (TRUE == m_bCheckClosingTime)
			{
				SetAutoSignOutTime(timeLineUTC);
			}

			break;

		case WORKING_STATUS_SIGNIN:
		case WORKING_STATUS_BREAKIN:
			//strError = "*** Already signed in";
			break;
		}
		break;

	case WORKING_ACTION_SIGNOUT:
		switch (m_nServerStatus)
		{
		case WORKING_STATUS_BREAKIN:
			bDoBreakOut = TRUE;
			bDoSignOut = TRUE;
			break;

		case WORKING_STATUS_SIGNIN:
			bDoSignOut = TRUE;
			break;

		case WORKING_STATUS_SIGNOUT:
			//strError = "***  Already signed out";
			break;
		}
		break;

	case WORKING_ACTION_BREAKIN:
		switch (m_nServerStatus)
		{
		case WORKING_STATUS_SIGNIN:
			m_nServerStatus = WORKING_STATUS_BREAKIN;
			m_strExportBreakInCSVDate = strCSVDateUTC + strCSVTimeUTC;
			m_strExportBreakInDate = strDateReport;
			m_strExportBreakInTime = strTimeReport;
			m_oleTimeBreakInUTC = timeLineUTC;
			break;

		case WORKING_STATUS_BREAKIN:
			//strError = "***  Already in break";
			break;

		case WORKING_STATUS_SIGNOUT:
			//strError = "***  Not signed in";
			break;
		}
		break;

	case WORKING_ACTION_BREAKOUT:
		switch (m_nServerStatus)
		{
		case WORKING_STATUS_BREAKIN:
			m_oleSpanCurrentShiftBreakUTC += timeLineUTC - m_oleTimeBreakInUTC;
			bDoBreakOut = TRUE;
			break;

		case WORKING_STATUS_SIGNIN:
			//strError = "***  Not in break";
			break;

		case WORKING_STATUS_SIGNOUT:
			//strError = "***  Not signed in";
			break;
		}
	}

	if (TRUE == bDoBreakOut)
	{
		m_nServerStatus = WORKING_STATUS_SIGNIN;

		if (TRUE == bBreaks)
		{
			COleDateTimeSpan oleSpanThisBreak = timeLineUTC - m_oleTimeBreakInUTC;
			int nBreakMinutes = GetTotalMinutesFromOleDateTimeSpan(oleSpanThisBreak);
				
			CCSV csv;
			csv.Add(1);
			csv.Add(m_strExportBreakInDate);
			csv.Add(m_strExportBreakInTime);
			csv.Add(strDateReport);
			csv.Add(strTimeReport);
			csv.Add(m_nExportSignInWeekday);
			csv.Add(nBreakMinutes);
			csv.Add(GetTimeSpanString(oleSpanThisBreak));
			csv.Add(bTidyUp);

			CEposReportConsolTimeAndAttendance infoSorted;
			infoSorted = ConsolInfo;
			infoSorted.m_strDate = m_strExportBreakInCSVDate;
			infoSorted.m_strTime = "";
			infoSorted.m_bSingleEvent = TRUE;
			infoSorted.m_strData = csv.GetLine();
			m_ConsolArrayStage1.ConsolidateWithHint(infoSorted, m_nServerConsolidationType);
		}
	}

	if (TRUE == bDoSignOut)
	{
		m_nServerStatus = WORKING_STATUS_SIGNOUT;

		COleDateTimeSpan oleSpanThisShift = timeLineUTC - m_oleTimeSignInUTC;
		int nShiftMinutes = GetTotalMinutesFromOleDateTimeSpan(oleSpanThisShift);
		int nBreakMinutes = GetTotalMinutesFromOleDateTimeSpan(m_oleSpanCurrentShiftBreakUTC);
			
		bool bGotBadShift = FALSE;
		if ((TRUE == m_bCheckClosingTime) && (FALSE == bTidyUp))
		{
			if (timeLineUTC >= m_oleTimeAutoSignOutUTC)
			{
				bGotBadShift = TRUE;
			}
		}

		CCSV csv;
		csv.Add(0);
		csv.Add(m_strExportSignInDate);
		csv.Add(m_strExportSignInTime);
		csv.Add(strDateReport);
		csv.Add(strTimeReport);
		csv.Add(m_nExportSignInWeekday);
		csv.Add(nShiftMinutes);
		csv.Add(GetTimeSpanString(oleSpanThisShift));
		csv.Add(bTidyUp);
		csv.Add(bGotBadShift);
		csv.Add(nBreakMinutes);

		CEposReportConsolTimeAndAttendance infoSorted;
		infoSorted = ConsolInfo;
		infoSorted.m_strDate = m_strExportSignInCSVDate;
		infoSorted.m_strTime = "";
		infoSorted.m_bSingleEvent = TRUE;
		infoSorted.m_strData = csv.GetLine();
		m_ConsolArrayStage1.ConsolidateWithHint(infoSorted, m_nServerConsolidationType);
	}
}

/**********************************************************************/

void CReportTimeAndAttendance::LocalTimeToUTC(COleDateTime& oleLocal, COleDateTime& oleUTC)
{
	oleUTC = oleLocal;

	TIME_ZONE_INFORMATION timeZoneInformation;
	DWORD dwTZ = GetTimeZoneInformation(&timeZoneInformation);
	if (dwTZ == TIME_ZONE_ID_STANDARD || dwTZ == TIME_ZONE_ID_DAYLIGHT)
	{
		SYSTEMTIME timeLocal;
		oleLocal.GetAsSystemTime(timeLocal);
		SYSTEMTIME timeUTC;
		TzSpecificLocalTimeToSystemTime(&timeZoneInformation, &timeLocal, &timeUTC);
		oleUTC = COleDateTime(timeUTC);
	}
}

/**********************************************************************/

void CReportTimeAndAttendance::UTCToOleDate(const char* szUTC, COleDateTime& oleUTC, COleDateTime& oleReport)
{
	CString strUTC = szUTC;

	if (strUTC.GetLength() != 14)
	{
		strUTC = "";
		::AddLeading(strUTC, 14, '0');
	}

	oleUTC = COleDateTime(
		atoi(strUTC.Left(4)),
		atoi(strUTC.Mid(4, 2)),
		atoi(strUTC.Mid(6, 2)),
		atoi(strUTC.Mid(8, 2)),
		atoi(strUTC.Mid(10, 2)),
		atoi(strUTC.Mid(12, 2)));

	UTCToOleDate(oleUTC, oleReport);
}

/**********************************************************************/

void CReportTimeAndAttendance::UTCToOleDate(COleDateTime & oleUTC, COleDateTime & oleReport)
{
	TIME_ZONE_INFORMATION timeZoneInformation;
	DWORD dwTZ = GetTimeZoneInformation(&timeZoneInformation);
	if(dwTZ == TIME_ZONE_ID_STANDARD || dwTZ == TIME_ZONE_ID_DAYLIGHT)
	{
		SYSTEMTIME timeUTC; 
		oleUTC.GetAsSystemTime(timeUTC);
		SYSTEMTIME timeLocal;
		SystemTimeToTzSpecificLocalTime(&timeZoneInformation, &timeUTC, &timeLocal);
		oleReport = COleDateTime(timeLocal);
	}
	else
	{
		oleReport = oleUTC;
	}
}

/**********************************************************************/

void CReportTimeAndAttendance::TidyTimeSpans()
{
	if (TRUE == m_bOpenShift)
	{
		COleDateTime dateTimeTidyUpUTC;
		LocalTimeToUTC(m_oleDateTimeTidyUp, dateTimeTidyUpUTC);

		CString strDateUTC = "";
		strDateUTC.Format("%4.4d%2.2d%2.2d",
			dateTimeTidyUpUTC.GetYear(),
			dateTimeTidyUpUTC.GetMonth(),
			dateTimeTidyUpUTC.GetDay()
		);

		CString strTimeUTC = "";
		strTimeUTC.Format("%2.2d%2.2d%2.2d",
			dateTimeTidyUpUTC.GetHour(),
			dateTimeTidyUpUTC.GetMinute(),
			dateTimeTidyUpUTC.GetSecond()
		);

		CString strDateReport = "";
		strDateReport.Format("%4.4d%2.2d%2.2d",
			m_oleDateTimeTidyUp.GetYear(),
			m_oleDateTimeTidyUp.GetMonth(),
			m_oleDateTimeTidyUp.GetDay()
		);

		CString strTimeReport = "";
		strTimeReport.Format("%2.2d%2.2d%2.2d",
			m_oleDateTimeTidyUp.GetHour(),
			m_oleDateTimeTidyUp.GetMinute(),
			m_oleDateTimeTidyUp.GetSecond()
		);

		CCSV csvEvent;
		csvEvent.Add(0); //EDIT COUNT
		csvEvent.Add(WORKING_ACTION_BREAKOUT);
		csvEvent.Add(1); //TERMINAL NO
		csvEvent.Add(strDateUTC);
		csvEvent.Add(strTimeUTC);
		csvEvent.Add(strDateReport);
		csvEvent.Add(strTimeReport);

		CEposReportConsolTimeAndAttendance ConsolInfo;
		CString strQuote = "\"";
		ConsolInfo.m_strData = strQuote + csvEvent.GetLine() + strQuote;

		switch (m_nTAReportType)
		{
		case TAREPORT_EXPORT:
		case TAREPORT_EXPORT_BREAKS:
		case TAREPORT_SESSION:
			ConsolInfo.m_nDbNo = m_currentServerInfo.m_nDbNo;
			ConsolInfo.m_nNwkNo = m_currentServerInfo.m_nNwkNo;
			ConsolInfo.m_nLocNo = m_currentServerInfo.m_nLocNo;
			ConsolInfo.m_nServerNo = m_currentServerInfo.m_nServerNo;
			switch (m_nServerStatus)
			{
			case WORKING_STATUS_BREAKIN:
				ProcessExportLine(ConsolInfo, m_nTAReportType != TAREPORT_EXPORT, TRUE);
				csvEvent.SetAt(1, WORKING_ACTION_SIGNOUT);
				ConsolInfo.m_strData = strQuote + csvEvent.GetLine() + strQuote;
				ProcessExportLine(ConsolInfo, m_nTAReportType != TAREPORT_EXPORT, TRUE);
				break;

			case WORKING_STATUS_SIGNIN:
				csvEvent.SetAt(1, WORKING_ACTION_SIGNOUT);
				ConsolInfo.m_strData = strQuote + csvEvent.GetLine() + strQuote;
				ProcessExportLine(ConsolInfo, m_nTAReportType != TAREPORT_EXPORT, TRUE);
				break;
			}
			break;

		default:
			switch (m_nServerStatus)
			{
			case WORKING_STATUS_BREAKIN:
				WriteReportLines(ConsolInfo, TRUE);
				csvEvent.SetAt(1, WORKING_ACTION_SIGNOUT);
				ConsolInfo.m_strData = strQuote + csvEvent.GetLine() + strQuote;
				WriteReportLines(ConsolInfo, TRUE);
				break;

			case WORKING_STATUS_SIGNIN:
				csvEvent.SetAt(1, WORKING_ACTION_SIGNOUT);
				ConsolInfo.m_strData = strQuote + csvEvent.GetLine() + strQuote;
				WriteReportLines(ConsolInfo, TRUE);
				break;
			}
			break;
		}
	}
}

/**********************************************************************/

void CReportTimeAndAttendance::ResetTotals()
{
	m_nServerStatus = WORKING_STATUS_SIGNOUT;
	m_strLastDateReport = "";
	m_oleSpanWorkUTC = COleDateTimeSpan( 0, 0, 0, 0 );
	m_oleSpanBreakUTC = COleDateTimeSpan( 0, 0, 0, 0 );
	
	m_oleTimeAutoSignOutUTC = COleDateTime( 2020, 1, 1, 0, 0, 0 );
	m_oleSpanCurrentShiftBreakUTC = COleDateTimeSpan( 0, 0, 0, 0 );
	m_oleSpanBadShiftBreakUTC = COleDateTimeSpan( 0, 0, 0, 0 );
	m_oleSpanBadShiftWorkUTC = COleDateTimeSpan( 0, 0, 0, 0 );
	m_bGotBadShift = FALSE;

	m_strExportSignInDate = "";
	m_strExportSignInTime = "";
	m_strExportBreakInDate = "";
	m_strExportBreakInTime = "";
	m_strExportSignInCSVDate = "";
	m_strExportBreakInCSVDate = "";

	m_ConsolArrayWorkHours.RemoveAll();
}
	
/**********************************************************************/

void CReportTimeAndAttendance::WriteTimeSpans(bool bSessionSummary, bool bGotOpenSession)
{
	m_ReportFile.WriteLine("");

	CCSVReportLine csv;
	csv.AppendString(m_currentServerInfo.m_strServerName);

	if ( TRUE == bSessionSummary )
	{
		csv.AppendBlanks(4);
	}

	csv.AppendString("Hours In");
	csv.AppendString("Break");
	csv.AppendString("Net Hours");
	m_ReportFile.WriteLine(csv.GetLine());
	m_ReportFile.WriteLine("<LI>");

	csv.RemoveAll();

	{
		CString strLabel = "Total Hours";

		if (FALSE == bSessionSummary)
		{
			csv.AppendString(strLabel);
		}
		else if (FALSE == bGotOpenSession)
		{
			AppendSubtotalLabel(csv, strLabel, TRUE);
		}
		else
		{
			csv.AppendBlanks(1);
			csv.AppendString(strLabel);
			csv.AppendString("<..>(includes open shift)");
			csv.AppendBlanks(2);
		}
	}

	csv.AppendString(GetTimeSpanString(m_oleSpanWorkUTC));
	csv.AppendString(GetTimeSpanString(m_oleSpanBreakUTC));
	csv.AppendString(GetTimeSpanString(m_oleSpanWorkUTC - m_oleSpanBreakUTC));
	m_ReportFile.WriteLine(csv.GetLine());

	if (TRUE == m_bGotBadShift)
	{
		csv.RemoveAll();

		AppendSubtotalLabel(csv, "Signed Out Late", bSessionSummary);

		csv.Add(GetTimeSpanString(m_oleSpanBadShiftWorkUTC));
		csv.Add(GetTimeSpanString(m_oleSpanBadShiftBreakUTC));
		csv.Add(GetTimeSpanString(m_oleSpanBadShiftWorkUTC - m_oleSpanBadShiftBreakUTC));
		m_ReportFile.WriteLine(csv.GetLine());

		m_ReportFile.WriteLine("<LI>");
		csv.RemoveAll();

		AppendSubtotalLabel(csv, "Signed Out OK", bSessionSummary);

		csv.AppendString(GetTimeSpanString(m_oleSpanWorkUTC - m_oleSpanBadShiftWorkUTC));
		csv.AppendString(GetTimeSpanString(m_oleSpanBreakUTC - m_oleSpanBadShiftBreakUTC));	
		csv.AppendString(GetTimeSpanString((m_oleSpanWorkUTC - m_oleSpanBreakUTC) - (m_oleSpanBadShiftWorkUTC - m_oleSpanBadShiftBreakUTC)));
		m_ReportFile.WriteLine(csv.GetLine());
	}

	m_ReportFile.WriteLine("");
}

/**********************************************************************/

void CReportTimeAndAttendance::AppendSubtotalLabel( CCSVReportLine& csv, CString strLabel, bool bSessionSummary )
{
	if ( TRUE == bSessionSummary )
	{
		csv.AppendBlanks(1);
	}

	csv.AppendString(strLabel);

	if ( TRUE == bSessionSummary )
	{
		csv.AppendBlanks(3);
	}
}

/**********************************************************************/

void CReportTimeAndAttendance::WriteWorkHours()
{
	int nDayCount = m_ConsolArrayWorkHours.GetSize();

	if (nDayCount > 0)
	{
		if (TAREPORT_ESTPAY != m_nTAReportType)
		{
			m_ReportFile.WriteLine("");
		}

		CCSV csv('\t');
		csv.Add(m_currentServerInfo.m_strServerName);
		csv.Add("");
		csv.Add("Date");
		csv.Add("Hours");
		csv.Add("Est Pay");
		csv.Add("Cap Pay");
		m_ReportFile.WriteLine(csv.GetLine());
		m_ReportFile.WriteLine("<LI>");

		double dTotalEstPay = 0.0;
		double dTotalCapPay = 0.0;
		int nTotalMinutesWorked = 0;

		for (int n = 0; n < nDayCount; n++)
		{
			CEposReportConsolTAWorkHours Hours;
			m_ConsolArrayWorkHours.GetAt(n, Hours);

			int nShiftHours = Hours.m_nMinutesWorked / 60;
			int nShiftMinutes = Hours.m_nMinutesWorked % 60;
			double dHoursWorked = (double)nShiftHours + (((double)nShiftMinutes) / 60.0);
			double dShiftPay = m_currentServerInfo.m_dHourlyRate * dHoursWorked;
			COleDateTimeSpan spanWorked(0, nShiftHours, nShiftMinutes, 0);
			dTotalEstPay += dShiftPay;

			double dCapPay = dShiftPay;
			int nCapHours = m_currentServerInfo.m_nDailyHours;
			if (nCapHours <= nShiftHours)
			{
				dCapPay = m_currentServerInfo.m_dHourlyRate * nCapHours;
			}
			dTotalCapPay += dCapPay;

			nTotalMinutesWorked += Hours.m_nMinutesWorked;

			CString strDate = "";
			strDate.Format("%s/%s/%s",
				(const char*)Hours.m_strDate.Right(2),
				(const char*)Hours.m_strDate.Mid(4, 2),
				(const char*)Hours.m_strDate.Left(4));

			CString strInfo1 = "";
			CString strInfo2 = "";
			
			if ( 0 == n )
			{ 
				strInfo1.Format("%.2f per hour", m_currentServerInfo.m_dHourlyRate);
				strInfo2.Format("%d hour day", m_currentServerInfo.m_nDailyHours);
			}

			CCSV csv('\t');
			csv.Add(strInfo1);
			csv.Add(strInfo2);
			csv.Add(strDate);
			csv.Add(GetTimeSpanString(spanWorked));
			csv.Add(dShiftPay,2);
			csv.Add(dCapPay,2);

			m_ReportFile.WriteLine(csv.GetLine());

			Hours.m_dEstPay = dShiftPay;
			Hours.m_dCapPay = dCapPay;
			m_ConsolArrayWorkTotal.Consolidate(Hours);
		}

		{
			COleDateTimeSpan spanWorked(0, nTotalMinutesWorked / 60, nTotalMinutesWorked % 60, 0);

			m_ReportFile.WriteLine("\t\t\t<LI>\t<LI>\t<LI>");

			CCSV csv('\t');
			csv.Add("");
			csv.Add("");
			csv.Add("");
			csv.Add(GetTimeSpanString(spanWorked));
			csv.Add(dTotalEstPay,2);
			csv.Add(dTotalCapPay, 2);

			m_ReportFile.WriteLine(csv.GetLine());
		}
	}
}

/**********************************************************************/

void CReportTimeAndAttendance::WriteWorkHoursTotal()
{
	int nDayCount = m_ConsolArrayWorkTotal.GetSize();

	if (nDayCount > 0)
	{
		m_ReportFile.WriteLine("");

		CCSV csv('\t');
		csv.Add("<..>Total Est Pay");
		csv.Add("");
		csv.Add("");
		csv.Add("Hours");
		csv.Add("Est Pay");
		csv.Add("Cap Pay");
		m_ReportFile.WriteLine(csv.GetLine());
		m_ReportFile.WriteLine("<LI>");

		double dTotalEstPay = 0.0;
		double dTotalCapPay = 0.0;
		int nTotalMinutesWorked = 0;

		for (int n = 0; n < nDayCount; n++)
		{
			CEposReportConsolTAWorkHours Hours;
			m_ConsolArrayWorkTotal.GetAt(n, Hours);

			int nShiftHours = Hours.m_nMinutesWorked / 60;
			int nShiftMinutes = Hours.m_nMinutesWorked % 60;
			COleDateTimeSpan spanWorked(0, nShiftHours, nShiftMinutes, 0);

			dTotalEstPay += Hours.m_dEstPay;
			dTotalCapPay += Hours.m_dCapPay;
			nTotalMinutesWorked += Hours.m_nMinutesWorked;

			CString strDate = "";
			strDate.Format("%s/%s/%s",
				(const char*)Hours.m_strDate.Right(2),
				(const char*)Hours.m_strDate.Mid(4, 2),
				(const char*)Hours.m_strDate.Left(4));

			CCSV csv('\t');
			csv.Add("");
			csv.Add("");
			csv.Add(strDate);
			csv.Add(GetTimeSpanString(spanWorked));
			csv.Add(Hours.m_dEstPay, 2);
			csv.Add(Hours.m_dCapPay, 2);

			m_ReportFile.WriteLine(csv.GetLine());
		}

		{
			COleDateTimeSpan spanWorked(0, nTotalMinutesWorked / 60, nTotalMinutesWorked % 60, 0);

			m_ReportFile.WriteLine("\t\t\t<LI>\t<LI>\t<LI>");

			CCSV csv('\t');
			csv.Add("");
			csv.Add("");
			csv.Add("");
			csv.Add(GetTimeSpanString(spanWorked));
			csv.Add(dTotalEstPay, 2);
			csv.Add(dTotalCapPay, 2);

			m_ReportFile.WriteLine(csv.GetLine());
		}
	}
}

/**********************************************************************/

void CReportTimeAndAttendance::WriteTimeSpans( COleDateTimeSpan spanWork, COleDateTimeSpan spanBreak )
{
	CString strTimeIn;
	strTimeIn.Format( "%dh  %dm",
		spanWork.GetHours() + ( spanWork.GetDays() * 24 ),
		spanWork.GetMinutes() );

	CString strBreak;
	strBreak.Format( "%dh  %dm",
		spanBreak.GetHours() + ( spanBreak.GetDays() * 24 ),
		spanBreak.GetMinutes() );

	spanWork -= ( spanBreak );

	CString strNetHours;
	strNetHours.Format( "%dh  %dm",
		spanWork.GetHours() + ( spanWork.GetDays() * 24 ),
		spanWork.GetMinutes() );
	
	CCSV csv( '\t' );
	csv.Add( "Total Hours In" );
	csv.Add( strTimeIn );
	m_ReportFile.WriteLine( csv.GetLine() );

	csv.RemoveAll();
	csv.Add( "Total Hours Break" );
	csv.Add( strBreak );
	m_ReportFile.WriteLine( csv.GetLine() );

	m_ReportFile.WriteLine( "" );

	csv.RemoveAll();
	csv.Add( "Total Hours Net" );
	csv.Add( strNetHours );
	m_ReportFile.WriteLine( csv.GetLine() );
}

/**********************************************************************/

const char* CReportTimeAndAttendance::GetTimeSpanString( const COleDateTimeSpan span )
{
	m_strTimeSpan.Format( "%dh  %dm",
		span.GetHours() + ( span.GetDays() * 24 ),
		span.GetMinutes() );

	return m_strTimeSpan;
}

/**********************************************************************/

int CReportTimeAndAttendance::GetTotalMinutesFromOleDateTimeSpan(COleDateTimeSpan& span)
{
	return (span.GetDays() * 1440) + (span.GetHours() * 60) + span.GetMinutes();
}

/**********************************************************************/

void CReportTimeAndAttendance::SetAutoSignOutTime(COleDateTime& oleSignInTimeUTC)
{
	COleDateTime localAutoSignOutTime = COleDateTime(
		oleSignInTimeUTC.GetYear(),
		oleSignInTimeUTC.GetMonth(),
		oleSignInTimeUTC.GetDay(),
		EcrmanOptions.GetReportsTimeAttendSignOutHour(),
		EcrmanOptions.GetReportsTimeAttendSignOutMinute(), 0);

	LocalTimeToUTC(localAutoSignOutTime, m_oleTimeAutoSignOutUTC);

	if (m_oleTimeAutoSignOutUTC <= oleSignInTimeUTC)
	{
		CDateRangeHelpers DateRangeHelpers;
		DateRangeHelpers.AddDaysToTime(m_oleTimeAutoSignOutUTC, 1);
	}
}

/**********************************************************************/
