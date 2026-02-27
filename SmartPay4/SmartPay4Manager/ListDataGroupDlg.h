#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\UserTypeText.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRowGroupFull.h"
/**********************************************************************/
#include "ListDataDlg.h"
#include "SingleListHelpers.h"
#include "SQLRowSetGroupFull.h"
/**********************************************************************/

class CSingleListContentHelpersGroup : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersGroup(CSQLRowSetGroupFull& RowSet, CUserTypeText& m_userType );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );
	
private:
	void FilterSelectedItemArray( CReportConsolidationArray<CSortedIntItem>&  arraySelected );

private:
	CUserTypeText& m_userType;
	CSQLRowSetGroupFull& m_RowSet;
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
	CSQLRowSetGroupFull m_RowSet;
	CUserTypeText m_userType;
	int m_nNextDefaultGroupNo;
};

/**********************************************************************/
