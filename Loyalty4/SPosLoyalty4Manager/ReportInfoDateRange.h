#pragma once
//$$******************************************************************
#include "ReportInfoDateRangeBase.h"
//$$******************************************************************

class CReportInfoDateRange : public CReportInfoDateRangeBase
{
public:
	CReportInfoDateRange( const char* szLabel, const char* szTitle );

public:
	//time
	bool GetTimeRangeFlag(){ return m_bTimeRange; }
	const char* GetTimeFromString();
	const char* GetTimeToString();
	CSSTime GetTimeFrom(){ return m_timeFrom; }
	CSSTime GetTimeTo(){ return m_timeTo; }
		
public:
	bool IsSummaryAllowed(){ return m_bSummaryAllowed; }
	bool IsTimeAllowed(){ return m_bTimeAllowed; }
	const char* GetSummaryText(){ return m_strSummaryText; }
	
public:
	bool IsValid ( const char* szDate, const char* szTime = "" );

public:
	//time
	void SetTimeRangeFlag( bool b ){ m_bTimeRange = b; }
	void SetTimeFrom( CTime& time );
	void SetTimeTo( CTime& time );
			
private:
	//time
	bool m_bTimeRange;
	CSSTime m_timeFrom;
	CSSTime m_timeTo;
	CString m_strTimeFrom;
	CString m_strTimeTo;
		
private:
	bool m_bSummaryAllowed;
	bool m_bTimeAllowed;
	CString m_strSummaryText;
};

//$$******************************************************************

