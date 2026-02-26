#pragma once
/**********************************************************************/
#include "Resource.h"
/**********************************************************************/
#include "SingleListHelpers.h"
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

	//{{AFX_DATA(CListDataDlg)
	enum { IDD = IDD_LIST_DATA };
	CButton	m_button1;
	CButton	m_button2;
	CButton	m_button3;
	CButton	m_button4;
	CButton	m_button5;
	CButton	m_button6;
	CButton	m_buttonOK;
	CButton	m_buttonCancel;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CListDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CListDataDlg)
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg void OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
	
public:
	int GetListSelection() { return m_nListSelection; }	
	void SelectLine( int nIndex );
	static bool CheckBeforeCancel( bool bWarnDownload, bool bWarnChange );
		
protected:
	virtual int GetArraySize();
	virtual void SetTitle() = 0;
	virtual void SetButtonTexts() = 0;
	virtual void SetColumns() = 0;
	virtual UINT GetDefaultButtonID() = 0;
	virtual bool PrepareLine( int nIndex );
	
protected:
	virtual void PrepareList();
	virtual void HandleClickHeader( int nColumn ){}
	virtual void HandleButton1(){}
	virtual void HandleButton2(){}
	virtual void HandleButton3(){}
	virtual void HandleButton4(){}
	virtual void HandleButton5(){}
	virtual void HandleButton6(){}
	virtual void HandleDefaultButton(){}
	virtual bool HandleCancel() { return CheckBeforeCancel( m_bWarnCancelAfterDownload, m_WarnCancelAfterChange.GetFlag() ); }
	virtual bool HandleOK();
	
protected:
	int GetSafeSelection();
	int GetSafeSelection( int nMin, int nMax );

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
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/
