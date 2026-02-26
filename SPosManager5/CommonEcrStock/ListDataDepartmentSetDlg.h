#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersDepartmentSet : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersDepartmentSet( CDepartmentSetCSVArray* pDeptSetArray );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );

private:
	CDepartmentSetCSVArray* m_pDeptSetArray;
};

/**********************************************************************/

class CListDataDepartmentSetDlg : public CListDataDlg
{
public:
	CListDataDepartmentSetDlg( bool bSystem, CWnd* pParent = NULL); 
	~CListDataDepartmentSetDlg();
	
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
	CDepartmentSetCSVArray* m_pDeptSetArray;
};

/**********************************************************************/
#endif
/**********************************************************************/
