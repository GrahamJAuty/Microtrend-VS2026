#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersHistoricalTax : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersHistoricalTax();

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );
};

/**********************************************************************/

class CListDataHistoricalTaxDlg : public CListDataDlg
{
public:
	CListDataHistoricalTaxDlg( CWnd* pParent = NULL); 
	~CListDataHistoricalTaxDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON2; }
	virtual void HandleButton1();	//ADD
	virtual void HandleButton2();	//EDIT
	virtual void HandleButton3();	//DELETE
	virtual void HandleButton4();	//DATE
	virtual void HandleDefaultButton() { HandleButton2(); }

private:
	bool PrepareReport();
};

/**********************************************************************/
#endif
/**********************************************************************/
