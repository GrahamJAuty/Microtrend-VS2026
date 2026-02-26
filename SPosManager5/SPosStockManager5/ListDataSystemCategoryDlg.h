#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "..\CommonEcrStock\ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersSystemCategory : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersSystemCategory();

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex ){}
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent ){}
};

/**********************************************************************/

class CListDataSystemCategoryDlg : public CListDataDlg
{
public:
	CListDataSystemCategoryDlg( int nCategory, CWnd* pParent = NULL); 
	~CListDataSystemCategoryDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual void PrepareList();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON1; }
	virtual void HandleButton1();	//EDIT
	virtual void HandleButton4();	//PRINT
	virtual void HandleDefaultButton() { HandleButton1(); }

private:
	int m_nInitialCategory;
};

/**********************************************************************/
#endif
/**********************************************************************/

