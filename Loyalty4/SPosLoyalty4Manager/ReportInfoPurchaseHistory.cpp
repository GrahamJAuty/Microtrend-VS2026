//$$******************************************************************
#include "AuditReportDlg.h"
#include "LoyaltyManager.h"
//$$******************************************************************
#include "ReportInfoPurchaseHistory.h"
//$$******************************************************************

CReportInfoPurchaseHistory::CReportInfoPurchaseHistory( const char* szLabel ) : m_TerminalListHandler(Filenames.GetTerminalListFilename())
{
	m_strReportTitle = "";
	m_strReportLabel = szLabel;
	m_strReportName	= Filenames.GetReportFilename ( m_strReportLabel );
	m_strParamsName = Filenames.GetReportParamsFilename ( m_strReportLabel );
	m_strReportKey = "";

	//date
	m_dateFrom.SetCurrentDate();					
	m_dateTo.SetCurrentDate();
	//time
	m_bTimeRange = FALSE;
	m_timeFrom.SetTime( "00:00" );
	m_timeTo.SetTime( "23:59" );
	//accounts
	m_nCardRangeType = 0;
	m_nRangeFromCardNo = 0;
	m_nRangeToCardNo = 0;
	m_bGotSingleGroup = FALSE;
	m_nSingleGroupNo = 0;
	m_bNewPage = FALSE;
	//terminals
	SetDbNo(0);
	SetFolderSet(0);
	SetTerminalListType(0);
	m_strTerminalList = "";
	//options
	m_bSummary = FALSE;
	m_nSalesTypeFilter = 0;
}

//$$******************************************************************

void CReportInfoPurchaseHistory::SetSalesTypeFilter(int n)
{
	if ((n >= 0) && (n <= 2))
	{
		m_nSalesTypeFilter = n;
	}
}

//$$******************************************************************

const char* CReportInfoPurchaseHistory::GetDateFromString()
{
	m_strDateFrom = m_dateFrom.GetDate();
	return m_strDateFrom;
}

//$$******************************************************************

const char* CReportInfoPurchaseHistory::GetDateToString()
{
	m_strDateTo = m_dateTo.GetDate();
	return m_strDateTo;
}

//$$******************************************************************

void CReportInfoPurchaseHistory::SetDateFrom( CTime& date )
{
	CTime time ( date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0 );
	m_dateFrom.SetDate( time );
}

//$$******************************************************************

void CReportInfoPurchaseHistory::SetDateTo( CTime& date )
{
	CTime time ( date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0 );
	m_dateTo.SetDate( time );
}

//$$******************************************************************

void CReportInfoPurchaseHistory::SetTimeFrom( CTime& time )
{
	m_timeFrom.SetTime( time );
}

//$$******************************************************************

void CReportInfoPurchaseHistory::SetTimeTo( CTime& time )
{
	m_timeTo.SetTime( time );
}

//$$******************************************************************

const char* CReportInfoPurchaseHistory::GetTimeFromString()
{
	m_strTimeFrom = m_timeFrom.GetTime();
	return m_strTimeFrom;
}

//$$******************************************************************

const char* CReportInfoPurchaseHistory::GetTimeToString()
{
	m_strTimeTo = m_timeTo.GetTime();
	return m_strTimeTo;
}

//$$******************************************************************

bool CReportInfoPurchaseHistory::NonSQLFilter(__int64 nUserID)
{
	bool bResult = TRUE;
	
	if (nREPORT_LIST == m_nCardRangeType)
	{
		return m_CardPicker.IsValid(SimpleFormatInt64Value(nUserID), Filenames.GetReportCardListFilename(m_strReportLabel));
	}

	return bResult;
}

//$$******************************************************************

void CReportInfoPurchaseHistory::SetDbNo(int n)
{
	m_nDbNo = n;
	m_TerminalListHandler.SetTargetDbNo(n);
}

//$$******************************************************************

void CReportInfoPurchaseHistory::SetFolderSet(int n)
{
	m_nFolderSet = n;
	m_TerminalListHandler.SetTargetFolderSet(n);
}

//$$******************************************************************

void CReportInfoPurchaseHistory::SetTerminalListType(int n)
{
	m_nTerminalListType = n;
	m_TerminalListHandler.SetTerminalSelectType(n);
}

//$$******************************************************************
