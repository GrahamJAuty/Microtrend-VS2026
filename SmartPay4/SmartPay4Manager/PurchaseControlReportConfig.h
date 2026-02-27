#pragma once
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\CardPicker.h"
//$$******************************************************************
#include "CommentHandler.h"
#include "TerminalListHandler.h"
//$$******************************************************************

class CPurchaseControlReportConfig
{
public:
	CPurchaseControlReportConfig(CString strLabel);

private:
	void SetLabelOptions();

public:
	CString GetReportLabel() { return m_strReportLabel; }
	CString GetReportFilename() { return m_strReportFilename; }
	CString GetReportCardListFilename() { return m_strReportCardListFilename; }
	CString GetParamsFilename() { return m_strParamsFilename; }
	CString GetArchiveExtension() { return "atc"; }

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
	bool GetShowDeletedLinesFlag() { return m_bShowDeletedLines; }
	int GetReportRangeType() { return m_nReportRangeType; }
	bool GetNewPageFlag() { return m_bNewPage; }
	bool GetSingleGroupFlag() { return m_bSingleGroupFlag; }
	int GetSingleGroupNo() { return m_nSingleGroupNo; }
	bool GetShowEPOSAwardFlag() { return m_bShowEPOSAward; }
	bool GetShowAutoDeleteFlag() { return m_bShowAutoDelete; }
	bool GetShowManualAddFlag() { return m_bShowManualAdd; }
	bool GetShowManualEditFlag() { return m_bShowManualEdit; }
	bool GetShowManualDeleteFlag() { return m_bShowManualDelete; }
	bool GetShowTransferFlag() { return m_bShowTransfer; }
	bool GetShowHotlistFlag() { return m_bShowHotlist; }
	int GetSingleRuleNo() { return m_nSingleRuleNo; }
	int GetFolderSet() { return m_nFolderSet; }
	int GetTerminalListType() { return m_nTerminalListType; }

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
	void SetShowDeletedLinesFlag(bool b) { m_bShowDeletedLines = b; }
	void SetReportRangeType(int n) { m_nReportRangeType = n; }
	void SetNewPageFlag(bool b) { m_bNewPage = b; }
	void SetSingleGroupFlag(bool b) { m_bSingleGroupFlag = b; }
	void SetSingleGroupNo(int n) { m_nSingleGroupNo = n; }
	void SetShowEPOSAwardFlag(bool b) { m_bShowEPOSAward = b; }
	void SetShowAutoDeleteFlag(bool b) { m_bShowAutoDelete = b; }
	void SetShowManualAddFlag(bool b) { m_bShowManualAdd = b; }
	void SetShowManualEditFlag(bool b) { m_bShowManualEdit = b; }
	void SetShowManualDeleteFlag(bool b) { m_bShowManualDelete = b; }
	void SetShowHotlistFlag(bool b) { m_bShowHotlist = b; }
	void SetShowTransferFlag(bool b) { m_bShowTransfer = b; }
	void SetSingleRuleNo(int n) { m_nSingleRuleNo = n; }
	void SetFolderSet(int n);
	void SetTerminalListType(int n);

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
	CString GetDateString();

public:
	void ResetCardPicker() { m_CardPicker.Reset(); }
	bool IsValidCard(const char* szCardNo, const char* szFilename) { return m_CardPicker.IsValid(szCardNo, szFilename); }

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

public:
	void SearchFilenamesReset() { m_arraySearchFilenames.RemoveAll(); }
	int SearchFilenamesGetCount() { return m_arraySearchFilenames.GetSize(); }
	void SearchFilenamesAddItem(CString str) { m_arraySearchFilenames.Add(str); }
	CString SearchFilenamesGetItem(int n) { return m_arraySearchFilenames.GetAt(n); }

private:
	CString m_strReportLabel;
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
	bool m_bShowDeletedLines;
	int m_nReportRangeType;
	bool m_bNewPage;
	bool m_bSingleGroupFlag;
	int m_nSingleGroupNo;
	bool m_bShowEPOSAward;
	bool m_bShowAutoDelete;
	bool m_bShowManualAdd;
	bool m_bShowManualEdit;
	bool m_bShowManualDelete;
	bool m_bShowHotlist;
	bool m_bShowTransfer;
	int m_nSingleRuleNo;
	int m_nFolderSet;
	int m_nTerminalListType;

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

private:
	CString m_strTimeFrom;
	CString m_strTimeTo;
	CString m_strDateFrom;
	CString m_strDateTo;

private:
	CCardPicker m_CardPicker;
	CTerminalListHandler m_TerminalListHandler;
};

//$$******************************************************************

