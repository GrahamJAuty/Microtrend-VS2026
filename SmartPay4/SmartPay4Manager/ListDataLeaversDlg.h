#pragma once
/**********************************************************************/
#include "ListDataDlg.h"
#include "SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersLeavers : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersLeavers(CStringArray& arrayYears);

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );
	void DeleteDatabaseEntry(int nIndex) {}

private:
	CStringArray& m_arrayYears;
};

/**********************************************************************/

class CListDataLeaversDlg : public CListDataDlg
{
public:
	CListDataLeaversDlg( CWnd* pParent = NULL); 
	~CListDataLeaversDlg();
		
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual void PrepareList();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON1; }
	virtual void HandleButton1();	//ADD
	virtual void HandleButton2();	//EDIT
	virtual void HandleButton4();	//MOVE UP
	virtual void HandleDefaultButton() { HandleButton1(); }

private:
	CString GetSelectedYear();
	void DoFind(CString strYear);

private:
	CString m_strInitialText;
	CString m_strSelectedText;

private:
	CStringArray m_arrayYears;
};

/**********************************************************************/
