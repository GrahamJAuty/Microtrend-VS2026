#pragma once
/**********************************************************************/
#include "ListDataDlg.h"
#include "SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersEmailAddress : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersEmailAddress();

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );
};

/**********************************************************************/

class CListDataEmailAddressDlg : public CListDataDlg
{
public:
	CListDataEmailAddressDlg( const char* szName, CWnd* pParent = NULL); 
	~CListDataEmailAddressDlg();
	
public:
	const char* GetSelectedText();
	
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
	virtual void HandleDefaultButton() { HandleButton2(); }

private:
	CString m_strInitialText;
	CString m_strInitialName;
	CString m_strSelectedText;
};

/**********************************************************************/
