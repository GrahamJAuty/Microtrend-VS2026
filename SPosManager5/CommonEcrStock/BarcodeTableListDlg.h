#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MySSListCtrl.h"
/**********************************************************************/

class CBarcodeTableListDlg : public CDialog
{
public:
	CBarcodeTableListDlg(CWnd* pParent = NULL);  

	//{{AFX_DATA(CBarcodeTableListDlg)
	enum { IDD = IDD_BARCODETABLE_LIST };
	CButton	m_buttonOK;
	CButton	m_buttonCancel;
	CButton	m_button2;
	CButton	m_button1;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CBarcodeTableListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CBarcodeTableListDlg)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	void OnOK();
	void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
public:
	bool IsModified() { return m_bIsModified; }
	int GetListSelection() { return m_nListSelection; }	
	void SelectLine( int nIndex );

protected:
	virtual int GetArraySize();
	UINT GetDefaultButtonID(){ return IDC_BUTTON1; }
	bool PrepareLine( int nIndex );
	
protected:
	void PrepareList();
	
protected:
	int GetSafeSelection();
	int GetSafeSelection( int nMin, int nMax );

protected:
	CSSListCtrlVirtual m_listData;
	CString m_strDisplayLine;
	bool m_bIsModified;
	int m_nListSelection;
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/
