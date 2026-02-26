#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "..\CommonEcrStock\ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersCustomFieldSet : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersCustomFieldSet();

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex ){}
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent ){}
};

/**********************************************************************/

class CListDataCustomFieldSetDlg : public CListDataDlg
{
public:
	CListDataCustomFieldSetDlg( CWnd* pParent = NULL); 
	~CListDataCustomFieldSetDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	
	virtual UINT GetDefaultButtonID() { return IDC_BUTTON1; }
	virtual void HandleButton1();	//VALUE
	virtual void HandleDefaultButton() { HandleButton1(); }
};

/**********************************************************************/
#endif
/**********************************************************************/
