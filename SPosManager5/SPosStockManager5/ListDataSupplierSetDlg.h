#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "..\CommonEcrStock\ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersSupplierSet : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersSupplierSet();

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );
};

/**********************************************************************/

class CListDataSupplierSetDlg : public CListDataDlg
{
public:
	CListDataSupplierSetDlg( CWnd* pParent = NULL); 
	~CListDataSupplierSetDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON2; }
	virtual void HandleButton1();	//ADD
	virtual void HandleButton2();	//EDIT
	virtual void HandleButton3();	//DELETE
	virtual void HandleButton4();	//MOVE UP
	virtual void HandleButton5();	//MOVE DOWN
	virtual void HandleDefaultButton() { HandleButton2(); }
};

/**********************************************************************/
#endif
/**********************************************************************/
