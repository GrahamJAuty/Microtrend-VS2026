#pragma once
//$$******************************************************************
#include "CardPicker.h"
#include "TerminalListHandler.h"
//$$******************************************************************

class CReportInfoPurchaseHistory
{
public:
	CReportInfoPurchaseHistory( const char* szLabel );

public:
	//date
	const char* GetDateFromString();
	const char* GetDateToString();
	CSSDate GetDateFrom(){ return m_dateFrom; }
	CSSDate GetDateTo(){ return m_dateTo; }
	//time
	bool GetTimeRangeFlag(){ return m_bTimeRange; }
	const char* GetTimeFromString();
	const char* GetTimeToString();
	CSSTime GetTimeFrom(){ return m_timeFrom; }
	CSSTime GetTimeTo(){ return m_timeTo; }
	//accounts
	int GetCardRangeType(){ return m_nCardRangeType; }
	__int64 GetRangeFromCardNo(){ return m_nRangeFromCardNo; }
	__int64 GetRangeToCardNo(){ return m_nRangeToCardNo; }
	bool GetSingleGroupFlag() { return m_bGotSingleGroup; }
	int GetSingleGroupNo(){ return m_nSingleGroupNo; }
	bool GetNewPageFlag(){ return m_bNewPage; }
	//terminals
	int GetDbNo() { return m_nDbNo; }
	int GetFolderSet() { return m_nFolderSet; }
	int GetTerminalListType(){ return m_nTerminalListType; }
	const char* GetTerminalListString(){ return m_strTerminalList; }
	//options
	bool GetSummaryFlag(){ return m_bSummary; }
	int GetSalesTypeFilter(){ return m_nSalesTypeFilter; }

public:
	CTerminalListHandler* GetTerminalListHandler() { return &m_TerminalListHandler; }
	
public:
	const char* GetReportTitle(){ return m_strReportTitle; }
	const char* GetReportLabel()	{ return m_strReportLabel; }
	const char* GetReportFilename() { return m_strReportName; }
	const char* GetParamsFilename() { return m_strParamsName; }
	const char* GetReportKey()		{ return m_strReportKey; }
	
public:
	bool NonSQLFilter(__int64 nUserID);

public:
	int TNoListGetSize() { return m_TerminalListHandler.TNoListGetSize(); }
	CString TNoListGetItem(int n) { return m_TerminalListHandler.TNoListGetItem(n); }
	void TNoListDeleteItem(CString str) { m_TerminalListHandler.TNoListDeleteItem(str); }
	void TNoListSetMaxLength(int n) { m_TerminalListHandler.TNoListSetMaxLength(n); }
	void TNoListSave(CString str) { m_TerminalListHandler.TNoListSave(str); }
	void TNoListExtractTNos(const char* szList) { m_TerminalListHandler.TNoListExtractTNos(szList); }

public:
	bool CheckTerminalFilter(int nDbNo, int nFolderSet, int nTNo) { return m_TerminalListHandler.CheckTerminalFilter(nDbNo, nFolderSet, nTNo); }
	int TNoArrayGetCount() { return m_TerminalListHandler.TNoArrayGetCount(); }
	void TNoArrayRemoveAll() { m_TerminalListHandler.TNoArrayRemoveAll(); }
	UINT TNoArrayGetAt(int n) { return m_TerminalListHandler.TNoArrayGetAt(n); }

public:
	//date
	void SetDateFrom( CTime& date );
	void SetDateTo( CTime& date );
	//time
	void SetTimeRangeFlag( bool b ){ m_bTimeRange = b; }
	void SetTimeFrom( CTime& time );
	void SetTimeTo( CTime& time );
	//accounts
	void SetCardRangeType( int n ){ m_nCardRangeType = n; }
	void SetRangeFromCardNo( __int64 n ){ m_nRangeFromCardNo = n; }
	void SetRangeToCardNo( __int64 n ){ m_nRangeToCardNo = n; }
	void SetGotSingleGroupFlag( bool b ){ m_bGotSingleGroup = b; }
	void SetSingleGroupNo( int n ){ m_nSingleGroupNo = n; }
	void SetNewPageFlag( bool b ){ m_bNewPage = b; }
	//terminals
	void SetDbNo(int n);
	void SetFolderSet(int n);
	void SetTerminalListType(int n);
	void SetTerminalListString( const char* sz ){ m_strTerminalList = sz; }
	//options
	void SetSummaryFlag( bool b ){ m_bSummary = b; }
	void SetSalesTypeFilter(int n);
	
public:
	void SetReportKey( const char* sz ){ m_strReportKey = sz; }
	void SetReportTitle( const char* sz ){ m_strReportTitle = sz; }

public:
	void ResetCardPicker(){ m_CardPicker.Reset(); }
	
private:
	//date
	CSSDate m_dateFrom;
	CSSDate m_dateTo;
	CString m_strDateFrom;
	CString m_strDateTo;
	//time
	bool m_bTimeRange;
	CSSTime m_timeFrom;
	CSSTime m_timeTo;
	CString m_strTimeFrom;
	CString m_strTimeTo;
	//accounts
	int m_nCardRangeType;
	__int64 m_nRangeFromCardNo;
	__int64 m_nRangeToCardNo;
	bool m_bGotSingleGroup;
	int m_nSingleGroupNo;
	bool m_bNewPage;
	//terminals
	int m_nDbNo;
	int m_nFolderSet;
	int m_nTerminalListType;
	CString m_strTerminalList;
	//options
	bool m_bSummary;
	int m_nSalesTypeFilter;
	
private:
	CString m_strReportTitle;
	CString m_strReportLabel;
	CString m_strReportName;
	CString m_strParamsName;
	CString m_strReportKey;

private:
	CCardPicker m_CardPicker;
	CTerminalListHandler m_TerminalListHandler;
};

//$$******************************************************************

