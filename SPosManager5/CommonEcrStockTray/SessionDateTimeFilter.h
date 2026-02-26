#pragma once
/**********************************************************************/
#include "CustomerSelectionListAccount.h"
#include "SessionCSVArray.h"
/**********************************************************************/

class CSessionDateTimeFilter
{
public:
	CSessionDateTimeFilter(void);
	void Reset( bool bAppluSession = TRUE );
	void SetSimpleSessionArray();
	
public:
	void SetDates ( COleDateTime& oleDateFrom, COleDateTime& oleDateTo, CSessionCSVRecord& ReportSession, bool bApplySession = TRUE );
	void SetCustomerBalanceCheckDates();
	void SetCustomerStatementCheckDates( CCustomerSelectionListAccountInfo infoCustomer );
	void SetDepositUnredeemedCheckDateTo();

public:
	void SetDepositDateRangeEvent();
	void SetDepositDateRangeRedeem();
	void SetDepositDateRangeNormal();

public:
	int CheckTime ( const CString& strDate, const CString& strTime );
	bool CheckDate ( CString& strDate );
	bool ValidateBusinessDate( CString& strDate, CString& strTime, int& nDateType, COleDateTime& dateTransaction );
	COleDateTime GetLatestAcceptedDateTime();

public:
	bool IncludesPMS(){ return m_bIncludesPMS; }
	bool GetCalendarDateOnlyFlag(){ return m_bCalendarDateOnly; }
	const char* GetSessionID(){ return m_strSessionID; }
	const char* GetSessionName(){ return m_strSessionName; }
	CSortedDateTimeItem GetDateFrom(){ return m_DateFrom; }
	CSortedDateTimeItem GetDateTo(){ return m_DateTo; }
	CSortedDateTimeItem GetCheckDateFrom(){ return m_CheckDateFrom; }
	CSortedDateTimeItem GetCheckDateTo(){ return m_CheckDateTo; }
	
private:
	bool m_bGotFilter;
	bool m_bIncludesPMS;
	int m_nPMSAllowMode;
	CSortedDateTimeItem m_DateFrom;
	CSortedDateTimeItem m_DateTo;
	CSortedDateTimeItem m_CheckDateFrom;
	CSortedDateTimeItem m_CheckDateTo;
	CString m_strCheckDateTimeStart;
	CString m_strCheckDateTimeEnd;

private:
	bool m_bSpecialDepositMode;
	CSortedDateTimeItem m_SavedCheckDateTo;
	CSortedDateTimeItem m_SavedCheckDateFrom;

private:
	CArray<CReportSessionInfo,CReportSessionInfo> m_arraySessionInfo;
	CString m_strSessionID;
	CString m_strSessionName;
	bool m_bCalendarDateOnly;
};

/**********************************************************************/
