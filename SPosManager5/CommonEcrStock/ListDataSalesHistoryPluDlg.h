#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersSalesHistoryPlu : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersSalesHistoryPlu();

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );
};

/**********************************************************************/

class CListDataSalesHistoryPluDlg : public CListDataDlg
{
public:
	CListDataSalesHistoryPluDlg( int nInitialSel, CWnd* pParent = NULL); 
	~CListDataSalesHistoryPluDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual void PrepareList();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON2; }
	virtual void HandleButton1();	//ADD
	virtual void HandleButton2();	//EDIT
	virtual void HandleButton3();	//DELETE
	virtual void HandleButton4();	//MOVE UP
	virtual void HandleButton5();	//MOVE DOWN
	virtual void HandleButton6();	//COPY
	virtual void HandleDefaultButton() { HandleButton2(); }

private:
	void AddOrFindReport( int nInsertPos, int nReportType );

private:
	int m_nInitialSel;
};

/**********************************************************************/
#endif
/**********************************************************************/
