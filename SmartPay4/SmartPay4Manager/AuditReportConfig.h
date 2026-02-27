//$$******************************************************************
#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\Defines.h"
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\CardPicker.h"
//$$******************************************************************
#include "TerminalListHandler.h"
//$$******************************************************************
static const char* szCURRENTAUDIT = "Current";
//$$******************************************************************

class CAuditReportConfig
{
public:
	CAuditReportConfig(CString strLabel, CString strAuditFilename);

private:
	void SetLabelOptions();

public:
	CString GetReportLabel() { return m_strReportLabel; }
	int GetReportLabelNumber();
	CString GetAuditFilename() { return m_strAuditFilename; }
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
	bool GetSortByGroupFlag() { return m_bSortByGroup; }
	CString GetReportTitle() { return m_strReportTitle; }
	int GetReportType() { return m_nReportType; }
	bool GetShowPosLinesFlag() { return m_bShowPosLines; }
	bool GetShowPcLinesFlag() { return m_bShowPcLines; }
	bool GetShowServerLinesFlag() { return m_bShowServerLines; }
	bool GetShowDeletedLinesFlag() { return m_bShowDeletedLines; }
	bool GetShowWebPaymentLinesFlag() { return m_bShowWebPaymentLines; }
	bool GetShowCardLinkLinesFlag() { return m_bShowCardLinkLines; }
	bool GetShowVendingLinesFlag() { return m_bShowVendingLines; }
	bool GetShowEODLinesFlag() { return m_bShowEODLines; }
	bool GetShowEcrTextFlag() { return m_bShowEcrText; }
	bool GetShowExternalLinesFlag() { return m_bShowExternalLines; }
	bool GetShowLookupScanFlag() { return m_bShowLookupScan; }
	bool GetShowLookupBioFlag() { return m_bShowLookupBio; }
	bool GetShowLookupManualFlag() { return m_bShowLookupManual; }
	bool GetShowLookupDefaultFlag() { return m_bShowLookupDefault; }
	bool GetShowLookupOtherFlag() { return m_bShowLookupOther; }
	bool GetShowLookupInternalFlag() { return m_bShowLookupInternal; }
	bool GetShowLookupPINFlag() { return m_bShowLookupPIN; }
	bool GetShowLookupUnknownFlag() { return m_bShowLookupUnknown; }
	int GetReportRangeType() { return m_nReportRangeType; }
	bool GetNewPageFlag() { return m_bNewPage; }
	bool GetSingleGroupFlag() { return m_bSingleGroupFlag; }
	int GetSingleGroupNo() { return m_nSingleGroupNo; }
	int GetPasswordFilter() { return m_nPasswordFilter; }
	int GetFolderSet() { return m_nFolderSet; }
	int GetTerminalListType() { return m_nTerminalListType; }

public:
	bool GetShowAllActionsFlag() { return m_bShowAllActions; }
	int GetShowActionsFlags1() { return m_nShowActionsFlags1; }
	int GetShowActionsFlags2() { return m_nShowActionsFlags2; }
	int GetShowActionsFlags3() { return m_nShowActionsFlags3; }
	bool GetShowActionFlag(int n);

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
	void SetSortByGroupFlag(bool b) { m_bSortByGroup = b; }
	void SetShowPosLinesFlag(bool b) { m_bShowPosLines = b; }
	void SetShowPcLinesFlag(bool b) { m_bShowPcLines = b; }
	void SetShowServerLinesFlag(bool b) { m_bShowServerLines = b; }
	void SetShowDeletedLinesFlag(bool b) { m_bShowDeletedLines = b; }
	void SetShowWebPaymentLinesFlag(bool b) { m_bShowWebPaymentLines = b; }
	void SetShowCardLinkLinesFlag(bool b) { m_bShowCardLinkLines = b; }
	void SetShowVendingLinesFlag(bool b) { m_bShowVendingLines = b; }
	void SetShowEODLinesFlag(bool b) { m_bShowEODLines = b; }
	void SetShowEcrTextFlag(bool b) { m_bShowEcrText = b; }
	void SetShowExternalLinesFlag(bool b) { m_bShowExternalLines = b; }
	void SetShowLookupScanFlag(bool b) { m_bShowLookupScan = b;	}
	void SetShowLookupBioFlag(bool b) { m_bShowLookupBio = b; }
	void SetShowLookupManualFlag(bool b) { m_bShowLookupManual = b; }
	void SetShowLookupDefaultFlag(bool b) { m_bShowLookupDefault = b; }
	void SetShowLookupOtherFlag(bool b) { m_bShowLookupOther = b; }
	void SetShowLookupInternalFlag(bool b) { m_bShowLookupInternal = b; }
	void SetShowLookupPINFlag(bool b) { m_bShowLookupPIN = b; }
	void SetShowLookupUnknownFlag(bool b) { m_bShowLookupUnknown = b; }
	void SetReportRangeType(int n) { m_nReportRangeType = n; }
	void SetNewPageFlag(bool b) { m_bNewPage = b; }
	void SetSingleGroupFlag(bool b) { m_bSingleGroupFlag = b; }
	void SetSingleGroupNo(int n) { m_nSingleGroupNo = n; }
	void SetPasswordFilter(int n) { m_nPasswordFilter = n; }	
	void SetFolderSet(int n);
	void SetTerminalListType(int n);

public:
	void SetShowAllActionsFlag(bool b) { m_bShowAllActions= b; }
	void SetShowActionsFlags1(int n) { m_nShowActionsFlags1 = n; }
	void SetShowActionsFlags2(int n) { m_nShowActionsFlags2 = n; }
	void SetShowActionsFlags3(int n) { m_nShowActionsFlags3 = n; }
	void SetShowActionFlag(int n, bool b);

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
	CString SearchFilenamesGetPathname(int n);

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
	bool m_bSortByGroup;
	CString m_strReportTitle;
	int m_nReportType;
	bool m_bShowPosLines;
	bool m_bShowPcLines;
	bool m_bShowServerLines;
	bool m_bShowDeletedLines;
	bool m_bShowWebPaymentLines;
	bool m_bShowCardLinkLines;
	bool m_bShowVendingLines;
	bool m_bShowEODLines;
	bool m_bShowEcrText;
	bool m_bShowLookupScan;
	bool m_bShowLookupBio;
	bool m_bShowLookupManual;
	bool m_bShowLookupDefault;
	bool m_bShowLookupOther;
	bool m_bShowLookupInternal;
	bool m_bShowLookupPIN;
	bool m_bShowLookupUnknown;
	bool m_bShowExternalLines;
	int m_nReportRangeType;
	bool m_bNewPage;
	bool m_bSingleGroupFlag;
	int m_nSingleGroupNo;
	int m_nPasswordFilter;
	int m_nFolderSet;
	int m_nTerminalListType;

private:
	bool m_bShowAllActions;
	int m_nShowActionsFlags1;
	int m_nShowActionsFlags2;
	int m_nShowActionsFlags3;

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
	CTerminalListHandler m_TerminalListHandler;
};

//$$******************************************************************

