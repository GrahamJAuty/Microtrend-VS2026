#pragma once
/**********************************************************************/
#include "resource.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PurchaseControlRule\SQLRowPurchaseControlRule.h"
/**********************************************************************/
#include "ListDataDlg.h"
#include "SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersPurchaseControlPlu : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersPurchaseControlPlu(CReportConsolidationArray<CSortedStringAndIntByInt64>& arrayPlu);

public:
	int GetArraySize();
	bool PrepareLine(int nIndex, CString& strLine);
	void DeleteDatabaseEntry(int nIndex);
	void SetColumns(CSSListCtrlVirtual& list, CWnd* pParent);

private:
	void FilterSelectedItemArray(CReportConsolidationArray<CSortedIntItem>& arraySelected);

private:
	CReportConsolidationArray<CSortedStringAndIntByInt64>& m_arrayPlu;
};

/**********************************************************************/

class CListDataPurchaseControlPluDlg : public CListDataDlg
{
public:
	CListDataPurchaseControlPluDlg(CSQLRowPurchaseControlRule& RowSO, CWnd* pParent = NULL);
	~CListDataPurchaseControlPluDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual void PrepareList();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON1; }
	virtual bool HandleOK();
	virtual bool HandleCancel();

private:
	CReportConsolidationArray<CSortedStringAndIntByInt64> m_arrayPlu;
	CString m_strTitle;
};

/**********************************************************************/
