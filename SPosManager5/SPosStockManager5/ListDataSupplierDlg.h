#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "..\CommonEcrStock\ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersSupplier : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersSupplier( CWordArray& arrayProtected );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );

public:
	void FilterSelectedItemArray( CReportConsolidationArray<CSortedIntItem>&  arraySelected );

private:
	CWordArray& m_arrayProtected;
};

/**********************************************************************/

class CListDataSupplierDlg : public CListDataDlg
{
public:
	CListDataSupplierDlg( int nSuppNo, CWnd* pParent = NULL); 
	~CListDataSupplierDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual void PrepareList();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON2; }
	virtual void HandleButton1();	//ADD
	virtual void HandleButton2();	//EDIT
	virtual void HandleButton3();	//DELETE
	virtual void HandleButton4();	//PRINT
	virtual void HandleButton5();	//IMPORT
	virtual void HandleDefaultButton() { HandleButton2(); }

private:
	int m_nInitialSuppNo;
	CWordArray m_arrayProtected;
};

/**********************************************************************/
#endif
/**********************************************************************/
