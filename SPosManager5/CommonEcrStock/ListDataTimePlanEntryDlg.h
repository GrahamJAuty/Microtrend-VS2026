#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersTimePlanEntry : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersTimePlanEntry(CTimePlanEntriesCSVArray* pTimePlanEntryArray, int nPlanNo );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );

private:
	int m_nPlanNo = 0;
	CTimePlanEntriesCSVArray* m_pTimePlanEntryArray = NULL;
};

/**********************************************************************/

class CListDataTimePlanEntryDlg : public CListDataDlg
{
public:
	CListDataTimePlanEntryDlg( bool bTemplateMode, int nPlanNo, CWnd* pParent = NULL); 
	~CListDataTimePlanEntryDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	
	virtual UINT GetDefaultButtonID() { return IDC_BUTTON2; }
	virtual void HandleButton1();	//ADD
	virtual void HandleButton2();	//EDIT
	virtual void HandleButton3();	//DELETE
	virtual void HandleButton4();	//NAME
	virtual void HandleDefaultButton() { HandleButton2(); }

private:
	int m_nPlanNo = 0;

private:
	bool m_bTemplateMode = FALSE;
	CString m_strEntityTypeNoCaps = "";
	CString m_strEntityTypeCaps = "";
	CTimePlanEntriesCSVArray* m_pTimePlanEntryArray = NULL;
};

/**********************************************************************/
#endif
/**********************************************************************/
