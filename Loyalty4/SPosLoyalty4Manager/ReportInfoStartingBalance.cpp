//$$******************************************************************
#include "ReportInfoStartingBalance.h"
//$$******************************************************************

CReportInfoStartingBalance::CReportInfoStartingBalance()
{
	//date
	m_dateBalance.SetCurrentDate();
	m_dateSearchFrom.SetCurrentDate();
	m_dateSearchTo.SetCurrentDate();
}

//$$******************************************************************

const char* CReportInfoStartingBalance::GetDateBalanceString()
{
	m_strDateBalance = m_dateBalance.GetDate();
	return m_strDateBalance;
}

//$$******************************************************************

const char* CReportInfoStartingBalance::GetDateSearchFromString()
{
	m_strDateSearchFrom = m_dateSearchFrom.GetDate();
	return m_strDateSearchFrom;
}

//$$******************************************************************

const char* CReportInfoStartingBalance::GetDateSearchToString()
{
	m_strDateSearchTo = m_dateSearchTo.GetDate();
	return m_strDateSearchTo;
}

//$$******************************************************************

void CReportInfoStartingBalance::SetDateBalance( CTime& date )
{
	CTime time ( date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0 );
	m_dateBalance.SetDate( time );
}

//$$******************************************************************

void CReportInfoStartingBalance::SetDateSearchFrom(CTime& date)
{
	CTime time(date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0);
	m_dateSearchFrom.SetDate(time);
}

//$$******************************************************************

void CReportInfoStartingBalance::SetDateSearchTo(CTime& date)
{
	CTime time(date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0);
	m_dateSearchTo.SetDate(time);
}

//$$******************************************************************
// -1 = Invalid date
//  0 = before report date
//  1 = on or after report date
//$$******************************************************************

int CReportInfoStartingBalance::IsBeforeReportDate(CAuditRecord* atcRecord)
{
	CString strDate;

	switch (atcRecord->GetSourceType())
	{
	case AUDIT_POS:
	case AUDIT_EXT:
		strDate = atcRecord->GetDateLastUsed();
		break;

	default:
		strDate = atcRecord->GetDateFiled();
		break;
	}

	CSSDate dateAudit(strDate);

	if (dateAudit.IsSet() == FALSE)
	{
		return -1;
	}

	if (dateAudit < m_dateBalance)
	{
		return 0;
	}

	return 1;
}

//$$******************************************************************


