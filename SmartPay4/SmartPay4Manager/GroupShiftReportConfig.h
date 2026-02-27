//$$******************************************************************
#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\Defines.h"
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\CardPicker.h"
//$$******************************************************************
#define nGROUPSHIFT_REPORT 0
//$$******************************************************************

class CGroupShiftReportConfig
{
public:
	CGroupShiftReportConfig(CString strLabel);
	void SetLabelOptions();

public:
	CString GetAuditFilename() { return m_strAuditFilename; }
	CString GetReportFilename() { return m_strReportFilename; }
	CString GetReportCardListFilename() { return m_strReportCardListFilename; }
	CString GetParamsFilename() { return m_strParamsFilename; }
	CString GetReportLabel() { return m_strReportLabel; }
	
public:
	CString GetReportKey() { return m_strReportKey; }
	bool GetSingleCardFlag() { return m_bSingleCard; }
	CString GetStringCardNoFrom() { return m_strFromCardNo; }
	CString GetStringCardNoTo() { return m_strToCardNo; }
	__int64 GetInt64CardNoFrom() { return m_nFromCardNo; }
	__int64 GetInt64CardNoTo() { return m_nToCardNo; }
	bool GetSortByCardNoFlag() { return m_bSortByCardNo; }
	CString GetReportTitle() { return m_strReportTitle; }
	int GetReportType() { return m_nReportType; }
	int GetReportRangeType() { return m_nReportRangeType; }
	bool GetNewPageFlag() { return m_bNewPage; }
	bool GetSingleGroupFlag() { return m_bSingleGroupFlag; }
	int GetSingleGroupNo() { return m_nSingleGroupNo; }
	bool GetShowDeletedLinesFlag() { return m_bShowDeletedLines; }
	bool GetShowSourceManagerFlag() { return m_bShowSourceManager; }
	bool GetShowSourceServerFlag() { return m_bShowSourceServer; }
	bool GetShowSourceBackgroundFlag() { return m_bShowSourceBackground; }
	bool GetShowSourceManualFlag() { return m_bShowSourceManual; }
	bool GetShowPreviousFlag() { return m_bShowPrevious; }

public:
	int GetSearchType() { return m_nSearchType; }
	CString GetSearchText() { return m_strSearchText; }
	CString GetSearchCardNo() { return m_strSearchCardNo; }

public:
	bool GetTimeRangeFlag() { return m_bTimeRange; }
	int GetTimeSelection() { return m_nTimeSelection; }
	CSSTime GetTimeFrom() { return m_timeFrom; }
	CSSTime GetTimeTo() { return m_timeTo; }
	CString GetDateRangeType() { return m_strDateRangeType; }
	CSSDate GetDateFrom() { return m_dateFrom; }
	CSSDate GetDateTo() { return m_dateTo; }
	bool GetContinousFlag() { return m_bContinuous; }

public:
	CString GetTimeFromString();
	CString GetTimeToString();
	CString GetDateFromString();
	CString GetDateToString();

private:
	void SetReportTitle(CString str) { m_strReportTitle = str; }
	void SetReportType(int n) { m_nReportType = n; }

public:
	void SetReportKey(CString str) { m_strReportKey = str; }
	void SetSingleCardFlag(bool b) { m_bSingleCard = b; }
	void SetStringCardNoFrom(CString str) { m_strFromCardNo = str; }
	void SetStringCardNoTo(CString str) { m_strToCardNo = str; }
	void UpdateInt64CardNo(bool bSingle);
	void SetSortByCardNoFlag(bool b) { m_bSortByCardNo = b; }
	void SetReportRangeType(int n) { m_nReportRangeType = n; }
	void SetNewPageFlag(bool b) { m_bNewPage = b; }
	void SetSingleGroupFlag(bool b) { m_bSingleGroupFlag = b; }
	void SetSingleGroupNo(int n) { m_nSingleGroupNo = n; }
	void SetShowDeletedLinesFlag(bool b) { m_bShowDeletedLines = b;  }
	void SetShowSourceManagerFlag(bool b) { m_bShowSourceManager = b; }
	void SetShowSourceServerFlag(bool b) { m_bShowSourceServer = b;}
	void SetShowSourceBackgroundFlag(bool b) { m_bShowSourceBackground = b; }
	void SetShowSourceManualFlag(bool b) { m_bShowSourceManual = b; }
	void SetShowPreviousFlag(bool b) { m_bShowPrevious = b; }

public:
	void SetSearchType(int n) { m_nSearchType = n; }
	void SetSearchText(CString str) { m_strSearchText = str; }
	void SetSearchCardNo(CString str) { m_strSearchCardNo = str; }

public:
	void SetTimeRangeFlag(bool b) { m_bTimeRange = b; }
	void SetTimeSelection(int n) { m_nTimeSelection = n; }
	void SetTimeFrom(CString strTime) { m_timeFrom.SetTime(strTime); }
	void SetTimeTo(CString strTime) { m_timeTo.SetTime(strTime); }
	void SetDateRangeType(CString str) { m_strDateRangeType = str; }
	void SetDateFrom(CTime time) { m_dateFrom.SetDate(time); }
	void SetDateTo(CTime time) { m_dateTo.SetDate(time); }
	void SetContinuousFlag(bool b) { m_bContinuous = b;  }
	CString GetDateString();

public:
	void ResetCardPicker() { m_CardPicker.Reset(); }
	bool IsValidCard(const char* szCardNo, const char* szFilename) { return m_CardPicker.IsValid(szCardNo, szFilename); }

private:
	CString m_strReportLabel;
	CString m_strAuditFilename;
	CString m_strReportFilename;
	CString m_strReportCardListFilename;
	CString m_strParamsFilename;

private:
	CString m_strReportKey;
	bool m_bSingleCard;
	CString m_strFromCardNo;
	CString m_strToCardNo;
	__int64 m_nFromCardNo;
	__int64 m_nToCardNo;
	bool m_bSortByCardNo;
	CString m_strReportTitle;
	int m_nReportType;
	int m_nReportRangeType;
	bool m_bNewPage;
	bool m_bSingleGroupFlag;
	int m_nSingleGroupNo;
	bool m_bShowDeletedLines;
	bool m_bShowSourceManager;
	bool m_bShowSourceServer;
	bool m_bShowSourceBackground;
	bool m_bShowSourceManual;
	bool m_bShowPrevious;

public:
	int m_nSearchType;
	CString m_strSearchText;
	CString m_strSearchCardNo;
	CStringArray m_arraySearchFilenames;

private:
	bool m_bTimeRange;
	int m_nTimeSelection;
	CSSTime m_timeFrom;
	CSSTime m_timeTo;
	CString m_strDateRangeType;
	CSSDate m_dateFrom;
	CSSDate m_dateTo;
	bool m_bContinuous;

private:
	CString m_strTimeFrom;
	CString m_strTimeTo;
	CString m_strDateFrom;
	CString m_strDateTo;

private:
	CCardPicker m_CardPicker;
};

//$$******************************************************************

