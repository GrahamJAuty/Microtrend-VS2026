#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "..\CommonEcrStock\ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersCategorySet : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersCategorySet( CCategorySetCSVArray* pCatSetArray );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );

private:
	CCategorySetCSVArray* m_pCatSetArray;
};

/**********************************************************************/

class CListDataCategorySetDlg : public CListDataDlg
{
public:
	CListDataCategorySetDlg( bool bSystem, CWnd* pParent = NULL); 
	~CListDataCategorySetDlg();
	
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

private:
	bool m_bSystem;
	CCategorySetCSVArray* m_pCatSetArray;
};

/**********************************************************************/
#endif
/**********************************************************************/
