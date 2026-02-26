#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/
#include "..\CommonEcrStock\ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersEcrmanSupplier : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersEcrmanSupplier();

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );
};

/**********************************************************************/

class CListDataEcrmanSupplierDlg : public CListDataDlg
{
public:
	CListDataEcrmanSupplierDlg( int nInitialSuppNo, CWnd* pParent = NULL); 
	~CListDataEcrmanSupplierDlg();
	
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

private:
	int m_nInitialSuppNo;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
