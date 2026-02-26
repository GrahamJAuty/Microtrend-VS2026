#pragma once
/**********************************************************************/
#include "ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

class CSingleListContentHelpersCustomerOneOff : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersCustomerOneOff();

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );

public:
	int GetArrayIdxFromListIdx( int nListIdx );
};

/**********************************************************************/

class CListDataCustomerOneOffDlg : public CListDataDlg
{
public:
	CListDataCustomerOneOffDlg( int nLocIdx, CWnd* pParent = NULL); 
	~CListDataCustomerOneOffDlg();

public:
	bool GetAdminOptionsFlag(){ return m_bAdminOptions; }
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();

	virtual void HandleClickHeader( int nColumn );
	virtual UINT GetDefaultButtonID() { return IDC_BUTTON2; }
	virtual void HandleButton2();	//EDIT
	virtual void HandleButton3();	//PRINT
	virtual void HandleButton4();	//DELETE CUSTOMERS
	virtual void HandleButton5();	//MOVE CUSTOMERS
	virtual void HandleButton6();	//ADD CUSTOMERS
	virtual void HandleDefaultButton() { HandleButton2(); }

private:
	bool m_bAdminOptions;
	int m_nLocIdx;

private:
	void SelectLineByID( int nID );
};

/**********************************************************************/
#endif
/**********************************************************************/
