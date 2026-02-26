#pragma once
//$$******************************************************************
#include "TerminalListHandler.h"
//$$******************************************************************

class CReportInfoAccumulator
{
public:
	CReportInfoAccumulator( const char* szLabel );

public:
	void Reset();

public:
	//criteria
	double GetThreshold(){ return m_dThreshold; }
	int GetRankByType(){ return m_nRankByType; }
	int GetTop100ChartSize(){ return m_nTop100ChartSize; }
	bool GetNoZeroSkipFlag(){ return m_bNoZeroSkip; }
	int GetDbNo() { return m_nDbNo; }
	int GetFolderSet() { return m_nFolderSet; }
	bool GetExcludeUnsoldFlag() { return m_bExcludeUnsold; }
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
	//terminals
	int GetTerminalListType(){ return m_nTerminalListType; }
	//plu list
	bool GetIncludeUnknownItemsFlag() { return m_bIncludeUnknownItems; }
	bool GetNewPageFlag() { return m_bNewPage; }	
	
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
	CTerminalListHandler* GetTerminalListHandler() { return &m_TerminalListHandler; }

public:
	//criteria
	void SetThreshold( double d ){ m_dThreshold = d; }
	void SetRankByType( int n ){ m_nRankByType = n; }
	void SetTop100ChartSize( int n ){ m_nTop100ChartSize = n; }
	void SetNoZeroSkipFlag( bool b ){ m_bNoZeroSkip = b; }
	void SetExcludeUnsoldFlag(bool b) { m_bExcludeUnsold = b; }
	//date
	void SetDateFrom( CTime& date );
	void SetDateTo( CTime& date );
	//time
	void SetTimeRangeFlag( bool b ){ m_bTimeRange = b; }
	void SetTimeFrom( CTime& time );
	void SetTimeTo( CTime& time );
	//terminals
	void SetDbNo(int n);
	void SetFolderSet(int n);
	void SetTerminalListType(int n);
	//plu list
	void SetNewPageFlag(bool b) { m_bNewPage = b; }
	void SetIncludeUnknownItemsFlag(bool b) { m_bIncludeUnknownItems = b; }	

public:
	const char* GetReportTitle();
	const char* GetReportLabel()	{ return m_strReportLabel; }
	const char* GetReportFilename() { return m_strReportName; }
	const char* GetParamsFilename() { return m_strParamsName; }
	const char* GetReportKey()		{ return m_strReportKey; }

public:
	const char* GetThresholdText();

public:
	const char* GetCaption();
	void SetReportKey( const char* sz ){ m_strReportKey = sz; }
	int GetReportType();

public:
	bool IsValid ( CDbExportHandler* pExport, CAuditRecord* atcRecord );
	bool CheckThreshold ( double dValue );

private:
	bool IsValidUsed ( CAuditRecord* atcRecord  );
	bool IsValidPoints ( CAuditRecord* atcRecord  );
	bool IsValidSpend ( CAuditRecord* atcRecord  );

private:
	const char* GetDateRangeString();
	const char* GetTimeRangeString();
	
private:
	//criteria
	double m_dThreshold;
	int m_nRankByType;
	int m_nTop100ChartSize;
	bool m_bNoZeroSkip;
	bool m_bExcludeUnsold;
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
	//terminals
	int m_nDbNo;
	int m_nFolderSet;	
	int m_nTerminalListType;
	CUIntArray m_arrayTNo;
	//plu list
	bool m_bNewPage;
	bool m_bIncludeUnknownItems;

private:
	CString m_strReportTitle;
	CString m_strReportLabel;
	CString m_strReportName;
	CString m_strParamsName;
	CString m_strReportKey;

private:
	CString m_strThresholdText;
	CString m_strDateRange;
	CString m_strTimeRange;
	CString m_strCaption;

private:
	CTerminalListHandler m_TerminalListHandler;
};

//$$******************************************************************

