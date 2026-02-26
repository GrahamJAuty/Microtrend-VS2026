/**********************************************************************/
#include <share.h>
/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "PMSCheckoutDateArray.h"
#include "PMSDefines.h"
#include "PMSInvoiceArray.h"
#include "PMSOptions.h"
#include "PMSReportFileFinder.h"
#include "PMSRoomStatusTable.h"
#include "StringDateChecker.h"
#include "SysInfo.h"
#include "TerminalDateRangeListBuilder.h"
/**********************************************************************/
#include "SalesHistoryReportPlu.h"
/**********************************************************************/

const char* CSalesHistoryReportPlu::GetRepMonth( COleDateTime date, bool bShortName, bool bFullLine )
{
	m_strRepMonth = ( bFullLine ) ? "<..>" : "";

	if ( bShortName == FALSE )
		m_strRepMonth += date.Format ( "%B %Y" );
	else
		m_strRepMonth += date.Format ( "%b %y" );

	return m_strRepMonth;
}

/**********************************************************************/

void CSalesHistoryReportPlu::WriteTerminalList()
{
	CSSFile fileReport;
	if (fileReport.Open(Super.ReportFilename(), "ab") == FALSE)
	{
		return;
	}

	CString strGuestAccountStatus = "";
	m_SelectArray.BuildLocSortArray();	

	if ( m_bEposSales == TRUE )
	{
		if (m_nConLevel == NODE_SYSTEM)
		{
			fileReport.WriteLine("<..>EPOS Sales for System");
		}
		else
		{
			CTerminalDateRangeListBuilder TerminalList(m_SelectArray, m_bEODMode, m_nTermDateType, m_bGuestToTerminal, PMS_ALLOWMODE_ALL, PMS_DATEMODE_ACTUAL);
			TerminalList.CreateTerminalList();

			for (int n = 0; n < TerminalList.GetLineCount(); n++)
			{
				fileReport.WriteLine(TerminalList.GetLine(n));
			}

			strGuestAccountStatus = TerminalList.GetGuestAccountStatus();
		}

		if ( strGuestAccountStatus != "" )
		{
			fileReport.WriteLine( "" );
			fileReport.WriteLine( strGuestAccountStatus );
		}
	}

	if ( m_bManualSales == TRUE )
	{
		if (m_nConLevel == NODE_SYSTEM)
		{
			fileReport.WriteLine("<..>Manual Sales for System");
		}
		else
		{
			for ( int nIndex = 0; nIndex < m_SelectArray.GetListSize(); nIndex++ )
			{
				CEposSelectArrayListItem ListItem;
				m_SelectArray.GetListItem( nIndex, ListItem );

				if ( EPOSSELECT_DB_MANUAL == ListItem.m_nLineType )
				{
					CString strName;
					bool bHideNoData = FALSE;
					strName.Format ( "<..>Manual Sales for %s %s", 
						dbDatabase.GetName( ListItem.m_nDbIdx ), 
						m_SelectArray.GetDateInfo( ListItem, bHideNoData ) );
					
					if (FALSE == bHideNoData)
					{
						fileReport.WriteLine(strName);
					}
				}
			}
		}
	}

	if ( m_bImportSales == TRUE )
	{
		if (m_nConLevel == NODE_SYSTEM)
		{
			fileReport.WriteLine("<..>Import Sales for System");
		}
		else
		{
			for ( int nIndex = 0; nIndex < m_SelectArray.GetListSize(); nIndex++ )
			{
				CEposSelectArrayListItem ListItem;
				m_SelectArray.GetListItem( nIndex, ListItem );

				if ( EPOSSELECT_DB_IMPORT == ListItem.m_nLineType )
				{
					CString strName;
					bool bHideNoData = FALSE;
					strName.Format ( "<..>Import Sales for %s %s", 
						dbDatabase.GetName( ListItem.m_nDbIdx ), 
						m_SelectArray.GetDateInfo( ListItem, bHideNoData ) );
					
					if (FALSE == bHideNoData)
					{
						fileReport.WriteLine(strName);
					}
				}
			}
		}
	}
}

/**********************************************************************/

