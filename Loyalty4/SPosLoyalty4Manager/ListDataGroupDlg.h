#pragma once
/**********************************************************************/
#include "resource.h"
/**********************************************************************/
#include "..\SPOSLoyalty4Shared\SQLTable_Group\SQLRowGroup.h"
#include "..\SPOSLoyalty4Shared\SQLTable_GroupTopupBonus\SQLRowGroupTopupBonus.h"
/**********************************************************************/
#include "ListDataDlg.h"
#include "SingleListHelpers.h"
#include "SQLRowSetGroup.h"
#include "SQLRowSetGroupTopupBonus.h"
/**********************************************************************/

class CSingleListContentHelpersGroup : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersGroup(CSQLRowSetGroup& RowSetGroup, CSQLRowSetGroupTopupBonus& RowSetGroupTopupBonus);

public:
	int GetArraySize();
	bool PrepareLine(int nIndex, CString& strLine);
	void DeleteDatabaseEntry(int nIndex);
	void SetColumns(CSSListCtrlVirtual& list, CWnd* pParent);

private:
	void FilterSelectedItemArray(CReportConsolidationArray<CSortedIntItem>& arraySelected);

private:
	CSQLRowSetGroup& m_RowSetGroup;
	CSQLRowSetGroupTopupBonus& m_RowSetGroupTopupBonus;
};

/**********************************************************************/

class CListDataGroupDlg : public CListDataDlg
{
public:
	CListDataGroupDlg( CWnd* pParent = NULL); 
	~CListDataGroupDlg();
	
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
	CSQLRowSetGroup m_RowSetGroup;
	CSQLRowSetGroupTopupBonus m_RowSetGroupTopupBonus;
	int m_nNextDefaultGroupNo;
};

/**********************************************************************/
