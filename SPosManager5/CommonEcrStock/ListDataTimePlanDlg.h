#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersTimePlan : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersTimePlan();

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );

public:
	void ClearDeletionList(){ m_arrayDeletionList.RemoveAll(); }
	int GetDeletionListSize(){ return m_arrayDeletionList.GetSize(); }
	int GetDeletionListIndex( int n );

private:
	CReportConsolidationArray<CSortedIntItem> m_arrayDeletionList;
};

/**********************************************************************/

class CListDataTimePlanDlg : public CListDataDlg
{
public:
	CListDataTimePlanDlg( CWnd* pParent = NULL); 
	~CListDataTimePlanDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON2; }
	virtual void HandleButton1();		//ADD
	virtual void HandleButton2();		//EDIT
	virtual void HandleButton3();		//COPY
	virtual void HandleButton4();		//DELETE
	virtual void HandleButton5();		//PRINT
	virtual void HandleButton6();		//DOWNLOAD
	virtual void HandleClickHeader( int nColumn );	//SORT
	virtual void HandleDefaultButton() { HandleButton2(); }

	void HandleDownload( CArray<int,int>& arrayLocIdx );

	bool SearchText(int nIndex, CString strSearchText);

private:
	void CopyTimePlanEntries(bool bTemplate, int nSourcePlanNo, int nDestPlanNo);
};

/**********************************************************************/
#endif
/**********************************************************************/
