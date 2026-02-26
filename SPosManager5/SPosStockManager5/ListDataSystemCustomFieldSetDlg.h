#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "..\CommonEcrStock\ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersSystemCustomFieldSet : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersSystemCustomFieldSet();

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex ){}
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent ){}
};

/**********************************************************************/

class CListDataSystemCustomFieldSetDlg : public CListDataDlg
{
public:
	CListDataSystemCustomFieldSetDlg( CWnd* pParent = NULL); 
	~CListDataSystemCustomFieldSetDlg();
	
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