void CSalesHistoryReportPlu::SetReportDates ( COleDateTime dateFrom, COleDateTime dateTo, CSessionCSVRecord& ReportSession, CTimeSliceMapCSVRecord& TimeSlice )
{
	SetReportDates( dateFrom, dateTo, dateFrom, dateTo, ReportSession, TimeSlice );
}

/**********************************************************************/

void CSalesHistoryReportPlu::SetReportDates ( COleDateTime dateFrom, COleDateTime dateTo, COleDateTime dateFrom2, COleDateTime dateTo2, CSessionCSVRecord& ReportSession, CTimeSliceMapCSVRecord& TimeSlice )
{
	m_ReportSession.CopySession(ReportSession);
	
	m_oleDateFromBlock1 = dateFrom;
	m_oleDateToBlock1 = dateTo;
	m_oleDateFromBlock2 = dateFrom2;
	m_oleDateToBlock2 = dateTo2;

	SelectDateBlock(1);
	
	m_bEODMode = ReportSession.GetEODFlag();

	m_TimeSliceMap.SetReportType( TimeSlice.GetReportType() );
	m_TimeSliceMap.SetTimeSliceFileNo( TimeSlice.GetTimeSliceFileNo() );

	if ( m_TimeSliceMap.GetReportType() == SH_CUSTOM )
		m_TimeMap.BuildMap( m_TimeSliceMap.GetTimeSliceFileNo() );
}

/**********************************************************************/

void CSalesHistoryReportPlu::SelectDateBlock(int n)
{
	if ( n == 2 )
		DataManagerNonDb.SessionDateTimeFilter.SetDates( m_oleDateFromBlock2, m_oleDateToBlock2, m_ReportSession );
	else	
		DataManagerNonDb.SessionDateTimeFilter.SetDates( m_oleDateFromBlock1, m_oleDateToBlock1, m_ReportSession );
}

/**********************************************************************/

bool CSalesHistoryReportPlu::CheckPMSAccountDateRange( CSSFile& fileSales, int nPMSDayFrom, int nPMSDayTo )
{
	CString strBuffer;
	fileSales.ReadString( strBuffer );
					
	if ( strBuffer.Left(5) != "#DATE" )
		return FALSE;

	CCSV csv( strBuffer );

	int nAccountDayFrom = csv.GetInt(2);
	int nAccountDayTo = csv.GetInt(3);

	if ( ( nPMSDayTo >= nAccountDayFrom ) && ( nAccountDayTo >= nPMSDayFrom ) )
		return TRUE;

	return FALSE;
}

/**********************************************************************/

const char* CSalesHistoryReportPlu::GetReportTitle( const char* szReportName )
{
	CSortedDateTimeItem DateFrom = DataManagerNonDb.SessionDateTimeFilter.GetDateFrom();
	CSortedDateTimeItem DateTo = DataManagerNonDb.SessionDateTimeFilter.GetDateTo();

	CString strDateRange = "";
	if ( SALESHISTORY_REPORTMODE_TWOBLOCK != m_nReportMode )
	{
		strDateRange.Format ( "%2.2d/%2.2d/%4.4d - %2.2d/%2.2d/%4.4d, ",
			DateFrom.m_oleDateTime.GetDay(),			
			DateFrom.m_oleDateTime.GetMonth(), 
			DateFrom.m_oleDateTime.GetYear(),	 
			DateTo.m_oleDateTime.GetDay(), 
			DateTo.m_oleDateTime.GetMonth(), 
			DateTo.m_oleDateTime.GetYear() );
	}

	m_strTitle.Format ( "%s (%s%s%s)",
		 szReportName,
		 (const char*) strDateRange,
		 (const char*) m_strSessionTitle,
		 m_ReportFilters.DeptFilter.GetFilterNameForReport() );
	
	return m_strTitle;
}

/**********************************************************************/

const char* CSalesHistoryReportPlu::GetEmailSubject()
{
	CString strTitle = m_strReportName;
	::TrimSpaces( strTitle, FALSE );
	if ( strTitle == "" ) strTitle = "Custom Plu Sales History";
	
	m_strEmailSubject = GetReportTitle( strTitle );
	return m_strEmailSubject;
}

/**********************************************************************/

