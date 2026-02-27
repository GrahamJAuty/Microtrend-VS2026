#pragma once
//*******************************************************************
#define nACCUMULATE_LOWPOINTS		0				// low points usage
#define nACCUMULATE_LOWSPEND		1				// low spend
#define nACCUMULATE_RANKPOINTS		2				// high ranked by points
#define nACCUMULATE_RANKSPEND		3				// high ranked by spend
#define nACCUMULATE_ACTIVE			4				// used cards in period
#define nACCUMULATE_INACTIVE		5				// cards not used in period
#define nACCUMULATE_RANKPURCHASES	6				// rank purchasess
//*******************************************************************
#define	nRANKBY_QTY		0
#define nRANKBY_VALUE	1
#define nRANKBY_POINTS	2				// only if Points allowed
//*******************************************************************
#include "..\SmartPay4Shared\SQLAuditRecord.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\DbExportHandler.h"
//*******************************************************************
#include "TerminalListHandler.h"
//*******************************************************************

struct CAccumulatorReportInfo
{
public:
	CAccumulatorReportInfo();
	void Reset();

public:
	bool GetNewPageFlag() { return m_bNewPage; }
	bool GetIncludeUnknownItemsFlag() { return m_bIncludeUnknownItems; }
	int GetRankByType() { return m_nRankBy; }
	int GetTop100ChartSize() { return m_nTop100; }
	int GetInfoSortType() { return m_nInfoSort; }
	bool GetNoZeroSkipFlag() { return m_bNoZeroSkip; }
	bool GetContinuousFlag() { return m_bContinuous; }
	double GetThreshold() { return m_dThreshold; }	
	//date
	CString GetDateFromString();
	CString GetDateToString();
	CSSDate GetDateFrom() { return m_dateFrom; }
	CSSDate GetDateTo() { return m_dateTo; }
	//time
	bool GetTimeRangeFlag() { return m_bTimeRange; }
	CString GetTimeFromString();
	CString GetTimeToString();
	CSSTime GetTimeFrom() { return m_timeFrom; }
	CSSTime GetTimeTo() { return m_timeTo; }
	//terminals
	int GetFolderSet() { return m_nFolderSet; }
	int GetTerminalListType() { return m_nTerminalListType; }

public:
	void SetNewPageFlag(bool b) { m_bNewPage = b; }
	void SetIncludeUnknownItemsFlag(bool b) { m_bIncludeUnknownItems = b; }
	void SetRankByType(int n) { m_nRankBy = n; }
	void SetTop100ChartSize(int n) { m_nTop100 = n; }
	void SetInfoSortType(int n) { m_nInfoSort = n; }
	void SetNoZeroSkipFlag(bool b) { m_bNoZeroSkip = b; }
	void SetContinuousFlag(bool b) { m_bContinuous = b; }
	void SetThreshold(double d) { m_dThreshold = d; }
	//date
	void SetDateFrom(CTime& date);
	void SetDateTo(CTime& date);
	//time
	void SetTimeRangeFlag(bool b) { m_bTimeRange = b; }
	void SetTimeFrom(CTime& time);
	void SetTimeTo(CTime& time);
	//terminals
	void SetFolderSet(int n);
	void SetTerminalListType(int n);

public:
	CString GetReportLabel() const { return m_strReportLabel; }
	int GetReportType() const { return m_nReportType; }
	CString GetCaption() const { return m_strCaption; }
	CString GetReportTitle();
	CString GetThresholdText();
	bool CheckThreshold(double dValue) const;
	
public:
	void SetReportLabel(CString str);

public:
	bool IsValid(CDbExportHandler* pExport, CSQLAuditRecord* atcRecord);

public:
	int TNoListGetSize() { return m_TerminalListHandler.TNoListGetSize(); }
	CString TNoListGetItem(int n) { return m_TerminalListHandler.TNoListGetItem(n); }
	void TNoListDeleteItem(CString str) { m_TerminalListHandler.TNoListDeleteItem(str); }
	void TNoListSetMaxLength(int n) { m_TerminalListHandler.TNoListSetMaxLength(n); }
	void TNoListSave(CString str) { m_TerminalListHandler.TNoListSave(str); }
	void TNoListExtractTNos(const char* szList) { m_TerminalListHandler.TNoListExtractTNos(szList); }

public:
	bool CheckTerminalFilter(int nFolderSet, int nTNo) { return m_TerminalListHandler.CheckTerminalFilter(nFolderSet, nTNo); }
	int TNoArrayGetCount() { return m_TerminalListHandler.TNoArrayGetCount(); }
	void TNoArrayRemoveAll() { m_TerminalListHandler.TNoArrayRemoveAll(); }
	UINT TNoArrayGetAt(int n) { return m_TerminalListHandler.TNoArrayGetAt(n); }

public:
	CTerminalListHandler* GetTerminalListHandler() { return &m_TerminalListHandler; }

private:
	bool IsValidPoints(CSQLAuditRecord* atcRecord);
	bool IsValidSpend(CSQLAuditRecord* atcRecord);
	bool IsValidUsed(CSQLAuditRecord* atcRecord);
	
public:
	CString m_strDateString;
	CString m_strTimeString;

private:
	bool m_bNewPage;
	bool m_bIncludeUnknownItems;
	int m_nRankBy;
	int m_nTop100;
	int m_nInfoSort;
	bool m_bNoZeroSkip;
	bool m_bContinuous;
	double m_dThreshold;
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
	int m_nFolderSet;
	int m_nTerminalListType;

private:
	CString m_strReportLabel;
	int m_nReportType;
	CString m_strCaption;
	CString m_strReportTitle;
	CString m_strThresholdText;

private:
	CTerminalListHandler m_TerminalListHandler;
};

//*******************************************************************