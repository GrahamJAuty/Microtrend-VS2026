#pragma once
/**********************************************************************/
#include "ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

class CSingleListContentHelpersCustomerAccount : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersCustomerAccount();

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	bool PrepareDeletionLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );

private:
	void FilterSelectedItemArray( CReportConsolidationArray<CSortedIntItem>&  arraySelected );

public:
	int GetArrayIdxFromListIdx( int nListIdx );
};

/**********************************************************************/

class CListDataCustomerAccountDlg : public CListDataDlg
{
public:
	CListDataCustomerAccountDlg( int nLocIdx, CWnd* pParent = NULL); 
	~CListDataCustomerAccountDlg();

public:
	bool GetAdminOptionsFlag(){ return m_bAdminOptions; }
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();

	virtual void HandleClickHeader( int nColumn );
	virtual UINT GetDefaultButtonID() { return IDC_BUTTON2; }
	virtual void HandleButton1();	//START DATE
	virtual void HandleButton2();	//EDIT
	virtual void HandleButton3();	//PRINT
	virtual void HandleButton4();	//DELETE CUSTOMERS
	virtual void HandleButton5();	//MOVE CUSTOMERS
	virtual void HandleButton6();	//ADD CUSTOMERS
	virtual void HandleDefaultButton() { HandleButton2(); }

private:
	int m_nLocIdx;
	bool m_bCheckedActivity;
	bool m_bAdminOptions;

private:
	void SelectLineByID( int nID );
};

/**********************************************************************/
#endif
/**********************************************************************/
