#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersCustomFieldValue : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersCustomFieldValue( bool bInContextEdit );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );

public:
	void FilterSelectedItemArray( CReportConsolidationArray<CSortedIntItem>&  arraySelected );

private:
	bool m_bInContextEdit;
};

/**********************************************************************/

class CListCustomFieldValueDlg : public CDialog
{
public:
	CListCustomFieldValueDlg( bool bInContextEdit, int nInitialValueNo, CWnd* pParent = NULL);  
	~CListCustomFieldValueDlg();

public:
	int GetCustomFieldValueNo(){ return m_nCustomFieldValueNo; }

	//{{AFX_DATA(CListCustomFieldValueDlg)
	enum { IDD = IDD_LIST_CUSTOM_FIELD };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CListCustomFieldValueDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	CEdit m_editName;
	CStatic m_staticName;

protected:
	//{{AFX_MSG(CListCustomFieldValueDlg)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnKillFocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
public:
	bool IsModified() { return m_bIsModified; }
	int GetListSelection() { return m_nListSelection; }	
	void SelectLine( int nIndex );

protected:
	int GetArraySize();
	bool PrepareLine( int nIndex );
		
protected:
	int GetSafeSelection();
	int GetSafeSelection( int nMin, int nMax );
	
private:
	bool m_bInContextEdit;
	int m_nInitialValueNo;
	int m_nCustomFieldValueNo;
	
protected:
	CSSListCtrlVirtual m_listData;
	CString m_strDisplayLine;
	bool m_bIsModified;
	int m_nListSelection;

public:
	CString m_strName;

private:
	CSingleListDisplayHelpers* m_pDisplayHelpers;
	CSingleListContentHelpersCustomFieldValue* m_pContentHelpers;

private:
	CString m_strDeletionText;
	CListDataDlgChangeFlag m_WarnCancelAfterChange;
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/

