#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "..\CommonEcrStock\ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersCustomField : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersCustomField();

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex ){}
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent ){}
};

/**********************************************************************/

class CListDataCustomFieldDlg : public CListDataDlg
{
public:
	CListDataCustomFieldDlg( CWnd* pParent = NULL); 
	~CListDataCustomFieldDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON1; }
	virtual void HandleButton1();	//NAME
	virtual void HandleDefaultButton() { HandleButton1(); }
};

/**********************************************************************/
#endif
/**********************************************************************/
