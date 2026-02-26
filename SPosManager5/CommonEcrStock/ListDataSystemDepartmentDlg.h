#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersSystemDepartment : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersSystemDepartment();

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );

private:
	void FilterSelectedItemArray( CReportConsolidationArray<CSortedIntItem>&  arraySelected );
};

/**********************************************************************/

class CListDataSystemDepartmentDlg : public CListDataDlg
{
public:
	CListDataSystemDepartmentDlg( CWnd* pParent = NULL); 
	~CListDataSystemDepartmentDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON2; }
	virtual void HandleButton1();	//ADD
	virtual void HandleButton2();	//EDIT
	virtual void HandleButton3();	//DELETE
	virtual void HandleButton4();	//AUTO
	virtual void HandleDefaultButton() { HandleButton2(); }

private:
	bool IsDepartmentZero( int nIndex );
	
private:
	void DepartmentZeroMessage();
};

/**********************************************************************/
#endif
/**********************************************************************/

