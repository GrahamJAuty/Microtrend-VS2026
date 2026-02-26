#pragma once
//$$******************************************************************
#include "AuditRecord.h"
//$$******************************************************************

class CReportInfoCashless
{
public:
	CReportInfoCashless( const char* szLabel, bool bLiabilityMode );

public:
	//date
	const char* GetDateFromString();
	const char* GetDateToString();
	const char* GetDatePreviousString();
	const char* GetDateNextString();
	const char* GetDateSearchFromString();
	const char* GetDateSearchToString();
	CSSDate GetDateFrom(){ return m_dateFrom; }
	CSSDate GetDateTo(){ return m_dateTo; }
	bool GetCutoffTimeFlag(){ return m_bCutoffTime; }
	bool GetCutoffNextDayFlag() { return m_bCutoffNextDay; }
	int GetRefundProcessType() { return m_nRefundProcessType; }
	//time
	const char* GetTimeCutoffString();
	CSSTime GetTimeCutoff(){ return m_timeCutoff; }
	
public:
	const char* GetReportTitle(){ return m_strReportTitle; }
	const char* GetReportKey(){ return m_strReportKey; }
	const char* GetReportLabel(){ return m_strReportLabel; }
		
public:
	const char* GetDateRangeString();
	bool IsValidReportDate(const char* szDate, const char* szTime);
	int IsBeforeReportDate(CAuditRecord* atcRecord);
	int IsBeforeReportDate(CSSDate& dateAudit, CSSTime& timeAudit);
	
public:
	//date
	void SetDateFrom( CTime& date );
	void SetDateTo( CTime& date );
	void SetDatePrevious( CTime& date );
	void SetDateNext( CTime& date);
	void SetDateSearchFrom( CTime& date );
	void SetDateSearchTo(CTime& date);
	void SetCutoffTimeFlag( bool b ){ m_bCutoffTime = b; }
	void SetCutoffNextDayFlag(bool b) { m_bCutoffNextDay = b; }
	void SetTimeCutoff(CTime& time);
	void SetRefundProcessType(int n);
	
public:
	void SetReportKey( const char* sz ){ m_strReportKey = sz; }
	void SetReportTitle( const char* sz ){ m_strReportTitle = sz; }
	
public:
	bool GetLiabilityModeFlag(){ return m_bLiabilityMode; }

private:
	void SetCSSDateFromCTime(CTime& source, CSSDate& dest);
	bool IsValidReportDateWithCutoffTime(CSSDate& dateAudit, CString strTime, CSSDate& dateFrom, CSSDate& dateTo);

private:
	//date
	CSSDate m_dateFrom;
	CSSDate m_dateTo;
	CSSDate m_datePrevious;
	CSSDate m_dateNext;
	CSSDate m_dateSearchFrom;
	CSSDate m_dateSearchTo;
	CString m_strDateFrom = "";
	CString m_strDateTo = "";
	CString m_strDatePrevious = "";
	CString m_strDateNext = "";
	bool m_bCutoffTime = FALSE;
	bool m_bCutoffNextDay = FALSE;
	CString m_strDateSearchFrom = "";
	CString m_strDateSearchTo = "";
	int m_nRefundProcessType = 0;
	//time
	CSSTime m_timeCutoff;
	CString m_strTimeCutoff = "";
	
private:
	CString m_strReportTitle = "";
	CString m_strReportLabel = "";
	CString m_strReportKey = "";

private:
	bool m_bLiabilityMode = FALSE;
	
private:
	CString m_strDateRange = "";
};

//$$******************************************************************

