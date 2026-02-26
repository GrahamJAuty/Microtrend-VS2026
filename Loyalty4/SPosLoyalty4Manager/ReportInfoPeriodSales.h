#pragma once
//$$******************************************************************
#include "ReportInfoDateRangeBase.h"
#include "TerminalListHandler.h"
//$$******************************************************************

class CReportInfoPeriodSales : public CReportInfoDateRangeBase
{
public:
	CReportInfoPeriodSales();

public:
	int GetSalesTypeFilter() { return m_nSalesTypeFilter;  }
	bool GetManualSpendFlag() { return m_bManualSpend; }
	bool GetDiscountFlag() { return m_bDiscount; }
	int GetDbNo() { return m_nDbNo; }
	int GetFolderSet() { return m_nFolderSet; }
	int GetTerminalSelectType() { return m_nTerminalSelectType; }

public:
	void SetSalesTypeFilter(int n);
	void SetManualSpendFlag(bool b) { m_bManualSpend = b; }
	void SetDiscountFlag(bool b) { m_bDiscount = b; }
	void SetDbNo(int n);
	void SetFolderSet(int n);
	void SetTerminalSelectType(int n);

public:
	CTerminalListHandler* GetTerminalListHandler() { return &m_TerminalListHandler; }

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

private:
	int m_nDbNo = 0;
	int m_nFolderSet = 0;
	int m_nTerminalSelectType = 0;
	CTerminalListHandler m_TerminalListHandler;
	int m_nSalesTypeFilter = 0;
	bool m_bManualSpend = FALSE;
	bool m_bDiscount = FALSE;
};

//$$******************************************************************

