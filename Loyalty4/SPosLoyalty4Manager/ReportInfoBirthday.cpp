//$$******************************************************************
#include "ReportInfoBirthday.h"
//$$******************************************************************

CReportInfoBirthday::CReportInfoBirthday()
{
	//expiry
	m_bExpiry = FALSE;
	m_strDateRangeType = "";
	m_dateFrom.SetCurrentDate();					
	m_dateTo.SetCurrentDate();
	m_bExpiryMonthFilter = FALSE;
	//dob
	m_bDOB = FALSE;
	for ( int n = 0; n < 12; n++ )
		m_bMonth[n] = FALSE;
}

//$$******************************************************************

const char* CReportInfoBirthday::GetDateFromString()
{
	m_strDateFrom = m_dateFrom.GetDate();
	return m_strDateFrom;
}

//$$******************************************************************

const char* CReportInfoBirthday::GetDateToString()
{
	m_strDateTo = m_dateTo.GetDate();
	return m_strDateTo;
}

//$$******************************************************************

void CReportInfoBirthday::SetDateFrom( CTime& date )
{
	CTime time ( date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0 );
	m_dateFrom.SetDate( time );
}

//$$******************************************************************

void CReportInfoBirthday::SetDateTo( CTime& date )
{
	CTime time ( date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0 );
	m_dateTo.SetDate( time );
}

//$$******************************************************************

const char* CReportInfoBirthday::GetDateRangeString()
{
	if ( m_dateFrom != m_dateTo )	
		m_strDateRange.Format ( " (%s - %s)", m_dateFrom.GetDate(),  m_dateTo.GetDate() );
	else							
		m_strDateRange.Format ( " (%s)", m_dateFrom.GetDate() );

	return m_strDateRange;
}

//$$******************************************************************

bool CReportInfoBirthday::GetMonthFlag( int nMonth )
{
	if ( ( nMonth >= 0 ) && ( nMonth < 12 ) )
		return m_bMonth[nMonth];
	else
		return FALSE;
}

//$$******************************************************************

void CReportInfoBirthday::SetMonthFlag( int nMonth, bool bFlag )
{
	if ( ( nMonth >= 0 ) && ( nMonth < 12 ) )
		m_bMonth[nMonth] = bFlag;
}

//$$******************************************************************
