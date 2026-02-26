#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersDepartment : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersDepartment( int& nGroupListMode );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );

private:
	void FilterSelectedItemArray( CReportConsolidationArray<CSortedIntItem>&  arraySelected );

private:
	int& m_nGroupListMode;
};

/**********************************************************************/

class CListDataDepartmentDlg : public CListDataDlg
{
public:
	CListDataDepartmentDlg( CWnd* pParent = NULL); 
	~CListDataDepartmentDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();

	virtual void HandleClickHeader( int nColumn );
	virtual UINT GetDefaultButtonID() { return IDC_BUTTON2; }
	virtual void HandleButton1();	//ADD
	virtual void HandleButton2();	//EDIT
	virtual void HandleButton3();	//DELETE
	virtual void HandleButton4();	//PRINT
	virtual void HandleButton5();	//UPLOAD
	virtual void HandleDefaultButton() { HandleButton2(); }

	bool SearchText(int nIndex, CString strSearchText);

private:
	bool IsDepartmentZero( int nIndex );
	void HandleDownload( int nDbIdx, CArray<int,int>& arrayLocIdx );

private:
	void DepartmentZeroMessage();

private:
	int m_nGroupListMode;
};

/**********************************************************************/
#endif
/**********************************************************************/

