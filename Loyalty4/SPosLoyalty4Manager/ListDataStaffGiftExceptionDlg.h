#pragma once
/**********************************************************************/
#include "resource.h"
/**********************************************************************/
#include "..\SPOSLoyalty4Shared\SQLTable_StaffGiftException\SQLRowStaffGiftException.h"
/**********************************************************************/
#include "ListDataDlg.h"
#include "SingleListHelpers.h"
#include "SQLRowSetStaffGiftException.h"
/**********************************************************************/

class CSingleListContentHelpersStaffGiftException : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersStaffGiftException(CSQLRowSetStaffGiftException& RowSet);

public:
	int GetArraySize();
	bool PrepareLine(int nIndex, CString& strLine);
	void DeleteDatabaseEntry(int nIndex);
	void SetColumns(CSSListCtrlVirtual& list, CWnd* pParent);

private:
	void FilterSelectedItemArray(CReportConsolidationArray<CSortedIntItem>& arraySelected);

private:
	CSQLRowSetStaffGiftException& m_RowSet;
};

/**********************************************************************/

class CListDataStaffGiftExceptionDlg : public CListDataDlg
{
public:
	CListDataStaffGiftExceptionDlg( CWnd* pParent = NULL);
	~CListDataStaffGiftExceptionDlg();

public:
	int GetExceptionCount() { return m_RowSet.GetSize(); }
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual void PrepareList();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON2; }
	virtual void HandleButton1();	
	virtual void HandleButton2();
	virtual void HandleButton3();
	virtual void HandleButton6();
	virtual void HandleDefaultButton() { HandleButton2(); }
	virtual bool HandleOK();
	virtual bool HandleCancel();

private:
	bool RetryGift(int nIndex, CSQLRowStaffGiftException& RowSG, CString& strError, bool bSingleLine );
	void RefreshRow(CSQLRowStaffGiftException& RowSG);

private:
	CSQLRowSetStaffGiftException m_RowSet;
	
private:
	CString m_strContinuePrompt;
};

/**********************************************************************/
