//**********************************************************************
#pragma once
//**********************************************************************
#define nPURCHASE_ITEMISED		0			// used as index				}
#define nPURCHASE_ACCOUNTITEM	1			// plu item summary by account	}
#define nPURCHASE_ACCOUNTDEPT	2			// plu dept summary by account	}
#define nPURCHASE_GROUPITEM		3			// plu item summary by group
#define nPURCHASE_GROUPDEPT		4			// plu dept summary by group
#define nPURCHASE_MAX			5			// number of options above
//**********************************************************************
#include "..\SmartPay4Shared\Defines.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//*******************************************************************
#include "CommentHandler.h"
#include "GroupPicker.h"
#include "TerminalListHandler.h"
//**********************************************************************

class CReportPurchaseHelpers
{
public:
	CReportPurchaseHelpers();

public:
	CString GetReportLabelInternal() { return m_strReportLabel; }
	CString GetReportLabelExternal();
	CString GetReportFilename();
	CString GetParamsFilename();
	CString GetCardListFilename();
	void SetGroupPickerFilename() { m_GroupPicker.SetFilename(GetGroupPickerFilename()); }
	CString GetGroupPickerFilename();
	void ResetGroupPicker() { m_GroupPicker.Reset(); }
	void SetSingleGroupNoFromGroupPicker() { m_nSingleGroupNo = m_GroupPicker.GetSingleGroupNo(); }
	CString GetWindowTitle();
	CString GetPurchaseReportTitle();
	CString GetBreakdownReportTitle();
	void ResetCardPicker() { m_CardPicker.Reset(); }
	bool CheckUserIDAgainstList(__int64 nUserID);
	void GetSQLSearchInfoPurchase(CPurchaseHistorySearchInfo& infoSearch, __int64 nUserID = -1);
	void GetSQLSearchInfoBreakdown(CPurchaseHistorySearchInfo& infoSearch, __int64 nUserID = -1);
	bool NonSQLFilterBreakdown(CSQLRowAccountFull& RowAccount);
	CString GetOrderByLabel();
	//terminals
	int GetFolderSet() { return m_nFolderSet; }
	int GetTerminalListType() { return m_nTerminalListType; }

public:
	bool GetSelectivePluFlag() { return m_bSelectivePlu; }
	CString GetSelectivePluText() { return m_strSelectivePluText; }
	CSSDate GetDateFrom() { return m_dateFrom; }
	CSSDate GetDateTo() { return m_dateTo; }
	CSSTime GetTimeFrom() { return m_timeFrom; }
	CSSTime GetTimeTo() { return m_timeTo; }
	__int64 GetRangeFromCardNo() { return m_nRangeFromCardNo; }
	__int64 GetRangeToCardNo() { return m_nRangeToCardNo; }
	CString GetFromCardNo() { return m_strFromCardNo; }
	CString GetToCardNo() { return m_strToCardNo; }
	CString GetInfoTextFilter() { return m_strInfoTextFilter;  }
	int GetInfoTextIndex() { return m_nInfoTextIndex; }
	int GetPurchaseReportType() { return m_nPurchaseReportType; }
	bool GetTimeRangeSetFlag() { return m_bTimeRangeSet; }
	int GetReportRangeType() { return m_nReportRangeType; }
	bool GetSortByGroupFlag() { return m_bSortByGroup;  }
	bool GetGroupListFlag() { return m_bGroupList; }
	int GetSingleGroupNo() { return m_nSingleGroupNo; }
	int GetGroupComboIndex() { return m_nGroupComboIndex; }
	bool GetNewPageFlag(int nReportNo = -1 );
	int GetAllNewPageFlags() { return m_nNewPageFlags; }
	bool GetExcludeZeroPriceFlag(int nReportNo = -1);
	int GetAllExcludeZeroPriceFlags() { return m_nExcludeZeroPriceFlags; }
	CString GetReportKey() { return m_strReportKey; }
	bool GetContinuousFlag() { return m_bContinuous; }
	bool GetShowAccountNameFlag() { return m_bShowAccountName; }
	bool GetSkipOnBlankFlag() { return m_bSkipOnBlank; }

public:
	bool IsStatementReport();
	bool IsStatementReportSingle();
	bool IsStatementReportAll();
	bool IsPurchaseHistoryReport();
	bool IsPurchaseHistoryReportSingle();
	bool IsPurchaseHistoryReportAll();
	bool IsSingleAccountReport();
	bool IsPurchaseBreakdownReport();
	
public:
	void SetReportLabel(CString str) { m_strReportLabel = str; }
	void SetSelectivePluFlag(bool b) { m_bSelectivePlu = b; }
	void SetSelectivePluText(CString str) { m_strSelectivePluText = str; }
	void SetDateFrom(CTime date) { m_dateFrom.SetDate(date); }
	void SetDateFrom( CSSDate date) { m_dateFrom = date; }
	void SetDateTo(CTime date) { m_dateTo.SetDate(date); }
	void SetDateTo( CSSDate date ) { m_dateTo = date; }
	void SetTimeFrom(CString strTime) { m_timeFrom.SetTime(strTime); }
	void SetTimeFrom( CSSTime time ) { m_timeFrom = time; }
	void SetTimeTo(CString strTime) { m_timeTo.SetTime(strTime); }
	void SetTimeTo( CSSTime time ) { m_timeTo = time; }
	void SetRangeFromCardNo(__int64 n) { m_nRangeFromCardNo = n;  }
	void SetRangeToCardNo(__int64 n) { m_nRangeToCardNo = n; }
	void SetFromCardNo(CString str) { m_strFromCardNo = str; }
	void SetToCardNo(CString str) { m_strToCardNo = str; }
	void SetInfoTextFilter(CString str) { m_strInfoTextFilter = str;  }
	void SetInfoTextIndex(int n) { m_nInfoTextIndex = n; }
	void SetPurchaseReportType(int n) { m_nPurchaseReportType = n; }
	void SetTimeRangeSetFlag(bool b) { m_bTimeRangeSet = b; }
	void SetReportRangeType(int n) { m_nReportRangeType = n; }
	void SetSortByGroupFlag(bool b) { m_bSortByGroup = b; }
	void SetGroupListFlag(bool b) { m_bGroupList = b; }
	void SetSingleGroupNo(int n) { m_nSingleGroupNo = n;  }
	void SetGroupComboIndex(int n) { m_nGroupComboIndex = n; }
	void SetNewPageFlag(int nReportNo, bool bFlag);
	void SetAllNewPageFlags(int n) { m_nNewPageFlags = n; }
	void SetExcludeZeroPriceFlag(int nReportNo, bool bFlag);
	void SetAllExcludeZeroPriceFlags(int n) { m_nExcludeZeroPriceFlags = n; }
	void SetReportKey(CString str) { m_strReportKey = str; }
	void SetContinuousFlag(bool b) { m_bContinuous = b; }
	void SetShowAccountNameFlag(bool b) { m_bShowAccountName = b; }
	void SetSkipOnBlankFlag(bool b) { m_bSkipOnBlank = b; }
	//terminals
	void SetFolderSet(int n);
	void SetTerminalListType(int n);

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
	bool m_bSelectivePlu;
	CString m_strSelectivePluText;
	CString m_strReportLabel;
	CSSDate m_dateFrom;
	CSSDate m_dateTo;
	CSSTime m_timeFrom;
	CSSTime m_timeTo;
	__int64 m_nRangeFromCardNo;
	__int64 m_nRangeToCardNo;
	CString m_strFromCardNo;
	CString m_strToCardNo;
	CString m_strInfoTextFilter;
	int m_nInfoTextIndex;
	int m_nPurchaseReportType;
	bool m_bTimeRangeSet;
	int m_nReportRangeType;
	bool m_bSortByGroup;
	int m_nSingleGroupNo;
	int m_nGroupComboIndex;
	int m_nNewPageFlags;
	int m_nExcludeZeroPriceFlags;
	CString m_strReportKey;
	bool m_bUseTerminalList;
	CString m_strTerminalList;
	bool m_bContinuous;
	bool m_bShowAccountName;
	bool m_bSkipOnBlank;
	bool m_bGroupList;
	int m_nFolderSet;
	int m_nTerminalListType;

private:
	CCardPicker m_CardPicker;
	CGroupPicker m_GroupPicker;
	CUIntArray m_arrayTNo;

private:
	CTerminalListHandler m_TerminalListHandler;
};

//**********************************************************************
