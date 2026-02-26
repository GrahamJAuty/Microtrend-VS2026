#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

/**********************************************************************/
//FORWARD DECLARATION TO AVOID HAVING HEADERS INCLUDE EACH OTHER
class CListDataSearchDlg;
/**********************************************************************/

class CListDataDlgChangeFlag
{
public:
	CListDataDlgChangeFlag(){ m_bFlag = FALSE; }
	
public:
	bool GetFlag(){ return m_bFlag; }
	void SetFlag( bool b ){ m_bFlag |= b; }
	void SetFlag(){ m_bFlag = TRUE; }

private:
	bool m_bFlag;
};

/**********************************************************************/

class CListDataDlg : public CSSDialog
{
public:
	CListDataDlg(CWnd* pParent = NULL); 
	~CListDataDlg();

	//{{AFX_DATA(CListDataDlg)
	enum { IDD = IDD_LIST_DATA };
	CButton	m_button1;
	CButton	m_button2;
	CButton	m_button3;
	CButton	m_button4;
	CButton	m_button5;
	CButton	m_button6;
	CButton	m_buttonHidden1;
	CButton	m_buttonHidden2;
	CButton	m_buttonOK;
	CButton	m_buttonCancel;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CListDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	void DeleteSearchDlg();
	virtual bool SearchText(int nIndex, CString strSearchText) { return FALSE; }

protected:
	//{{AFX_MSG(CListDataDlg)
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButtonHidden1();
	afx_msg void OnButtonHidden2();
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg long OnSearchMessage(WPARAM wIndex, LPARAM lParam);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int GetListSelection() { return m_nListSelection; }	
	void SelectLine( int nIndex );
	static bool CheckBeforeCancel( bool bWarnDownload, bool bWarnChange );
	
	void SaveSearchSettings(bool bPrevious, CString strText) {
		m_bSearchPrevious = bPrevious;
		m_strSearchText = strText;
	}

	virtual void HandleSearch();
		
protected:
	virtual int GetArraySize();
	virtual void SetTitle() = 0;
	virtual void SetButtonTexts() = 0;
	virtual void SetColumns() = 0;
	virtual UINT GetDefaultButtonID() = 0;
	virtual bool PrepareLine( int nIndex );
	
protected:
	virtual void PrepareList();
	virtual void HandleClickHeader(int nColumn);
	virtual void HandleButton1(){}
	virtual void HandleButton2(){}
	virtual void HandleButton3(){}
	virtual void HandleButton4(){}
	virtual void HandleButton5(){}
	virtual void HandleButton6(){}
	virtual void HandleButtonHidden1(){}
	virtual void HandleButtonHidden2() {}
	virtual void HandleDefaultButton(){}
	virtual bool HandleCancel() { return CheckBeforeCancel( m_bWarnCancelAfterDownload, m_WarnCancelAfterChange.GetFlag() ); }
	virtual bool HandleOK();
	
protected:
	int GetSafeSelection();
	int GetSafeSelection( int nMin, int nMax );

protected:
	void EnableHiddenButton1(CString strText);
	void EnableHiddenButton2(CString strText);

protected:
	CSSListCtrlVirtual m_listData;
	CString m_strDisplayLine;
	bool m_bMultiSelect;
	bool m_bEnableColour;
	int m_nListSelection;

protected:
	bool m_bWarnCancelAfterDownload;
	CListDataDlgChangeFlag m_WarnCancelAfterChange;
	
protected:
	CSingleListDisplayHelpers* m_pDisplayHelpers;
	CSingleListContentHelpers* m_pContentHelpers;

protected:
	CListDataSearchDlg* m_pSearchDlg;
	bool m_bSearchPrevious;
	CString m_strSearchText;
	int m_nSearchColumn;
	CString m_strSearchPrompt;
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/
#endif
/**********************************************************************/
