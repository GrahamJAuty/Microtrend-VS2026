#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "..\CommonEcrStock\ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersCategory : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersCategory();

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex ){}
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent ){}
};

/**********************************************************************/

class CListDataCategoryDlg : public CListDataDlg
{
public:
	CListDataCategoryDlg( int nCategory, CWnd* pParent = NULL); 
	~CListDataCategoryDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual void PrepareList();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON1; }
	virtual void HandleButton1();	//EDIT
	virtual void HandleButton2();	//DEFAULT
	virtual void HandleButton4();	//PRINT
	virtual void HandleDefaultButton() { HandleButton1(); }

private:
	int m_nInitialCategory;
};

/**********************************************************************/
#endif
/**********************************************************************/

