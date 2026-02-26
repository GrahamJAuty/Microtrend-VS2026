#pragma once
/**********************************************************************/
#include "resource.h"
/**********************************************************************/
#include "..\SPOSLoyalty4Shared\SQLTable_StaffCard\SQLRowStaffCard.h"
/**********************************************************************/
#include "ListDataDlg.h"
#include "SingleListHelpers.h"
#include "SQLRowSetStaffCard.h"
/**********************************************************************/

class CSingleListContentHelpersStaffCard : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersStaffCard(CSQLRowSetStaffCard& RowSet);

public:
	int GetArraySize();
	bool PrepareLine(int nIndex, CString& strLine);
	void DeleteDatabaseEntry(int nIndex);
	void SetColumns(CSSListCtrlVirtual& list, CWnd* pParent);

private:
	void FilterSelectedItemArray(CReportConsolidationArray<CSortedIntItem>& arraySelected);
	void AppendNumberOrAny(CCSV& csv, int nNumber);

private:
	CSQLRowSetStaffCard& m_RowSet;
};

/**********************************************************************/

class CListDataStaffCardDlg : public CListDataDlg
{
public:
	CListDataStaffCardDlg( CWnd* pParent = NULL); 
	~CListDataStaffCardDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual void PrepareList();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON2; }
	virtual void HandleButton1();	//ADD
	virtual void HandleButton2();	//EDIT
	virtual void HandleButton3();	//DELETE
	virtual void HandleDefaultButton() { HandleButton2(); }
	virtual bool HandleOK();
	virtual bool HandleCancel();

private:
	CSQLRowSetStaffCard m_RowSet;
};

/**********************************************************************/
