#pragma once
/**********************************************************************/
#include "resource.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PurchaseControlRule\SQLRowPurchaseControlRule.h"
/**********************************************************************/
#include "ListDataDlg.h"
#include "SingleListHelpers.h"
#include "SQLRowSetPurchaseControlInfo.h"
/**********************************************************************/

class CSingleListContentHelpersPurchaseControlInfo : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersPurchaseControlInfo(CSQLRowSetPurchaseControlInfo& RowSet);

public:
	int GetArraySize();
	bool PrepareLine(int nIndex, CString& strLine);
	void DeleteDatabaseEntry(int nIndex);
	void SetColumns(CSSListCtrlVirtual& list, CWnd* pParent);

private:
	void FilterSelectedItemArray(CReportConsolidationArray<CSortedIntItem>& arraySelected);

private:
	CSQLRowSetPurchaseControlInfo& m_RowSet;
};

/**********************************************************************/

class CListDataPurchaseControlInfoDlg : public CListDataDlg
{
public:
	CListDataPurchaseControlInfoDlg( CWnd* pParent = NULL); 
	~CListDataPurchaseControlInfoDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual void PrepareList();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON1; }
	virtual void HandleButton1();	//PLU LIST
	virtual void HandleButton3();	//DELETE EXPIRED
	virtual void HandleButton4();	//RESTORE EXPIRED
	virtual void HandleDefaultButton() { HandleButton1(); }
	virtual bool HandleOK();
	virtual bool HandleCancel();

private:
	CSQLRowSetPurchaseControlInfo m_RowSet;
};

/**********************************************************************/
